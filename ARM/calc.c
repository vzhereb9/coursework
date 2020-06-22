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
        calc_one_stripe_vector(raid[i], number_of_strips);
    }

    clock_gettime(CLOCK_MONOTONIC_RAW, &time2);
    time_ns = diff_ns(time1, time2);
    return time_ns;
}

uint64_t calc_RAIDIX(uint8_t** raid, unsigned int number_of_strips, unsigned int number_of_stripes)
{
    struct timespec time1, time2;
    uint64_t time_ns = 0;
    clock_gettime(CLOCK_MONOTONIC_RAW, &time1);

    for (unsigned int i = 0; i < number_of_stripes; i++)
    {
        calc_one_stripe_RAIDIX(raid[i], number_of_strips);
    }

    clock_gettime(CLOCK_MONOTONIC_RAW, &time2);
    time_ns = diff_ns(time1, time2);
    return time_ns;
}


void calc_one_stripe_classic_one_drive(uint8_t* const stripe, unsigned int number_of_strips)
{
    uint8_t* p_p = stripe + number_of_strips * size_of_strip;

    // zeroing of P
    for (unsigned int i = 0; i < size_of_strip; i++)
    {
        p_p[i] ^= p_p[i];
    }
    // computation of P
    for (unsigned int i = 0; i < number_of_strips * size_of_strip; i++)
    {
        p_p[i % size_of_strip] ^= stripe[i];
    }

    p_p = NULL;
}


void calc_one_stripe_vector_and_RAIDIX_one_drive(uint8_t* const stripe, unsigned int number_of_strips)
{
    uint8_t* p_p = stripe + number_of_strips * size_of_strip;

    // zeroing of P
    for (unsigned int i = 0; i < size_of_strip; i += 16)
    {
        vst1q_u8(p_p + i, vmovq_n_u8(0));
    }
    // computation of P
    for (unsigned int i = 0; i < number_of_strips * size_of_strip; i += 16)
    {
        vst1q_u8(p_p + (i % size_of_strip), veorq_u8(vld1q_u8(p_p + (i % size_of_strip)), vld1q_u8(stripe + i)));
    }

    p_p = NULL;
}


void calc_one_stripe_classic(uint8_t* const stripe, unsigned int number_of_strips)
{
    uint8_t* p_p = stripe + number_of_strips * size_of_strip;
    uint8_t* p_q = stripe + (number_of_strips + 1) * size_of_strip;

    // zeroing of P
    for (unsigned int i = 0; i < size_of_strip; i++)
    {
        p_p[i] ^= p_p[i];
    }
    // computation of P
    for (unsigned int i = 0; i < number_of_strips * size_of_strip; i++)
    {
        p_p[i % size_of_strip] ^= stripe[i];
    }

    for (unsigned int i = 0; i < size_of_strip; i++)
    {
        p_q[i] = stripe[i];
    }
    // computation of Q
    for (unsigned int i = 0; i < (number_of_strips - 1) * size_of_strip; i++)
    {
        p_q[i % size_of_strip] = multiply_by_X_classic(p_q[i % size_of_strip]);
        p_q[i % size_of_strip] ^= stripe[size_of_strip + i];

    }

    p_q = NULL;
    p_p = NULL;
}


void calc_one_stripe_vector(uint8_t* const stripe, unsigned int number_of_strips)
{
    uint8_t* p_p = stripe + number_of_strips * size_of_strip;
    uint8_t* p_q = stripe + (number_of_strips + 1) * size_of_strip;

    // zeroing of P
    for (unsigned int i = 0; i < size_of_strip; i += 16)
    {
        vst1q_u8(p_p + i, vmovq_n_u8(0));
    }
    // computation of P
    for (unsigned int i = 0; i < number_of_strips * size_of_strip; i += 16)
    {
        vst1q_u8(p_p + (i % size_of_strip), veorq_u8(vld1q_u8(p_p + (i % size_of_strip)), vld1q_u8(stripe + i)));
    }

    for (unsigned int i = 0; i < size_of_strip; i += 16)
    {
        vst1q_u8(p_q + i, vld1q_u8(stripe + i));
    }
    // computation of Q
    for (unsigned int i = 0; i < (number_of_strips - 1) * size_of_strip; i += 16)
    {
        vst1q_u8(p_q + (i % size_of_strip), multiply_by_X_vector(vld1q_u8(p_q + (i % size_of_strip))));
        vst1q_u8(p_q + (i % size_of_strip),
                 veorq_u8(vld1q_u8(p_q + (i % size_of_strip)), vld1q_u8(stripe + size_of_strip + i)));
    }

    p_q = NULL;
    p_p = NULL;
}

void calc_one_stripe_RAIDIX(uint8_t* const stripe, unsigned int number_of_strips)
{
    uint8_t* p_p = stripe + number_of_strips * size_of_strip;
    uint8_t* p_q = stripe + (number_of_strips + 1) * size_of_strip;

    // zeroing of P
    for (unsigned int i = 0; i < size_of_strip; i += 16)
    {
        vst1q_u8(p_p + i, vmovq_n_u8(0));
    }
    // computation of P
    for (unsigned int i = 0; i < number_of_strips * size_of_strip; i += 16)
    {
        vst1q_u8(p_p + (i % size_of_strip), veorq_u8(vld1q_u8(p_p + (i % size_of_strip)), vld1q_u8(stripe + i)));
    }

    for (unsigned int i = 0; i < size_of_strip; i += 16)
    {
        vst1q_u8(p_q + i, vld1q_u8(stripe + i));
    }
    // computation of Q
    for (int k = 0; k < size_of_strip / 128; k++)
    {
        for (int i = 1; i < number_of_strips; i++)
        {
            multiply_by_X_RAIDIX(p_q + (k * 8) * 16);
            for (int j = 0; j < 8; j++)
            {
                vst1q_u8(p_q + (j + k * 8) * 16, veorq_u8(vld1q_u8(p_q + (j + k * 8) * 16),
                                                          vld1q_u8(stripe + i * size_of_strip + (j + k * 8) * 16)));
            }
        }
    }

    p_q = NULL;
    p_p = NULL;
}
