#ifndef TESTC_CALC_H
#define TESTC_CALC_H

#include "table_values.h"
#include "basic_operators.h"
#include "test_env.h"

uint64_t calc_classic(uint8_t** raid, unsigned int number_of_strips, unsigned int number_of_stripes);

uint64_t calc_vector(uint8_t** raid, unsigned int number_of_strips, unsigned int number_of_stripes);

uint64_t calc_RAIDIX(uint8_t** raid, unsigned int number_of_strips, unsigned int number_of_stripes);

void calc_one_stripe_classic(uint8_t* const stripe, unsigned int number_of_strips);

void calc_one_stripe_vector(__m128i* const stripe, unsigned int number_of_strips);

void calc_one_stripe_RAIDIX(__m128i* const stripe, unsigned int number_of_strips);

#endif //TESTC_CALC_H
