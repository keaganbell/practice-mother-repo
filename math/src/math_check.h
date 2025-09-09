typedef struct {
    f64 input;
    f64 output;
} io_pair;

typedef struct {
    // for computing the avg
    // difference over the range
    f64 total_diff;
    f64 max_diff;
    u32 diff_count;
    
    f64 input_value_at_max_diff;
    f64 output_value_at_max_diff;
    f64 expected_value_at_max_diff;
    
    char label[64];
} math_test_result;

typedef struct {
    math_test_result results[256];
    math_test_result error_result;
    
    u32 result_count;
    u32 progress_result_count;
    
    b32 testing;
    u32 step_index;
    u32 result_offset;
    
    f64 input_value;
} math_tester;

typedef struct {
    string_c8 name;
    void *func;
} named_function;