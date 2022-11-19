#include "record.h"

record::record(int* fields, int r_len)
{
	this->fields = fields;
	this->size = r_len;
	this->byte_size = r_len * sizeof(int);
}

record::~record()
{
	delete fields;
}

char* record::to_bytes()
{
	return (char*)fields;
}