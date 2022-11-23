#include "natural_merge_two_plus_one.h"
#include "file_reader.h"
#include "file_writer.h"
#include <fstream>
#include <stdio.h>

natural_merge_two_plus_one::natural_merge_two_plus_one(const char* file_name, int block_size, int record_size, bool debug_mode, bool print_start_end)
{
	this->file_name = file_name;
	this->block_size = block_size;
    this->record_size = record_size;
    this->disk_accesses = 0;
    this->phases = 0;
    this->debug_mode = debug_mode;
    this->print_start_end = print_start_end;
}

void natural_merge_two_plus_one::sort()
{
    if (debug_mode || print_start_end)
    {
        int tmp = 0;
        record r(NULL, record_size);
        printf("\n\t\START:\n");
        file_reader* r_main = new file_reader(file_name, block_size, &tmp);  // tmp cuz i don't want this reader to be counted as disk accessess
        while (r_main->read(&r))
        {
            r.print();
        }
        delete r_main;
    }

    bool sorted = false;

    while (!sorted)
    {
        sorted = true;
        phases++;
        if (debug_mode) printf("\n\t\t=====PHASE %d=====", phases);
        // #################### SPLIT ####################
        file_reader* r_main = new file_reader(file_name, block_size, &disk_accesses);

        file_writer* t1 = new file_writer("t1.bin", block_size, &disk_accesses);
        file_writer* t2 = new file_writer("t2.bin", block_size, &disk_accesses);
        int t1_count = 0;
        int t2_count = 0;

        file_writer* tx = t1;

        long long last_rec_val = LLONG_MIN;
        if (debug_mode) printf("\n\t\tMAIN:\n");
        record r(NULL, record_size);
        while (r_main->read(&r))
        {
            if (debug_mode) r.print();
            long long rec_val = r.sum();
            if (rec_val < last_rec_val)
            {
                // switch tape
                if (tx == t2)   tx = t1;
                else            tx = t2;
            }
            tx->write(&r);
            if (tx == t1)
                t1_count++;
            else
                t2_count++;
            last_rec_val = rec_val;
        }

        delete r_main;
        delete t1;
        delete t2;

        int tmp = 0;
        file_reader* reader_t1 = new file_reader("t1.bin", block_size, &tmp);
        file_reader* reader_t2 = new file_reader("t2.bin", block_size, &tmp);

        if (debug_mode) printf("\n\t\tT1:\n");

        if (debug_mode) while (reader_t1->read(&r)) r.print();

        if (debug_mode) printf("\n\t\tT2:\n");
        if (debug_mode) while (reader_t2->read(&r)) r.print();

        delete reader_t1;
        delete reader_t2;


        if (t1_count == 0 || t2_count == 0 )
        {
            break;
        }

        // #################### MERGE ####################

        reader_t1 = new file_reader("t1.bin", block_size, &disk_accesses);
        reader_t2 = new file_reader("t2.bin", block_size, &disk_accesses);
        file_writer* w_main = new file_writer(file_name, block_size, &disk_accesses);

        // teraz zmergowac 

        record t1_record(NULL, record_size);
        record t2_record(NULL, record_size);

        bool t1_more = reader_t1->read(&t1_record);
        bool t2_more = reader_t2->read(&t2_record);

        long long t1_sum;
        long long t2_sum;

        bool t1_series_new = false;
        bool t2_series_new = false;

        long long last_saved_rec_sum = LLONG_MIN;

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
                        if (t2_record.sum() < last_saved_rec_sum) sorted = false;
                        last_saved_rec_sum = t2_record.sum();
                        //t2_record.print();
                    } while (t2_more = reader_t2->read(&t2_record));
                    break;
                }
                else // t2 empty
                {
                    do
                    {
                        w_main->write(&t1_record);
                        if (t1_record.sum() < last_saved_rec_sum) sorted = false;
                        last_saved_rec_sum = t1_record.sum();
                        //t1_record.print();
                    }while (t1_more = reader_t1->read(&t1_record));
                    break;
                }
            }
            t1_sum = t1_record.sum();
            t2_sum = t2_record.sum();

            if (!t1_series_new && !t2_series_new)
            {
                if (t1_sum < t2_sum) // t1 is smaller -> write t1 to main
                {
                    if (t1_sum < last_saved_rec_sum) sorted = false;
                    last_saved_rec_sum = t1_sum;
                    w_main->write(&t1_record);
                    //t1_record.print();
                    t1_more = reader_t1->read(&t1_record);
                    long long new_sum = t1_record.sum();
                    if (new_sum < t1_sum) t1_series_new = true;
                }
                else                        // t2 is smaller -> write t2 to main
                {
                    if (t2_sum < last_saved_rec_sum) sorted = false;
                    last_saved_rec_sum = t2_sum;
                    w_main->write(&t2_record);
                    //t2_record.print();
                    t2_more = reader_t2->read(&t2_record);
                    long long new_sum = t2_record.sum();
                    if (new_sum < t2_sum) t2_series_new = true;
                }
            }
            else if (t1_series_new && !t2_series_new) // t1 series end
            {
                if (t2_sum < last_saved_rec_sum) sorted = false;
                last_saved_rec_sum = t2_sum;
                w_main->write(&t2_record);
                //t2_record.print();
                t2_more = reader_t2->read(&t2_record);
                long long new_sum = t2_record.sum();
                if (new_sum < t2_sum) t2_series_new = true;
            }
            else if (!t1_series_new && t2_series_new) // t2 series end
            {
                if (t1_sum < last_saved_rec_sum) sorted = false;
                last_saved_rec_sum = t1_sum;
                w_main->write(&t1_record);
                //t1_record.print();
                t1_more = reader_t1->read(&t1_record);
                long long new_sum = t1_record.sum();
                if (new_sum < t1_sum) t1_series_new = true;
            }
            else if (t1_series_new && t2_series_new) // both series end
            {
                t1_series_new = false;
                t2_series_new = false;
                if (t1_sum < t2_sum) // t1 is smaller -> write t1 to main
                {
                    if (t1_sum < last_saved_rec_sum) sorted = false;
                    last_saved_rec_sum = t1_sum;
                    w_main->write(&t1_record);
                    //t1_record.print();
                    t1_more = reader_t1->read(&t1_record);
                    long long new_sum = t1_record.sum();
                    if (new_sum < t1_sum) t1_series_new = true;
                }
                else                        // t2 is smaller -> write t2 to main
                {
                    if (t2_sum < last_saved_rec_sum) sorted = false;
                    last_saved_rec_sum = t2_sum;
                    w_main->write(&t2_record);
                    //t2_record.print();
                    t2_more = reader_t2->read(&t2_record);
                    long long new_sum = t2_record.sum();
                    if (new_sum < t2_sum) t2_series_new = true;
                }
            }
        }

        delete reader_t1;
        delete reader_t2;
        delete w_main;
    }
    if (debug_mode || print_start_end)
    {
        int tmp = 0;
        record r(NULL, record_size);
        printf("\n\t\tDONE:\n");
        file_reader* r_main = new file_reader(file_name, block_size, &tmp);  // tmp cuz i don't want this reader to be counted as disk accessess
        while (r_main->read(&r))
        {
            r.print();
        }
        delete r_main;
    }
}