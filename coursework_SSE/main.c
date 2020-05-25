#include "test_env.h"

int main() {
    srand(time(NULL));
    unsigned int flag = 0;
    double avg_speed_calc[16][3], avg_speed_recover_one_drive[16][3], avg_speed_recover[16][3];
	
	FILE *file;
        file = fopen("output.txt", "w");
    
    find_avg_speeds(avg_speed_calc, avg_speed_recover_one_drive, avg_speed_recover, flag);
    for (int r = 4; r <= 64; r += 4)
    {
		printf("Classic calc speed for %u drives med is %.2f MB/s, margin of error is %.2f MB/s, avg speed is %.2f MB/s\n", r, avg_speed_calc[(r / 4) - 1][0], avg_speed_calc[(r / 4) - 1][1], avg_speed_calc[(r / 4) - 1][2]);
    }
	for (int r = 4; r <= 64; r += 4)
    {
        printf("Classic recover one drive speed for %u drives med is %.2f MB/s, margin of error is %.2f MB/s, avg speed is %.2f MB/s\n", r, avg_speed_recover_one_drive[(r / 4) - 1][0], avg_speed_recover_one_drive[(r / 4) - 1][1], avg_speed_recover_one_drive[(r / 4) - 1][2]);
    }
    for (int r = 4; r <= 64; r += 4)
    {
        printf("Classic recover two drives speed for %u drives med is %.2f MB/s, margin of error is %.2f MB/s, avg speed is %.2f MB/s\n", r, avg_speed_recover[(r / 4) - 1][0], avg_speed_recover[(r / 4) - 1][1], avg_speed_recover[(r / 4) - 1][2]);
    }

	fputs("Calc classic: \n", file);
	for (int r = 4; r <= 64; r += 4)
		fprintf(file, "%.2f, ", avg_speed_calc[(r / 4) - 1][2]);
	fputs("\nRecover one drive classic: \n", file);
	for (int r = 4; r <= 64; r += 4)
		fprintf(file, "%.2f, ", avg_speed_recover_one_drive[(r / 4) - 1][2]);
	fputs("\nRecover two drives classic: \n", file);
	for (int r = 4; r <= 64; r += 4)
		fprintf(file, "%.2f, ", avg_speed_recover[(r / 4) - 1][2]);

    flag = 1;
    find_avg_speeds(avg_speed_calc, avg_speed_recover_one_drive, avg_speed_recover, flag);
    for (int r = 4; r <= 64; r += 4)
    {
		printf("Vector calc speed for %u drives med is %.2f MB/s, margin of error is %.2f MB/s, avg speed is %.2f MB/s\n", r, avg_speed_calc[(r / 4) - 1][0], avg_speed_calc[(r / 4) - 1][1], avg_speed_calc[(r / 4) - 1][2]);
    }
	for (int r = 4; r <= 64; r += 4)
    {
        printf("Vector recover one drive speed for %u drives med is %.2f MB/s, margin of error is %.2f MB/s, avg speed is %.2f MB/s\n", r, avg_speed_recover_one_drive[(r / 4) - 1][0], avg_speed_recover_one_drive[(r / 4) - 1][1], avg_speed_recover_one_drive[(r / 4) - 1][2]);
    }
    for (int r = 4; r <= 64; r += 4)
    {
        printf("Vector recover two drives speed for %u drives med is %.2f MB/s, margin of error is %.2f MB/s, avg speed is %.2f MB/s\n", r, avg_speed_recover[(r / 4) - 1][0], avg_speed_recover[(r / 4) - 1][1], avg_speed_recover[(r / 4) - 1][2]);
    }

	fputs("\n\nCalc vector: \n", file);
	for (int r = 4; r <= 64; r += 4)
		fprintf(file, "%.2f, ", avg_speed_calc[(r / 4) - 1][2]);
	fputs("\nRecover one drive vector: \n", file);
	for (int r = 4; r <= 64; r += 4)
		fprintf(file, "%.2f, ", avg_speed_recover_one_drive[(r / 4) - 1][2]);
	fputs("\nRecover two drives vector: \n", file);
	for (int r = 4; r <= 64; r += 4)
		fprintf(file, "%.2f, ", avg_speed_recover[(r / 4) - 1][2]);

    flag = 2;
    find_avg_speeds(avg_speed_calc, avg_speed_recover_one_drive, avg_speed_recover, flag);
    for (int r = 4; r <= 64; r += 4)
    {
		printf("RAIDIX calc speed for %u drives med is %.2f MB/s, margin of error is %.2f MB/s, avg speed is %.2f MB/s\n", r, avg_speed_calc[(r / 4) - 1][0], avg_speed_calc[(r / 4) - 1][1], avg_speed_calc[(r / 4) - 1][2]);
    }
	for (int r = 4; r <= 64; r += 4)
    {
        printf("RAIDIX recover one drive speed for %u drives med is %.2f MB/s, margin of error is %.2f MB/s, avg speed is %.2f MB/s\n", r, avg_speed_recover_one_drive[(r / 4) - 1][0], avg_speed_recover_one_drive[(r / 4) - 1][1], avg_speed_recover_one_drive[(r / 4) - 1][2]);
    }
    for (int r = 4; r <= 64; r += 4)
    {
        printf("RAIDIX recover two drives speed for %u drives med is %.2f MB/s, margin of error is %.2f MB/s, avg speed is %.2f MB/s\n", r, avg_speed_recover[(r / 4) - 1][0], avg_speed_recover[(r / 4) - 1][1], avg_speed_recover[(r / 4) - 1][2]);
    }

	fputs("\n\nCalc RAIDIX: \n", file);
	for (int r = 4; r <= 64; r += 4)
		fprintf(file, "%.2f, ", avg_speed_calc[(r / 4) - 1][2]);
	fputs("\nRecover one drive RAIDIX: \n", file);
	for (int r = 4; r <= 64; r += 4)
		fprintf(file, "%.2f, ", avg_speed_recover_one_drive[(r / 4) - 1][2]);
	fputs("\nRecover two drives RAIDIX: \n", file);
	for (int r = 4; r <= 64; r += 4)
		fprintf(file, "%.2f, ", avg_speed_recover[(r / 4) - 1][2]);

	fclose(file);
    return 0;
}
