#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "datatypes.h"
#include "config.h"

int ms_init(MemoryStruct *ms){
    if (!ms || ms->len != 0 || ms->memory) {
        DEBUG_ERROR("argument error");
        goto fail;
    }
    ms->len = 0;
    ms->memory = NULL;

    return SUCCESS;
fail:
    return FAILURE;
}

int ms_free(MemoryStruct *ms){
    if (!ms) 

    free(ms->memory);

    return SUCCESS;
fail:
    return FAILURE;
}

int ms_init(MemoryStruct *ms){
    if (!ms || ms->len != 0 || ms->memory) 

    

    return SUCCESS;
fail:
    return FAILURE;
}