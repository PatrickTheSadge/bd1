#pragma once
class file_manager
{
protected:
	const char* file_name;
	char* block;
	int block_capacity;
	int block_index;
public:
	file_manager(const char* file_name, const int block_size);
	~file_manager();
};
