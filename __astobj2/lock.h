#ifndef __LOCK_H__
#define __LOCK_H__

/*! Atomic += */
#define ast_atomic_fetch_add(ptr, val, memorder)  __sync_fetch_and_add((ptr), (val))
#define ast_atomic_add_fetch(ptr, val, memorder)  __sync_add_and_fetch((ptr), (val))

/*! Atomic -= */
#define ast_atomic_fetch_sub(ptr, val, memorder)  __sync_fetch_and_sub((ptr), (val))
#define ast_atomic_sub_fetch(ptr, val, memorder)  __sync_sub_and_fetch((ptr), (val))

/*! Atomic &= */
#define ast_atomic_fetch_and(ptr, val, memorder)  __sync_fetch_and_and((ptr), (val))
#define ast_atomic_and_fetch(ptr, val, memorder)  __sync_and_and_fetch((ptr), (val))

/*! Atomic |= */
#define ast_atomic_fetch_or(ptr, val, memorder)  __sync_fetch_and_or((ptr), (val))
#define ast_atomic_or_fetch(ptr, val, memorder)  __sync_or_and_fetch((ptr), (val))

/*! Atomic xor = */
#define ast_atomic_fetch_xor(ptr, val, memorder)  __sync_fetch_and_xor((ptr), (val))
#define ast_atomic_xor_fetch(ptr, val, memorder)  __sync_xor_and_fetch((ptr), (val))

#endif /* __LOCK_H__ */
