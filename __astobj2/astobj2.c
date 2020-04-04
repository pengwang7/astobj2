#include <stdlib.h>
#include <pthread.h>
#include <inttypes.h>

#include "atomic.h"
#include "astobj2.h"

struct private_data {
    ao2_destructor destructor_fn;
    int32_t ref_counter;
    uint32_t options:3;
    uint32_t lockused:1;
    uint32_t magic:28;
};

struct astobj2 {
    struct private_data priv_data;
    void* user_data[0];
};

struct astseobj2 {
    pthread_mutex_t lock;
    struct private_data priv_data;
    void* user_data[0];
};

#define AO2_MAGIC (0xa70b123)

#define IS_AO2_MAGIC_BAD(p) (AO2_MAGIC != (p->priv_data.magic | 1))

#define EXTERNAL_OBJ(p) ((p) == NULL ? NULL : (p)->user_data)

#define INTERNAL_OBJ(user_data) \
	(struct astobj2 *) ((char *) user_data - sizeof(struct astobj2))

#define INTERNAL_SEOBJ(user_data) \
    ((struct astseobj2 *) (((char *) (user_data)) - sizeof(struct astseobj2)))

#define INTERNAL_OBJ_CHECK(user_data) ({ \
    struct astobj2* p; \
    if (!user_data \
    || !(p = INTERNAL_OBJ(user_data)) \
    || IS_AO2_MAGIC_BAD(p)) { \
        p = NULL; \
    } \
    (p); \
})

static void* ao2_internal_alloc(size_t data_size, ao2_destructor destructor_fn, enum ao2_alloc_options opt) {
    struct astobj2* obj = NULL;
    struct astseobj2* seobj = NULL;
    size_t overhead = 0;

    switch (opt & AO2_ALLOC_LOCK_MASK) {
    case AO2_ALLOC_WITH_NOLOCK:
        overhead = sizeof(*obj);
        obj = (struct astobj2*)calloc(1, overhead + data_size);
        if (!obj) {
            return NULL;
        }
        break;

    case AO2_ALLOC_WITH_LOCK:
        overhead = sizeof(*seobj);
        seobj = (struct astseobj2*)calloc(1, overhead + data_size);
        if (!seobj) {
            return NULL;
        }

        pthread_mutex_init(&seobj->lock, NULL);
        obj = (struct astobj2*)(&seobj->priv_data);
        break;

    default:
        return NULL;
    }

    obj->priv_data.destructor_fn = destructor_fn;
    obj->priv_data.ref_counter = 1;
    obj->priv_data.options = opt;
    obj->priv_data.magic = AO2_MAGIC;

    return EXTERNAL_OBJ(obj);
}

static int ao2_internal_ref(void* user_data, int delta) {
    struct astobj2* obj = INTERNAL_OBJ_CHECK(user_data);
    struct astseobj2* seobj = NULL;

    if (!obj) {
        return -1;
    }

    if (delta == 0) {
        return obj->priv_data.ref_counter;
    }

    int ret = ast_atomic_fetch_add(&obj->priv_data.ref_counter, delta, __ATOMIC_RELAXED);
    int current_value = ret + delta;

    if (current_value > 0) {
        return ret;
    } else if (current_value < 0) {
        return -1;
    }

    if (obj->priv_data.destructor_fn) {
        obj->priv_data.destructor_fn(user_data);
    }

    obj->priv_data.magic = 0;

    switch (obj->priv_data.options & AO2_ALLOC_LOCK_MASK) {
    case AO2_ALLOC_WITH_LOCK:
        seobj = INTERNAL_SEOBJ(user_data);
        pthread_mutex_destroy(&seobj->lock);
        free(seobj);
        break;

    case AO2_ALLOC_WITH_NOLOCK:
        free(obj);
        break;

    default:
        break;
    }

    return ret;
}

void* __ao2_alloc(size_t data_size, ao2_destructor destructor_fn, enum ao2_alloc_options opt) {
    return ao2_internal_alloc(data_size, destructor_fn, opt);
}

int __ao2_ref(void* user_data, int delta) {
    return ao2_internal_ref(user_data, delta);
}

void __ao2_cleanup(void* obj) {
    if (obj) {
        __ao2_ref(obj, -1);
    }
}
