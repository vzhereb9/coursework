#include "recover.h"

uint64_t recover_classic(uint8_t** raid, unsigned int number_of_strips, unsigned int number_of_stripes,
                         unsigned int a, unsigned int b)
{
    struct timespec time_recover_one_stripe1, time_recover_one_stripe2;
    uint64_t time_recover = 0;


    uint8_t* stripe_for_check = NULL;
    stripe_for_check = (uint8_t*) (uint8_t*) memalign(32,size_of_strip * (number_of_strips + 2) * sizeof(uint8_t));

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
    stripe_for_check = (uint8_t*) memalign(32, size_of_strip * (number_of_strips + 2) * sizeof(uint8_t));

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
        recover_one_stripe_vector((__m256i*) raid[i], number_of_strips, a, b);
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
    stripe_for_check = (uint8_t*) (uint8_t*) memalign(32,size_of_strip * (number_of_strips + 2) * sizeof(uint8_t));

    /*uint8_t** raid_new = NULL;
    raid_new = (uint8_t**) memalign(32, (number_of_stripes) * sizeof(uint8_t*)); //доп 2 дисковых массива для P Q
    for (int i = 0; i < number_of_stripes; i++)
    {
        // выделение места для каждого страйпа
        raid_new[i] = (uint8_t*) memalign(32,
                                          size_of_strip * (number_of_strips + 2) * sizeof(uint8_t)); //sizeof(uint8_t) = 1
    }

    uint8_t bit;*/
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

        /*//clock_gettime(CLOCK_MONOTONIC_RAW, &time_recover_one_stripe1);
        // Изменяем расположение битов
        for (int i = 0; i < size_of_strip * (number_of_strips + 2); i++)
        {
            for (int j = 7; j >= 0; j--)
            {
                bit = ((raid[k])[i] >> j) & 0x1;
                (raid_new[k])[(7 - j) * 32 + (i % 256) / 8 + (i / 256) * 256] ^= bit << (7 - (i % 8));
                //(raid_new[k])[(7 - j) * 32 + (i / 8)] ^= bit << (7 - (i % 8));
            }
        }*/
        clock_gettime(CLOCK_MONOTONIC_RAW, &time_recover_one_stripe1);
        recover_one_stripe_RAIDIX((__m256i*) raid_new[k], number_of_strips, a, b);
        clock_gettime(CLOCK_MONOTONIC_RAW, &time_recover_one_stripe2);
        time_recover += diff_ns(time_recover_one_stripe1, time_recover_one_stripe2);
        /*// Изменение расположения битов на изначальное
        for (int i = 0; i < size_of_strip * (number_of_strips + 2); i++)
        {
            (raid[k])[i] = 0x0;
        }

        for (int i = 0; i < size_of_strip * (number_of_strips + 2); i++)
        {
            for (int j = 7; j >= 0; j--)
            {
                bit = ((raid_new[k])[i] >> j) & 0x1;
                (raid[k])[(7 - j) + (i % 32) * 8 + (i / 256) * 256] ^= bit << (7 - ((i % 256) / 32));
                //(raid[k])[(7 - j) + (i % 32) * 8] ^= bit << (7 - (i / 32));
            }
        }

        //clock_gettime(CLOCK_MONOTONIC_RAW, &time_recover_one_stripe2);
        //time_recover += diff_ns(time_recover_one_stripe1, time_recover_one_stripe2);*/
        // Проверка правильности работы программы (сравнение страйпов до и после вызова функции)
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
    /*for (int i = 0; i < number_of_stripes; i++)
    {
        free(raid_new[i]);
    }
    free(raid_new);*/

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

void recover_one_stripe_vector(__m256i* const stripe, unsigned int number_of_strips, unsigned int a, unsigned int b)
{
    __m256i* new_syndromes;
    // new_syndromes для хранения старых значений P Q
    new_syndromes = (__m256i*) memalign(32, size_of_strip_for_m256i * 2 * sizeof(__m256i));
    __m256i* p_da = stripe + a * size_of_strip_for_m256i;
    __m256i* p_db = stripe + b * size_of_strip_for_m256i;
    __m256i* p_p = stripe + number_of_strips * size_of_strip_for_m256i;
    __m256i* p_q = stripe + (number_of_strips + 1) * size_of_strip_for_m256i;

    // Обнуление Da и Db и занесение в new_syndromes старых значений P и Q
    for (int i = 0; i < size_of_strip_for_m256i; i++)
    {
        p_da[i] = _mm256_setzero_si256();
        p_db[i] = _mm256_setzero_si256();
        _mm256_store_si256((new_syndromes + i), p_p[i]);
        _mm256_store_si256((new_syndromes + size_of_strip_for_m256i + i), p_q[i]);
    }

    calc_one_stripe_vector(stripe, number_of_strips);

    // Вычисление значений P' и Q', хранятся на месте синдромов
    for (int i = 0; i < size_of_strip_for_m256i * 2; i++)
    {
        // Q находится непосредственно за P
        _mm256_store_si256((p_p + i), _mm256_xor_si256(p_p[i], new_syndromes[i]));
    }

    __m256i numerator;
    // Вычисление Db
    for (int i = 0; i < size_of_strip_for_m256i; i++)
    {
        // Q' * x^(a-N+1)
        numerator = multiply_A_by_B_vector(p_q[i], second_table[number_of_strips - a - 1]);
        // P' - (Q' * x^(a-N-1))
        numerator = _mm256_xor_si256(numerator, p_p[i]);
        // Db = (P' - (Q' * x^(a-N-1))) * 1/(x^(a-b))
        _mm256_store_si256((p_db + i), multiply_A_by_B_vector(numerator, first_table[b - a]));
    }

    // Вычисление Da
    for (int i = 0; i < size_of_strip_for_m256i; i++)
    {
        p_da[i] = _mm256_xor_si256(p_p[i], p_db[i]); // Da = P' - Db
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

void recover_one_stripe_RAIDIX(__m256i* const stripe_new, unsigned int number_of_strips, unsigned int a, unsigned int b)
{
    __m256i* new_syndromes;
    // new_syndromes для хранения старых значений P Q
    new_syndromes = (__m256i*) memalign(32, size_of_strip_for_m256i * 2 * sizeof(__m256i));
    __m256i* p_da = stripe_new + a * size_of_strip_for_m256i;
    __m256i* p_db = stripe_new + b * size_of_strip_for_m256i;
    __m256i* p_p = stripe_new + number_of_strips * size_of_strip_for_m256i;
    __m256i* p_q = stripe_new + (number_of_strips + 1) * size_of_strip_for_m256i;

    // Обнуление Da и Db и занесение в new_syndromes старых значений P и Q
    for (int i = 0; i < size_of_strip_for_m256i; i++)
    {
        p_da[i] = _mm256_setzero_si256();
        p_db[i] = _mm256_setzero_si256();
        _mm256_store_si256((new_syndromes + i), p_p[i]);
        _mm256_store_si256((new_syndromes + size_of_strip_for_m256i + i), p_q[i]);
    }

    calc_one_stripe_RAIDIX(stripe_new, number_of_strips);

    // Вычисление значений P' и Q', хранятся на месте синдромов
    for (int i = 0; i < size_of_strip_for_m256i * 2; i++)
    {
        // Q находится непосредственно за P
        _mm256_store_si256((p_p + i), _mm256_xor_si256(p_p[i], new_syndromes[i]));
    }

    __m256i* numerator;
    numerator = (__m256i*) memalign(32, 8 * sizeof(__m256i));
    // Вычисление Db
    for (int i = 0; i < size_of_strip / 256; i++)
    {
        for (int j = 0; j < 8; j++)
            numerator[j] = _mm256_setzero_si256();
        // Q' * x^(a-N+1)
        multiply_A_by_B_RAIDIX(p_q + (i * 8), second_table[number_of_strips - a - 1], numerator);
        //P' - (Q' * x^(a-N-1))
        for (int j = 0; j < 8; j++)
            numerator[j] = _mm256_xor_si256(numerator[j], p_p[j + (i * 8)]);
        // Db = (P' - (Q' * x^(a-N-1))) * 1/(x^(a-b))
        multiply_A_by_B_RAIDIX(numerator, first_table[b - a], p_db + (i * 8));
    }

    // Вычисление Da
    for (int i = 0; i < size_of_strip_for_m256i; i++)
    {
        p_da[i] = _mm256_xor_si256(p_p[i], p_db[i]); // Da = P' - Db
    }

    // Восстановление синдромов
    calc_one_stripe_RAIDIX(stripe_new, number_of_strips);
    p_da = NULL;
    p_db = NULL;
    p_p = NULL;
    p_q = NULL;
    free(new_syndromes);
    new_syndromes = NULL;
    free(numerator);
    numerator = NULL;

}