#include "records_generator.h"
#include <fstream>
#include <stdlib.h>
#include <time.h>
using namespace std;

records_generator::records_generator(const char* file_name, int record_length, int page_size, int num_of_records)
{
	this->file_name = file_name;
	this->record_length = record_length;
	this->page_size = page_size;
	this->num_of_records = num_of_records;
}

records_generator::~records_generator()
{

}

void records_generator::generate()
{
	ofstream MyFile("filename.txt");
	srand(time(NULL));

	for (int i = 0; i < this->num_of_records; i++)
	{
		MyFile << rand() % 100 << " ";
	}

	MyFile.close();
	printf("done\n");
}