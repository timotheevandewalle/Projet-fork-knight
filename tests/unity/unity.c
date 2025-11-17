/* Unity Test Framework - Implementation */

#include "unity.h"

UnityState Unity;

void Unity_Begin(void) {
    Unity.tests_run = 0;
    Unity.tests_passed = 0;
    Unity.tests_failed = 0;
    Unity.abort_enabled = 0;
    
    printf("\n");
    printf("========================================\n");
    printf("   UNITY TEST FRAMEWORK\n");
    printf("========================================\n\n");
}

int Unity_End(void) {
    printf("\n");
    printf("========================================\n");
    printf("   TEST SUMMARY\n");
    printf("========================================\n");
    printf("Tests Run:    %d\n", Unity.tests_run);
    printf("%sPassed:%s       %d\n", UNITY_COLOR_GREEN, UNITY_COLOR_RESET, Unity.tests_passed);
    printf("%sFailed:%s       %d\n", UNITY_COLOR_RED, UNITY_COLOR_RESET, Unity.tests_failed);
    printf("========================================\n\n");
    
    if (Unity.tests_failed == 0) {
        printf("%s✓ ALL TESTS PASSED!%s\n\n", UNITY_COLOR_GREEN, UNITY_COLOR_RESET);
        return 0;
    } else {
        printf("%s✗ SOME TESTS FAILED%s\n\n", UNITY_COLOR_RED, UNITY_COLOR_RESET);
        return 1;
    }
}

void Unity_RunTest(void (*func)(void), const char* name) {
    Unity.tests_run++;
    
    printf("Running: %s ... ", name);
    fflush(stdout);
    
    // Simple exception handling with setjmp/longjmp
    if (setjmp(Unity.abort_frame) == 0) {
        Unity.abort_enabled = 1;
        func();
        Unity.abort_enabled = 0;
        
        Unity.tests_passed++;
        printf("%s✓ PASS%s\n", UNITY_COLOR_GREEN, UNITY_COLOR_RESET);
    } else {
        // Test failed (longjmp was called)
        Unity.abort_enabled = 0;
        Unity.tests_failed++;
        // Error message already printed by Unity_Fail*
    }
}

void Unity_Fail(int line, const char* msg) {
    printf("\n");
    printf("%s✗ FAIL%s\n", UNITY_COLOR_RED, UNITY_COLOR_RESET);
    printf("  Line %d: %s\n", line, msg);
    
    if (Unity.abort_enabled) {
        longjmp(Unity.abort_frame, 1);
    }
}

void Unity_FailInt(int line, int expected, int actual) {
    printf("\n");
    printf("%s✗ FAIL%s\n", UNITY_COLOR_RED, UNITY_COLOR_RESET);
    printf("  Line %d: Expected %d but was %d\n", line, expected, actual);
    
    if (Unity.abort_enabled) {
        longjmp(Unity.abort_frame, 1);
    }
}

void Unity_FailString(int line, const char* expected, const char* actual) {
    printf("\n");
    printf("%s✗ FAIL%s\n", UNITY_COLOR_RED, UNITY_COLOR_RESET);
    printf("  Line %d:\n", line);
    printf("    Expected: \"%s\"\n", expected);
    printf("    Actual:   \"%s\"\n", actual);
    
    if (Unity.abort_enabled) {
        longjmp(Unity.abort_frame, 1);
    }
}

void Unity_FailPtr(int line, const void* expected, const void* actual) {
    printf("\n");
    printf("%s✗ FAIL%s\n", UNITY_COLOR_RED, UNITY_COLOR_RESET);
    printf("  Line %d:\n", line);
    printf("    Expected: %p\n", expected);
    printf("    Actual:   %p\n", actual);
    
    if (Unity.abort_enabled) {
        longjmp(Unity.abort_frame, 1);
    }
}
