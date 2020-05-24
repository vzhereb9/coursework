#ifndef RECOVER_H_
#define RECOVER_H_

#include "table_values.h"
#include "basic_operators.h"
#include "test_env.h"


uint64_t recover_classic(uint8_t** raid, unsigned int number_of_strips, unsigned int number_of_stripes,
                         unsigned int a, unsigned int b, uint8_t* const stripe_for_check, uint8_t* const new_syndromes);

uint64_t recover_vector(uint8_t** raid, unsigned int number_of_strips, unsigned int number_of_stripes,
                        unsigned int a, unsigned int b, uint8_t* const stripe_for_check, uint8_t* const new_syndromes);

uint64_t recover_RAIDIX(uint8_t** raid, unsigned int number_of_strips, unsigned int number_of_stripes,
                        unsigned int a, unsigned int b, uint8_t* const stripe_for_check, uint8_t* const new_syndromes, uint8_t* const numerator);


uint64_t recover_classic_one_drive(uint8_t** raid, unsigned int number_of_strips, unsigned int number_of_stripes,
                         unsigned int a, uint8_t* const stripe_for_check, uint8_t* const new_syndromes);

uint64_t recover_vector_one_drive(uint8_t** raid, unsigned int number_of_strips, unsigned int number_of_stripes,
                         unsigned int a, uint8_t* const stripe_for_check, uint8_t* const new_syndromes);

uint64_t recover_RAIDIX_one_drive(uint8_t** raid, unsigned int number_of_strips, unsigned int number_of_stripes,
                         unsigned int a, uint8_t* const stripe_for_check, uint8_t* const new_syndromes);


void recover_one_stripe_classic_one_drive(uint8_t* const stripe, unsigned int number_of_strips, unsigned int a, uint8_t* const new_syndromes);

void recover_one_stripe_vector_and_RAIDIX_one_drive(uint8_t* const stripe, unsigned int number_of_strips, unsigned int a, uint8_t* const new_syndromes);


void recover_one_stripe_classic(uint8_t* const stripe, unsigned int number_of_strips, unsigned int a, unsigned int b, uint8_t* const new_syndromes);

void recover_one_stripe_vector(uint8_t* const stripe, unsigned int number_of_strips, unsigned int a, unsigned int b, uint8_t* const new_syndromes);

void recover_one_stripe_RAIDIX(uint8_t* const stripe, unsigned int number_of_strips, unsigned int a, unsigned int b, uint8_t* const new_syndromes, uint8_t* const numerator);

#endif /* RECOVER_H_ */
