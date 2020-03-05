#include "table_values.h"

const uint8_t first_table[65] = {0x0, 0xD1, 0x9E, 0xAC, 0xFD, 0xE9, 0x3D, 0xAA, 0x7B, 0x54, 0x1A, 0x19, 0x95, 0xC6, 0x29,
                                 0x5E, 0xB7, 0x4C, 0x57, 0xEB, 0x35, 0xB5, 0x30, 0x86, 0xB8, 0x2, 0xFA, 0xED, 0xF4, 0xE7,
                                 0x13, 0xD9, 0x9, 0xC1, 0x66, 0x2F, 0x52, 0xDD, 0x1E, 0x27, 0xD5, 0x58, 0xD, 0xA8, 0xC4, 0xAF,
                                 0xCC, 0x8B, 0x5C, 0x7D, 0x4, 0xF1, 0x6E, 0x3E, 0xA, 0x70, 0x3A, 0xD2, 0x4E, 0x69, 0x74, 0x78,
                                 0xDE, 0x65, 0x41}; // values 1/(1 - x^(a-b))

const uint8_t second_table[65] = {0x1, 0xB8, 0x5C, 0x2E, 0x17, 0xB3, 0xE1, 0xC8, 0x64, 0x32, 0x19, 0xB4, 0x5A, 0x2D, 0xAE,
                                  0x57, 0x93, 0xF1, 0xC0, 0x60, 0x30, 0x18, 0xC, 0x6, 0x3, 0xB9, 0xE4, 0x72, 0x39, 0xA4, 0x52,
                                  0x29, 0xAC, 0x56, 0x2B, 0xAD, 0xEE, 0x77, 0x83, 0xF9, 0xC4, 0x62, 0x31, 0xA0, 0x50, 0x28,
                                  0x14, 0xA, 0x5, 0xBA, 0x5D, 0x96, 0x4B, 0x9D, 0xF6, 0x7B, 0x85, 0xFA, 0x7D, 0x86, 0x43,
                                  0x99, 0xF4, 0x7A, 0x3D}; // values x^(a-N+1)

       /*for (j = 0; j < size_of_disk; j++)
            {
                raid[0][j] = 0x05;
            }
            for (j = 0; j < size_of_disk; j++)
            {
                raid[0][j + size_of_disk] = 0x04;
            }
            for (j = 0; j < size_of_disk; j++)
            {
                raid[0][j+ 2 *size_of_disk] = 0x03;
            }
            for (j = 0; j < size_of_disk; j++)
            {
                raid[0][j+ 3 * size_of_disk] = 0x02;
            }
            for (j = 0; j < size_of_disk; j++)
            {
                raid[0][j+ 4 * size_of_disk] = 0x01;
            }*/

/*void find_1_div_1_min_x_pow_a_min_b(uint8_t first_table[65], uint8_t second_table[65])
{
    uint8_t x_pow_254 = 1;
    first_table[0] = 1;
    second_table[0] = 1;
    for (int i = 0; i < 254; i++)
        x_pow_254 = multiplication_by_X_classic(x_pow_254);
    for (int i = 1; i < 65; i++)
    {
        first_table[i] = multiplication_A_by_B(first_table[i - 1],x_pow_254);
                    // x^(a-b) = x^(-(b-a)) = x^(254*(b-a)) first table
        second_table[i] = multiplication_A_by_B(second_table[i - 1], x_pow_254);
                    // x^(a-N+1) = x^(-(N-a-1)) = x^(254*(N-a-1)) second table
    }

    // 1/(1 - x^(a-b)) = (1 - x^(a-b))^(-1) = (1 - x^(a-b))^(254)
    for (int i = 0; i < 65; i++)
    {
        first_table[i] ^= 1;
        uint8_t temp = first_table[i];
        for (int j = 0; j < 253; j++)
            first_table[i] = multiply_A_by_B_classic(first_table[i], temp);
    }
}


void tables()
{
    find_1_div_1_min_x_pow_a_min_b(first_table, second_table);
    //for (int i = 0; i < 65; i++)
    //printf("0x%X, ", first_table[i]);
    for (int i = 0; i < 65; i++)
        printf("0x%X, ", second_table[i]);
}*/