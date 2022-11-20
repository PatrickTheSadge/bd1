#pragma once
#include "record.h"
class file_reader
{
private:
	const char* file_name;
	int block_size;
	char* block;
	int block_index;
	long long file_accesses = 0;
	long long file_size;
	long long file_next_block_pos = 0;
	void read_next_block_from_file();
	void print_block();
	bool more_data(int r_size);
public:
	file_reader(const char* file_name, const int block_size);
	~file_reader();
	bool read(record* r);
	long long disk_accesses();
};


