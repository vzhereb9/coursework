#include "recover.h"

uint64_t recover_classic(uint8_t** raid, unsigned int number_of_strips, unsigned int number_of_stripes,
                         unsigned int a, unsigned int b)
{
    struct timespec time_recover_one_stripe1, time_recover_one_stripe2;
    uint64_t time_recover = 0;


    uint8_t* stripe_for_check = NULL;
    stripe_for_check = (uint8_t*) memalign(16,size_of_strip * (number_of_strips + 2) * sizeof(uint8_t));

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
        recover_one_stripe_classic(raid[i], number_of_strips, a, b);
        clock_gettime(CLOCK_MONOTONIC_RAW, &time_recover_one_stripe2);
        time_recover += diff_ns(time_recover_one_stripe1, time_recover_one_stripe2);

        // Проверка правильности работы программы (сравнение страйпов до и после вызова функции)
        for (unsigned int j = 0; j < size_of_strip * (number_of_strips + 2); j++)
        {
            if (stripe_for_check[j] != raid[i][j])
            {
                printf("Recovery function does not work properly\n");
            }
        }
    }

    free(stripe_for_check);
    stripe_for_check = NULL;

    return time_recover;
}

uint64_t recover_vector(uint8_t** raid, unsigned int number_of_strips, unsigned int number_of_stripes,
                        unsigned int a, unsigned int b)
{
    struct timespec time_recover_one_stripe1, time_recover_one_stripe2;
    uint64_t time_recover = 0;
    uint8_t* stripe_for_check = NULL;
    stripe_for_check = (uint8_t*) memalign(16, size_of_strip * (number_of_strips + 2) * sizeof(uint8_t));

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
        recover_one_stripe_vector((__m128i*) raid[i], number_of_strips, a, b);
        clock_gettime(CLOCK_MONOTONIC_RAW, &time_recover_one_stripe2);
        time_recover += diff_ns(time_recover_one_stripe1, time_recover_one_stripe2);

        // Проверка правильности работы программы (сравнение страйпов до и после вызова функции)
        for (unsigned int j = 0; j < size_of_strip * (number_of_strips + 2); j++)
        {
            if (stripe_for_check[j] != raid[i][j])
            {
                printf("Recovery function does not work properly\n");
                break;
            }
        }
    }
    free(stripe_for_check);
    stripe_for_check = NULL;

    return time_recover;
}

uint64_t recover_RAIDIX(uint8_t** raid_new, unsigned int number_of_strips, unsigned int number_of_stripes,
                        unsigned int a, unsigned int b)
{
    struct timespec time_recover_one_stripe1, time_recover_one_stripe2;
    uint64_t time_recover = 0;
    uint8_t* stripe_for_check = NULL;
    stripe_for_check = (uint8_t*) (uint8_t*) memalign(16,size_of_strip * (number_of_strips + 2) * sizeof(uint8_t));

    for (unsigned int k = 0; k < number_of_stripes; k++)
    {
        for (unsigned int j = 0; j < size_of_strip * (number_of_strips + 2); j++)
        {
            stripe_for_check[j] = raid_new[k][j];
        }

        for (unsigned int j = 0; j < size_of_strip; j++)
        {
            (raid_new[k])[a * size_of_strip + j] = 0x0;
            (raid_new[k])[b * size_of_strip + j] = 0x0;
        }

        clock_gettime(CLOCK_MONOTONIC_RAW, &time_recover_one_stripe1);
        recover_one_stripe_RAIDIX((__m128i*) raid_new[k], number_of_strips, a, b);
        clock_gettime(CLOCK_MONOTONIC_RAW, &time_recover_one_stripe2);
        time_recover += diff_ns(time_recover_one_stripe1, time_recover_one_stripe2);
        for (unsigned int j = 0; j < size_of_strip * (number_of_strips + 2); j++)
        {
            if (stripe_for_check[j] != raid_new[k][j])
            {
                printf("Recovery function does not work properly\n");
            }
        }
    }

    free(stripe_for_check);
    stripe_for_check = NULL;

    return time_recover;
}

void recover_one_stripe_classic(uint8_t* const stripe, unsigned int number_of_strips, unsigned int a, unsigned int b)
{
    uint8_t* new_syndromes;
    // new_syndromes для хранения старых значений P Q
    new_syndromes = (uint8_t*) malloc(size_of_strip * 2 * sizeof(uint8_t));
    uint8_t* p_da = stripe + a * size_of_strip;
    uint8_t* p_db = stripe + b * size_of_strip;
    uint8_t* p_p = stripe + number_of_strips * size_of_strip;
    uint8_t* p_q = stripe + (number_of_strips + 1) * size_of_strip;

    // Обнуление Da и Db и занесение в new_syndromes старых значений P и Q
    for (int i = 0; i < size_of_strip; i++)
    {
        p_da[i] ^= p_da[i];
        p_db[i] ^= p_db[i];
        new_syndromes[i] = p_p[i];
        new_syndromes[size_of_strip + i] = p_q[i];
    }

    calc_one_stripe_classic(stripe, number_of_strips);

    // Вычисление значений P' и Q', хранятся на месте синдромов
    for (int i = 0; i < size_of_strip * 2; i++)
    {
        // Q находится непосредственно за P
        p_p[i] ^= new_syndromes[i];
    }

    uint8_t numerator;
    // Вычисление Db
    for (int i = 0; i < size_of_strip; i++)
    {
        // Q' * x^(a-N+1)
        numerator = multiply_A_by_B_classic(p_q[i], second_table[number_of_strips - a - 1]);
        // P' - (Q' * x^(a-N-1))
        numerator ^= p_p[i];
        // Db = (P' - (Q' * x^(a-N-1))) * 1/(x^(a-b))
        p_db[i] = multiply_A_by_B_classic(numerator, first_table[b - a]);
    }

    // Вычисление Da
    for (int i = 0; i < size_of_strip; i++)
    {
        p_da[i] = p_p[i] ^ p_db[i]; // Da = P' - Db
    }

    // Восстановление синдромов
    calc_one_stripe_classic(stripe, number_of_strips);

    p_da = NULL;
    p_db = NULL;
    p_p = NULL;
    p_q = NULL;
    free(new_syndromes);
    new_syndromes = NULL;
}

void recover_one_stripe_vector(__m128i* const stripe, unsigned int number_of_strips, unsigned int a, unsigned int b)
{
    __m128i* new_syndromes;
    // new_syndromes для хранения старых значений P Q
    new_syndromes = (__m128i*) memalign(16, size_of_strip_for_m128i * 2 * sizeof(__m128i));
    __m128i* p_da = stripe + a * size_of_strip_for_m128i;
    __m128i* p_db = stripe + b * size_of_strip_for_m128i;
    __m128i* p_p = stripe + number_of_strips * size_of_strip_for_m128i;
    __m128i* p_q = stripe + (number_of_strips + 1) * size_of_strip_for_m128i;

    // Обнуление Da и Db и занесение в new_syndromes старых значений P и Q
    for (int i = 0; i < size_of_strip_for_m128i; i++)
    {
        p_da[i] = _mm_setzero_si128();
        p_db[i] = _mm_setzero_si128();
        _mm_store_si128((new_syndromes + i), p_p[i]);
        _mm_store_si128((new_syndromes + size_of_strip_for_m128i + i), p_q[i]);
    }

    calc_one_stripe_vector(stripe, number_of_strips);

    // Вычисление значений P' и Q', хранятся на месте синдромов
    for (int i = 0; i < size_of_strip_for_m128i * 2; i++)
    {
        // Q находится непосредственно за P
        _mm_store_si128((p_p + i), _mm_xor_si128(p_p[i], new_syndromes[i]));
    }

    __m128i numerator;
    // Вычисление Db
    for (int i = 0; i < size_of_strip_for_m128i; i++)
    {
        // Q' * x^(a-N+1)
        numerator = multiply_A_by_B_vector(p_q[i], second_table[number_of_strips - a - 1]);
        // P' - (Q' * x^(a-N-1))
        numerator = _mm_xor_si128(numerator, p_p[i]);
        // Db = (P' - (Q' * x^(a-N-1))) * 1/(x^(a-b))
        _mm_store_si128((p_db + i), multiply_A_by_B_vector(numerator, first_table[b - a]));
    }

    // Вычисление Da
    for (int i = 0; i < size_of_strip_for_m128i; i++)
    {
        p_da[i] = _mm_xor_si128(p_p[i], p_db[i]); // Da = P' - Db
    }

    // Восстановление синдромов
    calc_one_stripe_vector(stripe, number_of_strips);
    p_da = NULL;
    p_db = NULL;
    p_p = NULL;
    p_q = NULL;
    free(new_syndromes);
    new_syndromes = NULL;
}

void recover_one_stripe_RAIDIX(__m128i* const stripe, unsigned int number_of_strips, unsigned int a, unsigned int b)
{
    __m128i* new_syndromes;
    // new_syndromes для хранения старых значений P Q
    new_syndromes = (__m128i*) memalign(16, size_of_strip_for_m128i * 2 * sizeof(__m128i));
    __m128i* p_da = stripe + a * size_of_strip_for_m128i;
    __m128i* p_db = stripe + b * size_of_strip_for_m128i;
    __m128i* p_p = stripe + number_of_strips * size_of_strip_for_m128i;
    __m128i* p_q = stripe + (number_of_strips + 1) * size_of_strip_for_m128i;

    // Обнуление Da и Db и занесение в new_syndromes старых значений P и Q
    for (int i = 0; i < size_of_strip_for_m128i; i++)
    {
        p_da[i] = _mm_setzero_si128();
        p_db[i] = _mm_setzero_si128();
        _mm_store_si128((new_syndromes + i), p_p[i]);
        _mm_store_si128((new_syndromes + size_of_strip_for_m128i + i), p_q[i]);
    }

    calc_one_stripe_RAIDIX(stripe, number_of_strips);

    // Вычисление значений P' и Q', хранятся на месте синдромов
    for (int i = 0; i < size_of_strip_for_m128i * 2; i++)
    {
        // Q находится непосредственно за P
        _mm_store_si128((p_p + i), _mm_xor_si128(p_p[i], new_syndromes[i]));
    }

    __m128i* numerator;
    numerator = (__m128i*) memalign(16, 8 * sizeof(__m128i));
    // Вычисление Db
    for (int i = 0; i < size_of_strip / 128; i++)
    {
        for (int j = 0; j < 8; j++)
            numerator[j] = _mm_setzero_si128();
        // Q' * x^(a-N+1)
        multiply_A_by_B_RAIDIX(p_q + (i * 8), second_table[number_of_strips - a - 1], numerator);
        //P' - (Q' * x^(a-N-1))
        for (int j = 0; j < 8; j++)
            numerator[j] = _mm_xor_si128(numerator[j], p_p[j + (i * 8)]);
        // Db = (P' - (Q' * x^(a-N-1))) * 1/(x^(a-b))
        multiply_A_by_B_RAIDIX(numerator, first_table[b - a], p_db + (i * 8));
    }

    // Вычисление Da
    for (int i = 0; i < size_of_strip_for_m128i; i++)
    {
        p_da[i] = _mm_xor_si128(p_p[i], p_db[i]); // Da = P' - Db
    }

    // Восстановление синдромов
    calc_one_stripe_RAIDIX(stripe, number_of_strips);
    p_da = NULL;
    p_db = NULL;
    p_p = NULL;
    p_q = NULL;
    free(new_syndromes);
    new_syndromes = NULL;
    free(numerator);
    numerator = NULL;
}