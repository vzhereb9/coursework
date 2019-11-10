#include <stdio.h>
#include <time.h>
#include "test_env.h"

int main() {
    double speed;
    speed = run_test_env();
    printf("Average speed is %.2f KB/s\n", speed);
    return 0;
}
