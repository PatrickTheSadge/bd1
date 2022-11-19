#include <iostream>
#include "records_generator.h"
#include <stdio.h>
#include "file_writer.h"
#include "file_reader.h"

int main()
{
    file_writer* writer = new file_writer("file_writer_test.bin", 10);
    records_generator a = records_generator(15, 4, writer);
    a.generate();
    file_reader* reader = new file_reader("file_writer_test.bin", 10);
    delete reader;

    return 0;
}
