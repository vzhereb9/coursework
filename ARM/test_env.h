#ifndef TESTC_TEST_ENV_H
#define TESTC_TEST_ENV_H

#include "calc.h"
#include "recover.h"
#include <math.h>

#define size_of_strip 4096 //4096

int compare_double(const void* x1, const void* x2);

double find_avg_speed_of_suitable_values(const unsigned int number_of_experiments, double* speed);

void find_avg_speeds(double avg_speed_calc[16], double avg_speed_recover_one_drive[16], double avg_speed_recover[16],
                     unsigned int flag);

#endif //TESTC_TEST_ENV_H
