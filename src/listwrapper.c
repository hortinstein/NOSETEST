#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "listwrapper.h"

void teardown(void *n)
{
    free (((MemoryStruct*)n)->memory);
    free (n);
}


int ll_init(SerializableList * l){
    if (!l) return FAILURE;
    l->total_size = 0;
    l->list = ll_new(teardown);

    return SUCCESS;
}

int ll_free(SerializableList * l){
    if (!l) return FAILURE;

    ll_delete(l->list);
    return SUCCESS;
}


int ll_pop(MemoryStruct * ms, SerializableList * l){
    if (!l || !l->list || !ms || ms->memory) return FAILURE;
    
    MemoryStruct * temp_ms = NULL;
    temp_ms = (MemoryStruct*)ll_get_first(l->list);
    
    if (NULL == temp_ms){
        DEBUG_PRINT("ll_pop failed\n");
        goto fail;
    }

    ms->size = temp_ms->size; 
    ms->memory = (uint8_t*)malloc(ms->size);    
    memcpy(ms->memory, temp_ms->memory,ms->size);

    ll_remove_first(l->list);
    return SUCCESS;
fail:
    return FAILURE;
}


int ll_push(SerializableList * l, MemoryStruct * ms){
    if (!l || !l->list || !ms || !ms->memory) return FAILURE;

    //DANGER DANGER
    ///todo talk about testing here
    MemoryStruct * store_ms = (MemoryStruct*)malloc(sizeof(MemoryStruct));
    store_ms->size = ms->size; 
    store_ms->memory = (uint8_t*)malloc(ms->size);    
    memcpy(store_ms->memory,ms->memory,ms->size);
    
    if (-1 == ll_insert_first(l->list,store_ms)){
        DEBUG_PRINT("ll_push failed\n");
        goto fail;    
    }

    l->total_size += ms->size;

    return SUCCESS;
fail:
    free (store_ms->memory);
    free (store_ms);
    return FAILURE;
}

int ll_serialize(MemoryStruct * ms, SerializableList * l){
    if (!l || ms) return FAILURE;


    return SUCCESS;
}

int ll_deserialize(SerializableList * l, MemoryStruct * ms){
    if (!l || ms) return FAILURE;


    return SUCCESS;
}
