#include "test_env.h"

int main() {
	// Disable trapping of accessing in EL3 and EL2.
	__asm ("MSR CPTR_EL3, XZR\n\t"
	"MSR CPTR_EL3, XZR\n\t"
	// Disable access trapping in EL1 and EL0.
	"MOV X1, #(0x3 << 20)\n\t" // FPEN disables trapping to EL1.
	"MSR CPACR_EL1, X1\n\t"
	"ISB\n\t");
    srand(time(NULL));
    unsigned int flag = 0;
    double avg_speed_calc[16], avg_speed_recover[16];
    memset(avg_speed_calc,0,sizeof(clock_t)*16);
    memset(avg_speed_recover,0,sizeof(clock_t)*16);
    find_avg_speeds(avg_speed_calc, avg_speed_recover, flag);
    for (int r = 4; r <= 64; r += 4)
    {
        printf("Average classic calc speed for %u drives is %.2f s\n", r, avg_speed_calc[(r / 4) - 1]);
    }
    for (int r = 4; r <= 64; r += 4)
    {
        printf("Average classic recover speed for %u drives is %.2f s\n", r, avg_speed_recover[(r / 4) - 1]);
    }
    /*for (int r = 4; r <= 64; r += 4)
    {
        printf("Average classic calc speed for %u drives is %.2f MB/s\n", r, avg_speed_calc[(r / 4) - 1]);
    }
    for (int r = 4; r <= 64; r += 4)
    {
        printf("Average classic recover speed for %u drives is %.2f MB/s\n", r, avg_speed_recover[(r / 4) - 1]);
    }*/

    flag = 1;
    find_avg_speeds(avg_speed_calc, avg_speed_recover, flag);
    for (int r = 4; r <= 64; r += 4)
    {
        printf("Average vector calc speed for %u drives is %.2f s\n", r, avg_speed_calc[(r / 4) - 1]);
    }
    for (int r = 4; r <= 64; r += 4)
    {
        printf("Average vector recover speed for %u drives is %.2f s\n", r, avg_speed_recover[(r / 4) - 1]);
    }
    /*for (int r = 4; r <= 64; r += 4)
    {
        printf("Average vector calc speed for %u drives is %.2f MB/s\n", r, avg_speed_calc[(r / 4) - 1]);
    }
    for (int r = 4; r <= 64; r += 4)
    {
        printf("Average vector recover speed for %u drives is %.2f MB/s\n", r, avg_speed_recover[(r / 4) - 1]);
    }*/

    flag = 2;
    find_avg_speeds(avg_speed_calc, avg_speed_recover, flag);
    for (int r = 4; r <= 64; r += 4)
    {
        printf("Average RAIDIX calc speed for %u drives is %.2f s\n", r, avg_speed_calc[(r / 4) - 1]);
    }
    for (int r = 4; r <= 64; r += 4)
    {
        printf("Average RAIDIX recover speed for %u drives is %.2f s\n", r, avg_speed_recover[(r / 4) - 1]);
    }
    /*for (int r = 4; r <= 64; r += 4)
    {
        printf("Average RAIDIX calc speed for %u drives is %.2f MB/s\n", r, avg_speed_calc[(r / 4) - 1]);
    }
    for (int r = 4; r <= 64; r += 4)
    {
        printf("Average RAIDIX recover speed for %u drives is %.2f MB/s\n", r, avg_speed_recover[(r / 4) - 1]);
    }*/

    return 0;
}
