﻿#pragma once

#define RESUMEF_LIB_INCLUDE	1

//#include <yvals.h>
#include <atomic>
#include <memory>
#include <chrono>
#include <thread>
#include <list>
#include <vector>
#include <deque>
#include <mutex>
#include <functional>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <optional>

#include <assert.h>

#include <experimental/coroutine>
//#include <experimental/generator>
//替代<experimental/generator>，因为VS2015/VS2017的generator<>未实现return_value，导致yield后不能return
#include "generator.h"		

#if defined(RESUMEF_DLL_EXPORT)
#	define RF_API __declspec(dllexport)
#elif defined(RESUMEF_DLL_IMPORT)
#	define RF_API __declspec(dllimport)
#else
#	define RF_API
#endif

#if RESUMEF_DEBUG_COUNTER
extern std::mutex g_resumef_cout_mutex;
extern std::atomic<intptr_t> g_resumef_state_count;
extern std::atomic<intptr_t> g_resumef_task_count;
extern std::atomic<intptr_t> g_resumef_evtctx_count;
extern std::atomic<intptr_t> g_resumef_state_id;
#endif

namespace std
{
#if !_HAS_CXX20
	template<class T>
	struct remove_cvref
	{
		typedef std::remove_cv_t<std::remove_reference_t<T>> type;
	};
#if _HAS_CXX17
	template<class T>
	using remove_cvref_t = typename std::remove_cvref<T>::type;
#endif
#endif
}

namespace resumef
{
#if _HAS_CXX17
	template<class... _Mutexes>
	using scoped_lock = std::scoped_lock<_Mutexes...>;
#else
	template<class... _Mutexes>
	using scoped_lock = std::lock_guard<_Mutexes...>;
#endif

	template<typename _PromiseT = void>
	using coroutine_handle = std::experimental::coroutine_handle<_PromiseT>;
	template <typename _Ty = std::nullptr_t, typename _Alloc = std::allocator<char>>
	using generator_t = std::experimental::generator<_Ty, _Alloc>;

	enum struct error_code
	{
		none,
		not_ready,			// get_value called when value not available
		already_acquired,	// attempt to get another future
		unlock_more,		// unlock 次数多余lock次数
		read_before_write,	// 0容量的channel，先读后写
		timer_canceled,		// 定时器被意外取消

		max__
	};

	const char * get_error_string(error_code fe, const char * classname);

	struct future_exception : std::exception
	{
		error_code _error;
		future_exception(error_code fe) 
			: exception(get_error_string(fe, "future_exception"))
			, _error(fe)
		{
		}
	};

	struct lock_exception : std::exception
	{
		error_code _error;
		lock_exception(error_code fe)
			: exception(get_error_string(fe, "lock_exception"))
			, _error(fe)
		{
		}
	};

	struct channel_exception : std::exception
	{
		error_code _error;
		channel_exception(error_code fe)
			: exception(get_error_string(fe, "channel_exception"))
			, _error(fe)
		{
		}
	};

	struct timer_canceled_exception : public std::exception
	{
		error_code _error;
		timer_canceled_exception(error_code fe)
			: exception(get_error_string(fe, "timer canceled"))
			, _error(fe)
		{
		}
	};

	struct scheduler_t;

	struct state_base_t;

	template<class _Ty = void>
	struct future_t;
	
	using future_vt [[deprecated]] = future_t<>;

	template<class _Ty = void>
	struct promise_t;

	template<class _Ty = void>
	struct awaitable_t;

	template<class _PromiseT>
	struct is_promise : std::false_type {};
	template<class _Ty>
	struct is_promise<promise_t<_Ty>> : std::true_type {};
	template<class _Ty>
	_INLINE_VAR constexpr bool is_promise_v = is_promise<std::remove_cvref_t<_Ty>>::value;

	template<class _PromiseT>
	struct is_future : std::false_type {};
	template<class _Ty>
	struct is_future<future_t<_Ty>> : std::true_type {};
	template<class _Ty>
	_INLINE_VAR constexpr bool is_future_v = is_future<std::remove_cvref_t<_Ty>>::value;

	template<class _G>
	struct is_generator : std::false_type {};
	template <typename _Ty, typename _Alloc>
	struct is_generator<generator_t<_Ty, _Alloc>> : std::true_type {};
	template<class _Ty>
	_INLINE_VAR constexpr bool is_generator_v = is_generator<std::remove_cvref_t<_Ty>>::value;

	//获得当前线程下的调度器
	scheduler_t* this_scheduler();
}

#define co_yield_void co_yield nullptr
#define co_return_void co_return nullptr
