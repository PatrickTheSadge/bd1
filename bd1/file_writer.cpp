#include "file_writer.h"
#include "record.h"
#include <stdio.h>
#include <fstream>

file_writer::file_writer(const char* file_name, const int block_size, int* disk_accesses)
{
	this->disk_accesses = disk_accesses;
	this->file_name = file_name;
	this->block_size = block_size;
	this->block = new char[block_size];
	this->block_capacity = block_size;
	std::fstream file(file_name, std::ios::binary | std::ios::out);
	file.close();
}

file_writer::~file_writer()
{
	if (block_capacity >= 4)
	{
		block[block_index] = 0;
		block[block_index + 1] = 0;
		block[block_index + 2] = 0;
		block[block_index + 3] = 0x80;
		block_index+=4;
		block_capacity-=4;
		//printf("{0x80 0x00 0x00 0x00 => no more records}");
	}

	if (block_index > 0)
	{
		//printf("{");
		for(int i=block_index; i<block_size; i++)
		{
			block[i] = 0;
			//printf("0");
			block_index++;
			block_capacity--;
		}
		//printf("}");
		push_block_to_file();
	}
	delete block;
	//printf("\nFile writer accesses: %d\n", file_accesses);
	(*disk_accesses) += file_accesses;
}

void file_writer::write(record* r)
{
	if (block_capacity < r->byte_size)
	{
		
		//printf("{");
		while (block_capacity > 0)
		{
			//printf("0");
			block[block_index] = 0;
			block_capacity--;
			block_index++;
		}
		//printf("}");
		push_block_to_file();
	}

	char* byted_record = r->to_bytes();

	for (int i = 0; i < r->byte_size; i++)
	{
		block[block_index] = byted_record[i];
		block_capacity--;
		block_index++;
	}
	//r->print();
}

void file_writer::push_block_to_file()
{
	std::fstream file(this->file_name, std::ios::app | std::ios::binary | std::ios::out);
	file.write(block, block_size);
	file_accesses++;
	block_capacity = block_size;
	block_index = 0;
	//printf("\nPUSH");
	file.close();
}