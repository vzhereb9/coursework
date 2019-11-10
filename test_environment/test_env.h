#ifndef TESTC_TEST_ENV_H
#define TESTC_TEST_ENV_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int compare(const void* x1, const void* x2);

#define byte unsigned char
#define size_of_disk 4096 // в 4 Кб 4096 байт, 32768 бит

double run_test_env();

double speed_of_one_check(unsigned int number_of_disks);

void calc(byte** raid, unsigned int number_of_disks);

#endif //TESTC_TEST_ENV_H
