#ifndef TEST_ENV_H_
#define TEST_ENV_H_

#define size_of_strip 4096 //4096
#define size_of_strip_for_uint8x16_t (size_of_strip / 16)

#include "calc.h"
#include "recover.h"
#include <math.h>

int compare_double(const void* x1, const void* x2);

int bin_search(const unsigned int number_of_experiments, const double speed[number_of_experiments], double key);

double find_avg_speed_confidence_interval(const unsigned int number_of_experiments, double * speed);

void find_avg_speeds(double avg_speed_calc[16], double avg_speed_recover[16], unsigned int flag);

#endif /* TEST_ENV_H_ */
