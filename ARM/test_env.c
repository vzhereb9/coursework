#include "test_env.h"

int compare_double(const void* x1, const void* x2)
{
    return (*(double*) x1 - *(double*) x2);
}

double find_avg_speed_of_suitable_values(const unsigned int number_of_experiments, double* speed)
{
    qsort(speed, number_of_experiments, sizeof(double), compare_double);

    double avg_speed = 0.0;
    // we do not consider 5% of the largest and smallest values
    double const_for_avg_speed = 1.0 / (number_of_experiments * 0.9);
    for (int i = (int) (number_of_experiments * 0.05); i < (int) (number_of_experiments * 0.95); i++)
        avg_speed += const_for_avg_speed * speed[i];

    return avg_speed;
}

void find_avg_speeds(double avg_speed_calc[16], double avg_speed_recover_one_drive[16], double avg_speed_recover[16],
                     unsigned int flag)
{
    // variable for storing the number of tests performed at each iteration
    const unsigned int number_of_experiments = 10000; // 1000
    // array of data processing speeds for each number of drives (4, 8, ... , 64)
    double speed_calc_for_each_number_of_drives[number_of_experiments];
    double speed_recover_one_drive_for_each_number_of_drives[number_of_experiments];
    double speed_recover_for_each_number_of_drives[number_of_experiments];

    for (int r = 4; r <= 64; r = r + 4)
    {
        unsigned int number_of_stripes = 1, number_of_strips = r;

        for (int l = 0; l < number_of_experiments; l++)
        {
            uint8_t** raid = NULL;
            raid = (uint8_t**) malloc((number_of_stripes) * sizeof(uint8_t * ));
            for (unsigned int i = 0; i < number_of_stripes; i++)
            {
                raid[i] = (uint8_t*) malloc(size_of_strip * (number_of_strips + 2) * sizeof(uint8_t));
            }

            uint8_t* new_syndromes;
            // new_syndromes for storing old P Q
            new_syndromes = (uint8_t*) malloc(size_of_strip * 2 * sizeof(uint8_t));
            uint8_t* stripe_for_check;
            stripe_for_check = (uint8_t*) malloc(size_of_strip * (number_of_strips + 2) * sizeof(uint8_t));

            uint64_t time_calc = 0;
            uint64_t time_recover_one_drive = 0;
            uint64_t time_recover = 0;
            unsigned int a, b;

            for (int i = 0; i < number_of_stripes; i++)
            {
                for (int j = 0; j < size_of_strip * (number_of_strips); j++)
                {
                    raid[i][j] = rand() % 255;
                }
            }

            switch (flag)
            {
                case 0:
                {
                    // calculation of checksums and calculation of the time for which the calculation took place
                    time_calc = calc_classic(raid, number_of_strips, number_of_stripes);

                    a = rand() % (number_of_strips - 1);

                    // recovery 1 drive and calculation of the time for which the recovery took place
                    time_recover_one_drive = recover_classic_one_drive(raid, number_of_strips, number_of_stripes, a,
                                                                       stripe_for_check, new_syndromes);

                    b = rand() % (number_of_strips - 1) + 1;
                    a = rand() % b;

                    // recovery 2 drives and calculation of the time for which the recovery took place
                    time_recover = recover_classic(raid, number_of_strips, number_of_stripes, a, b, stripe_for_check,
                                                   new_syndromes);
                    break;
                }
                case 1:
                {
                    // calculation of checksums and calculation of the time for which the calculation took place
                    time_calc = calc_vector(raid, number_of_strips, number_of_stripes);

                    a = rand() % (number_of_strips - 1);

                    // recovery 1 drive and calculation of the time for which the recovery took place
                    time_recover_one_drive = recover_vector_one_drive(raid, number_of_strips, number_of_stripes, a,
                                                                      stripe_for_check, new_syndromes);

                    b = rand() % (number_of_strips - 1) + 1;
                    a = rand() % b;

                    // recovery 2 drives and calculation of the time for which the recovery took place
                    time_recover = recover_vector(raid, number_of_strips, number_of_stripes, a, b, stripe_for_check,
                                                  new_syndromes);
                    break;
                }
                case 2:
                {
                    // calculation of checksums and calculation of the time for which the calculation took place
                    time_calc = calc_RAIDIX(raid, number_of_strips, number_of_stripes);

                    a = rand() % (number_of_strips - 1);

                    // recovery 1 drive and calculation of the time for which the recovery took place
                    time_recover_one_drive = recover_RAIDIX_one_drive(raid, number_of_strips, number_of_stripes, a,
                                                                      stripe_for_check, new_syndromes);

                    b = rand() % (number_of_strips - 1) + 1;
                    a = rand() % b;

                    uint8_t* numerator;
                    numerator = (uint8_t*) malloc(8 * 16 * sizeof(uint8_t));

                    // recovery 2 drives and calculation of the time for which the recovery took place
                    time_recover = recover_RAIDIX(raid, number_of_strips, number_of_stripes, a, b, stripe_for_check,
                                                  new_syndromes, numerator);

                    free(numerator);
                    numerator = NULL;

                    break;
                }
                default:
                {
                    printf("You enter the wrong number");
                    break;
                }
            }

            // return speed of current experiment in MB/s
            speed_calc_for_each_number_of_drives[l] =
                    ((size_of_strip / (1024.0 * 1024.0)) * number_of_strips * number_of_stripes * 1000000000.0) /
                    (time_calc);
            speed_recover_one_drive_for_each_number_of_drives[l] =
                    ((size_of_strip / (1024.0 * 1024.0)) * number_of_strips * number_of_stripes * 1000000000.0) /
                    (time_recover_one_drive);
            speed_recover_for_each_number_of_drives[l] =
                    ((size_of_strip / (1024.0 * 1024.0)) * number_of_strips * number_of_stripes * 1000000000.0) /
                    (time_recover);

            for (int i = 0; i < number_of_stripes; i++)
            {
                free(raid[i]);
            }
            free(raid);
            free(new_syndromes);
            new_syndromes = NULL;
            free(stripe_for_check);
            stripe_for_check = NULL;
        }
        avg_speed_calc[(r / 4) - 1] = find_avg_speed_of_suitable_values(number_of_experiments,
                                                                         speed_calc_for_each_number_of_drives);
        avg_speed_recover_one_drive[(r / 4) - 1] = find_avg_speed_of_suitable_values(number_of_experiments,
                                                                                      speed_recover_one_drive_for_each_number_of_drives);
        avg_speed_recover[(r / 4) - 1] = find_avg_speed_of_suitable_values(number_of_experiments,
                                                                            speed_recover_for_each_number_of_drives);
    }
}