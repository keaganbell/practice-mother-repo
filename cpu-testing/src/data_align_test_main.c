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

typedef void aligned_read_func_ptr(u64 buffer_size, u8 *buffer);
typedef struct {
    const char *name;
    aligned_read_func_ptr *func;
} aligned_read_func;


extern void aligned_read_32x8(u64 buffer_size, u8 *buffer);
#pragma comment (lib, "alignment_test")

aligned_read_func aligned_read_funcs[] = {
    { "aligned_read_32x8", aligned_read_32x8 }
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

static void tester_print_report(cpu_tester *tester, u64 byte_count, const char *name, u64 alignment) {
    (void)name;
    f64 total_time = (f64)tester->timer_accumulated/tester->cpu_frequency;
    f64 avg_time = (f64)total_time/tester->trial_count;
    f64 read_speed = (f64)byte_count/avg_time;
    fprintf(stdout, "%llu, %f\n", alignment, (f64)read_speed/GiB);
}

int main() {
    u64 alignments[] = { 0, 1, 2, 3, 15, 16, 17, 31, 32, 33, 63, 64, 65 };
    u64 buffer_size = 32*MiB;
    u8 *buffer = (u8 *)platform_alloc(buffer_size + 256);
    fprintf(stdout, "alignment, speed\n");
    for (u32 i = 0; i < array_count(alignments); ++i) {
        cpu_tester tester = {};
        tester.target_trial_count = 100000;
        while(tester_is_running(&tester)) {
            tester_start_timer(&tester);
            aligned_read_funcs[0].func(buffer_size, buffer + alignments[i]);
            tester_end_timer(&tester);
        }
        tester_print_report(&tester, buffer_size, aligned_read_funcs[0].name, alignments[i]);
    }
    return 0;
}
