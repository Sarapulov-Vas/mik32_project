#include "mock_mik32_hal_adc.h"
#include "unity.h"
#include "utilities.h"

void test_int_to_str_should_convert_correctly(void) {
    char str[5];
    int_to_str(str, 123);
    TEST_ASSERT_EQUAL_STRING("123", str);
}

void test_POT_TO_VALUE_should_scale_correctly(void) {
    uint32_t value = POT_TO_VALUE(2000, 0, 180, 2);
    TEST_ASSERT_EQUAL_UINT32(84, value);
}
