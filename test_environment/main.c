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
    /*double final_avg_speed_calc_classic = find_avg_speed_confidence_interval(16, avg_speed_calc);
    double final_avg_speed_recover_classic = find_avg_speed_confidence_interval(16, avg_speed_recover);
    printf("Average classic calc speed for 4,8,..,64 drives is %.2f MB/s\n", final_avg_speed_calc_classic);
    printf("Average classic recover speed for 4,8,..,64 drives is %.2f MB/s\n", final_avg_speed_recover_classic);*/

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
    /*double final_avg_speed_calc_vector = find_avg_speed_confidence_interval(16, avg_speed_calc);
    double final_avg_speed_recover_vector = find_avg_speed_confidence_interval(16, avg_speed_recover);
    printf("Average vector calc speed for 4,8,..,64 drives is %.2f MB/s\n", final_avg_speed_calc_vector);
    printf("Average vector recover speed for 4,8,..,64 drives is %.2f MB/s\n", final_avg_speed_recover_vector);*/

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
    /*double final_avg_speed_calc_RAIDIX = find_avg_speed_confidence_interval(16, avg_speed_calc);
    double final_avg_speed_recover_RAIDIX = find_avg_speed_confidence_interval(16, avg_speed_recover);
    printf("Average RAIDIX calc speed for 4,8,..,64 drives is %.2f MB/s\n", final_avg_speed_calc_RAIDIX);
    printf("Average RAIDIX recover speed for 4,8,..,64 drives is %.2f MB/s\n", final_avg_speed_recover_RAIDIX);*/
    return 0;
}