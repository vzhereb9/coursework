#include "test_env.h"

int main() {
    srand(time(NULL));
    unsigned int flag = 0;
    double avg_speed_calc[16], avg_speed_recover[16];
    memset(avg_speed_calc,0,sizeof(double)*16);
    memset(avg_speed_recover,0,sizeof(double)*16);
    find_avg_speeds(avg_speed_calc, avg_speed_recover, flag);
    for (int r = 4; r <= 64; r += 4)
    {
        printf("Average classic calc speed for %u drives is %.2f MB/s\n", r, avg_speed_calc[(r / 4) - 1]);
    }
    for (int r = 4; r <= 64; r += 4)
    {
        printf("Average classic recover speed for %u drives is %.2f MB/s\n", r, avg_speed_recover[(r / 4) - 1]);
    }

    flag = 1;
    find_avg_speeds(avg_speed_calc, avg_speed_recover, flag);
    for (int r = 4; r <= 64; r += 4)
    {
        printf("Average vector calc speed for %u drives is %.2f MB/s\n", r, avg_speed_calc[(r / 4) - 1]);
    }
    for (int r = 4; r <= 64; r += 4)
    {
        printf("Average vector recover speed for %u drives is %.2f MB/s\n", r, avg_speed_recover[(r / 4) - 1]);
    }

    flag = 2;
    find_avg_speeds(avg_speed_calc, avg_speed_recover, flag);
    for (int r = 4; r <= 64; r += 4)
    {
        printf("Average RAIDIX calc speed for %u drives is %.2f MB/s\n", r, avg_speed_calc[(r / 4) - 1]);
    }
    for (int r = 4; r <= 64; r += 4)
    {
        printf("Average RAIDIX recover speed for %u drives is %.2f MB/s\n", r, avg_speed_recover[(r / 4) - 1]);
    }

    return 0;
}