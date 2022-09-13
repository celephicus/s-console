#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "unity.h"

#define SCONSOLE_WANT_INTERNAL_DEFS
#include "sconsole.h"


void ts_SetupScGlobals() {
    memset(&g_sc, 0xee, sizeof(sc_globals_t));
}
void ts_DestroyScGlobals() {
    TEST_ASSERT_EQUAL_UINT8(0xee, g_sc.t_heap_pre);
    TEST_ASSERT_EQUAL_UINT8(0xee, g_sc.t_heap_post);
    memset(&g_sc, 0x00, sizeof(sc_globals_t));
    g_sc.t_heap_pre = g_sc.t_heap_post = 0x00;
}
void ts_SetupStackTestContext() {
    ts_SetupScGlobals();
    sc_ctx = malloc(sizeof(sc_context_t));
    memset(sc_ctx, 0xee, sizeof(sc_context_t));
    scInitContext(sc_ctx);
    sc_ctx->t_u_stack_pre = sc_ctx->t_u_stack_post = 0x12345678;
}
void ts_DestroyStackTestContext() {
    TEST_ASSERT_EQUAL(SC_EXC_OK, sc_ctx->err);
    TEST_ASSERT_EQUAL_UINT32(0x12345678, sc_ctx->t_u_stack_pre);
    TEST_ASSERT_EQUAL_UINT32(0x12345678, sc_ctx->t_u_stack_post);
    free(sc_ctx);
    sc_ctx = NULL;
    ts_DestroyScGlobals();
}
