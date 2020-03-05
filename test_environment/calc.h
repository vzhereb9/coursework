#ifndef TESTC_CALC_H
#define TESTC_CALC_H

#include <time.h>
#include <immintrin.h>
#include "table_values.h"
#include "basic_operators.h"
#include "test_env.h"

clock_t calc_classic(uint8_t** raid, unsigned int number_of_disks, unsigned int number_of_stripes);

clock_t calc_vector(uint8_t** raid, unsigned int number_of_disks, unsigned int number_of_stripes);

clock_t calc_RAIDIX(uint8_t** raid, unsigned int number_of_disks, unsigned int number_of_stripes);

void calc_one_stripe_classic(uint8_t* const stripe, unsigned int number_of_disks);

void calc_one_stripe_vector(__m256i* const stripe, unsigned int number_of_disks);

void calc_one_stripe_RAIDIX(__m256i* const stripe, unsigned int number_of_disks);

#endif //TESTC_CALC_H
