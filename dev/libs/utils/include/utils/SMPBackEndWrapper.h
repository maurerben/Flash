//
// Created by bene on 3/27/25.
//

#pragma once


// This header provides a unified parallel backend abstraction layer.
// It supports TBB, OpenMP, or falls back to serial execution.

// Usage:
// parallel_for(start, end, [](index_t i) { ... });//
// parallel_for_2d(rows, cols, [](index_t i, index_t j) { ... });


#include <functional>
#include <utils/types.h>

namespace flashlight{
namespace utils {

#if defined(USE_TBB)

#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>
#include <tbb/blocked_range2d.h>

#elif defined(_OPENMP)

#include <omp.h>

#endif

template <typename F>
inline void parallel_for(index_t start, index_t end, F&& func) {
#if defined(USE_TBB)
    tbb::parallel_for(
        tbb::blocked_range<index_t>(start, end),
        [&](const tbb::blocked_range<index_t>& range) {
            for (index_t i = range.begin(); i < range.end(); ++i) {
                func(i);
            }
        });
#elif defined(_OPENMP)
    #pragma omp parallel for schedule(static)
    for (index_t i = start; i < end; ++i) {
        func(i);
    }
#else
    for (index_t i = start; i < end; ++i) {
        func(i);
    }
#endif
}

template <typename F>
inline void parallel_for_2d(index_t rows, index_t cols, F&& func) {
#if defined(USE_TBB)
    tbb::parallel_for(
    tbb::blocked_range2d<index_t>(0, rows, 0, cols),
    [&](const tbb::blocked_range2d<index_t>& range) {
        for (index_t i = range.rows().begin(); i < range.rows().end(); ++i) {
            for (index_t j = range.cols().begin(); j < range.cols().end(); ++j) {
                func(i, j);
            }
        }
    });
#elif defined(_OPENMP)
    #pragma omp parallel for collapse(2) schedule(static)
        for (index_t i = 0; i < rows; ++i) {
            for (index_t j = 0; j < cols; ++j) {
                func(i, j);
            }
        }
    }
#else
    for (index_t i = 0; i < rows; ++i) {
        for (index_t j = 0; j < cols; ++j) {
            func(i, j);
        }
    }
#endif
}



inline int get_thread_id() {
#if defined(USE_TBB)
    return oneapi::tbb::this_task_arena::current_thread_index();
#elif defined(_OPENMP)
    return omp_get_thread_num();
#else
    return 0; // Serial fallback
#endif
}

inline int get_num_threads() {
#if defined(USE_TBB)
    return oneapi::tbb::this_task_arena::max_concurrency();
#elif defined(_OPENMP)
    return omp_get_max_threads();
#else
    return 1;
#endif
}

template <typename Container, typename Predicate>
std::vector<typename Container::value_type>
filter_parallel(const Container& container, Predicate&& pred) {
    using T = typename Container::value_type;
    const index_t n = container.size();
    const int num_threads = get_num_threads();

    std::vector<std::vector<T>> thread_local_buffers(num_threads);

    parallel_for(0, n, [&](index_t i) {
        if (pred(container[i])) {
            thread_local_buffers[get_thread_id()].push_back(container[i]);
        }
    });

    std::vector<T> result;
    for (const auto& local : thread_local_buffers) {
        result.insert(result.end(), local.begin(), local.end());
    }

    return result;
}

}
}
