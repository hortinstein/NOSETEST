
#include <stdio.h>

#define FAILURE 1
#define SUCCESS 0

#define DEBUG_ERROR(fmt, args...) fprintf(stderr, "ERROR: %s:%d:%s(): " fmt "\n", \
    __FILE__, __LINE__, __func__, ##args)



int check(int *a,int *b)
{
    CHECK_ARGS((!a || !b),"argument error");

    return SUCCESS;
fail:
    return FAILURE;
}


int main(void){

    int a=4;
    int b=5; 
    CHECK_SUCCESS(check(NULL,NULL),"check1 failed");
    CHECK_SUCCESS(check(&a,&b),"check2 failed");

    return SUCCESS;
fail:
    return FAILURE;
}