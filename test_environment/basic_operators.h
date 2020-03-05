#ifndef TESTC_BASIC_OPERATORS_H
#define TESTC_BASIC_OPERATORS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint-gcc.h>
#include <string.h>
#include <malloc.h>
#include "test_env.h"
#define GF_2_8_module 0x71

uint8_t multiplication_by_X_classic(uint8_t a);

uint8_t multiply_A_by_B_classic(uint8_t a, uint8_t b);

__m256i multiplication_by_X_vector(__m256i a);

__m256i multiply_A_by_B_vector(__m256i a, __m256i b);

//void print256_num(__m256i var);

void print_stripe(uint8_t* const stripe, unsigned int number_of_disks);

#endif //TESTC_BASIC_OPERATORS_H
