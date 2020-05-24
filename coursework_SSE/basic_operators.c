#include "basic_operators.h"

uint8_t multiply_by_X_classic(uint8_t a)
{
    return (a << 1) ^ ((a >> 7) * GF_2_8_module);
}

__m128i multiply_by_X_vector(__m128i a)
{
    // _mm256_cmpgt_epi8 - Compare packed 8-bit integers in a and b for greater-than
    __m128i mask = _mm_cmpgt_epi8(_mm_setzero_si128(), a);
    // _mm256_set1_epi8 - Broadcast 8-bit integer a to all elements of dst
    mask = _mm_and_si128(mask, _mm_set1_epi8(GF_2_8_module));
    // _mm256_slli_epi16 - Shift packed 16-bit integers in a left by imm8 while shifting in zeros, and store the results in dst
    a = _mm_slli_epi16(a, 1);
    a = _mm_and_si128(a, _mm_set1_epi8(0xFE));
    a = _mm_xor_si128(a, mask);
    return a;
}

void multiply_by_X_RAIDIX(__m128i* a)
{
	for (int i = 0; i < 7; i++)
    {
        _mm_store_si128(a + i, _mm_xor_si128(a[i], a[i + 1]));
        _mm_store_si128(a + i + 1, _mm_xor_si128(a[i + 1], a[i]));
        _mm_store_si128(a + i, _mm_xor_si128(a[i], a[i + 1]));
    }
    _mm_store_si128(a + 1, _mm_xor_si128(a[1], a[7]));
    _mm_store_si128(a + 2, _mm_xor_si128(a[2], a[7]));
    _mm_store_si128(a + 3, _mm_xor_si128(a[3], a[7]));
}

uint8_t multiply_A_by_B_classic(uint8_t a, uint8_t b)
{
    uint8_t sum = 0;
    while (a && b)
    {
        if (b & 1)
            sum ^= a;
        a = multiply_by_X_classic(a);
        b = b >> 1;
    }
    return sum;
}

__m128i multiply_A_by_B_vector(__m128i a, uint8_t b)
{
    __m128i sum = _mm_setzero_si128();
    while (b > 0)
    {
        if (b & 0x1)
        {
            sum = _mm_xor_si128(sum, a);
        }
        a = multiply_by_X_vector(a);
        b = b >> 1;
    }
    return sum;
}

void multiply_A_by_B_RAIDIX(__m128i* a, uint8_t b, __m128i* sum)
{
    while (b > 0)
    {
        if (b & 0x1)
        {
            for (int j = 0; j < 8; j++)
            {
                _mm_store_si128(sum + j, _mm_xor_si128(sum[j], a[j]));
            }
        }
        multiply_by_X_RAIDIX(a);
        b = b >> 1;
    }
}


uint64_t diff_ns(struct timespec start, struct timespec end)
{
    struct timespec diff;

    //if number of nanoseconds in end less then in start we should take 1 sec
    if ((end.tv_nsec - start.tv_nsec) < 0)
    {
        diff.tv_sec = end.tv_sec - start.tv_sec - 1;
        diff.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
    } else
    {
        diff.tv_sec = end.tv_sec - start.tv_sec;
        diff.tv_nsec = end.tv_nsec - start.tv_nsec;
    }

    return diff.tv_sec * 1000000000 + diff.tv_nsec;
}

void print_stripe(uint8_t* const stripe, unsigned int number_of_disks)
{
    for (unsigned int i = 0; i < (number_of_disks + 2) * size_of_strip; i++)
    {
        if (!(i % size_of_strip))
            printf(" ");
        printf("%.2X", *(stripe + i));
    }
    printf("\n");
}
