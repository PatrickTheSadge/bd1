#include "file_reader.h"
#include <fstream>
#include <stdio.h>
#include "record.h"

file_reader::file_reader(const char* file_name, const int block_size)
{
	this->file_size = 0;
	this->file_name = file_name;
	this->block_size = block_size;
	this->block = new char[block_size];
	std::fstream file;
	file.open(file_name, std::ios_base::binary | std::ios_base::in);
	file.seekg(0, file.end);
	this->file_size = file.tellg();
	file.close();


	//this->block_index = block_size; //bigger than block indexes so block will be loaded on first read
	read_next_block_from_file();
}

file_reader::~file_reader()
{
	delete block;
	//printf("\nFile reader accesses: %I64d\n", file_accesses);
}

void file_reader::read_next_block_from_file()
{
	std::fstream file(this->file_name, std::ios::binary | std::ios::in);
	file.seekg(file_next_block_pos);
	file.read(block, block_size);
	block_index = 0;
	file_accesses++;
	//printf("\nPULL");
	//print_block();
	file.close();

	file_next_block_pos = this->file_accesses * this->block_size;
}

void file_reader::print_block()
{
	printf("\n");
	for (int i=0; i<block_size; i++)
	{
		printf("%02x ", block[i]);
	}
}

bool file_reader::read(record* r)
{
	if (!more_data(r->byte_size)) //if no more data return record that consists of zeros
	{
		int* flds = new int[r->size];
		for (int i = 0; i < r->size; i++)	flds[i] = 0;
		r->fields = flds;
		return false;
	}

	if (block_index > block_size - r->byte_size )
	{
		read_next_block_from_file();
	}

	int* flds = new int[r->size];
	std::memcpy((void*)flds, &(block[block_index]), r->byte_size);
	block_index += r->byte_size;
	r->fields = flds;

	return true;
}

bool file_reader::more_data(int r_size)
{
	if (file_next_block_pos > file_size)
												return false;
	else if (file_next_block_pos == file_size && block_index > block_size - r_size)
												return false;
	else if (block[block_index] == -128 &&
			block[block_index + 1] == 0x00 &&
			block[block_index + 2] == 0x00 &&
			block[block_index + 3] == 0x00)
												return false;
	

	return true;
}

long long file_reader::disk_accesses()
{
	return file_accesses;
}