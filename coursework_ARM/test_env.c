#include "test_env.h"

// функция сравнения элементов массива
int compare_double(const void* x1, const void* x2)
{
    // если результат вычитания равен 0, то числа равны, < 0: x1 < x2; > 0: x1 > x2
    return (*(double*) x1 - *(double*) x2);
}

int bin_search(const unsigned int number_of_experiments, const double speed[number_of_experiments], double key)
{
    long long l = -1;
    long long r = number_of_experiments;
    long long m = 0;
    while (l < r - 1)
    {
        m = (l + r) / 2;
        if (speed[m] < key)
            l = m;
        else
            r = m;
    }
    return (int) r;
}

double find_avg_speed_confidence_interval(const unsigned int number_of_experiments, double* speed)
{
    qsort(speed, number_of_experiments, sizeof(double), compare_double);

    double med = 0.0;
    double arithmetic_mean = 0.0;
    double const_for_arithmetic_mean = 1.0 / number_of_experiments;
    double variance = 0.0;
    double const_for_variance = 1.0 / (number_of_experiments - 1);
    if (number_of_experiments % 2 == 1)
        med = speed[number_of_experiments / 2];
    else
        med = 0.5 * (speed[number_of_experiments / 2 - 1] + speed[number_of_experiments / 2]);
    for (int i = 0; i < number_of_experiments; i++)
        arithmetic_mean += const_for_arithmetic_mean * speed[i];
    for (int i = 0; i < number_of_experiments; i++)
        variance += const_for_variance * (speed[i] - arithmetic_mean) * (speed[i] - arithmetic_mean);
    double standard_deviation = sqrt(variance);

    double avg_speed = 0.0;
    double confidence_interval_min = med - standard_deviation;
    double confidence_interval_max = med + standard_deviation;

    int index_confidence_interval_min = bin_search(number_of_experiments, speed, confidence_interval_min);
    int index_confidence_interval_max = bin_search(number_of_experiments, speed, confidence_interval_max);
    double const_for_avg_speed = 1.0 / (index_confidence_interval_max - index_confidence_interval_min);
    for (int i = index_confidence_interval_min; i < index_confidence_interval_max; i++)
        avg_speed += const_for_avg_speed * speed[i];

    return avg_speed;
}

void find_avg_speeds(double avg_speed_calc[16], double avg_speed_recover[16], unsigned int flag)
{
    // число экспериментов, для которых запускаем тест.
    const unsigned int number_of_experiments = 10; // 1024
    // массив скоростей обработки данных для каждого количества дисков (4, 8, ... , 64)
    double speed_calc_for_each_number_of_drives[number_of_experiments];
    double speed_recover_for_each_number_of_drives[number_of_experiments];

    for (int r = 4; r <= 64; r = r + 4)
    {
        unsigned int number_of_stripes = 1, number_of_strips = r;
        uint8_t** raid = NULL;
        raid = (uint8_t**) calloc((number_of_stripes), sizeof(uint8_t*));
        for (unsigned int i = 0; i < number_of_stripes; i++)
        {
            raid[i] = (uint8_t*) calloc(size_of_strip * (number_of_strips + 2), sizeof(uint8_t));
        }


       for (int l = 0; l < number_of_experiments; l++)
        {
        	double time_calc = 0.0;
        	double time_recover = 0.0;

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
                    time_calc = ((double)calc_classic(raid, number_of_strips, number_of_stripes)) / CLOCKS_PER_SEC;

                    int b = rand() % (number_of_strips - 1) + 1;
                    int a = rand() % b;

                    time_recover = ((double)recover_classic(raid, number_of_strips, number_of_stripes, a, b)) / CLOCKS_PER_SEC;
                    break;
                }
                case 1:
                {
                	time_calc = ((double)calc_vector(raid, number_of_strips, number_of_stripes)) / (double)CLOCKS_PER_SEC;

                	unsigned int b = rand() % (number_of_strips - 1) + 1;
                	unsigned int a = rand() % b;

                	time_recover = ((double)recover_vector(raid, number_of_strips, number_of_stripes, a, b)) / (double)CLOCKS_PER_SEC;
                	break;
                }
                case 2:
                {
                	time_calc = ((double)calc_RAIDIX(raid, number_of_strips, number_of_stripes)) / CLOCKS_PER_SEC;

                    unsigned int b = rand() % (number_of_strips - 1) + 1;
                    unsigned int a = rand() % b;

                    // восстановление 2х дисков и вычисление времени, за которое происходит восстановление
                    time_recover = ((double)recover_RAIDIX(raid, number_of_strips, number_of_stripes, a, b)) / CLOCKS_PER_SEC;
                    break;
                }
                default:
                {
                    printf("Enter the wrong number");
                    break;
                }
            }

            speed_calc_for_each_number_of_drives[l] = time_calc;
            speed_recover_for_each_number_of_drives[l] = time_recover;
            //speed_calc_for_each_number_of_drives[l] = ((size_of_strip * number_of_strips * number_of_stripes) / (1024.0)) / (time_calc);
            //speed_recover_for_each_number_of_drives[l] = ((size_of_strip * number_of_strips * number_of_stripes) / (1024.0)) / (time_recover);
        }

        for (int i = 0; i < number_of_stripes; i++)
        {
            free(raid[i]);
        }
        free(raid);

        avg_speed_calc[(r / 4) - 1] = find_avg_speed_confidence_interval(number_of_experiments,
                                                                         speed_calc_for_each_number_of_drives);
        avg_speed_recover[(r / 4) - 1] = find_avg_speed_confidence_interval(number_of_experiments,
                                                                            speed_recover_for_each_number_of_drives);
    }
}
