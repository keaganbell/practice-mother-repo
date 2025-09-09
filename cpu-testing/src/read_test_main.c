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

typedef void read_test_func_ptr(u64 buffer_size, u8 *buffer);
typedef struct {
    const char *name;
    read_test_func_ptr *func;
} read_test_func;

extern void read_1x1(u64 buffer_size, u8 *buffer);
extern void read_1x2(u64 buffer_size, u8 *buffer);
extern void read_2x1(u64 buffer_size, u8 *buffer);
extern void read_2x2(u64 buffer_size, u8 *buffer);
extern void read_4x1(u64 buffer_size, u8 *buffer);
extern void read_4x2(u64 buffer_size, u8 *buffer);
extern void read_8x2(u64 buffer_size, u8 *buffer);
extern void read_16x2(u64 buffer_size, u8 *buffer);
extern void read_32x2(u64 buffer_size, u8 *buffer);
#pragma comment (lib, "read_test")

read_test_func read_functions[] = {
    { "read_1x1",  read_1x1  },
    { "read_1x2",  read_1x2  },
    { "read_2x1",  read_2x1  },
    { "read_2x2",  read_2x2  },
    { "read_4x1",  read_4x1  },
    { "read_4x2",  read_4x2  },
    { "read_8x2",  read_8x2  },
    { "read_16x2", read_16x2 },
    { "read_32x2", read_32x2 },
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

static void tester_print_report(cpu_tester *tester, const char *name) {
    fprintf(stdout, "test %s\n", name);
    fprintf(stdout, "  trial count:  %d\n", tester->trial_count);
    f64 total_time = (f64)tester->timer_accumulated/tester->cpu_frequency;
    f64 avg_time = (f64)total_time/tester->trial_count;
    fprintf(stdout, "  average time: %f\n", avg_time);
    f64 read_speed = (f64)(1*GiB)/avg_time;
    fprintf(stdout, "  read speed:   %f GiB/s\n\n", (f64)read_speed/GiB);
}

int main() {
    u64 buffer_size = 1*GiB;
    u8 *buffer = (u8 *)platform_alloc(buffer_size);
    for (u32 i = 0; i < array_count(read_functions); ++i) {
        cpu_tester tester = {};
        while(tester_is_running(&tester)) {
            tester_start_timer(&tester);
            read_functions[i].func(buffer_size, buffer);
            tester_end_timer(&tester);
        }
        tester_print_report(&tester, read_functions[i].name);
    }
    getchar();
    return 0;
}
