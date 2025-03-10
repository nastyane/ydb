/* SPDX-License-Identifier: MIT */
#ifndef LIBURING_LIB_H
#define LIBURING_LIB_H

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#if defined(__x86_64__) || defined(__i386__)
#include "arch/x86/lib.h"
#elif defined(__aarch64__)
#include "arch/aarch64/lib.h"
#else
/*
 * We don't have nolibc support for this arch. Must use libc!
 */
#ifdef CONFIG_NOLIBC
#error "This arch doesn't support building liburing without libc"
#endif
/* libc wrappers. */
#error #include "arch/generic/lib.h"
#endif


#ifndef offsetof
#define offsetof(TYPE, FIELD) ((size_t) &((TYPE *)0)->FIELD)
#endif

#ifndef container_of
#define container_of(PTR, TYPE, FIELD) ({			\
	__typeof__(((TYPE *)0)->FIELD) *__FIELD_PTR = (PTR);	\
	(TYPE *)((char *) __FIELD_PTR - offsetof(TYPE, FIELD));	\
})
#endif

#define __maybe_unused		__attribute__((__unused__))
#define __hot			__attribute__((__hot__))
#define __cold			__attribute__((__cold__))

void *__uring_malloc(size_t len);
void __uring_free(void *p);

static inline void *uring_malloc(size_t len)
{
#ifdef CONFIG_NOLIBC
	return __uring_malloc(len);
#else
	return malloc(len);
#endif
}

static inline void uring_free(void *ptr)
{
#ifdef CONFIG_NOLIBC
	__uring_free(ptr);
#else
	free(ptr);
#endif
}

#endif /* #ifndef LIBURING_LIB_H */
