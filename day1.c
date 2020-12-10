#include <stdio.h>
#include <stdlib.h>

#define INPUT_FILE "inputFiles/day1.txt"
#define MAX_CHAR 10

void fatalError(char* msg) {
    printf("Fatal error: %s\n", msg);
    exit(1);
}

int main() {

    FILE* dayOneInput = fopen(INPUT_FILE, "r");
    if (!dayOneInput) {
        fatalError("Failed to open input file");
    }

    char buckets[2000] = { 0 };

    for (char buffer[MAX_CHAR]; fgets(buffer, MAX_CHAR, dayOneInput);) {
        int num = atoi(buffer);
        if (num) {
            int searchVal = 2020 - num;
            if (buckets[searchVal]) {
                printf("Nums: %d, %d\n", num, searchVal);
                printf("Answer: %d\n", num * searchVal);
                break;
            }
            buckets[num] = 1;
        }
    }

    fclose(dayOneInput);

    return 0;
}