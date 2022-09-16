#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "unity.h"

#define SCONSOLE_WANT_INTERNAL_DEFS
#include "sconsole.h"

#include "t_support.h"

TT_BEGIN_FIXTURE(ts_SetupStackTestContext, NULL, ts_DestroyStackTestContext);

void testU_StackInit(void) {
    // Check guards are contiguous. 
    TEST_ASSERT_EQUAL_PTR(&CTX->t_u_stack_pre+1, &CTX->u_stack[0]);
    TEST_ASSERT_EQUAL_PTR(&CTX->t_u_stack_post-1, &CTX->u_stack[SC_U_STACK_SIZE-1]);

	// Check stack initialised by fixture.
    TEST_ASSERT_EQUAL(0, u_depth());
    TEST_ASSERT_EQUAL(SC_U_STACK_SIZE, u_size());
}

void testU_StackPopPush(void) {
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

void testU_StackReset(void) {
    u_reset();
    TEST_ASSERT_EQUAL(0, u_depth());
    u_push(1);
    u_reset();
    TEST_ASSERT_EQUAL(0, u_depth());

    for (int i = 0; i < SC_U_STACK_SIZE; i += 1) 
        u_push(i);
    TEST_ASSERT_EQUAL(SC_U_STACK_SIZE, u_depth());
    u_reset();
    TEST_ASSERT_EQUAL(0, u_depth());
}

void testU_StackPeek(void) {
    for (int i = 0; i < SC_U_STACK_SIZE; i += 1)
        u_push(i);

    for (int i = 0; i < SC_U_STACK_SIZE; i += 1)
        TEST_ASSERT_EQUAL(SC_U_STACK_SIZE - i - 1, u_peek(i));
}

void testU_StackTosNos(void) {
    u_push(0);
    TEST_ASSERT_EQUAL_UINT32(0, u_tos);

    for (int i = 1; i < SC_U_STACK_SIZE; i += 1) {
        u_push(i);
        TEST_ASSERT_EQUAL_UINT32(i - 1, u_nos);
        TEST_ASSERT_EQUAL_UINT32(i, u_tos);
    }
}

void testU_StackCanPop(void) {
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

void testU_StackCanPush(void) {
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

void testU_ErrorPop(void) {
    u_pop();
    TEST_ASSERT_EQUAL(SC_FAULT_U_STACK_UFLOW, FAULT);
    FAULT = 0;
}
void testU_ErrorPush(void) {
    for (int i = 0; i < SC_U_STACK_SIZE; i += 1)
        u_push(i + 33);
    u_push(1234);
    TEST_ASSERT_EQUAL(SC_FAULT_U_STACK_OFLOW, FAULT);
    FAULT = 0;
}
void testU_ErrorTos(void) {
    (void)u_tos;
    TEST_ASSERT_EQUAL(SC_FAULT_U_STACK_ACCESS, FAULT);
    FAULT = 0;
}
void testU_ErrorNos(void) {
    u_push(1234);
    (void)u_nos;
    TEST_ASSERT_EQUAL(SC_FAULT_U_STACK_ACCESS, FAULT);
    FAULT = 0;
}
void testU_ErrorPeek(void) {
    u_push(1234);
    u_push(34234);
    u_push(444);
    (void)u_peek(0);
    (void)u_peek(2);
    TEST_ASSERT_EQUAL(SC_FAULT_OK, FAULT);
    (void)u_peek(3);
    TEST_ASSERT_EQUAL(SC_FAULT_U_STACK_ACCESS, FAULT);
    FAULT = 0;
}


TT_IGNORE_FROM_HERE()
#if 0
void tes tHeapAccessByte(void) {
    scHeapWriteByte(ctx, 0, 123); TEST_ASSERT_EQUAL_UINT8(123, scHeapReadByte(ctx, 0));
    scHeapWriteByte(ctx, SC_HEAP_SIZE - 1, 123); TEST_ASSERT_EQUAL_UINT8(123, scHeapReadByte(ctx, SC_HEAP_SIZE - 1));
    scHeapWriteByte(ctx, SC_HEAP_SIZE, 123); TEST_ASSERT_EQUAL_UINT8(-1, scHeapReadByte(ctx, SC_HEAP_SIZE));
}
void tes tHeapAccessWord(void) {
    scHeapWriteCell(ctx, 0, 0x76543210); TEST_ASSERT_EQUAL_HEX32(0x76543210, scHeapReadCell(ctx, 0));
    scHeapWriteCell(ctx, SC_HEAP_SIZE - sizeof(sc_cell_t), 0x76543211);
    TEST_ASSERT_EQUAL_HEX32(0x76543211, scHeapReadCell(ctx, SC_HEAP_SIZE - sizeof(sc_cell_t)));
    scHeapWriteCell(ctx, SC_HEAP_SIZE - sizeof(sc_cell_t) + 1, 0x76543212);
    TEST_ASSERT_EQUAL_HEX32(-1, scHeapReadCell(ctx, SC_HEAP_SIZE - sizeof(sc_cell_t) + 1));
}
#endif
