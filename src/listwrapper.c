#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "listwrapper.h"

#define MEM_HDR_SIZE sizeof(MemoryStruct) - sizeof(uint8_t*)

void teardown(void *n){
    if (!n) {
        DEBUG_PRINT("argument error");
        
    } else {

        free (((MemoryStruct*)n)->memory);
        free (n);
    }
}


int ll_init(SerializableList * l){
    if (!l) {
        DEBUG_PRINT("argument error");
        return FAILURE;
    } 
    l->total_size = 0;
    l->list = ll_new(teardown);

    return SUCCESS;
}

int ll_free(SerializableList * l){
    if (!l) {
        DEBUG_PRINT("argument error");
        return FAILURE;
    } 

    ll_delete(l->list);
    return SUCCESS;
}


int ll_pop(MemoryStruct * ms, SerializableList * l){
    if (!l || !l->list || !ms || ms->memory) {
        DEBUG_PRINT("argument error");
        return FAILURE;
    } 
    
    MemoryStruct * temp_ms = NULL;
    temp_ms = (MemoryStruct*)ll_get_first(l->list);
    
    if (NULL == temp_ms){
        DEBUG_PRINT("ll_pop failed, ll get first failure\n");
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
    if (!l || !l->list || !ms || !ms->memory) {
        DEBUG_PRINT("argument error");
        return FAILURE;
    } 

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
    if (!l || !ms || ms->memory) {
        DEBUG_PRINT("argument error");
        return FAILURE;
    } 
    int num_items = l->list->len;

    MemoryStruct * temp_ms_node = NULL;
    
    int dest_mem_i = 0;
    ms->size = l->total_size;

    //allocates extra memory for the size of each element
    ms->size += MEM_HDR_SIZE * num_items;
    
    //allocate a contigous block of memory to store the serialized array
    ms->memory = (uint8_t*) malloc(ms->size);
    if (ms->memory == NULL){
        DEBUG_PRINT("malloc failed");
        goto fail;
    } 

    
    for (int i=0;i<num_items;i++){
        temp_ms_node = (MemoryStruct*) ll_get_n(l->list,i);
        //if retrieving the node fails, abort
        if (temp_ms_node == NULL){
            DEBUG_PRINT("node retreival failed");
            goto fail;
        }

        //copies the header into the contigious array
        memcpy(ms->memory + dest_mem_i,temp_ms_node,MEM_HDR_SIZE);
        dest_mem_i += MEM_HDR_SIZE;

        //copies the memory into the contigious array
        memcpy(ms->memory + dest_mem_i,temp_ms_node->memory,temp_ms_node->size);
        dest_mem_i += temp_ms_node->size;
       
    }

    return SUCCESS;

fail:
    free (ms->memory);
    return FAILURE;
}

int ll_deserialize(SerializableList * l, MemoryStruct * ms){
    if (!l || !ms || !ms->memory) return FAILURE;



    return SUCCESS;
}
