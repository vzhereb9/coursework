#include "basic_operators.h"

uint8_t multiply_by_X_classic(uint8_t a)
{
    return (a << 1) ^ ((a >> 7) * GF_2_8_module);
}

uint8x16_t multiply_by_X_vector(uint8x16_t a)
{
 	uint8x16_t mask = vcgtq_s8(vmovq_n_s8(0), (int8x16_t)a);
 	mask = vandq_u8(mask, vmovq_n_u8(GF_2_8_module));
    a = vshlq_n_u8(a, 1);
    a = vandq_u8(a, vmovq_n_u8(0xFE));
    a = veorq_u8(a, mask);
    return a;
}

void multiply_by_X_RAIDIX(uint8_t* a)
{
	uint8x16_t 
	for (int i = 0; i < 7; i++)
    {
    	vst1q_u8(a + i * 16, veorq_u8(vld1q_u8(a + i * 16), vld1q_u8(a + (i + 1) * 16)));
    	vst1q_u8(a + (i + 1) * 16, veorq_u8(vld1q_u8(a + (i + 1) * 16), vld1q_u8(a + i * 16)));
        vst1q_u8(a + i * 16, veorq_u8(vld1q_u8(a + i * 16), vld1q_u8(a + (i + 1) * 16)));
    }
    vst1q_u8(a + 1 * 16, veorq_u8(vld1q_u8(a + 1 * 16), vld1q_u8(a + 7 * 16)));
    vst1q_u8(a + 2 * 16, veorq_u8(vld1q_u8(a + 2 * 16), vld1q_u8(a + 7 * 16)));
    vst1q_u8(a + 3 * 16, veorq_u8(vld1q_u8(a + 3 * 16), vld1q_u8(a + 7 * 16)));
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

uint8x16_t multiply_A_by_B_vector(uint8x16_t a, uint8_t b)
{
	uint8x16_t sum = vmovq_n_u8(0);
    while (b > 0)
    {
        if (b & 0x1)
        {
            sum = veorq_u8(a, sum);
        }
        a = multiply_by_X_vector(a);
        b = b >> 1;
    }
    return sum;
}

void multiply_A_by_B_RAIDIX(uint8_t* a, uint8_t b, uint8_t* sum)
{
    while (b > 0)
    {
        if (b & 0x1)
        {
            for (int j = 0; j < 8; j++)
            {
            	vst1q_u8(sum + j * 16, veorq_u8(vld1q_u8(sum + j * 16), vld1q_u8(a + j * 16)));
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

void print_uint8x16_t(uint8x16_t a)
{
	for(int i = 0; i < 16; i++)
	{
		 printf("%.2X", a[i]);
	}
	printf("\n");
}
