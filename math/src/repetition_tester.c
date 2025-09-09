
typedef struct {
    const char *name;
    test_func_ptr *func;
} test_func;

#define DEFAULT_TARGET_TRIAL_COUNT 100000
typedef struct {
    u32 target_trial_count;
    u32 trial_count;
    u64 timer_start;
    u64 timer_accumulated;
    u64 timer_elapsed_this_trial;
} repitition_tester;

static b32 tester_is_running(cpu_tester *tester) {
    b32 result = true;
    if (tester->target_trial_count == 0) {
        tester->target_trial_count = DEFAULT_TARGET_TRIAL_COUNT;
    }
    tester->timer_accumulated += tester->timer_elapsed_this_trial;
    ++tester->trial_count;
    if (tester->trial_count == tester->target_trial_count) {
        result = false;
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
