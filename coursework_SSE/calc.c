#include "calc.h"

uint64_t calc_classic(uint8_t** raid, unsigned int number_of_strips, unsigned int number_of_stripes)
{
    struct timespec time1, time2;
    uint64_t time_ns = 0;
    clock_gettime(CLOCK_MONOTONIC_RAW, &time1);

    for (unsigned int i = 0; i < number_of_stripes; i++)
    {
        calc_one_stripe_classic(raid[i], number_of_strips);
    }

    clock_gettime(CLOCK_MONOTONIC_RAW, &time2);
    time_ns = diff_ns(time1, time2);
    return time_ns;
}

uint64_t calc_vector(uint8_t** raid, unsigned int number_of_strips, unsigned int number_of_stripes)
{
    struct timespec time1, time2;
    uint64_t time_ns = 0;
    clock_gettime(CLOCK_MONOTONIC_RAW, &time1);

    for (unsigned int i = 0; i < number_of_stripes; i++)
    {
        calc_one_stripe_vector((__m128i*) (raid[i]), number_of_strips);
    }

    clock_gettime(CLOCK_MONOTONIC_RAW, &time2);
    time_ns = diff_ns(time1, time2);
    return time_ns;
}

uint64_t calc_RAIDIX(uint8_t** raid_new, unsigned int number_of_strips, unsigned int number_of_stripes)
{
    struct timespec time1, time2;
    uint64_t time_ns = 0;
    clock_gettime(CLOCK_MONOTONIC_RAW, &time1);

    for (unsigned int i = 0; i < number_of_stripes; i++)
    {
        calc_one_stripe_RAIDIX((__m128i*) (raid_new[i]), number_of_strips);
    }

    clock_gettime(CLOCK_MONOTONIC_RAW, &time2);
    time_ns = diff_ns(time1, time2);
    return time_ns;
}

void calc_one_stripe_classic(uint8_t* const stripe, unsigned int number_of_strips)
{
    uint8_t* p_p = stripe + number_of_strips * size_of_strip;
    uint8_t* p_q = stripe + (number_of_strips + 1) * size_of_strip;

    // Обнуление P
    for (unsigned int i = 0; i < size_of_strip; i++)
    {
        p_p[i] ^= p_p[i];
    }
    // Вычисление P
    for (unsigned int i = 0; i < number_of_strips * size_of_strip; i++)
    {
        p_p[i % size_of_strip] ^= stripe[i];
    }

    // Q присваивается значение первого блока D0
    for (unsigned int i = 0; i < size_of_strip; i++)
    {
        p_q[i] = stripe[i];
    }
    // Вычисление Q
    for (unsigned int i = 0; i < (number_of_strips - 1) * size_of_strip; i++)
    {
        // Умножение текущей ячейки Q на x
        p_q[i % size_of_strip] = multiply_by_X_classic(p_q[i % size_of_strip]);
        // Прибаление к текущей ячейке Q значение соотвествующей ячейки блока D(i+1)
        p_q[i % size_of_strip] ^= stripe[size_of_strip + i];
    }

    p_q = NULL;
    p_p = NULL;
}

void calc_one_stripe_vector(__m128i* const stripe, unsigned int number_of_strips)
{
    __m128i* p_p = stripe + number_of_strips * size_of_strip_for_m128i;
    __m128i* p_q = stripe + (number_of_strips + 1) * size_of_strip_for_m128i;

    // Обнуление P
    for (unsigned int i = 0; i < size_of_strip_for_m128i; i++)
    {
        _mm_store_si128(p_p + i, _mm_setzero_si128());
    }
    // Вычисление P
    for (unsigned int i = 0; i < number_of_strips * size_of_strip_for_m128i; i++)
    {
        _mm_store_si128(p_p + i % size_of_strip_for_m128i,
                        _mm_xor_si128(p_p[i % size_of_strip_for_m128i], stripe[i]));
    }

    // Q присваивается значение первого блока D0
    for (unsigned int i = 0; i < size_of_strip_for_m128i; i++)
    {
        _mm_store_si128(p_q + i, stripe[i]);
    }
    // Вычисление Q
    for (unsigned int i = 0; i < (number_of_strips - 1) * size_of_strip_for_m128i; i++)
    {
        // Умножение текущей ячейки Q на x
        _mm_store_si128(p_q + i % size_of_strip_for_m128i,
                           multiply_by_X_vector(p_q[i % size_of_strip_for_m128i]));
        // Прибаление к текущей ячейке Q значение соотвествующей ячейки блока D(i+1)
        _mm_store_si128(p_q + i % size_of_strip_for_m128i,
                        _mm_xor_si128(p_q[i % size_of_strip_for_m128i], stripe[(size_of_strip / 16) + i]));
    }

    p_q = NULL;
    p_p = NULL;
}

void calc_one_stripe_RAIDIX(__m128i* const stripe, unsigned int number_of_strips)
{
    __m128i* p_p = stripe + number_of_strips * size_of_strip_for_m128i;
    __m128i* p_q = stripe + (number_of_strips + 1) * size_of_strip_for_m128i;

    // Обнуление P
    for (unsigned int i = 0; i < size_of_strip_for_m128i; i++)
    {
        _mm_store_si128(p_p + i, _mm_setzero_si128());
    }
    // Вычисление P
    for (unsigned int i = 0; i < number_of_strips * size_of_strip_for_m128i; i++)
    {
        _mm_store_si128(p_p + i % size_of_strip_for_m128i,
                        _mm_xor_si128(p_p[i % size_of_strip_for_m128i], stripe[i]));
    }

    // Q присваивается значение первого блока D0
    for (unsigned int i = 0; i < size_of_strip_for_m128i; i++)
    {
        _mm_store_si128(p_q + i, stripe[i]);
    }
    // Вычисление Q
    for (int k = 0; k < size_of_strip / 128; k++)
    {
        for (int i = 1; i < number_of_strips; i++)
        {
            multiply_by_X_RAIDIX(p_q + (k * 8));
            for (int j = 0; j < 8; j++)
            {
                _mm_store_si128(p_q + j + (k * 8), _mm_xor_si128(p_q[j + (k * 8)],
                                                                 stripe[(i * size_of_strip_for_m128i) + j +
                                                                                  (k * 8)]));
            }
        }
    }

    p_q = NULL;
    p_p = NULL;
}
