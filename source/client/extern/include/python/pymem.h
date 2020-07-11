/* The PyMem_ family:  low-level memory allocation interfaces.
   See objimpl.h for the PyObject_ memory family.
*/

#ifndef Py_PYMEM_H
#define Py_PYMEM_H

#include "pyport.h"

#ifdef __cplusplus
extern "C" {
#endif

PyAPI_FUNC(void *) PyMem_Malloc(size_t);
PyAPI_FUNC(void *) PyMem_Realloc(void *, size_t);
PyAPI_FUNC(void) PyMem_Free(void *);

#ifdef PYMALLOC_DEBUG
/* Redirect all memory operations to Python's debugging allocator. */
#define PyMem_MALLOC		_PyMem_DebugMalloc
#define PyMem_REALLOC		_PyMem_DebugRealloc
#define PyMem_FREE		_PyMem_DebugFree

#else	/* ! PYMALLOC_DEBUG */

/* PyMem_MALLOC(0) means malloc(1). Some systems would return NULL
   for malloc(0), which would be treated as an error. Some platforms
   would return a pointer with no memory behind it, which would break
   pymalloc. To solve these problems, allocate an extra byte. */
/* Returns NULL to indicate error if a negative size or size larger than
   Py_ssize_t can represent is supplied.  Helps prevents security holes. */
#define PyMem_MALLOC(n)		((size_t)(n) > (size_t)PY_SSIZE_T_MAX ? NULL \
				: malloc((n) ? (n) : 1))
#define PyMem_REALLOC(p, n)	((size_t)(n) > (size_t)PY_SSIZE_T_MAX  ? NULL \
				: realloc((p), (n) ? (n) : 1))
#define PyMem_FREE		free

#endif	/* PYMALLOC_DEBUG */

/*
 * Type-oriented memory interface
 * ==============================
 *
 * Allocate memory for n objects of the given type.  Returns a new pointer
 * or NULL if the request was too large or memory allocation failed.  Use
 * these macros rather than doing the multiplication yourself so that proper
 * overflow checking is always done.
 */

#define PyMem_New(type, n) \
  ( ((size_t)(n) > PY_SSIZE_T_MAX / sizeof(type)) ? NULL :	\
	( (type *) PyMem_Malloc((n) * sizeof(type)) ) )
#define PyMem_NEW(type, n) \
  ( ((size_t)(n) > PY_SSIZE_T_MAX / sizeof(type)) ? NULL :	\
	( (type *) PyMem_MALLOC((n) * sizeof(type)) ) )

/*
 * The value of (p) is always clobbered by this macro regardless of success.
 * The caller MUST check if (p) is NULL afterwards and deal with the memory
 * error if so.  This means the original value of (p) MUST be saved for the
 * caller's memory error handler to not lose track of it.
 */
#define PyMem_Resize(p, type, n) \
  ( (p) = ((size_t)(n) > PY_SSIZE_T_MAX / sizeof(type)) ? NULL :	\
	(type *) PyMem_Realloc((p), (n) * sizeof(type)) )
#define PyMem_RESIZE(p, type, n) \
  ( (p) = ((size_t)(n) > PY_SSIZE_T_MAX / sizeof(type)) ? NULL :	\
	(type *) PyMem_REALLOC((p), (n) * sizeof(type)) )

/* PyMem{Del,DEL} are left over from ancient days, and shouldn't be used
 * anymore.  They're just confusing aliases for PyMem_{Free,FREE} now.
 */
#define PyMem_Del		PyMem_Free
#define PyMem_DEL		PyMem_FREE

#ifdef __cplusplus
}
#endif

#endif /* !Py_PYMEM_H */
