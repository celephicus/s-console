#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "unity.h"

#define SCONSOLE_WANT_INTERNAL_DEFS
#include "sconsole.h"

#include "t_support.h"
static sc_cell_t addr;
void setup_test_vm() {
	ts_SetupStackTestContext();
	addr = 0;
}
void destroy_test_vm() {
	TEST_ASSERT_EQUAL(0, u_depth());
	TEST_ASSERT_EQUAL(0, r_depth());
	ts_DestroyStackTestContext();
}

TT_BEGIN_FIXTURE(setup_test_vm, NULL, destroy_test_vm);

// Test return value of scRun against expected value, FAULT state & IP. 
#define TEST_ASSERT_RUN(n_, fault_, ip_exp_) do {	\
	TEST_ASSERT_EQUAL((fault_), scRun(n_));			\
	TEST_ASSERT_EQUAL((ip_exp_), IP);				\
	TEST_ASSERT_EQUAL((fault_), FAULT);				\
	FAULT = 0;										\
} while (0)

// Verify stack contents and clear.
static void verify_stack(int depth, ...) {
	va_list ap;
	TEST_ASSERT_EQUAL(depth, u_depth());
	va_start(ap, depth);
	while (--depth >= 0)
		TEST_ASSERT_EQUAL(va_arg(ap, sc_cell_t), u_peek(depth));
	u_reset();
}

// Tests for sc_Run behaviour, num instructions & bad opcodes.
void testscRunNone() {
	TEST_ASSERT_RUN(0, SC_FAULT_OK, 0);
}
void testBadOpcode() {
	TS_ASSEMBLER_CODE(&addr, 0xff); 
	TEST_ASSERT_RUN(1, SC_FAULT_BAD_OPCODE, 1);
}
void testVmRunFault() { // VM does not run if fault set. 
	FAULT = 99;		// Set a fault
	TEST_ASSERT_RUN(-1, 99, 0);
}
void testVmRun() {
	enum { N = 7 };
	for (uint8_t i = 0; i < N; i++)
		TS_ASSEMBLER_CODE(&addr, SC_OP_LIT8, i); 
	TS_ASSEMBLER_CODE(&addr, SC_OP_LIT8, 99, SC_OP_FAULT); 

	TEST_ASSERT_RUN(2, SC_FAULT_OK, 4);
	TEST_ASSERT_EQUAL(2, u_depth());

	TEST_ASSERT_RUN(-2, 99, N*2+2+1);
	TEST_ASSERT_EQUAL(N, u_depth());
	u_reset();
}

// Check FAULT opcode.
void testVmOpFault() {	
	TS_ASSEMBLER_CODE(&addr, SC_OP_FAULT); 
	u_push(113);
	TEST_ASSERT_RUN(1, 113, 1);
}
void testVmOpFaultFail() {	// Check stack underflow on FAULT opcode.
	TS_ASSEMBLER_CODE(&addr, SC_OP_FAULT); 
	TEST_ASSERT_RUN(1, SC_FAULT_U_STACK_UFLOW, 1);
}
	
// Helpers for unary ops.
void testVmUnop(uint8_t op, sc_cell_t a, sc_cell_t r) { // Verify unary op result.
	TS_ASSEMBLER_CODE(&addr, op);
	u_push(a);
	TEST_ASSERT_RUN(1, SC_FAULT_OK, 1);
	verify_stack(1, r);
}
void testBinop(uint8_t op, sc_cell_t a1, sc_cell_t a2, sc_cell_t r) {  // Verify binary op result.
	TS_ASSEMBLER_CODE(&addr, op);
	u_push(a1);
	u_push(a2);
	TEST_ASSERT_RUN(1, SC_FAULT_OK, 1);
	verify_stack(1, r);
}
void testVmOp_FailUnderflow(uint8_t op, int n_args) { // Verify operator sets fault on underflow with N items on stack
	TS_ASSEMBLER_CODE(&addr, op);
	while (n_args-- > 0) u_push(1234);
	TEST_ASSERT_RUN(1, SC_FAULT_U_STACK_UFLOW, 1);
	u_reset();
}

void testVmOp_FailOverflow(uint8_t op, int n_free) { // Verify operator sets fault on underflow with N _FREE_ items on stack
	TS_ASSEMBLER_CODE(&addr, op);
	while (u_free() > n_free)
		u_push(1234);
	TEST_ASSERT_RUN(1, SC_FAULT_U_STACK_OFLOW, 1);
	u_reset();
}

// Word tests!
//

// LIT8 -- byte literal.
void testByteLiteral() {
	TS_ASSEMBLER_CODE(&addr, SC_OP_LIT8, 122);
	
	TEST_ASSERT_RUN(1, SC_FAULT_OK, 2);
	verify_stack(1, 122);
}

// LIT -- cell literal. 
void testCellLiteral() {
	TS_ASSEMBLER_CODE(&addr, SC_OP_LIT);
	TS_ASSEMBLER_CELL(&addr, 0x12345678);
	
	TEST_ASSERT_RUN(1, SC_FAULT_OK, 1+sizeof(sc_cell_t));
	verify_stack(1, 0x12345678);
}

// NEG -- Unary minus
TT_TEST_CASE(testVmUnop(SC_OP_NEGATE, 0x12345678, -0x12345678));
TT_TEST_CASE(testVmOp_FailUnderflow(SC_OP_NEGATE, 0));

// 0= -- Boolean invert
TT_TEST_CASE(testVmUnop(SC_OP_ZERO_EQUALS, 0, -1));
TT_TEST_CASE(testVmUnop(SC_OP_ZERO_EQUALS, -1, 0));
TT_TEST_CASE(testVmUnop(SC_OP_ZERO_EQUALS, 12345, 0));
TT_TEST_CASE(testVmOp_FailUnderflow(SC_OP_ZERO_EQUALS, 0));

// - -- minus
TT_TEST_CASE(testBinop(SC_OP_MINUS, 0x12345678, -1, 0x12345679));
TT_TEST_CASE(testVmOp_FailUnderflow(SC_OP_MINUS, 1));
TT_TEST_CASE(testVmOp_FailUnderflow(SC_OP_MINUS, 0));

// * -- times
TT_TEST_CASE(testBinop(SC_OP_TIMES, 1234, 5678, 1234*5678));
TT_TEST_CASE(testVmOp_FailUnderflow(SC_OP_TIMES, 1));
TT_TEST_CASE(testVmOp_FailUnderflow(SC_OP_TIMES, 0));

// /MOD -- get quotient & remainder.
// TODO: check the rounding of the quotient & remainder. If you compile on a pre C99 compiler then the division rounding is weird. 
void testSlashMod(sc_cell_t a1, sc_cell_t a2, sc_cell_t q, sc_cell_t r) {
	TS_ASSEMBLER_CODE(&addr, SC_OP_SLASH_MOD);
	u_push(a1);
	u_push(a2);
	TEST_ASSERT_RUN(1, SC_FAULT_OK, 1);
	verify_stack(2, q, r);
}
TT_TEST_CASE(testSlashMod(38, 5, 38/5, 38%5));
TT_TEST_CASE(testVmOp_FailUnderflow(SC_OP_SLASH_MOD, 1));
TT_TEST_CASE(testVmOp_FailUnderflow(SC_OP_SLASH_MOD, 0));

// Stack operators.
void testVmOpDrop() {	// DROP
	TS_ASSEMBLER_CODE(&addr, SC_OP_DROP);
	u_push(1234);
	TEST_ASSERT_RUN(1, SC_FAULT_OK, 1);
	TEST_ASSERT_EQUAL(0, u_depth());
}
TT_TEST_CASE(testVmOp_FailUnderflow(SC_OP_DROP, 0));

void testVmOpDup() { 	// DUP
	TS_ASSEMBLER_CODE(&addr, SC_OP_DUP);
	u_push(1234);
	TEST_ASSERT_RUN(1, SC_FAULT_OK, 1);
	verify_stack(2, 1234, 1234);
}
TT_TEST_CASE(testVmOp_FailUnderflow(SC_OP_DUP, 0));
TT_TEST_CASE(testVmOp_FailOverflow(SC_OP_DUP, 0));

void testVmOpSwap() { 	// SWAP
	TS_ASSEMBLER_CODE(&addr, SC_OP_SWAP);
	u_push(1234);
	u_push(5678);
	TEST_ASSERT_RUN(1, SC_FAULT_OK, 1);
	verify_stack(2, 5678, 1234);
}
TT_TEST_CASE(testVmOp_FailUnderflow(SC_OP_SWAP, 0));
TT_TEST_CASE(testVmOp_FailUnderflow(SC_OP_SWAP, 1));

TT_IGNORE_FROM_HERE()
/*
void testLoad() {
	CODE(OP_LIT, 6, OP_LOAD, OP_FAULT, FAULT_USER, 0, 122);
	
	TEST_ASSERT_EQUAL(FAULT_USER, scRun(-1));
	TEST_ASSERT_EQUAL(5, IP);	
	TEST_ASSERT_EQUAL(122, val);
}

void testLoadBad() {
	CODE(OP_LIT, 66, OP_LOAD, OP_FAULT, FAULT_USER, 0, 122);
	TEST_ASSERT_EQUAL(FAULT_MEM, scRun(-1));
	TEST_ASSERT_EQUAL(3, IP);	
}

void testOverrun1() {
	code[VM_CODE_SIZE-1] = OP_INC;
	IP = VM_CODE_SIZE-1;
	
	TEST_ASSERT_EQUAL(FAULT_OK, scRun(1));		// Run last instruction OK.
	TEST_ASSERT_EQUAL(VM_CODE_SIZE, IP);	
	TEST_ASSERT_EQUAL(1, val);
	
	TEST_ASSERT_EQUAL(FAULT_BAD_IP, scRun(1));		// Now should fault as outside memory space.
	TEST_ASSERT_EQUAL(VM_CODE_SIZE+1, IP);			// IP goes to next, which is also illegal. 
	TEST_ASSERT_EQUAL(1, val);
}

void testOverrun2() {
	code[VM_CODE_SIZE-1] = OP_LIT; 
	IP = VM_CODE_SIZE-1;
	
	TEST_ASSERT_EQUAL(FAULT_BAD_IP, scRun(1));	// Opcode OK, but loads from IP which is now out of memory.
	TEST_ASSERT_EQUAL(VM_CODE_SIZE+1, IP);		// IP goes to next, which is also illegal, so does not inrement again as is normal for LITx. 
	// Value of val is now undefined. 
}

*/