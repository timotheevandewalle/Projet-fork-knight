/* Unity Test Framework - Simplified Version
 * Original: ThrowTheSwitch.org
 * This is a minimal implementation for educational purposes
 */

#ifndef UNITY_H
#define UNITY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>

// Test counters
typedef struct {
    int tests_run;
    int tests_passed;
    int tests_failed;
    jmp_buf abort_frame;
    int abort_enabled;
} UnityState;

extern UnityState Unity;

// Color codes for terminal output
#define UNITY_COLOR_GREEN   ""
#define UNITY_COLOR_RED     ""
#define UNITY_COLOR_YELLOW  ""
#define UNITY_COLOR_RESET   ""

// Macros
#define UNITY_BEGIN() Unity_Begin()
#define UNITY_END() Unity_End()
#define RUN_TEST(func) Unity_RunTest(func, #func)

#define TEST_ASSERT(condition) \
    do { if (!(condition)) Unity_Fail(__LINE__, "Assertion failed: " #condition); } while(0)

#define TEST_ASSERT_TRUE(condition) TEST_ASSERT(condition)
#define TEST_ASSERT_FALSE(condition) TEST_ASSERT(!(condition))
#define TEST_ASSERT_NULL(ptr) TEST_ASSERT((ptr) == NULL)
#define TEST_ASSERT_NOT_NULL(ptr) TEST_ASSERT((ptr) != NULL)
#define TEST_ASSERT_EQUAL_INT(expected, actual) \
    do { if ((expected) != (actual)) Unity_FailInt(__LINE__, expected, actual); } while(0)
#define TEST_ASSERT_EQUAL_STRING(expected, actual) \
    do { if (strcmp((expected), (actual)) != 0) Unity_FailString(__LINE__, expected, actual); } while(0)
#define TEST_ASSERT_EQUAL_PTR(expected, actual) \
    do { if ((expected) != (actual)) Unity_FailPtr(__LINE__, expected, actual); } while(0)

// Function prototypes
void Unity_Begin(void);
int Unity_End(void);
void Unity_RunTest(void (*func)(void), const char* name);
void Unity_Fail(int line, const char* msg);
void Unity_FailInt(int line, int expected, int actual);
void Unity_FailString(int line, const char* expected, const char* actual);
void Unity_FailPtr(int line, const void* expected, const void* actual);

#endif // UNITY_H
