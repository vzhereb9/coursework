#include "recover.h"

clock_t recover_classic(uint8_t** raid, unsigned int number_of_disks, unsigned int number_of_stripes,
                     unsigned int a, unsigned int b)
{
    clock_t time_recover_classic = 0, time_recover_one_stripe;
    uint8_t* stripe_for_check = NULL;
    stripe_for_check = (uint8_t*) malloc(size_of_disk * (number_of_disks + 2) * sizeof(uint8_t));

    for (unsigned int i = 0; i < number_of_stripes; i++)
    {
        for (unsigned int j = 0; j < size_of_disk * (number_of_disks + 2); j++)
        {
            stripe_for_check[j] = raid[i][j];
        }

        for (unsigned int j = 0; j < size_of_disk; j++)
        {
            (raid[i])[a * size_of_disk + j] = rand() % 255;
            (raid[i])[b * size_of_disk + j] = rand() % 255;
        }

        time_recover_one_stripe = clock();
        recover_one_stripe_classic(raid[i], number_of_disks, a, b);
        time_recover_classic += clock() - time_recover_one_stripe;

        // Проверка правильности работы программы (сравнение страйпов до и после вызова функции)
        for (unsigned int j = 0; j < size_of_disk * (number_of_disks + 2); j++)
        {
            if (stripe_for_check[j] != raid[i][j])
            {
                printf("Recovery function does not work properly\n");
            }
        }
    }

    free(stripe_for_check);
    stripe_for_check = NULL;

    return time_recover_classic;
}

clock_t recover_vector(uint8_t** raid, unsigned int number_of_disks, unsigned int number_of_stripes,
                        unsigned int a, unsigned int b)
{
    clock_t time_recover_vector = 0, time_recover_one_stripe;
    uint8_t* stripe_for_check = NULL;
    stripe_for_check = (uint8_t*) malloc(size_of_disk * (number_of_disks + 2) * sizeof(uint8_t));

    for (unsigned int i = 0; i < number_of_stripes; i++)
    {
        for (unsigned int j = 0; j < size_of_disk * (number_of_disks + 2); j++)
        {
            stripe_for_check[j] = raid[i][j];
        }

        for (unsigned int j = 0; j < size_of_disk; j++)
        {
            (raid[i])[a * size_of_disk + j] = rand() % 255;
            (raid[i])[b * size_of_disk + j] = rand() % 255;
        }

        time_recover_one_stripe = clock();
        recover_one_stripe_vector((__m256i*) raid[i], number_of_disks, a, b);
        time_recover_vector += clock() - time_recover_one_stripe;

        // Проверка правильности работы программы (сравнение страйпов до и после вызова функции)
        for (unsigned int j = 0; j < size_of_disk * (number_of_disks + 2); j++)
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

    return time_recover_vector;
}

clock_t recover_RAIDIX(uint8_t** raid, unsigned int number_of_disks, unsigned int number_of_stripes,
                       unsigned int a, unsigned int b)
{
    clock_t time_recover_vector = 0, time_recover_one_stripe;
    uint8_t* stripe_for_check = NULL;
    stripe_for_check = (uint8_t*) malloc(size_of_disk * (number_of_disks + 2) * sizeof(uint8_t));

    for (unsigned int i = 0; i < number_of_stripes; i++)
    {
        for (unsigned int j = 0; j < size_of_disk * (number_of_disks + 2); j++)
        {
            stripe_for_check[j] = raid[i][j];
        }

        for (unsigned int j = 0; j < size_of_disk; j++)
        {
            (raid[i])[a * size_of_disk + j] = rand() % 255;
            (raid[i])[b * size_of_disk + j] = rand() % 255;
        }

        time_recover_one_stripe = clock();
        recover_one_stripe_RAIDIX((__m256i*) raid[i], number_of_disks, a, b);
        time_recover_vector += clock() - time_recover_one_stripe;

        // Проверка правильности работы программы (сравнение страйпов до и после вызова функции)
        for (unsigned int j = 0; j < size_of_disk * (number_of_disks + 2); j++)
        {
            if (stripe_for_check[j] != raid[i][j])
            {
                printf("Recovery function does not work properly\n");
            }
        }
    }
    free(stripe_for_check);
    stripe_for_check = NULL;

    return time_recover_vector;
}

void recover_one_stripe_classic(uint8_t* const stripe, unsigned int number_of_disks, unsigned int a, unsigned int b)
{
    uint8_t* new_syndromes;
    // new_syndromes для хранения старых значений P Q
    new_syndromes = (uint8_t*) malloc(size_of_disk * 2 * sizeof(uint8_t));
    uint8_t* p_da = stripe + a * size_of_disk;
    uint8_t* p_db = stripe + b * size_of_disk;
    uint8_t* p_p = stripe + number_of_disks * size_of_disk;
    uint8_t* p_q = stripe + (number_of_disks + 1) * size_of_disk;

    // Обнуление Da и Db и занесение в new_syndromes старых значений P и Q
    for (int i = 0; i < size_of_disk; i++)
    {
        *(p_da + i) ^= *(p_da + i);
        *(p_db + i) ^= *(p_db + i);
        *(new_syndromes + i) = *(p_p + i);
        *(new_syndromes + size_of_disk + i) = *(p_q + i);
    }

    calc_one_stripe_classic(stripe, number_of_disks);

    // Вычисление значений P' и Q', хранятся на месте синдромов
    for (int i = 0; i < size_of_disk * 2; i++)
    {
        // Q находится непосредственно за P
        *(p_p + i) ^= *(new_syndromes + i);
    }

    uint8_t numerator;
    // Вычисление Db
    for (int i = 0; i < size_of_disk; i++)
    {
        // Q' * x^(a-N+1)
        numerator = multiply_A_by_B_classic(*(p_q + i), second_table[number_of_disks - a - 1]);
        // P' - (Q' * x^(a-N-1))
        numerator ^= *(p_p + i);
        // Db = (P' - (Q' * x^(a-N-1))) * 1/(x^(a-b))
        *(p_db + i) = multiply_A_by_B_classic(numerator, first_table[b - a]);
    }

    // Вычисление Da
    for (int i = 0; i < size_of_disk; i++)
    {
        *(p_da + i) = (*(p_p + i)) ^ (*(p_db + i)); // Da = P' - Db
    }

    // Восстановление синдромов
    calc_one_stripe_classic(stripe, number_of_disks);

    p_da = NULL;
    p_db = NULL;
    p_p = NULL;
    p_q = NULL;
    free(new_syndromes);
    new_syndromes = NULL;
}

void recover_one_stripe_vector(__m256i* const stripe, unsigned int number_of_disks, unsigned int a, unsigned int b)
{
    __m256i* new_syndromes;
    // new_syndromes для хранения старых значений P Q
    new_syndromes = (__m256i*) memalign(32, size_of_disk_for_m256i * 2 * sizeof(__m256i));
    __m256i* p_da = stripe + a * size_of_disk_for_m256i;
    __m256i* p_db = stripe + b * size_of_disk_for_m256i;
    __m256i* p_p = stripe + number_of_disks * size_of_disk_for_m256i;
    __m256i* p_q = stripe + (number_of_disks + 1) * size_of_disk_for_m256i;

    // Обнуление Da и Db и занесение в new_syndromes старых значений P и Q
    for (int i = 0; i < size_of_disk_for_m256i; i++)
    {
        *(p_da + i) = _mm256_setzero_si256();
        *(p_db + i) = _mm256_setzero_si256();
        _mm256_storeu_si256((new_syndromes + i), *(p_p + i));
        _mm256_storeu_si256((new_syndromes + size_of_disk_for_m256i + i), *(p_q + i));
    }

    calc_one_stripe_vector(stripe, number_of_disks);

    // Вычисление значений P' и Q', хранятся на месте синдромов
    for (int i = 0; i < size_of_disk_for_m256i * 2; i++)
    {
        // Q находится непосредственно за P
        //*(p_p + i) = _mm256_xor_si256(*(p_p + i), *(new_syndromes + i));
        _mm256_storeu_si256((p_p + i), _mm256_xor_si256(*(p_p + i), *(new_syndromes + i)));
    }

    __m256i numerator;
    // Вычисление Db
    for (int i = 0; i < size_of_disk_for_m256i; i++)
    {
        // Q' * x^(a-N+1)
        numerator = multiply_A_by_B_vector(*(p_q + i), _mm256_set1_epi8(second_table[number_of_disks - a - 1]));
        // P' - (Q' * x^(a-N-1))
        numerator = _mm256_xor_si256(numerator, *(p_p + i));
        // Db = (P' - (Q' * x^(a-N-1))) * 1/(x^(a-b))
        //*(p_db + i) = multiply_A_by_B_vector(numerator, _mm256_set1_epi8(first_table[b - a]));
        _mm256_storeu_si256((p_db + i), multiply_A_by_B_vector(numerator, _mm256_set1_epi8(first_table[b - a])));
    }

    // Вычисление Da
    for (int i = 0; i < size_of_disk_for_m256i; i++)
    {
        *(p_da + i) = _mm256_xor_si256(*(p_p + i), (*(p_db + i))); // Da = P' - Db
    }

    // Восстановление синдромов
    calc_one_stripe_vector(stripe, number_of_disks);
    p_da = NULL;
    p_db = NULL;
    p_p = NULL;
    p_q = NULL;
    free(new_syndromes);
    new_syndromes = NULL;
}

void recover_one_stripe_RAIDIX(__m256i* const stripe, unsigned int number_of_disks, unsigned int a, unsigned int b)
{

}