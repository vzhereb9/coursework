#ifndef TESTC_TEST_ENV_H
#define TESTC_TEST_ENV_H

#include "calc.h"
#include "recover.h"
#include <math.h>

#define size_of_strip 1024 //4096 // в 4 Кб 4096 байт, 32768 бит
#define size_of_strip_for_m256i (size_of_strip / 32)

int compare_double(const void* x1, const void* x2);

int bin_search(const unsigned int number_of_experiments, const double speed[number_of_experiments], double key);

double find_avg_speed_confidence_interval(const unsigned int number_of_experiments, double * speed);

void find_avg_speeds(double avg_speed_calc[16], double avg_speed_recover[16], unsigned int flag);

#endif //TESTC_TEST_ENV_H
