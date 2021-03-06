#pragma once

#ifndef _offset_of
#define _offset_of(c, m) reinterpret_cast<size_t>(&static_cast<c *>(0)->m)
#endif

#define co_yield_void co_yield nullptr
#define co_return_void co_return nullptr

#if !defined(_DISABLE_RESUMEF_GO_MACRO)
#define go (*::resumef::this_scheduler()) + 
#define GO (*::resumef::this_scheduler()) + [=]()mutable->resumef::future_t<>
#endif

#define current_scheduler() (co_await ::resumef::get_current_scheduler())
#define root_state() (co_await ::resumef::get_root_state())
#define current_task() (co_await ::resumef::get_current_task())

#if defined(__clang__) || defined(__GNUC__)
#ifndef likely
#define likely(x) __builtin_expect(!!(x), 1)
#endif // likely
#ifndef unlikely
#define unlikely(x) __builtin_expect(!!(x), 0)
#endif // unlikely
#else  // defined(__clang__) || defined(__GNUC__)
#ifndef likely
#define likely(x) x
#endif // likely
#ifndef unlikely
#define unlikely(x) x
#endif // unlikely
#endif // defined(__clang__) || defined(__GNUC__)
