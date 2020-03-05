#include "calc.h"

clock_t calc_classic(uint8_t** raid, unsigned int number_of_disks, unsigned int number_of_stripes)
{
    clock_t time_calc_classic = clock();
    for (unsigned int i = 0; i < number_of_stripes; i++)
    {
        calc_one_stripe_classic(raid[i], number_of_disks);
    }
    return clock() - time_calc_classic;
}

clock_t calc_vector(uint8_t** raid, unsigned int number_of_disks, unsigned int number_of_stripes)
{
    clock_t time_calc_vector = clock();
    for (unsigned int i = 0; i < number_of_stripes; i++)
    {
        calc_one_stripe_vector((__m256i*) (raid[i]), number_of_disks);
    }
    return clock() - time_calc_vector;
}

clock_t calc_RAIDIX(uint8_t** raid, unsigned int number_of_disks, unsigned int number_of_stripes)
{
    clock_t time_calc_RAIDIX = clock();
    for (unsigned int i = 0; i < number_of_stripes; i++)
    {
        //print_stripe(raid[i], number_of_disks);
        calc_one_stripe_RAIDIX((__m256i*) (raid[i]), number_of_disks);
        //print_stripe(raid[i], number_of_disks);
    }
    return clock() - time_calc_RAIDIX;
}

void calc_one_stripe_classic(uint8_t* const stripe, unsigned int number_of_disks)
{
    uint8_t* p_p = stripe + number_of_disks * size_of_disk;
    uint8_t* p_q = stripe + (number_of_disks + 1) * size_of_disk;

    // Обнуление P
    for (unsigned int i = 0; i < size_of_disk; i++)
    {
        *(p_p + i) ^= *(p_p + i);
    }
    // Вычисление P
    for (unsigned int i = 0; i < number_of_disks * size_of_disk; i++)
    {
        *(p_p + i % size_of_disk) ^= *(stripe + i);
    }

    // Q присваивается значение первого блока D0
    for (unsigned int i = 0; i < size_of_disk; i++)
    {
        *(p_q + i) = *(stripe + i);
    }
    // Вычисление Q
    for (unsigned int i = 0; i < (number_of_disks - 1) * size_of_disk; i++)
    {
        // Умножение текущей ячейки Q на x
        *(p_q + i % size_of_disk) = multiplication_by_X_classic(*(p_q + i % size_of_disk));
        // Прибаление к текущей ячейке Q значение соотвествующей ячейки блока D(i+1)
        *(p_q + i % size_of_disk) ^= *(stripe + size_of_disk + i);

    }

    p_q = NULL;
    p_p = NULL;
}

void calc_one_stripe_vector(__m256i* const stripe, unsigned int number_of_disks)
{
    __m256i* p_p = stripe + number_of_disks * size_of_disk_for_m256i;
    __m256i* p_q = stripe + (number_of_disks + 1) * size_of_disk_for_m256i;
    // Обнуление P
    for (unsigned int i = 0; i < size_of_disk_for_m256i; i++)
    {
        *(p_p + i) = _mm256_setzero_si256();
        //_mm256_storeu_si256((p_p + i), _mm256_setzero_si256());
    }
    //print_stripe(stripe, number_of_disks);
    // Вычисление P
    for (unsigned int i = 0; i < number_of_disks * size_of_disk_for_m256i; i++)
    {
        *(p_p + i % size_of_disk_for_m256i) = _mm256_xor_si256(*(p_p + i % size_of_disk_for_m256i), *(stripe + i));
        //_mm256_storeu_si256((p_p + i % size_of_disk_for_m256i), _mm256_xor_si256(*(p_p + i % size_of_disk_for_m256i),
                                                                                 //*(stripe + i)));
    }
    //print_stripe(p_p, 0);

    // Q присваивается значение первого блока D0
    for (unsigned int i = 0; i < size_of_disk_for_m256i; i++)
    {
        *(p_q + i) =  *(stripe + i);
        //_mm256_storeu_si256((p_q + i), *(stripe + i));
    }
    // Вычисление Q
    for (unsigned int i = 0; i < (number_of_disks - 1) * size_of_disk_for_m256i; i++)
    {
        // Умножение текущей ячейки Q на x
        *(p_q + i % size_of_disk_for_m256i) = multiplication_by_X_vector(*(p_q + i % size_of_disk_for_m256i));
        //_mm256_storeu_si256((p_q + i % size_of_disk_for_m256i),
                           // multiplication_by_X_vector(*(p_q + i % size_of_disk_for_m256i)));
        // Прибаление к текущей ячейке Q значение соотвествующей ячейки блока D(i+1)
        *(p_q + i % size_of_disk_for_m256i) = _mm256_xor_si256(*(p_q + i % size_of_disk_for_m256i),
        *(stripe + (size_of_disk / 32) + i));
        //_mm256_storeu_si256((p_q + i % size_of_disk_for_m256i), _mm256_xor_si256(*(p_q + i % size_of_disk_for_m256i),
                                                                                 //*(stripe + (size_of_disk / 32) + i)));
    }
    //print_stripe(p_p, 0);
    p_q = NULL;
    p_p = NULL;
}

void calc_one_stripe_RAIDIX(__m256i* const stripe, unsigned int number_of_disks)
{

}
