#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "unity.h"
#include "t_support.h"

TT_BEGIN_INCLUDE()
//#include "testOne.h"
TT_END_INCLUDE()

#define SCONSOLE_WANT_INTERNAL_DEFS
#include "sconsole.h"

TT_BEGIN_FIXTURE(ts_SetupStackTestContext, NULL, ts_DestroyStackTestContext);

void testStackInit(void) {
    // Check guards are contiguous. 
    TEST_ASSERT_EQUAL_PTR(&sc_ctx->t_ustack_pre+1, &sc_ctx->ustack[0]);
    TEST_ASSERT_EQUAL_PTR(&sc_ctx->t_ustack_post-1, &sc_ctx->ustack[SC_DATA_STACK_SIZE-1]);
    // sc_ctx->t_ustack_post += 1;

    TEST_ASSERT_EQUAL(0, u_depth());
    TEST_ASSERT_EQUAL(SC_DATA_STACK_SIZE, u_free());
}

void testStackPopPush(void) {
    for (int i = 0; i < SC_DATA_STACK_SIZE; i += 1) {
        u_push(i);
        TEST_ASSERT_EQUAL(i+1, u_depth());
        TEST_ASSERT_EQUAL(SC_DATA_STACK_SIZE - i - 1, u_free());
    }
    for (int i = 0; i < SC_DATA_STACK_SIZE; i += 1) {
        TEST_ASSERT_EQUAL_UINT32(SC_DATA_STACK_SIZE - i - 1, u_pop());
        TEST_ASSERT_EQUAL(SC_DATA_STACK_SIZE - i - 1, u_depth());
        TEST_ASSERT_EQUAL(i + 1, u_free());
    }
}

void testStackReset(void) {
    u_reset();
    TEST_ASSERT_EQUAL(0, u_depth());
    u_push(1);
    u_reset();
    TEST_ASSERT_EQUAL(0, u_depth());

    for (int i = 0; i < SC_DATA_STACK_SIZE; i += 1) 
        u_push(i);
    TEST_ASSERT_EQUAL(SC_DATA_STACK_SIZE, u_depth());
    u_reset();
    TEST_ASSERT_EQUAL(0, u_depth());
}

void testStackPeek(void) {
    for (int i = 0; i < SC_DATA_STACK_SIZE; i += 1)
        u_push(i);

    for (int i = 0; i < SC_DATA_STACK_SIZE; i += 1)
        TEST_ASSERT_EQUAL(SC_DATA_STACK_SIZE - i - 1, u_peek(i));
}

void testStackTosNos(void) {
    u_push(0);
    TEST_ASSERT_EQUAL_UINT32(0, u_tos);

    for (int i = 1; i < SC_DATA_STACK_SIZE; i += 1) {
        u_push(i);
        TEST_ASSERT_EQUAL_UINT32(i - 1, u_nos);
        TEST_ASSERT_EQUAL_UINT32(i, u_tos);
    }
}

void testStackCanPop(void) {
    TEST_ASSERT_TRUE(u_can_pop(0));
    TEST_ASSERT_FALSE(u_can_pop(1));
    TEST_ASSERT_FALSE(u_can_pop(SC_DATA_STACK_SIZE));
    TEST_ASSERT_FALSE(u_can_pop(SC_DATA_STACK_SIZE + 1));

    u_push(33);
    TEST_ASSERT_TRUE(u_can_pop(0));
    TEST_ASSERT_TRUE(u_can_pop(1));
    TEST_ASSERT_FALSE(u_can_pop(2));
    TEST_ASSERT_FALSE(u_can_pop(SC_DATA_STACK_SIZE));
    TEST_ASSERT_FALSE(u_can_pop(SC_DATA_STACK_SIZE + 1));

    for (int i = 1; i < SC_DATA_STACK_SIZE - 1; i += 1) 
        u_push(i + 33);
    TEST_ASSERT_TRUE(u_can_pop(SC_DATA_STACK_SIZE - 1));
    TEST_ASSERT_FALSE(u_can_pop(SC_DATA_STACK_SIZE));
    TEST_ASSERT_FALSE(u_can_pop(SC_DATA_STACK_SIZE + 1));

    u_push(333);
    TEST_ASSERT_TRUE(u_can_pop(SC_DATA_STACK_SIZE));
    TEST_ASSERT_FALSE(u_can_pop(SC_DATA_STACK_SIZE + 1));
}

void testStackCanPush(void) {
    TEST_ASSERT_TRUE(u_can_push(0));
    TEST_ASSERT_TRUE(u_can_push(1));
    TEST_ASSERT_TRUE(u_can_push(SC_DATA_STACK_SIZE));
    TEST_ASSERT_FALSE(u_can_push(SC_DATA_STACK_SIZE + 1));

    u_push(33);
    TEST_ASSERT_TRUE(u_can_push(SC_DATA_STACK_SIZE - 1));
    TEST_ASSERT_FALSE(u_can_push(SC_DATA_STACK_SIZE));

    for (int i = 1; i < SC_DATA_STACK_SIZE - 1; i += 1)
        u_push(i + 33);
    TEST_ASSERT_TRUE(u_can_push(0));
    TEST_ASSERT_TRUE(u_can_push(1));
    TEST_ASSERT_FALSE(u_can_push(2));

    u_push(333);
    TEST_ASSERT_TRUE(u_can_push(0));
    TEST_ASSERT_FALSE(u_can_push(1));
}

void testErrorPop(void) {
    u_pop();
    TEST_ASSERT_EQUAL(SC_EXC_USER_STACK_UFLOW, sc_ctx->err);
    sc_ctx->err = 0;
}
void testErrorPush(void) {
    for (int i = 0; i < SC_DATA_STACK_SIZE; i += 1)
        u_push(i + 33);
    u_push(1234);
    TEST_ASSERT_EQUAL(SC_EXC_USER_STACK_OFLOW, sc_ctx->err);
    sc_ctx->err = 0;
}
void testErrorTos(void) {
    (void)u_tos;
    TEST_ASSERT_EQUAL(SC_EXC_USER_STACK_ACCESS, sc_ctx->err);
    sc_ctx->err = 0;
}
void testErrorNos(void) {
    u_push(1234);
    (void)u_nos;
    TEST_ASSERT_EQUAL(SC_EXC_USER_STACK_ACCESS, sc_ctx->err);
    sc_ctx->err = 0;
}
void testErrorPeek(void) {
    u_push(1234);
    u_push(34234);
    u_push(444);
    (void)u_peek(0);
    (void)u_peek(2);
    TEST_ASSERT_EQUAL(SC_EXC_OK, sc_ctx->err);
    (void)u_peek(3);
    TEST_ASSERT_EQUAL(SC_EXC_USER_STACK_ACCESS, sc_ctx->err);
    sc_ctx->err = 0;
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
