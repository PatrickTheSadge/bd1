#pragma once

class natural_merge_two_plus_one
{
private:
	const char* file_name;
	int block_size;
	int record_size;
	bool debug_mode;
	bool print_start_end;
public:
	int disk_accesses;
	int phases;
	natural_merge_two_plus_one(const char* file_name, int block_size, int record_size, bool debug_mode, bool print_start_end);
	void sort();
};

