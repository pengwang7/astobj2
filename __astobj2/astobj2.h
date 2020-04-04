#ifndef __AST_OBJECT2_H__
#define __AST_OBJECT2_H__

typedef void (* ao2_destructor) (void* args);

enum ao2_alloc_options {
    AO2_ALLOC_WITH_LOCK = (0 << 0),
    AO2_ALLOC_WITH_RWLOCK = (1 << 0),
    AO2_ALLOC_WITH_NOLOCK = (2 << 0),
    AO2_ALLOC_LOCK_MASK = (3 << 0),
};

#define ao2_alloc(vartype, data_size, destructor_fn) \
    (vartype)__ao2_alloc((data_size), (destructor_fn), AO2_ALLOC_WITH_LOCK);

#define ao2_ref(obj, delta) __ao2_ref(obj, delta);

#define ao2_cleanup(obj) __ao2_cleanup(obj);

#define ao2_lock(obj) __ao2_lock(obj);

#define ao2_unlock(obj) __ao2_unlock(obj);

#define ao2_trylock(obj) __ao2_try_lock(obj);

void __ao2_cleanup(void* obj);

void* __ao2_alloc(size_t data_size, ao2_destructor destructor_fn, enum ao2_alloc_options opt);

int __ao2_ref(void* user_data, int delta);

int __ao2_lock(void* obj);

int __ao2_unlock(void* obj);

int __ao2_try_lock(void* obj);

#endif /* __AST_OBJECT2_H__ */
