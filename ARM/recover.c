#include "recover.h"

uint64_t recover_classic(uint8_t** raid, unsigned int number_of_strips, unsigned int number_of_stripes,
                         unsigned int a, unsigned int b, uint8_t* stripe_for_check, uint8_t* new_syndromes)
{
    struct timespec time_recover_one_stripe1, time_recover_one_stripe2;
    uint64_t time_recover = 0;

    for (unsigned int i = 0; i < number_of_stripes; i++)
    {
        for (unsigned int j = 0; j < size_of_strip * (number_of_strips + 2); j++)
        {
            stripe_for_check[j] = raid[i][j];
        }

        for (unsigned int j = 0; j < size_of_strip; j++)
        {
            (raid[i])[a * size_of_strip + j] = 0;
            (raid[i])[b * size_of_strip + j] = 0;
        }

        clock_gettime(CLOCK_MONOTONIC_RAW, &time_recover_one_stripe1);

        recover_one_stripe_classic(raid[i], number_of_strips, a, b, new_syndromes);

        clock_gettime(CLOCK_MONOTONIC_RAW, &time_recover_one_stripe2);
        time_recover += diff_ns(time_recover_one_stripe1, time_recover_one_stripe2);

        // syndromes recovery
        calc_one_stripe_classic(raid[i], number_of_strips);

        // checking the correct operation of the program (comparison of strips before and after calling the function)
        for (unsigned int j = 0; j < size_of_strip * (number_of_strips + 2); j++)
        {
            if (stripe_for_check[j] != raid[i][j])
            {
                printf("Recovery function does not work properly\n");
            }
        }
    }

    return time_recover;
}

uint64_t recover_vector(uint8_t** raid, unsigned int number_of_strips, unsigned int number_of_stripes,
                        unsigned int a, unsigned int b, uint8_t* stripe_for_check, uint8_t* new_syndromes)
{
    struct timespec time_recover_one_stripe1, time_recover_one_stripe2;
    uint64_t time_recover = 0;

    for (unsigned int i = 0; i < number_of_stripes; i++)
    {
        for (unsigned int j = 0; j < size_of_strip * (number_of_strips + 2); j++)
        {
            stripe_for_check[j] = raid[i][j];
        }

        for (unsigned int j = 0; j < size_of_strip; j++)
        {
            (raid[i])[a * size_of_strip + j] = 0;
            (raid[i])[b * size_of_strip + j] = 0;
        }

        clock_gettime(CLOCK_MONOTONIC_RAW, &time_recover_one_stripe1);

        recover_one_stripe_vector(raid[i], number_of_strips, a, b, new_syndromes);

        clock_gettime(CLOCK_MONOTONIC_RAW, &time_recover_one_stripe2);
        time_recover += diff_ns(time_recover_one_stripe1, time_recover_one_stripe2);

        // syndromes recovery
        calc_one_stripe_vector(raid[i], number_of_strips);

        // checking the correct operation of the program (comparison of strips before and after calling the function)
        for (unsigned int j = 0; j < size_of_strip * (number_of_strips + 2); j++)
        {
            if (stripe_for_check[j] != raid[i][j])
            {
                printf("Recovery function does not work properly\n");
                break;
            }
        }
    }

    return time_recover;
}

uint64_t recover_RAIDIX(uint8_t** raid, unsigned int number_of_strips, unsigned int number_of_stripes,
                        unsigned int a, unsigned int b, uint8_t* stripe_for_check, uint8_t* new_syndromes,
                        uint8_t* numerator)
{
    struct timespec time_recover_one_stripe1, time_recover_one_stripe2;
    uint64_t time_recover = 0;

    for (unsigned int i = 0; i < number_of_stripes; i++)
    {
        for (unsigned int j = 0; j < size_of_strip * (number_of_strips + 2); j++)
        {
            stripe_for_check[j] = raid[i][j];
        }

        for (unsigned int j = 0; j < size_of_strip; j++)
        {
            (raid[i])[a * size_of_strip + j] = 0;
            (raid[i])[b * size_of_strip + j] = 0;
        }

        clock_gettime(CLOCK_MONOTONIC_RAW, &time_recover_one_stripe1);

        recover_one_stripe_RAIDIX(raid[i], number_of_strips, a, b, new_syndromes, numerator);

        clock_gettime(CLOCK_MONOTONIC_RAW, &time_recover_one_stripe2);
        time_recover += diff_ns(time_recover_one_stripe1, time_recover_one_stripe2);

        // syndromes recovery
        calc_one_stripe_RAIDIX(raid[i], number_of_strips);

        // checking the correct operation of the program (comparison of strips before and after calling the function)
        for (unsigned int j = 0; j < size_of_strip * (number_of_strips + 2); j++)
        {
            if (stripe_for_check[j] != raid[i][j])
            {
                printf("Recovery function does not work properly\n");
                break;
            }
        }
    }

    return time_recover;
}


uint64_t recover_classic_one_drive(uint8_t** raid, unsigned int number_of_strips, unsigned int number_of_stripes,
                                   unsigned int a, uint8_t* stripe_for_check, uint8_t* new_syndromes)
{
    struct timespec time_recover_one_stripe1, time_recover_one_stripe2;
    uint64_t time_recover = 0;

    for (unsigned int i = 0; i < number_of_stripes; i++)
    {
        for (unsigned int j = 0; j < size_of_strip * (number_of_strips + 2); j++)
        {
            stripe_for_check[j] = raid[i][j];
        }

        for (unsigned int j = 0; j < size_of_strip; j++)
        {
            (raid[i])[a * size_of_strip + j] = 0;
        }

        clock_gettime(CLOCK_MONOTONIC_RAW, &time_recover_one_stripe1);

        recover_one_stripe_classic_one_drive(raid[i], number_of_strips, a, new_syndromes);

        clock_gettime(CLOCK_MONOTONIC_RAW, &time_recover_one_stripe2);
        time_recover += diff_ns(time_recover_one_stripe1, time_recover_one_stripe2);

        // syndromes recovery
        calc_one_stripe_classic(raid[i], number_of_strips);

        // checking the correct operation of the program (comparison of strips before and after calling the function)
        for (unsigned int j = 0; j < size_of_strip * (number_of_strips + 2); j++)
        {
            if (stripe_for_check[j] != raid[i][j])
            {
                printf("Recovery function does not work properly\n");
            }
        }
    }

    return time_recover;
}

uint64_t recover_vector_one_drive(uint8_t** raid, unsigned int number_of_strips, unsigned int number_of_stripes,
                                  unsigned int a, uint8_t* stripe_for_check, uint8_t* new_syndromes)
{
    struct timespec time_recover_one_stripe1, time_recover_one_stripe2;
    uint64_t time_recover = 0;

    for (unsigned int i = 0; i < number_of_stripes; i++)
    {
        for (unsigned int j = 0; j < size_of_strip * (number_of_strips + 2); j++)
        {
            stripe_for_check[j] = raid[i][j];
        }

        for (unsigned int j = 0; j < size_of_strip; j++)
        {
            (raid[i])[a * size_of_strip + j] = 0;
        }

        clock_gettime(CLOCK_MONOTONIC_RAW, &time_recover_one_stripe1);

        recover_one_stripe_vector_and_RAIDIX_one_drive(raid[i], number_of_strips, a, new_syndromes);

        clock_gettime(CLOCK_MONOTONIC_RAW, &time_recover_one_stripe2);
        time_recover += diff_ns(time_recover_one_stripe1, time_recover_one_stripe2);

        // syndromes recovery
        calc_one_stripe_vector(raid[i], number_of_strips);

        // checking the correct operation of the program (comparison of strips before and after calling the function)
        for (unsigned int j = 0; j < size_of_strip * (number_of_strips + 2); j++)
        {
            if (stripe_for_check[j] != raid[i][j])
            {
                printf("Recovery function does not work properly\n");
            }
        }
    }

    return time_recover;
}

uint64_t recover_RAIDIX_one_drive(uint8_t** raid, unsigned int number_of_strips, unsigned int number_of_stripes,
                                  unsigned int a, uint8_t* stripe_for_check, uint8_t* new_syndromes)
{
    struct timespec time_recover_one_stripe1, time_recover_one_stripe2;
    uint64_t time_recover = 0;

    for (unsigned int i = 0; i < number_of_stripes; i++)
    {
        for (unsigned int j = 0; j < size_of_strip * (number_of_strips + 2); j++)
        {
            stripe_for_check[j] = raid[i][j];
        }

        for (unsigned int j = 0; j < size_of_strip; j++)
        {
            (raid[i])[a * size_of_strip + j] = 0;
        }

        clock_gettime(CLOCK_MONOTONIC_RAW, &time_recover_one_stripe1);

        recover_one_stripe_vector_and_RAIDIX_one_drive(raid[i], number_of_strips, a, new_syndromes);

        clock_gettime(CLOCK_MONOTONIC_RAW, &time_recover_one_stripe2);
        time_recover += diff_ns(time_recover_one_stripe1, time_recover_one_stripe2);

        // syndromes recovery
        calc_one_stripe_RAIDIX(raid[i], number_of_strips);

        // checking the correct operation of the program (comparison of strips before and after calling the function)
        for (unsigned int j = 0; j < size_of_strip * (number_of_strips + 2); j++)
        {
            if (stripe_for_check[j] != raid[i][j])
            {
                printf("Recovery function does not work properly\n");
            }
        }
    }

    return time_recover;
}


void recover_one_stripe_classic_one_drive(uint8_t* const stripe, unsigned int number_of_strips, unsigned int a,
                                          uint8_t* const new_syndromes)
{
    uint8_t* p_da = stripe + a * size_of_strip;
    uint8_t* p_p = stripe + number_of_strips * size_of_strip;

    // zeroing of Da and storing old P in new_syndromes
    for (int i = 0; i < size_of_strip; i++)
    {
        p_da[i] ^= p_da[i];
        new_syndromes[i] = p_p[i];
    }

    calc_one_stripe_classic_one_drive(stripe, number_of_strips);

    // computation of Da
    for (int i = 0; i < size_of_strip; i++)
    {
        p_da[i] = p_p[i] ^ new_syndromes[i]; // Da = P'
    }

    p_da = NULL;
    p_p = NULL;
}

void
recover_one_stripe_vector_and_RAIDIX_one_drive(uint8_t* const stripe, unsigned int number_of_strips, unsigned int a,
                                               uint8_t* const new_syndromes)
{
    uint8_t* p_da = stripe + a * size_of_strip;
    uint8_t* p_p = stripe + number_of_strips * size_of_strip;

    // zeroing of Da and storing old P in new_syndromes
    for (int i = 0; i < size_of_strip; i += 16)
    {
        vst1q_u8(p_da + i, vmovq_n_u8(0));
        vst1q_u8(new_syndromes + i, vld1q_u8(p_p + i));
    }

    calc_one_stripe_vector_and_RAIDIX_one_drive(stripe, number_of_strips);

    // computation of Da
    for (int i = 0; i < size_of_strip; i += 16)
    {
        vst1q_u8(p_da + i, veorq_u8(vld1q_u8(p_p + i), vld1q_u8(new_syndromes + i))); // Da = P'
    }

    p_da = NULL;
    p_p = NULL;
}


void recover_one_stripe_classic(uint8_t* const stripe, unsigned int number_of_strips, unsigned int a, unsigned int b,
                                uint8_t* const new_syndromes)
{
    uint8_t* p_da = stripe + a * size_of_strip;
    uint8_t* p_db = stripe + b * size_of_strip;
    uint8_t* p_p = stripe + number_of_strips * size_of_strip;
    uint8_t* p_q = stripe + (number_of_strips + 1) * size_of_strip;

    // zeroing of Da and Db and storing old P and Q in new_syndromes
    for (int i = 0; i < size_of_strip; i++)
    {
        p_da[i] ^= p_da[i];
        p_db[i] ^= p_db[i];
        new_syndromes[i] = p_p[i];
        new_syndromes[size_of_strip + i] = p_q[i];
    }

    calc_one_stripe_classic(stripe, number_of_strips);

    // computation of P' и Q'
    for (int i = 0; i < size_of_strip * 2; i++)
    {
        // Q is directly behind P
        p_p[i] ^= new_syndromes[i];
    }

    uint8_t numerator;
    // computation of Db
    for (int i = 0; i < size_of_strip; i++)
    {
        // Q' * x^(a-N+1)
        numerator = multiply_A_by_B_classic(p_q[i], second_table[number_of_strips - a - 1]);
        // P' - (Q' * x^(a-N-1))
        numerator ^= p_p[i];
        // Db = (P' - (Q' * x^(a-N-1))) * 1/(x^(a-b))
        p_db[i] = multiply_A_by_B_classic(numerator, first_table[b - a]);
    }

    // computation of Da
    for (int i = 0; i < size_of_strip; i++)
    {
        p_da[i] = p_p[i] ^ p_db[i]; // Da = P' - Db
    }

    p_da = NULL;
    p_db = NULL;
    p_p = NULL;
    p_q = NULL;
}

void recover_one_stripe_vector(uint8_t* const stripe, unsigned int number_of_strips, unsigned int a, unsigned int b,
                               uint8_t* const new_syndromes)
{
    uint8_t* p_da = stripe + a * size_of_strip;
    uint8_t* p_db = stripe + b * size_of_strip;
    uint8_t* p_p = stripe + number_of_strips * size_of_strip;
    uint8_t* p_q = stripe + (number_of_strips + 1) * size_of_strip;

    // zeroing of Da and Db and storing old P and Q in new_syndromes
    for (int i = 0; i < size_of_strip; i += 16)
    {
        vst1q_u8(p_da + i, vmovq_n_u8(0));
        vst1q_u8(p_db + i, vmovq_n_u8(0));
        vst1q_u8(new_syndromes + i, vld1q_u8(p_p + i));
        vst1q_u8(new_syndromes + size_of_strip + i, vld1q_u8(p_q + i));
    }

    calc_one_stripe_vector(stripe, number_of_strips);

    // computation of P' и Q'
    for (int i = 0; i < size_of_strip * 2; i += 16)
    {
        // Q is directly behind P
        vst1q_u8(p_p + i, veorq_u8(vld1q_u8(p_p + i), vld1q_u8(new_syndromes + i)));
    }

    uint8x16_t numerator;
    // computation of Db
    for (int i = 0; i < size_of_strip; i += 16)
    {
        // Q' * x^(a-N+1)
        numerator = multiply_A_by_B_vector(vld1q_u8(p_q + i), second_table[number_of_strips - a - 1]);
        // P' - (Q' * x^(a-N-1))
        numerator = veorq_u8(numerator, vld1q_u8(p_p + i));
        // Db = (P' - (Q' * x^(a-N-1))) * 1/(x^(a-b))
        vst1q_u8(p_db + i, multiply_A_by_B_vector(numerator, first_table[b - a]));
    }

    // computation of Da
    for (int i = 0; i < size_of_strip; i += 16)
    {
        vst1q_u8(p_da + i, veorq_u8(vld1q_u8(p_p + i), vld1q_u8(p_db + i))); // Da = P' - Db
    }

    p_da = NULL;
    p_db = NULL;
    p_p = NULL;
    p_q = NULL;
}


void recover_one_stripe_RAIDIX(uint8_t* const stripe, unsigned int number_of_strips, unsigned int a, unsigned int b,
                               uint8_t* const new_syndromes, uint8_t* const numerator)
{
    uint8_t* p_da = stripe + a * size_of_strip;
    uint8_t* p_db = stripe + b * size_of_strip;
    uint8_t* p_p = stripe + number_of_strips * size_of_strip;
    uint8_t* p_q = stripe + (number_of_strips + 1) * size_of_strip;

    // zeroing of Da and Db and storing old P and Q in new_syndromes
    for (int i = 0; i < size_of_strip; i += 16)
    {
        vst1q_u8(p_da + i, vmovq_n_u8(0));
        vst1q_u8(p_db + i, vmovq_n_u8(0));
        vst1q_u8(new_syndromes + i, vld1q_u8(p_p + i));
        vst1q_u8(new_syndromes + size_of_strip + i, vld1q_u8(p_q + i));
    }

    calc_one_stripe_RAIDIX(stripe, number_of_strips);

    // computation of P' и Q'
    for (int i = 0; i < size_of_strip * 2; i += 16)
    {
        // Q is directly behind P
        vst1q_u8(p_p + i, veorq_u8(vld1q_u8(p_p + i), vld1q_u8(new_syndromes + i)));
    }

    // computation of Db
    for (int i = 0; i < size_of_strip / 128; i++)
    {
        for (int j = 0; j < 8; j++)
            vst1q_u8(numerator + j * 16, vmovq_n_u8(0));
        // Q' * x^(a-N+1)
        multiply_A_by_B_RAIDIX(p_q + (i * 8) * 16, second_table[number_of_strips - a - 1], numerator);
        // P' - (Q' * x^(a-N-1))
        for (int j = 0; j < 8; j++)
            vst1q_u8(numerator + j * 16, veorq_u8(vld1q_u8(numerator + j * 16), vld1q_u8(p_p + (j + (i * 8)) * 16)));
        // Db = (P' - (Q' * x^(a-N-1))) * 1/(x^(a-b))
        multiply_A_by_B_RAIDIX(numerator, first_table[b - a], p_db + (i * 8) * 16);
    }

    // computation of Da
    for (int i = 0; i < size_of_strip; i += 16)
    {
        vst1q_u8(p_da + i, veorq_u8(vld1q_u8(p_p + i), vld1q_u8(p_db + i))); // Da = P' - Db
    }

    p_da = NULL;
    p_db = NULL;
    p_p = NULL;
    p_q = NULL;
}
