#pragma once

class natural_merge_two_plus_one
{
private:
	const char* file_name;
	int block_size;
	int record_size;
public:
	natural_merge_two_plus_one(const char* file_name, int block_size, int record_size);
	int sort();
};

