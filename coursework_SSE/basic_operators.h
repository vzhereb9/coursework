#ifndef TESTC_BASIC_OPERATORS_H
#define TESTC_BASIC_OPERATORS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint-gcc.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include <emmintrin.h>
#include "test_env.h"
#define GF_2_8_module 0x71

uint8_t multiply_by_X_classic(uint8_t a);

uint8_t multiply_A_by_B_classic(uint8_t a, uint8_t b);

__m128i multiply_by_X_vector(__m128i a);

__m128i multiply_A_by_B_vector(__m128i a, uint8_t b);

void multiply_by_X_RAIDIX(__m128i* a);

void multiply_A_by_B_RAIDIX(__m128i* a, uint8_t b, __m128i* sum);

//return difference in ns
uint64_t diff_ns(struct timespec start, struct timespec end);

void print_stripe(uint8_t* const stripe, unsigned int number_of_strips);

#endif //TESTC_BASIC_OPERATORS_H
