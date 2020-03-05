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

double find_confidence_interval(const unsigned int number_of_experiments, double speed[number_of_experiments])
{
    // сортировка массива
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
    const unsigned int number_of_experiments = 100; // Вообще расчитано на 1024
    // массив скоростей обработки данных для каждого количества дисков (4, 8, ... , 64)
    double speed_calc_for_each_number_of_drives[number_of_experiments];
    double speed_recover_for_each_number_of_drives[number_of_experiments];
    //memset(speed_calc_for_each_number_of_drives,0,sizeof(double)*number_of_experiments);
    //memset(speed_recover_for_each_number_of_drives,0,sizeof(double)*number_of_experiments);

    for (int r = 4; r <= 4; r = r + 4)
    {
        uint8_t** raid = NULL;
        unsigned int i, j, number_of_stripes = 1, number_of_disks = r;
        // заполнение number_of_stripes штук страйпов, каждый страйп занимает по size_of_disk * (number_of_disks +2).
        // это реализовано как двумерный массив
        raid = (uint8_t**) malloc((number_of_stripes) * sizeof(uint8_t*)); //доп 2 дисковых массива для P Q
        for (i = 0; i < number_of_stripes; i++)
        {
            // выделение места для каждого страйпа
            raid[i] = (uint8_t*) malloc(size_of_disk * (number_of_disks + 2) * sizeof(uint8_t)); //sizeof(uint8_t) = 1
        }

        // эксперимент запускается number_of_experiments раз и записывается скорость выполнения каждого эксперимента для calc и recover (КБ/с) в массив
        for (int k = 0; k < number_of_experiments; k++)
        {
            clock_t time_calc = 0;
            clock_t time_recover = 0;

            // заполнение каждого блока D[0] - D[number_of_disks - 1] случайными значениями
            for (i = 0; i < number_of_stripes; i++)
            {
                for (j = 0; j < size_of_disk * (number_of_disks); j++)
                {
                    raid[i][j] = rand() % 255;
                }
            }

            switch (flag)
            {
                case 0:
                {
                    // подсчет контрольных сумм и вычисление времени, за которое происходит подсчет
                    time_calc = calc_classic(raid, number_of_disks, number_of_stripes);

                    unsigned int b = rand() % (number_of_disks - 1) + 1;
                    unsigned int a = rand() % b;

                    // восстановление 2х дисков и вычисление времени, за которое происходит восстановление
                    time_recover = recover_classic(raid, number_of_disks, number_of_stripes, a, b);
                    break;
                }
                case 1:
                {
                    // подсчет контрольных сумм и вычисление времени, за которое происходит подсчет
                    time_calc = calc_vector(raid, number_of_disks, number_of_stripes);

                    unsigned int b = rand() % (number_of_disks - 1) + 1;
                    unsigned int a = rand() % b;

                    // восстановление 2х дисков и вычисление времени, за которое происходит восстановление
                    time_recover = recover_vector(raid, number_of_disks, number_of_stripes, a, b);
                    break;
                }
                case 2:
                {
                    // подсчет контрольных сумм и вычисление времени, за которое происходит подсчет
                    time_calc = calc_RAIDIX(raid, number_of_disks, number_of_stripes);

                    unsigned int b = rand() % (number_of_disks - 1) + 1;
                    unsigned int a = rand() % b;

                    // восстановление 2х дисков и вычисление времени, за которое происходит восстановление
                    time_recover = recover_RAIDIX(raid, number_of_disks, number_of_stripes, a, b);
                    break;
                }
                default:
                {
                    printf("Enter the wrong number");
                    break;
                }
            }

            // возвращение скорости в KБ/с текущего эксперимента
            //   time / (double) CLOCKS_PER_SEC возвращает количество единиц значения времени в одной секунде
            //   size_of_disk * number_of_disks * number_of_stripes - общий размер всего рейда, над которым проводится эксперимент
            //   деление  на 1024, т.к. получен результат в байтах, а нужно в KБ
            speed_calc_for_each_number_of_drives[k] =
                    ((size_of_disk / 1024.0) * number_of_disks * number_of_stripes) /
                    ((time_calc) / (double) CLOCKS_PER_SEC);
            speed_recover_for_each_number_of_drives[k] =
                    ((size_of_disk / 1024.0) * number_of_disks * number_of_stripes) /
                    ((time_recover) / (double) CLOCKS_PER_SEC);
        }

        // очистка памяти
        for (i = 0; i < number_of_stripes; i++)
        {
            free(raid[i]);
        }
        free(raid);

        avg_speed_calc[(r / 4) - 1] = find_confidence_interval(number_of_experiments,
                                                               speed_calc_for_each_number_of_drives);
        avg_speed_recover[(r / 4) - 1] = find_confidence_interval(number_of_experiments,
                                                                  speed_recover_for_each_number_of_drives);
    }
}