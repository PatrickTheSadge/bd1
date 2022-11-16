#pragma once
#include <stdio.h>

class records_generator
{
private:
	const char* file_name;
	int record_length;
	int page_size;
	int num_of_records;

public:
	// file_name - name of the file to be generated
	// record_length - number of record fields
	// page_size - size of a disk page
	records_generator(const char* file_name, int record_length, int page_size, int num_of_records);

	~records_generator();

	void generate();
};

