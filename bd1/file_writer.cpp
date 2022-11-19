#include "file_writer.h"
#include "record.h"
#include <stdio.h>
#include <fstream>

file_writer::file_writer(const char* file_name, const int block_size)
{
	this->file_name = file_name;
	this->block_size = block_size;
	this->block = new char[block_size];
	this->block_capacity = block_size;
	std::fstream file(file_name, std::ios::binary | std::ios::out);
	file.close();
}

file_writer::~file_writer()
{
	if (block_index > 0)
	{
		for(int i=block_index; i<block_size; i++)
		{
			block[i] = 0;
			block_index++;
			block_capacity--;
		}
		push_block_to_file();
	}
	delete block;
	printf("\nFile writer accesses: %d\n", file_accesses);
}

void file_writer::write(record* r)
{
	if (block_capacity < r->byte_size)
	{
		
		printf("\n{");
		while (block_capacity > 0)
		{
			printf("0");
			block[block_index] = 0;
			block_capacity--;
			block_index++;
		}
		printf("}");
		push_block_to_file();
	}
	printf("\n");

	char* byted_record = r->to_bytes();

	for (int i = 0; i < r->byte_size; i++)
	{
		block[block_index] = byted_record[i];
		block_capacity--;
		block_index++;
	}
	for (int i = 0; i < r->size; i++)
	{
		printf("%d ", r->fields[i]);
	}
}

void file_writer::push_block_to_file()
{
	std::fstream file(this->file_name, std::ios::app | std::ios::binary | std::ios::out);
	file.write(block, block_size);
	file_accesses++;
	block_capacity = block_size;
	block_index = 0;
	printf("\nPUSH");
	file.close();
}