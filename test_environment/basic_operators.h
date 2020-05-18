#ifndef TESTC_BASIC_OPERATORS_H
#define TESTC_BASIC_OPERATORS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint-gcc.h>
#include <malloc.h>
#include <time.h>
#include <immintrin.h>
#include "test_env.h"
#define GF_2_8_module 0x71

uint8_t multiply_by_X_classic(uint8_t a);

uint8_t multiply_A_by_B_classic(uint8_t a, uint8_t b);

__m256i multiply_by_X_vector(__m256i a);

__m256i multiply_A_by_B_vector(__m256i a, uint8_t b);

void multiply_by_X_RAIDIX(__m256i* a);

void multiply_A_by_B_RAIDIX(__m256i* a, uint8_t b, __m256i* sum);

//return difference in ns
uint64_t diff_ns(struct timespec start, struct timespec end);

void print_stripe(uint8_t* const stripe, unsigned int number_of_strips);

#endif //TESTC_BASIC_OPERATORS_H
