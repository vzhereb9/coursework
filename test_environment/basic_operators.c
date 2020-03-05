#include "basic_operators.h"

uint8_t multiplication_by_X_classic(uint8_t a)
{
    return (a << 1) ^ ((a >> 7) * GF_2_8_module);
}

__m256i multiplication_by_X_vector(__m256i a)
{
    // _mm256_cmpgt_epi8 - Compare packed 8-bit integers in a and b for greater-than
    __m256i mask = _mm256_cmpgt_epi8(_mm256_setzero_si256(), a);
    // _mm256_set1_epi8 - Broadcast 8-bit integer a to all elements of dst
    mask = _mm256_and_si256(mask, _mm256_set1_epi8(GF_2_8_module));
    // _mm256_slli_epi16 - Shift packed 16-bit integers in a left by imm8 while shifting in zeros, and store the results in dst
    a = _mm256_slli_epi16(a, 1);
    a = _mm256_and_si256(a, _mm256_set1_epi8(0xFE));
    a = _mm256_xor_si256(a, mask);
}

uint8_t multiply_A_by_B_classic(uint8_t a, uint8_t b)
{
    uint8_t sum = 0;
    while (a && b)
    {
        if (b & 1)
            sum ^= a;
        a = multiplication_by_X_classic(a);
        b = b >> 1;
    }
    return sum;
}

__m256i multiply_A_by_B_vector(__m256i a, __m256i b)
{
    __m256i sum = _mm256_setzero_si256(), mask;
    for (unsigned int i = 0; i < 8; i++)
    {
        mask = _mm256_set1_epi8(0x01);
        mask = _mm256_and_si256(mask, b);
        mask = _mm256_cmpgt_epi8(mask, _mm256_setzero_si256());
        mask = _mm256_and_si256(mask, a);
        sum = _mm256_xor_si256(sum, mask);
        a = multiplication_by_X_vector(a);
        b = _mm256_srli_epi16(b, 0x01);
        mask = _mm256_set1_epi8(0x7F);
        b = _mm256_and_si256(b, mask);
    }
    return sum;
}

/*void print256_num(__m256i var)
{
    int64_t *v64val = (int64_t*) &var;
    printf("%.16llx %.16llx %.16llx %.16llx\n", v64val[3], v64val[2], v64val[1], v64val[0]);
}*/

void print_stripe(uint8_t* const stripe, unsigned int number_of_disks)
{
    for (unsigned int i = 0; i < (number_of_disks + 2) * size_of_disk; i++)
    {
        if (!(i % size_of_disk))
            printf(" ");
        printf("%.2X", *(stripe + i));
    }
    printf("\n");
}