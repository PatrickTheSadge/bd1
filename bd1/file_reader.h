#pragma once
#include "record.h"
class file_reader
{
protected:
	const char* file_name;
	int block_size;
	char* block;
	int block_index = 0;
	int file_accesses = 0;
	bool more_data = true;
public:
	void read_next_block_from_file();
public:
	file_reader(const char* file_name, const int block_size);
	~file_reader();
	bool read();
};


