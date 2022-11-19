#include "file_reader.h"
#include <fstream>
#include <stdio.h>

file_reader::file_reader(const char* file_name, const int block_size)
{
	this->file_name = file_name;
	this->block_size = block_size;
	this->block = new char[block_size];
}

file_reader::~file_reader()
{
	delete block;
	printf("\nFile reader accesses: %d\n", file_accesses);
}

void file_reader::read_next_block_from_file()
{
	int next_file_index = this->file_accesses * this->block_size;
	std::fstream file(this->file_name, std::ios::binary | std::ios::in);
	file.read(block, block_size);
	if (file.eof()) more_data = false;
	block_index = 0;
	file_accesses++;
	printf("\nPULL");
	file.close();
}

bool file_reader::read()
{
	if (!more_data)
	{
		return false;
	}
	read_next_block_from_file();
	return true;
}