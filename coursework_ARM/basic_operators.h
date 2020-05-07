#ifndef BASIC_OPERATORS_H_
#define BASIC_OPERATORS_H_

typedef unsigned char uint8_t;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <arm_neon.h>
#include "test_env.h"

#define GF_2_8_module 0x71

uint8_t multiply_by_X_classic(uint8_t a);

uint8_t multiply_A_by_B_classic(uint8_t a, uint8_t b);

uint8x16_t multiply_by_X_vector(uint8x16_t a);

uint8x16_t multiply_A_by_B_vector(uint8x16_t a, uint8_t b);

void multiply_by_X_RAIDIX(uint8_t* a);

void multiply_A_by_B_RAIDIX(uint8_t* a, uint8_t b, uint8_t* sum);

void print_stripe(uint8_t* const stripe, unsigned int number_of_strips);

void print_uint8x16_t(uint8x16_t a);

#endif /* BASIC_OPERATORS_H_ */
