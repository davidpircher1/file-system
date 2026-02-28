#include "idpool.h"

void idpool_init(IDPool* pool) {
    pool->count = 2 * MAX_FILES;
    for (int i = 0; i < 2 * MAX_FILES; i++) {
        pool->used[i] = 0;
    }
}

short idpool_alloc(IDPool* pool) {
    if (pool->count == 0) return -1; // not enough space
    for (int i = 0; i < 2 * MAX_FILES; i++) {
        if (!pool->used[i]) {
            pool->used[i] = 1;
            return i;
        }
    }
    return -1; // plné 
}

void idpool_free(IDPool* pool, short id) {
    if (id >= 0 && id < 2 * MAX_FILES)
        pool->used[id] = 0;
}
