#include <iostream>
#include "records_generator.h"
#include <stdio.h>
#include "natural_merge_two_plus_one.h"

int main(int argc, char** argv)
{   
    /*switch ()
    {
    case 'r':
        break;
    case 'g':
        break;
    case 's':
        break;
    case 'i':
        break;
    default:
        printf("Only valid options are :\n\
            \t r record_maximum_length (this option is required)\n\
            \t g (generate)\n\
            \t s (sort)\n\
            \t i (insert from file)\n");
    }*/

    file_writer * writer = new file_writer("file_writer_test.bin", 600);
        records_generator* a = new records_generator(15, 60, writer);
        a->generate();
        //must be deleted cuz on delete block is also being flushed to the file
        delete a;

    natural_merge_two_plus_one* sorter = new natural_merge_two_plus_one("file_writer_test.bin", 600, 15);
    printf("\nNATURAL MERGE 2+1:\n\tDISK ACCESESS: %d\n", sorter->sort());
    delete sorter;

    return 0;
}
