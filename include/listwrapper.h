#ifndef LISTWRAPPER_H
#define LISTWRAPPER_H

#include "ll.h"
#include "datatypes.h"
#include "debug.h"
extern "C"{

    #include "binn.h"
}

// serializable linked list
typedef struct _SerializableList
{
    uint8_t total_size;
    ll * list;
} SerializableList;

extern "C" int ll_init(SerializableList * l);
extern "C" int ll_free(SerializableList * l);

extern "C" int ll_serialize(MemoryStruct * ms, SerializableList * l);
extern "C" int ll_deserialize(SerializableList * l, MemoryStruct * ms);

extern "C" int ll_pop(MemoryStruct * ms, SerializableList * l);
extern "C" int ll_push(SerializableList * l, MemoryStruct * ms);


#endif
