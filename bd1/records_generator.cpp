#include "records_generator.h"
#include <stdlib.h>
#include <time.h>
#include "file_writer.h"

records_generator::records_generator(int record_length, int num_of_records, file_writer* writer)
{
	this->record_length = record_length;
	this->num_of_records = num_of_records;
	this->writer = writer;
}

records_generator::~records_generator()
{
	delete writer;
}

void records_generator::generate()
{
	srand(time(NULL));
	for (int j = 0; j < num_of_records; j++)
	{
		int* fields = new int[record_length];
		int rec_rand = rand() % 6 + 10;

		for (int i = 0; i < record_length; i++)
		{
			if (i <= rec_rand)
			{
				fields[i] = rand() % 100;
			}
			else
			{
				fields[i] = 0;
			}
		}
		record r = record(fields, record_length);
		writer->write(&r);
	}
}