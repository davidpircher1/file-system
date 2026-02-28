#pragma once
#include "stdint.h"
#define MAX_FILES 100

typedef struct IDPool{
    uint8_t used[2 * MAX_FILES]; // 0 = vo¾né, 1 = obsadené
    int count;                // poèet vo¾ných ID
} IDPool;


void idpool_init(IDPool* pool);

short idpool_alloc(IDPool* pool);

void idpool_free(IDPool* pool, short id);