#pragma once
#include "record.h"

class file_writer
{
protected:
	const char* file_name;
	int block_size;
	char* block;
	int block_capacity;
	int block_index = 0;
	int file_accesses = 0;
private:
	void push_block_to_file();
public:
	file_writer(const char* file_name, const int block_size);
	~file_writer();
	void write(record* r);
};

