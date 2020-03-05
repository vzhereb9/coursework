#ifndef TESTC_RECOVER_H
#define TESTC_RECOVER_H

#include <time.h>
#include "table_values.h"
#include "basic_operators.h"
#include "test_env.h"

clock_t recover_classic(uint8_t** raid, unsigned int number_of_disks, unsigned int number_of_stripes,
                     unsigned int a, unsigned int b);

clock_t recover_vector(uint8_t** raid, unsigned int number_of_disks, unsigned int number_of_stripes,
                        unsigned int a, unsigned int b);

clock_t recover_RAIDIX(uint8_t** raid, unsigned int number_of_disks, unsigned int number_of_stripes,
                       unsigned int a, unsigned int b);

void recover_one_stripe_classic(uint8_t* const stripe, unsigned int number_of_disks, unsigned int a, unsigned int b);

void recover_one_stripe_vector(__m256i* const stripe, unsigned int number_of_disks, unsigned int a, unsigned int b);

void recover_one_stripe_RAIDIX(__m256i* const stripe, unsigned int number_of_disks, unsigned int a, unsigned int b);

#endif //TESTC_RECOVER_H
