#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <intrin.h>

#define _CRT_NONSTDC_NO_WARNINGS
#include <stdio.h>

#include "base.h"
#include "win32_platform.c"

typedef enum {
    Tester_Status_uninitialized,
    Tester_Status_in_progress,
    Tester_Status_completed,
} tester_status;

typedef void test_func_ptr(u64 read_count, u8 *buffer, u64 cache_lines_per_read, u64 stride);
typedef struct {
    const char *name;
    test_func_ptr *func;
} test_func;


extern void cache_index_read(u64 read_count, u8 *buffer, u64 cache_lines_per_read, u64 stride);
#pragma comment (lib, "cache_index_test")

test_func test_funcs[] = {
    { "cache_index_read", cache_index_read }
};

#define DEFAULT_TARGET_TRIAL_COUNT 10
typedef struct {
    tester_status status;
    u64 cpu_frequency;
    u32 target_trial_count;
    u32 trial_count;
    u64 timer_start;
    u64 timer_accumulated;
    u64 timer_elapsed_this_trial;
} cpu_tester;

static b32 tester_is_running(cpu_tester *tester) {
    b32 result = true;
    switch (tester->status) {
        case Tester_Status_uninitialized: {
            tester->status = Tester_Status_in_progress;
            tester->cpu_frequency = win32_estimate_cpu_frequency();
            if (tester->target_trial_count == 0) {
                tester->target_trial_count = DEFAULT_TARGET_TRIAL_COUNT;
            }
        } break;

        case Tester_Status_in_progress: {
            ++tester->trial_count;
            tester->timer_accumulated += tester->timer_elapsed_this_trial;
            if (tester->trial_count == tester->target_trial_count) {
                tester->status = Tester_Status_completed;
                result = false;
            }
        } break;

        case Tester_Status_completed: {
            result = false;
        } break;

        default: {
            // invalid default case
            kab_assert(0);
        }
    }
    return result;
}

static void tester_start_timer(cpu_tester *tester) {
    tester->timer_start = win32_read_cpu_timer();
}

static void tester_end_timer(cpu_tester *tester) {
    u64 timer_end = win32_read_cpu_timer();
    tester->timer_elapsed_this_trial = timer_end - tester->timer_start;
}

static void tester_print_report(cpu_tester *tester, u64 byte_count, const char *name, u64 stride) {
    (void)name;
    f64 total_time = (f64)tester->timer_accumulated/tester->cpu_frequency;
    f64 avg_time = (f64)total_time/tester->trial_count;
    f64 read_speed = (f64)byte_count/avg_time;
    fprintf(stdout, "%llu, %f\n", stride, (f64)read_speed/GiB);
}

int main() {
    cpu_tester testers[128] = {};
    u64 cache_line_size = 64;
    u64 buffer_size = 1*GiB;
    u8 *buffer = (u8 *)platform_alloc(buffer_size);
    fprintf(stdout, "stride (cache lines), speed (GiB/s)\n");
    for (u32 i = 0; i < array_count(testers); ++i) {
        cpu_tester *tester = &testers[i];
        tester->target_trial_count = 100000;
        u64 stride = cache_line_size*i;
        u64 read_count = 64;
        u64 cache_lines_per_read = 256;
        u64 byte_count = read_count*cache_lines_per_read*cache_line_size;
        while(tester_is_running(tester)) {
            tester_start_timer(tester);
            test_funcs[0].func(read_count, buffer, cache_lines_per_read, stride);
            tester_end_timer(tester);
        }
        tester_print_report(tester, byte_count, test_funcs[0].name, i);
    }
    return 0;
}

