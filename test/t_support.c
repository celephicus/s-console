#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "unity.h"

#define SCONSOLE_WANT_INTERNAL_DEFS
#include "sconsole.h"

// Need this for margins in structs. 
#ifndef TEST
#error "TEST must be defined"
#endif

#include "t_support.h"

// Assembler location counter. Access with TS_ASSEMBLER_LOC.
uint16_t g_ts_assembler_loc;

static ScContext* t_ctx;
void ts_SetupScGlobals(ScContext* ctx) {
	t_ctx = ctx;
	scInit(t_ctx);
	g_sc_state.t_heap_pre = g_sc_state.t_heap_post = 0xee;
    memset(&g_sc_state.heap, 0xee, SC_HEAP_SIZE);
}
void ts_DestroyScGlobals() {
    TEST_ASSERT_EQUAL_UINT8(0xee, g_sc_state.t_heap_pre);
    TEST_ASSERT_EQUAL_UINT8(0xee, g_sc_state.t_heap_post);
	TEST_ASSERT_EQUAL_PTR(t_ctx, CTX);	// Check state not munged.
    memset(&g_sc_state, 0x00, sizeof(ScState));
}

char ts_put[50];
char* ts_putp;
void ts_putc(char c) { *ts_putp++ = c; }
char* tsGetPutc() { *ts_putp = '\0'; return ts_put; }

char ts_get[50];
char* ts_getp;
int16_t ts_getc() { return ('\0' == *ts_getp) ? -1 : *ts_getp++; }
void tsSetGetc(const char* s) { strcpy(ts_get, s); }

void ts_SetupStackTestContext() {
	// Build context.
    ScContext* ctx = malloc(sizeof(ScContext));
    ctx->t_u_stack_pre = ctx->t_u_stack_post = 0x12345678;
    ctx->t_r_stack_pre = ctx->t_r_stack_post = 0x12345678;
    scInitContext(ctx);
	
	// Set console read/write functions.
	ctx->getc = ts_getc;
	ctx->putc = ts_putc;
	ts_putp = ts_put;
	ts_get[0] = '\0';
	ts_getp = ts_get;
	
	// Set to state.
    ts_SetupScGlobals(ctx);
	IP = 0;		// Start running at start of RAM.
}
void ts_DestroyStackTestContext() {
	// Verify context & destroy.
    //TEST_ASSERT_EQUAL_MESSAGE(SC_FAULT_OK, FAULT, "FAULT");
    TEST_ASSERT_EQUAL_UINT32(0x12345678, CTX->t_u_stack_pre);
    TEST_ASSERT_EQUAL_UINT32(0x12345678, CTX->t_u_stack_post);
    TEST_ASSERT_EQUAL_UINT32(0x12345678, CTX->t_r_stack_pre);
    TEST_ASSERT_EQUAL_UINT32(0x12345678, CTX->t_r_stack_post);
	free(CTX);
	
    ts_DestroyScGlobals();
}

// Build a VM fixture.
void tsSetupVmFixture() {
	ts_SetupStackTestContext();
	TS_ASSEMBLER_LOC = 0;
}
void tsDestroyVmFixture() {
	//TEST_ASSERT_EQUAL_MESSAGE(0, u_depth(), "u_depth");
	//TEST_ASSERT_EQUAL_MESSAGE(0, r_depth(), "r_depth");
	ts_DestroyStackTestContext();
}

#include <stdio.h>
void tsDumpVmFixture() {
	printf("\n  U stack: ");
	for (int i = u_depth(); i > 0; --i)
		printf("%d ", u_peek(i-1));
	printf("\n  R stack: ");
	for (int i = r_depth(); i > 0; --i)
		printf("%d ", r_peek(i-1));
}

// Test return value of scRun against expected value, FAULT state & IP. 
void tsAssertVmRun(int n, uint8_t fault, uint16_t ip_exp)  {						
	TEST_ASSERT_EQUAL_MESSAGE(fault, scRun(n), "scRun() retval");	
	TEST_ASSERT_EQUAL_MESSAGE(ip_exp, IP, "IP");						
	TEST_ASSERT_EQUAL_MESSAGE(fault, FAULT, "FAULT");				
	FAULT = 0;															
} 

// Verify stack contents.
void tsAssertUStack(int depth, ...) {
	va_list ap;
	TEST_ASSERT_EQUAL_MESSAGE(depth, u_depth(), "u_depth");
	va_start(ap, depth);
	while (--depth >= 0)
		TEST_ASSERT_EQUAL(va_arg(ap, sc_cell_t), u_peek(depth));
}
void tsAssertRStack(int depth, ...) {
	va_list ap;
	TEST_ASSERT_EQUAL_MESSAGE(depth, r_depth(), "r_depth");
	va_start(ap, depth);
	while (--depth >= 0)
		TEST_ASSERT_EQUAL(va_arg(ap, sc_cell_t), r_peek(depth));
}

