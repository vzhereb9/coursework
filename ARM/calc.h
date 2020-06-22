#ifndef CALC_H_
#define CALC_H_

#include "table_values.h"
#include "basic_operators.h"
#include "test_env.h"

void calc_one_stripe_classic_one_drive(uint8_t* const stripe, unsigned int number_of_strips);

void calc_one_stripe_vector_and_RAIDIX_one_drive(uint8_t* const stripe, unsigned int number_of_strips);


uint64_t calc_classic(uint8_t** raid, unsigned int number_of_strips, unsigned int number_of_stripes);

uint64_t calc_vector(uint8_t** raid, unsigned int number_of_strips, unsigned int number_of_stripes);

uint64_t calc_RAIDIX(uint8_t** raid, unsigned int number_of_strips, unsigned int number_of_stripes);


void calc_one_stripe_classic(uint8_t* const stripe, unsigned int number_of_strips);

void calc_one_stripe_vector(uint8_t* const stripe, unsigned int number_of_strips);

void calc_one_stripe_RAIDIX(uint8_t* const stripe, unsigned int number_of_strips);

#endif /* CALC_H_ */
