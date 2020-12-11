#include <stdio.h>
#include <stdlib.h>

void fatalError(char* msg) {
    printf("Fatal error: %s\n", msg);
    exit(1);
}

void intPrinter(void* intp) {
    printf("%d\n", *(int*)intp);
}