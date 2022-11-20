#include "record.h"
#include <stdio.h>

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

void record::print()
{
	printf("[%I64d] ", sum());
	/*printf("\n [%I64d]: ", sum());
	for (int i = 0; i < size; i++)
	{
		printf("%d ", fields[i]);
	}*/
}

long long record::sum()
{
	long long sum = 0;
	for (int i = 0; i < size; i++)	sum += (long long)fields[i];
	return sum;
}