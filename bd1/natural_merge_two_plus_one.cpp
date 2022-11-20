#include "natural_merge_two_plus_one.h"
#include "file_reader.h"
#include "file_writer.h"
#include <fstream>
#include <stdio.h>

natural_merge_two_plus_one::natural_merge_two_plus_one(const char* file_name, int block_size, int record_size)
{
	this->file_name = file_name;
	this->block_size = block_size;
    this->record_size = record_size;
}

int natural_merge_two_plus_one::sort()
{
    long long disk_accesses = 0;
    while (true)
    {
        // #################### SPLIT ####################
        file_reader* r_main = new file_reader(file_name, block_size);

        file_writer* t1 = new file_writer("t1.bin", block_size);
        file_writer* t2 = new file_writer("t2.bin", block_size);

        file_writer* tx = t1;

        long long last_rec_val = LLONG_MIN;
        printf("\n\t\tMAIN:\n");
        record r(NULL, record_size);
        while (r_main->read(&r))
        {
            r.print();
            long long rec_val = r.sum();
            if (rec_val < last_rec_val)
            {
                // switch tape
                if (tx == t2)   tx = t1;
                else            tx = t2;
            }
            tx->write(&r);
            last_rec_val = rec_val;
        }

        disk_accesses = disk_accesses + r_main->disk_accesses() + t1->disk_accesses() + t2->disk_accesses();
        delete r_main;
        delete t1;
        delete t2;

        file_reader* reader_t1 = new file_reader("t1.bin", block_size);
        file_reader* reader_t2 = new file_reader("t2.bin", block_size);

        printf("\n\t\tT1:\n");
        int t1_count = 0;
        int t2_count = 0;

        while (reader_t1->read(&r))
        {
            r.print();
            t1_count++;
        }

        printf("\n\t\tT2:\n");
        while (reader_t2->read(&r))
        {
            r.print();
            t2_count++;
        }

        disk_accesses = disk_accesses + reader_t1->disk_accesses() + reader_t2->disk_accesses();

        delete reader_t1;
        delete reader_t2;

        if (t1_count == 0 || t2_count == 0 )
        {
            break;
        }

        // #################### MERGE ####################

        reader_t1 = new file_reader("t1.bin", block_size);
        reader_t2 = new file_reader("t2.bin", block_size);
        file_writer* w_main = new file_writer(file_name, block_size);

        // teraz zmergowac 

        record t1_record(NULL, record_size);
        record t2_record(NULL, record_size);

        bool t1_more = reader_t1->read(&t1_record);
        bool t2_more = reader_t2->read(&t2_record);

        long long t1_rec_sum = t1_record.sum();
        long long t2_rec_sum = t2_record.sum();

        long long t1_rec_last_sum = t1_rec_sum;
        long long t2_rec_last_sum = t2_rec_sum;

        //printf("\n\t\tALSO main:\n");
        while (t1_more || t2_more)
        {
            if (!t1_more || !t2_more)
            {
                // do while cuz there may be not more in file but we got it in our local buffer
                if (!t1_more) // t1 empty
                {
                    do
                    {
                        w_main->write(&t2_record);
                        //t2_record.print();
                    } while (t2_more = reader_t2->read(&t2_record));
                    break;
                }
                else // t2 empty
                {
                    do
                    {
                        w_main->write(&t1_record);
                        //t1_record.print();
                    }while (t1_more = reader_t1->read(&t1_record));
                    break;
                }
            }

            if (t1_rec_sum >= t1_rec_last_sum && t2_rec_sum >= t2_rec_last_sum)
            {
                if (t1_rec_sum < t2_rec_sum) // t1 is smaller -> write t1 to main
                {
                    w_main->write(&t1_record);
                    //t1_record.print();
                    t1_rec_last_sum = t1_rec_sum;
                    t1_more = reader_t1->read(&t1_record);
                    t1_rec_sum = t1_record.sum();
                }
                else                        // t2 is smaller -> write t2 to main
                {
                    w_main->write(&t2_record);
                    //t2_record.print();
                    t2_rec_last_sum = t2_rec_sum;
                    t2_more = reader_t2->read(&t2_record);
                    t2_rec_sum = t2_record.sum();
                }
            }
            else if (t1_rec_sum < t1_rec_last_sum && t2_rec_sum >= t2_rec_last_sum) // t1 series end
            {
                w_main->write(&t2_record);
                //t2_record.print();
                t2_rec_last_sum = t2_rec_sum;
                t2_more = reader_t2->read(&t2_record);
                t2_rec_sum = t2_record.sum();
            }
            else if (t1_rec_sum >= t1_rec_last_sum && t2_rec_sum < t2_rec_last_sum) // t2 series end
            {
                w_main->write(&t1_record);
                //t1_record.print();
                t1_rec_last_sum = t1_rec_sum;
                t1_more = reader_t1->read(&t1_record);
                t1_rec_sum = t1_record.sum();
            }
            else if (t1_rec_sum < t1_rec_last_sum && t2_rec_sum < t2_rec_last_sum)
            {
                if (t1_rec_sum < t2_rec_sum) // t1 is smaller -> write t1 to main
                {
                    w_main->write(&t1_record);
                    //t1_record.print();
                    t1_more = reader_t1->read(&t1_record);
                    t1_rec_sum = t1_record.sum();
                }
                else                        // t2 is smaller -> write t2 to main
                {
                    w_main->write(&t2_record);
                    //t2_record.print();
                    t2_more = reader_t2->read(&t2_record);
                    t2_rec_sum = t2_record.sum();
                }
                t1_rec_last_sum = t1_rec_sum;   // reset series
                t2_rec_last_sum = t2_rec_sum;   // reset series
            }
        }

        disk_accesses = disk_accesses + reader_t1->disk_accesses() + reader_t2->disk_accesses() + w_main->disk_accesses();
        delete reader_t1;
        delete reader_t2;
        delete w_main;
    }

    record r(NULL, record_size);
    printf("\n\t\tDONE:\n");
    file_reader* r_main = new file_reader(file_name, block_size);
    while (r_main->read(&r))
    {
        r.print();
    }
    delete r_main;

    return disk_accesses;
}