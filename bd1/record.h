#pragma once
class record
{
public:
	record(int* fields, int r_len);
	~record();
	int byte_size;
	int size;
	int* fields;
	char* to_bytes();
	void print();
	long long sum();
};

