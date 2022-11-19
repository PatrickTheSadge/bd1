#pragma once
#include <stdio.h>
#include "file_writer.h"

class records_generator
{
private:
	int record_length;
	int num_of_records;
	file_writer* writer;

public:
	// record_length - number of record fields
	records_generator(int record_length, int num_of_records, file_writer* writer);

	~records_generator();

	void generate();
};

