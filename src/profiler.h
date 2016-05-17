// Copyright (c) 2015-2016 Sergio Gonzalez. All rights reserved.
// License: https://github.com/serge-rgb/milton#license

#pragma once


#include "milton_configuration.h"
#include "common.h"

#if defined(__cplusplus)
extern"C"{
#endif

// Profiler
//
// The PROFILE_RASTER_BEGIN and PROFILE_RASTER_PUSH macros report the rasterizer performance
// to the Windows console output.
//
// The PROFILE_GRAPH_BEGIN and PROFILE_GRAPH_PUSH macros write to the
// g_profiler_last array.
//
//
enum
{
    PROF_RASTER_render_canvas,
    PROF_RASTER_sse2,
    PROF_RASTER_preamble,
    PROF_RASTER_load,
    PROF_RASTER_work,
    PROF_RASTER_gather,
    PROF_RASTER_sampling,
    PROF_RASTER_total_work_loop,
    PROF_RASTER_sample,


    PROF_RASTER_COUNT, // Number of raster profiler counters for Win32 Console output


    PROF_GRAPH_polling=0,     // Time spent gathering input.
    PROF_GRAPH_update,      // Time spent processing input.
    PROF_GRAPH_raster,      // Time spent rasterizing the canvas.
    PROF_GRAPH_GL,          // Time spent sending data and draw commands to GL

    PROF_COUNT = PROF_RASTER_COUNT+4,
    //PROF_COUNT,

};



static char* g_profiler_names[PROF_RASTER_COUNT] =
{
    "render_canvas",
    "sse2",
    "preamble",
    "load",
    "work",
    "gather",
    "sampling",
    "total_work_loop",
    "sample",
};

void profiler_init();
void profiler_output();
void profiler_reset();

#if defined(_WIN64) && MILTON_ENABLE_PROFILING

#define PROFILER_IMPLEMENTATION

u64 g_profiler_ticks[PROF_COUNT];     // Total cpu clocks
u64 g_profiler_last[PROF_COUNT];
u64 g_graph_last[PROF_COUNT-PROF_RASTER_COUNT]; // Visualization
u64 g_profiler_count[PROF_COUNT];     // How many calls


static u32 TSC_AUX;
static int CPUID_AUX1[4];
static int CPUID_AUX2;

#define PROFILE_RASTER_BEGIN(name) __cpuid(CPUID_AUX1, CPUID_AUX2); u64 profile_##name##_start = __rdtsc();
#define PROFILE_RASTER_PUSH_(name, start)\
    g_profiler_count[PROF_RASTER_##name] += 1;\
    u64 profile_##name##_ncycles = __rdtscp(&TSC_AUX) - start; \
    g_profiler_ticks[PROF_RASTER_##name] += profile_##name##_ncycles; \
    g_profiler_last[PROF_RASTER_##name] = profile_##name##_ncycles;

#define PROFILE_RASTER_PUSH(name) PROFILE_RASTER_PUSH_(name, profile_##name##_start)
/////////
#define PROFILE_GRAPH_BEGIN(name) \
        u64 graph_##name##_start = perf_counter()
        //u64 graph_##name##_start = __rdtsc();
#define PROFILE_GRAPH_PUSH(name)  \
        g_graph_last[PROF_GRAPH_##name] = perf_counter() - graph_##name##_start
        //g_graph_last[PROF_GRAPH_##name] = __rdtsc() - graph_##name##_start

#else

#define PROFILE_RASTER_BEGIN(name)
#define PROFILE_RASTER_PUSH(name)
/////////
#define PROFILE_GRAPH_BEGIN(name)
#define PROFILE_GRAPH_PUSH(name)

#endif

#if defined(__cplusplus)
}
#endif
