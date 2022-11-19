#include "file_manager.h"

file_manager::file_manager(const char* file_name,const int block_size)
{
	this->file_name = file_name;
	this->block = new char[block_size];
	this->block_capacity = block_size;
	this->block_index = 0;
}

file_manager::~file_manager()
{
	delete block;
}
