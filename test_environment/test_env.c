#include "test_env.h"

int compare(const void *x1, const void *x2)   // функция сравнения элементов массива
{
    return (*(int *) x1 - *(int *) x2); // если результат вычитания равен 0, то числа равны, < 0: x1 < x2; > 0: x1 > x2
}

double run_test_env()
{
    //число экспериментов, для которых запускаем тест. Число дисков от 4 до 64
    const unsigned int number_of_experiments = 1024;
    //массив скоростей обработки данных для каждого эксперимента
    double speed[number_of_experiments];
    //запускаем эксперимент number_of_experiments раз и записываем скорость выполнения эксперимента (КБ/с) в массив
    for (int i = 0; i < number_of_experiments; i++)
    {
        //число дисков от 4 до 64
        speed[i] = speed_of_one_check(i % 61 + 4);
    }

    //сортируем массив
    qsort(speed, number_of_experiments, sizeof(double), compare);

    double avg_speed = 0.0;
    //выбираем 5% экспериментов, чтобы не учитывать их при более оптимальном подсчете средней скорости выполнения
    unsigned int number_of_unnecessary_experiments = number_of_experiments / 20;

    //подсчитываем среднюю скорость выполнения (KБ/с)
    for (unsigned int i = number_of_unnecessary_experiments;
         i < number_of_experiments - number_of_unnecessary_experiments; i++)
    {
        avg_speed += speed[i];
    }
    avg_speed /= (number_of_experiments - 2 * number_of_unnecessary_experiments);
    return avg_speed;
}

double speed_of_one_check(unsigned int number_of_disks)
{
    byte **raid = NULL;
    unsigned int i, j;
    clock_t time;

    //заполнили number_of_disks штук дисков по 4Кб (4096 Б) каждый, реализовали это как двумерный массив
    raid = (byte **) malloc((number_of_disks + 2) * sizeof(byte *)); //доп 2 дисковых массива для P Q
    for (i = 0; i < number_of_disks + 2; i++)
    {
        raid[i] = (byte *) malloc(size_of_disk * sizeof(byte));
    }

    //byte* p = raid[number_of_disks];
    //byte* q = raid[number_of_disks + 1];

    //заполнили этот массив случайными значениями
    for (i = 0; i < number_of_disks; i++)
    {
        for (j = 0; j < size_of_disk; j++)
        {
            raid[i][j] = rand() % 255;
        }
    }

    time = clock();

    //выполняем подсчет контрольных сумм
    calc(raid, number_of_disks);

    //вычислили затраченное время, затраченное процессором на выполнение программы
    time = clock() - time;

    //очищаем память
    for (i = 0; i < number_of_disks + 2; i++)
    {
        free(raid[i]);
    }
    free(raid);

    //возвращаем скорость в KБ/с текущего эксперимента
    //time / (double) CLOCKS_PER_SEC возвращает количество единиц значения времени в одной секунде
    //size_of_disk * number_of_disks - общий размер всех дисков, над которыми проводим эксперимент
    //делим на 1024, т.к. получили результат в байтах, а хотим в КБ
    return ((size_of_disk * number_of_disks) / (time / (double) CLOCKS_PER_SEC)) / 1024;
}

void calc(byte **raid, unsigned int number_of_disks)
{
    //просто подсчитываем сумму всех элементов и записываем ее в P, Q пока никак не изменяем
    for (unsigned int j = 0; j < size_of_disk; j++)
    {
        for (unsigned int i = 0; i < number_of_disks; i++)
        {
            raid[number_of_disks][j] += raid[i][j];
        }
    }
}
