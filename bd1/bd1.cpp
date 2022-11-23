#include <iostream>
#include "records_generator.h"
#include <stdio.h>
#include "natural_merge_two_plus_one.h"
#include <fstream>

struct options
{
    bool debug_mode = false;
    bool print_start_end = true;
    bool generate = false;
    bool input = false;
    bool sort = false;
    bool help = false;
    bool output = false;
    char output_file_name[128] = "output.txt";
    char file_name[128] = "default.bin";
    int record_length = 15;
    int num_of_records = 10;
    int block_size = 250;
};

void parse_options(int argc, char** argv, options* opts);

int main(int argc, char** argv)
{   
    options opts;
    parse_options(argc, argv, &opts);
    
    
    
    if (opts.help)
    {
        printf("Possible arguments:\n");
        printf("\t-g => generate records\n");
        printf("\t-i => insert records from keyboard (and save to filename)            REQUIRES: -l <int> -n <int> otherwise defaults will be used\n");
        printf("\t-s => sort records (from filename)\n");
        printf("\t-d => debug mode (prints each phase)\n");
        printf("\t-p => disable print records before and after sorting\n");
        printf("\t-h => help\n");
        printf("\t-f <string> => filename                                              default=%s (max_str_len=127)\n", opts.file_name);
        printf("\t-l <int> =>  max number of fields in record,                         default=%d (integers)\n", opts.record_length);
        printf("\t-n <int> =>  number of records to be generated,                      default=%d\n", opts.num_of_records);
        printf("\t-b <int> =>  block size in bytes in file (simulated disk behaviour), default=%d\n", opts.block_size);
        printf("\t-o <string> => output to file (append) 'disk_accesses phases\\n'     default=printing to standard output (max_str_len=127)");
        return 0;
    }

    if (opts.print_start_end)
    {
        printf("file_name:\t%s\n", opts.file_name);
        printf("block_size:\t%d bytes\n", opts.block_size);
        printf("num_of_records:\t%d\n", opts.num_of_records);
        printf("record_length:\t%d integers\n", opts.record_length);
    }

    if (opts.input)
    {
        int tmp = 0;
        file_writer* writer = new file_writer(opts.file_name, opts.block_size, &tmp);  //tmp cuz i do not want this reader to be counted as disk accesses
        printf("Plesae insert %d records made of %d integers:\n<int> <int> <int>...\n<int> <int> <int>...\n", opts.num_of_records, opts.record_length);
        for (int i = 0; i < opts.num_of_records; i++)
        {
            //printf("\n");
            int* fields = new int[opts.record_length];
            for (int j = 0; j < opts.record_length; j++)
            {
                scanf("%d", &(fields[j]));
            }
            record r(fields, opts.record_length);
            //r.print();
            writer->write(&r);
        }
        delete writer;
    }

    if (opts.generate)
    {
        int tmp = 0;
        file_writer* writer = new file_writer(opts.file_name, opts.block_size, &tmp);
        records_generator* a = new records_generator(opts.record_length, opts.num_of_records, writer);
        a->generate();
        //must be deleted cuz on delete block is also being flushed to the file
        delete a;
    }
    if (opts.sort)
    {
        natural_merge_two_plus_one* sorter = new natural_merge_two_plus_one(opts.file_name, opts.block_size, opts.record_length, opts.debug_mode, opts.print_start_end);
        sorter->sort();
        if (opts.output)
        {
            std::fstream file(opts.output_file_name, std::ios::app | std::ios::out);
            file << opts.num_of_records << " " << opts.block_size << " " << sorter->disk_accesses << " " << sorter->phases << "\n";
            file.close();
            //printf("\nNATURAL MERGE 2+1:\n\tDISK ACCESESS: %d\n\tPHASES: %d\n", sorter->disk_accesses, sorter->phases);
        }
        else
            printf("\nNATURAL MERGE 2+1:\n\tDISK ACCESESS: %d\n\tPHASES: %d\n", sorter->disk_accesses, sorter->phases);
        delete sorter;
    }
    
    return 0;
}


void parse_options(int argc, char** argv, options* opts)
{
    if (argc < 2)
    {
        opts->generate = true;
        opts->sort = true;
        printf("Default options: ");
    }
    else
    {
        for (int i = 1; i < argc; i++)
        {
            if (std::strcmp(argv[i], "-g") == 0)
                opts->generate = true;
            else if (std::strcmp(argv[i], "-i") == 0)
                opts->input = true;
            else if (std::strcmp(argv[i], "-d") == 0)
                opts->debug_mode = true;
            else if (std::strcmp(argv[i], "-p") == 0)
                opts->print_start_end = false;
            else if (std::strcmp(argv[i], "-s") == 0)
                opts->sort = true;
            else if (std::strcmp(argv[i], "-h") == 0)
                opts->help = true;
            else if (std::strcmp(argv[i], "-f") == 0)
                std::strcpy(opts->file_name, argv[i + 1]);
            else if (std::strcmp(argv[i], "-l") == 0)
                opts->record_length = std::atoi(argv[i + 1]);
            else if (std::strcmp(argv[i], "-n") == 0)
                opts->num_of_records = std::atoi(argv[i + 1]);
            else if (std::strcmp(argv[i], "-b") == 0)
                opts->block_size = std::atoi(argv[i + 1]);
            else if (std::strcmp(argv[i], "-o") == 0)
            {
                opts->output = true;
                std::strcpy(opts->output_file_name, argv[i + 1]);
            }
                
        }
    }
}
