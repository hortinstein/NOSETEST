#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

extern "C"{

    #include "binn.h"
}


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
    DEBUG_PRINT("adding element");

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
    DEBUG_PRINT("serializing %d items",num_items);
    MemoryStruct * temp_ms_node = NULL;
    
    binn *obj;

    // create a new object
    obj = binn_list();
    if (obj == NULL){
        DEBUG_PRINT("binn list alloc failed");
        goto fail;
    }

    // add values to it
    //binn_list_add_int32(obj, num_items);

    
    for (int i=0;i<num_items;i++){
        temp_ms_node = (MemoryStruct*) ll_get_n(l->list,i);
        //if retrieving the node fails, abort
        if (temp_ms_node == NULL){
            DEBUG_PRINT("node retreival failed");
            goto fail;
        }
        if (!binn_list_add_blob( obj, 
                            temp_ms_node->memory,
                            temp_ms_node->size)){
            DEBUG_PRINT("adding the element to the list failed");
            goto fail;
        }
    
       
    }

    ms->memory = (uint8_t*)malloc(binn_size(obj));
    if (NULL == ms->memory) 
    {
        DEBUG_PRINT("malloc failed");
        goto fail;
    }
    ms->size = binn_size(obj);
    DEBUG_PRINT("copying %d bytes",ms->size);
    memcpy(ms->memory,binn_ptr(obj),ms->size);
    
    binn_free(obj);    
    return SUCCESS;

fail:
    binn_free(obj);
    free (ms->memory);
    return FAILURE;
}

int ll_deserialize(SerializableList * l, MemoryStruct * ms){
    if (!l || !ms || !ms->memory) {
        DEBUG_PRINT("argument error");
        return FAILURE;
    } 
    int i, count;
    void *obj = (binn_struct*)ms->memory;
    
    MemoryStruct ms_temp;
    binn value; //dest value for the item before insertion into list
    
    count = binn_count((binn_struct*)ms->memory);
    
    DEBUG_PRINT("copying %d items into new list",count);

    if (ll_init(l)== FAILURE){
        DEBUG_PRINT("list creation failed");
        goto fail;
    }

    
    for(i=1; i<=count; i++) {
        if (binn_list_get_value(obj, 5, &value) == FALSE){
            DEBUG_PRINT("retrieving item failed");
            goto fail;
        }
        ms_temp.memory = (uint8_t*)value.ptr;
        ms_temp.size = value.size;
        DEBUG_PRINT("%s, %d",(char*)value.ptr,value.size);
        ll_push(l,&ms_temp);
    }

    return SUCCESS;
fail:
    ll_delete(l->list);
    return FAILURE;

}
