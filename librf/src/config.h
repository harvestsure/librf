#pragma once

#ifndef RESUMEF_INLINE_STATE
#if defined(__clang__) || defined(_MSC_VER)
/* #undef RESUMEF_INLINE_STATE */
#else
/* #undef RESUMEF_INLINE_STATE */
#endif	//defined(__clang__) || defined(_MSC_VER)
#endif	//RESUMEF_INLINE_STATE

#ifndef RESUMEF_DEBUG_COUNTER
/* #undef RESUMEF_DEBUG_COUNTER */
#endif	//RESUMEF_DEBUG_COUNTER

#ifndef RESUMEF_ENABLE_CONCEPT
#ifdef __cpp_lib_concepts
#define RESUMEF_ENABLE_CONCEPT 1
#else
#define RESUMEF_ENABLE_CONCEPT 1
#endif	//#ifdef __cpp_lib_concepts
#endif	//#ifndef RESUMEF_ENABLE_CONCEPT

#ifndef _WITH_LOCK_FREE_Q_KEEP_REAL_SIZE
/* #undef _WITH_LOCK_FREE_Q_KEEP_REAL_SIZE */
#endif	//_WITH_LOCK_FREE_Q_KEEP_REAL_SIZE

/* #undef RESUMEF_USE_CUSTOM_SPINLOCK */