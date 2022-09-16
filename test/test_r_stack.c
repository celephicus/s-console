#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "unity.h"

#define SCONSOLE_WANT_INTERNAL_DEFS
#include "sconsole.h"

#include "t_support.h"

TT_BEGIN_FIXTURE(ts_SetupStackTestContext, NULL, ts_DestroyStackTestContext);

void testR_StackInit(void) {
    // Check guards are contiguous. 
    TEST_ASSERT_EQUAL_PTR(&CTX->t_r_stack_pre+1, &CTX->r_stack[0]);
    TEST_ASSERT_EQUAL_PTR(&CTX->t_r_stack_post-1, &CTX->r_stack[SC_R_STACK_SIZE-1]);

	// Check stack initialised by fixture.
    TEST_ASSERT_EQUAL(0, r_depth());
    TEST_ASSERT_EQUAL(SC_R_STACK_SIZE, r_size());
}

void testR_StackPopPush(void) {
    for (int i = 0; i < SC_R_STACK_SIZE; i += 1) {
        r_push(i);
        TEST_ASSERT_EQUAL(i+1, r_depth());
        TEST_ASSERT_EQUAL(SC_R_STACK_SIZE - i - 1, r_free());
    }
    for (int i = 0; i < SC_R_STACK_SIZE; i += 1) {
        TEST_ASSERT_EQUAL_UINT32(SC_R_STACK_SIZE - i - 1, r_pop());
        TEST_ASSERT_EQUAL(SC_R_STACK_SIZE - i - 1, r_depth());
        TEST_ASSERT_EQUAL(i + 1, r_free());
    }
}

void testR_StackReset(void) {
    r_reset();
    TEST_ASSERT_EQUAL(0, r_depth());
    r_push(1);
    r_reset();
    TEST_ASSERT_EQUAL(0, r_depth());

    for (int i = 0; i < SC_R_STACK_SIZE; i += 1) 
        r_push(i);
    TEST_ASSERT_EQUAL(SC_R_STACK_SIZE, r_depth());
    r_reset();
    TEST_ASSERT_EQUAL(0, r_depth());
}

void testR_StackPeek(void) {
    for (int i = 0; i < SC_R_STACK_SIZE; i += 1)
        r_push(i);

    for (int i = 0; i < SC_R_STACK_SIZE; i += 1)
        TEST_ASSERT_EQUAL(SC_R_STACK_SIZE - i - 1, r_peek(i));
}

void testR_StackTosNos(void) {
    r_push(0);
    TEST_ASSERT_EQUAL_UINT32(0, r_tos);

    for (int i = 1; i < SC_R_STACK_SIZE; i += 1) {
        r_push(i);
        TEST_ASSERT_EQUAL_UINT32(i - 1, r_nos);
        TEST_ASSERT_EQUAL_UINT32(i, r_tos);
    }
}

void testR_StackCanPop(void) {
    TEST_ASSERT_TRUE(r_can_pop(0));
    TEST_ASSERT_FALSE(r_can_pop(1));
    TEST_ASSERT_FALSE(r_can_pop(SC_R_STACK_SIZE));
    TEST_ASSERT_FALSE(r_can_pop(SC_R_STACK_SIZE + 1));

    r_push(33);
    TEST_ASSERT_TRUE(r_can_pop(0));
    TEST_ASSERT_TRUE(r_can_pop(1));
    TEST_ASSERT_FALSE(r_can_pop(2));
    TEST_ASSERT_FALSE(r_can_pop(SC_R_STACK_SIZE));
    TEST_ASSERT_FALSE(r_can_pop(SC_R_STACK_SIZE + 1));

    for (int i = 1; i < SC_R_STACK_SIZE - 1; i += 1) 
        r_push(i + 33);
    TEST_ASSERT_TRUE(r_can_pop(SC_R_STACK_SIZE - 1));
    TEST_ASSERT_FALSE(r_can_pop(SC_R_STACK_SIZE));
    TEST_ASSERT_FALSE(r_can_pop(SC_R_STACK_SIZE + 1));

    r_push(333);
    TEST_ASSERT_TRUE(r_can_pop(SC_R_STACK_SIZE));
    TEST_ASSERT_FALSE(r_can_pop(SC_R_STACK_SIZE + 1));
}

void testR_StackCanPush(void) {
    TEST_ASSERT_TRUE(r_can_push(0));
    TEST_ASSERT_TRUE(r_can_push(1));
    TEST_ASSERT_TRUE(r_can_push(SC_R_STACK_SIZE));
    TEST_ASSERT_FALSE(r_can_push(SC_R_STACK_SIZE + 1));

    r_push(33);
    TEST_ASSERT_TRUE(r_can_push(SC_R_STACK_SIZE - 1));
    TEST_ASSERT_FALSE(r_can_push(SC_R_STACK_SIZE));

    for (int i = 1; i < SC_R_STACK_SIZE - 1; i += 1)
        r_push(i + 33);
    TEST_ASSERT_TRUE(r_can_push(0));
    TEST_ASSERT_TRUE(r_can_push(1));
    TEST_ASSERT_FALSE(r_can_push(2));

    r_push(333);
    TEST_ASSERT_TRUE(r_can_push(0));
    TEST_ASSERT_FALSE(r_can_push(1));
}

