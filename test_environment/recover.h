#ifndef TESTC_RECOVER_H
#define TESTC_RECOVER_H

#include "table_values.h"
#include "basic_operators.h"
#include "test_env.h"

uint64_t recover_classic(uint8_t** raid, unsigned int number_of_strips, unsigned int number_of_stripes,
                         unsigned int a, unsigned int b);

uint64_t recover_vector(uint8_t** raid, unsigned int number_of_strips, unsigned int number_of_stripes,
                        unsigned int a, unsigned int b);

uint64_t recover_RAIDIX(uint8_t** raid, unsigned int number_of_strips, unsigned int number_of_stripes,
                        unsigned int a, unsigned int b);

void recover_one_stripe_classic(uint8_t* const stripe, unsigned int number_of_strips, unsigned int a, unsigned int b);

void recover_one_stripe_vector(__m256i* const stripe, unsigned int number_of_strips, unsigned int a, unsigned int b);

void recover_one_stripe_RAIDIX(__m256i* const stripe, unsigned int number_of_strips, unsigned int a, unsigned int b);

#endif //TESTC_RECOVER_H
