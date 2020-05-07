#include "calc.h"

clock_t calc_classic(uint8_t** raid, unsigned int number_of_strips, unsigned int number_of_stripes)
{
	clock_t time_calc = clock();
    for (unsigned int i = 0; i < number_of_stripes; i++)
    {
        calc_one_stripe_classic(raid[i], number_of_strips);
    }
    return clock() - time_calc;
}


clock_t calc_vector(uint8_t** raid, unsigned int number_of_strips, unsigned int number_of_stripes)
{
    clock_t time_calc = clock();

    for (unsigned int i = 0; i < number_of_stripes; i++)
    {
        calc_one_stripe_vector(raid[i], number_of_strips);
    }
    return clock() - time_calc;
}

clock_t calc_RAIDIX(uint8_t** raid, unsigned int number_of_strips, unsigned int number_of_stripes)
{
	clock_t time_calc = clock();

	for (unsigned int i = 0; i < number_of_stripes; i++)
	{
		calc_one_stripe_RAIDIX(raid[i], number_of_strips);
	}

	return clock() - time_calc;
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
        // Прибавление к текущей ячейке Q значение соотвествующей ячейки блока D(i+1)
        p_q[i % size_of_strip] ^= stripe[size_of_strip + i];

    }

    p_q = NULL;
    p_p = NULL;
}


void calc_one_stripe_vector(uint8_t* const stripe, unsigned int number_of_strips)
{
	uint8_t* p_p = stripe + number_of_strips * size_of_strip;
	uint8_t* p_q = stripe + (number_of_strips + 1) * size_of_strip;

	// Обнуление P
	for (unsigned int i = 0; i < size_of_strip; i += 16)
	{
		vst1q_u8(p_p + i, vmovq_n_u8(0));
	}
    // Вычисление P
    for (unsigned int i = 0; i < number_of_strips * size_of_strip; i += 16)
    {
    	vst1q_u8(p_p + (i % size_of_strip), veorq_u8(vld1q_u8(p_p + (i % size_of_strip)), vld1q_u8(stripe + i)));
    }

    // Q присваивается значение первого блока D0
    for (unsigned int i = 0; i < size_of_strip; i += 16)
    {
    	vst1q_u8(p_q + i, vld1q_u8(stripe + i));
    }
    // Вычисление Q
    for (unsigned int i = 0; i < (number_of_strips - 1) * size_of_strip; i += 16)
    {
    	// Умножение текущей ячейки Q на x
    	vst1q_u8(p_q + (i % size_of_strip), multiply_by_X_vector(vld1q_u8(p_q + (i % size_of_strip))));
    	// Прибаление к текущей ячейке Q значение соотвествующей ячейки блока D(i+1)
    	vst1q_u8(p_q + (i % size_of_strip), veorq_u8(vld1q_u8(p_q + (i % size_of_strip)), vld1q_u8(stripe + size_of_strip + i)));
    }
    p_q = NULL;
    p_p = NULL;
}

void calc_one_stripe_RAIDIX(uint8_t* const stripe, unsigned int number_of_strips)
{
	uint8_t* p_p = stripe + number_of_strips * size_of_strip;
	uint8_t* p_q = stripe + (number_of_strips + 1) * size_of_strip;

	// Обнуление P
    for (unsigned int i = 0; i < size_of_strip; i += 16)
    {
    	vst1q_u8(p_p + i, vmovq_n_u8(0));
    }
    // Вычисление P
    for (unsigned int i = 0; i < number_of_strips * size_of_strip; i += 16)
    {
    	vst1q_u8(p_p + (i % size_of_strip), veorq_u8(vld1q_u8(p_p + (i % size_of_strip)), vld1q_u8(stripe + i)));
    }

    // Q присваивается значение первого блока D0
    for (unsigned int i = 0; i < size_of_strip; i += 16)
    {
    	vst1q_u8(p_q + i, vld1q_u8(stripe + i));
    }
    // Вычисление Q
    for (int k = 0; k < size_of_strip / 128; k++)
    {
    	for (int i = 1; i < number_of_strips; i++)
        {
    		multiply_by_X_RAIDIX(p_q + (k * 8) * 16);
            for (int j = 0; j < 8; j++)
            {
            	vst1q_u8(p_q + (j + k * 8) * 16, veorq_u8(vld1q_u8(p_q + (j + k * 8) * 16), vld1q_u8(stripe + i * size_of_strip + (j + k * 8) * 16)));
            }
        }
    }
    p_q = NULL;
    p_p = NULL;
}
