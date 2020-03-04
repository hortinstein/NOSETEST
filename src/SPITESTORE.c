
#include <stdio.h>
#include <stdlib.h>

extern "C" {
    #include "ll.h"
}

void num_teardown(void *n) {
    *(int *)n *= -1; // just so we can visually inspect removals afterwards
}

void num_printer(void *n) {
    printf(" %d", *(int *)n);
}

int num_equals_3(void *n) {
    return *(int *)n == 3;
}

int main() {
    
}