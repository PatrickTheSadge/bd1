#include <iostream>
#include "records_generator.h"
#include <stdio.h>

int main()
{
    records_generator a = records_generator("disk_simulated.txt", 10, 1024, 10);
    a.generate();

    return 0;
}
