#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "unity.h"

#define SCONSOLE_WANT_INTERNAL_DEFS
#include "sconsole.h"

#include "t_support.h"

/* To make tests for r stack:
	copy file to test_r_stack.c
	replace testU -> testR
	replace u_ -> r_
	replace SC_U_STACK_SIZE -> SC_R_STACK_SIZE
*/
//TODO: Automate!

TT_BEGIN_FIXTURE(ts_SetupStackTestContext, NULL, ts_DestroyStackTestContext);

void testUStackInit(void) {
    // Check guards are contiguous. 
    TEST_ASSERT_EQUAL_PTR(&CTX->t_u_stack_pre+1, &CTX->u_stack[0]);
    TEST_ASSERT_EQUAL_PTR(&CTX->t_u_stack_post-1, &CTX->u_stack[SC_U_STACK_SIZE-1]);

	// Check stack initialised by fixture.
    TEST_ASSERT_EQUAL(0, u_depth());
    TEST_ASSERT_EQUAL(SC_U_STACK_SIZE, u_size());
}

void testUStackSize(void) {
	TEST_ASSERT_EQUAL(SC_U_STACK_SIZE, u_size());
}

void testUStackPopPush(void) {
    for (int i = 0; i < SC_U_STACK_SIZE; i += 1) {
        u_push(i);
        TEST_ASSERT_EQUAL(i+1, u_depth());
        TEST_ASSERT_EQUAL(SC_U_STACK_SIZE - i - 1, u_free());
    }
    for (int i = 0; i < SC_U_STACK_SIZE; i += 1) {
        TEST_ASSERT_EQUAL_UINT32(SC_U_STACK_SIZE - i - 1, u_pop());
        TEST_ASSERT_EQUAL(SC_U_STACK_SIZE - i - 1, u_depth());
        TEST_ASSERT_EQUAL(i + 1, u_free());
    }
}

void testUStackReset(void) {
    u_clear();
    TEST_ASSERT_EQUAL(0, u_depth());
    u_push(1);
    u_clear();
    TEST_ASSERT_EQUAL(0, u_depth());

    for (int i = 0; i < SC_U_STACK_SIZE; i += 1) 
        u_push(i);
    TEST_ASSERT_EQUAL(SC_U_STACK_SIZE, u_depth());
    u_clear();
    TEST_ASSERT_EQUAL(0, u_depth());
}

void testUStackPeek(void) {
    for (int i = 0; i < SC_U_STACK_SIZE; i += 1)
        u_push(i);

    for (int i = 0; i < SC_U_STACK_SIZE; i += 1)
        TEST_ASSERT_EQUAL(SC_U_STACK_SIZE - i - 1, u_peek(i));
}

void testUStackTosNos(void) {
    u_push(0);
    TEST_ASSERT_EQUAL_UINT32(0, u_tos);

    for (int i = 1; i < SC_U_STACK_SIZE; i += 1) {
        u_push(i);
        TEST_ASSERT_EQUAL_UINT32(i - 1, u_nos);
        TEST_ASSERT_EQUAL_UINT32(i, u_tos);
    }
}

void testUStackCanPop(void) {
    TEST_ASSERT_TRUE(u_can_pop(0));
    TEST_ASSERT_FALSE(u_can_pop(1));
    TEST_ASSERT_FALSE(u_can_pop(SC_U_STACK_SIZE));
    TEST_ASSERT_FALSE(u_can_pop(SC_U_STACK_SIZE + 1));

    u_push(33);
    TEST_ASSERT_TRUE(u_can_pop(0));
    TEST_ASSERT_TRUE(u_can_pop(1));
    TEST_ASSERT_FALSE(u_can_pop(2));
    TEST_ASSERT_FALSE(u_can_pop(SC_U_STACK_SIZE));
    TEST_ASSERT_FALSE(u_can_pop(SC_U_STACK_SIZE + 1));

    for (int i = 1; i < SC_U_STACK_SIZE - 1; i += 1) 
        u_push(i + 33);
    TEST_ASSERT_TRUE(u_can_pop(SC_U_STACK_SIZE - 1));
    TEST_ASSERT_FALSE(u_can_pop(SC_U_STACK_SIZE));
    TEST_ASSERT_FALSE(u_can_pop(SC_U_STACK_SIZE + 1));

    u_push(333);
    TEST_ASSERT_TRUE(u_can_pop(SC_U_STACK_SIZE));
    TEST_ASSERT_FALSE(u_can_pop(SC_U_STACK_SIZE + 1));
}

void testUStackCanPush(void) {
    TEST_ASSERT_TRUE(u_can_push(0));
    TEST_ASSERT_TRUE(u_can_push(1));
    TEST_ASSERT_TRUE(u_can_push(SC_U_STACK_SIZE));
    TEST_ASSERT_FALSE(u_can_push(SC_U_STACK_SIZE + 1));

    u_push(33);
    TEST_ASSERT_TRUE(u_can_push(SC_U_STACK_SIZE - 1));
    TEST_ASSERT_FALSE(u_can_push(SC_U_STACK_SIZE));

    for (int i = 1; i < SC_U_STACK_SIZE - 1; i += 1)
        u_push(i + 33);
    TEST_ASSERT_TRUE(u_can_push(0));
    TEST_ASSERT_TRUE(u_can_push(1));
    TEST_ASSERT_FALSE(u_can_push(2));

    u_push(333);
    TEST_ASSERT_TRUE(u_can_push(0));
    TEST_ASSERT_FALSE(u_can_push(1));
}

