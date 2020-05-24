#include "test_env.h"

int main() {
    srand(time(NULL));
    unsigned int flag = 0;
    double avg_speed_calc[16], avg_speed_recover_one_drive[16], avg_speed_recover[16];
    memset(avg_speed_calc,0,sizeof(double)*16);
    memset(avg_speed_recover,0,sizeof(double)*16);
	
	FILE *file;
        file = fopen("output.txt", "w");
    
    find_avg_speeds(avg_speed_calc, avg_speed_recover_one_drive, avg_speed_recover, flag);
    for (int r = 4; r <= 64; r += 4)
    {
        printf("Average classic calc speed for %u drives is %.2f MB/s\n", r, avg_speed_calc[(r / 4) - 1]);
    }
	for (int r = 4; r <= 64; r += 4)
    {
        printf("Average classic recover one drive speed for %u drives is %.2f MB/s\n", r, avg_speed_recover_one_drive[(r / 4) - 1]);
    }
    for (int r = 4; r <= 64; r += 4)
    {
        printf("Average classic recover two drives speed for %u drives is %.2f MB/s\n", r, avg_speed_recover[(r / 4) - 1]);
    }

	fputs("Calc classic: \n", file);
	for (int r = 4; r <= 64; r += 4)
		fprintf(file, "%.2f, ", avg_speed_calc[(r / 4) - 1]);
	fputs("\nRecover one drive classic: \n", file);
	for (int r = 4; r <= 64; r += 4)
		fprintf(file, "%.2f, ", avg_speed_recover_one_drive[(r / 4) - 1]);
	fputs("\nRecover two drives classic: \n", file);
	for (int r = 4; r <= 64; r += 4)
		fprintf(file, "%.2f, ", avg_speed_recover[(r / 4) - 1]);

    flag = 1;
    find_avg_speeds(avg_speed_calc, avg_speed_recover_one_drive, avg_speed_recover, flag);
    for (int r = 4; r <= 64; r += 4)
    {
        printf("Average vector calc speed for %u drives is %.2f MB/s\n", r, avg_speed_calc[(r / 4) - 1]);
    }
	for (int r = 4; r <= 64; r += 4)
    {
        printf("Average vector recover one drive speed for %u drives is %.2f MB/s\n", r, avg_speed_recover_one_drive[(r / 4) - 1]);
    }
    for (int r = 4; r <= 64; r += 4)
    {
        printf("Average vector recover two drives speed for %u drives is %.2f MB/s\n", r, avg_speed_recover[(r / 4) - 1]);
    }

	fputs("\n\nCalc vector: \n", file);
	for (int r = 4; r <= 64; r += 4)
		fprintf(file, "%.2f, ", avg_speed_calc[(r / 4) - 1]);
	fputs("\nRecover one drive vector: \n", file);
	for (int r = 4; r <= 64; r += 4)
		fprintf(file, "%.2f, ", avg_speed_recover_one_drive[(r / 4) - 1]);
	fputs("\nRecover two drives vector: \n", file);
	for (int r = 4; r <= 64; r += 4)
		fprintf(file, "%.2f, ", avg_speed_recover[(r / 4) - 1]);

    flag = 2;
    find_avg_speeds(avg_speed_calc, avg_speed_recover_one_drive, avg_speed_recover, flag);
    for (int r = 4; r <= 64; r += 4)
    {
        printf("Average RAIDIX calc speed for %u drives is %.2f MB/s\n", r, avg_speed_calc[(r / 4) - 1]);
    }
	for (int r = 4; r <= 64; r += 4)
    {
        printf("Average RAIDIX recover one drive speed for %u drives is %.2f MB/s\n", r, avg_speed_recover_one_drive[(r / 4) - 1]);
    }
    for (int r = 4; r <= 64; r += 4)
    {
        printf("Average RAIDIX recover two drives speed for %u drives is %.2f MB/s\n", r, avg_speed_recover[(r / 4) - 1]);
    }

	fputs("\n\nCalc RAIDIX: \n", file);
	for (int r = 4; r <= 64; r += 4)
		fprintf(file, "%.2f, ", avg_speed_calc[(r / 4) - 1]);
	fputs("\nRecover one drive RAIDIX: \n", file);
	for (int r = 4; r <= 64; r += 4)
		fprintf(file, "%.2f, ", avg_speed_recover_one_drive[(r / 4) - 1]);
	fputs("\nRecover two drives RAIDIX: \n", file);
	for (int r = 4; r <= 64; r += 4)
		fprintf(file, "%.2f, ", avg_speed_recover[(r / 4) - 1]);

    return 0;
}
