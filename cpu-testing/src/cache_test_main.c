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

typedef void cache_test_func_ptr(u64 buffer_size, u8 *buffer, u64 mask);
typedef struct {
    const char *name;
    cache_test_func_ptr *func;
} cache_test_func;


extern void cache_read(u64 buffer_size, u8 *buffer, u64 mask);
#pragma comment (lib, "cache_test")

cache_test_func read_functions[] = {
    { "cache_read", cache_read }
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

static void tester_print_report(cpu_tester *tester, const char *name, u64 mask) {
    (void)name;
    f64 total_time = (f64)tester->timer_accumulated/tester->cpu_frequency;
    f64 avg_time = (f64)total_time/tester->trial_count;
    f64 read_speed = (f64)(1*GiB)/avg_time;
    fprintf(stdout, "%f, %f\n", (f64)mask/GiB, (f64)read_speed/GiB);
}

int main() {
    u64 masks[] = {
        0x1fff, 0x3fff, 0x5fff, 0x7fff, 0x9fff, 0xbfff, 0xdfff, 0xffff,
        0x1ffff, 0x3ffff, 0x5ffff, 0x7ffff, 0x9ffff, 0xbffff, 0xdffff, 0xfffff,
        0x1fffff, 0x3fffff, 0x5fffff, 0x7fffff, 0x9fffff, 0xbfffff, 0xdfffff, 0xffffff,
        0x1ffffff, 0x3ffffff, 0x5ffffff, 0x7ffffff, 0x9ffffff, 0xbffffff, 0xdffffff, 0xfffffff,
        0x1fffffff, 0x3fffffff
    };
    u64 buffer_size = 1*GiB;
    u8 *buffer = (u8 *)platform_alloc(buffer_size);
    fprintf(stdout, "size, speed\n");
    for (u32 i = 0; i < array_count(masks); ++i) {
        cpu_tester tester = {};
        tester.target_trial_count = 100;
        while(tester_is_running(&tester)) {
            tester_start_timer(&tester);
            read_functions[0].func(buffer_size, buffer, masks[i]);
            tester_end_timer(&tester);
        }
        tester_print_report(&tester, read_functions[0].name, masks[i]);
    }
    return 0;
}
