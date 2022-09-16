#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

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

// Test return value of scRun against expected value and FAULT variable. 
#define TEST_ASSERT_RUN(n_, fault_) do {		\
	TEST_ASSERT_EQUAL((fault_), scRun(n_));		\
	TEST_ASSERT_EQUAL((fault_), FAULT);			\
	FAULT = 0;									\
} while (0)
	
// Tests for sc_Run behaviour, num instructions & bad opcodes.
void testscRunNone() {
	TEST_ASSERT_RUN(0, SC_FAULT_OK);
	TEST_ASSERT_EQUAL(0, IP);
}
void testBadOpcode() {
	TS_ASSEMBLER_CODE(&addr, 0xff); 
	TEST_ASSERT_RUN(1, SC_FAULT_BAD_OPCODE);
	TEST_ASSERT_EQUAL(1, IP);	
}
void testVmRunFault() { // VM does not run if fault set. 
	FAULT = 99;		// Set a fault
	TEST_ASSERT_RUN(-1, 99);
	TEST_ASSERT_EQUAL(0, IP);
}
void testVmOpFault() {	// Check FAULT opcode.
	TS_ASSEMBLER_CODE(&addr, SC_OP_FAULT); 
	u_push(113);
	TEST_ASSERT_RUN(1, 113);
	TEST_ASSERT_EQUAL(1, IP);	
}
void testVmOpFaultFail() {	// Check stack underflow on FAULT opcode.
	TS_ASSEMBLER_CODE(&addr, SC_OP_FAULT); 
	TEST_ASSERT_RUN(1, SC_FAULT_U_STACK_UFLOW);
	TEST_ASSERT_EQUAL(1, IP);	
}
	
// Helpers for unary ops.
void testUnop(uint8_t op, sc_cell_t a, sc_cell_t r) {
	TS_ASSEMBLER_CODE(&addr, op);
	u_push(a);
	TEST_ASSERT_RUN(1, SC_FAULT_OK);
	TEST_ASSERT_EQUAL(1, IP);	
	TEST_ASSERT_EQUAL(1, u_depth());
	TEST_ASSERT_EQUAL_SC_CELL(r, u_pop());
}
void testUnopFail(uint8_t op) {
	TS_ASSEMBLER_CODE(&addr, op);
	TEST_ASSERT_RUN(-1, SC_FAULT_U_STACK_UFLOW);
	TEST_ASSERT_EQUAL(1, IP);	
}

// Helpers for binary ops.
void testBinop(uint8_t op, sc_cell_t a1, sc_cell_t a2, sc_cell_t r) {
	TS_ASSEMBLER_CODE(&addr, op);
	u_push(a1);
	u_push(a2);
	TEST_ASSERT_RUN(1, SC_FAULT_OK);
	TEST_ASSERT_EQUAL(1, IP);	
	TEST_ASSERT_EQUAL(1, u_depth());
	TEST_ASSERT_EQUAL_SC_CELL(r, u_pop());
}
void testBinopFail(uint8_t op, int n_args) {
	TS_ASSEMBLER_CODE(&addr, op);
	while (n_args-- > 0) u_push(1234);
	TEST_ASSERT_RUN(1, SC_FAULT_U_STACK_UFLOW);
	TEST_ASSERT_EQUAL(1, IP);	
	u_reset();
}

// Word tests!
//

// LIT8 -- byte literal.
void testByteLiteral() {
	TS_ASSEMBLER_CODE(&addr, SC_OP_LIT8, 122);
	
	TEST_ASSERT_RUN(1, SC_FAULT_OK);
	TEST_ASSERT_EQUAL(2, IP);	
	TEST_ASSERT_EQUAL(1, u_depth());
	TEST_ASSERT_EQUAL_SC_CELL(122, u_pop());
}

// LIT -- cell literal. 
void testCellLiteral() {
	TS_ASSEMBLER_CODE(&addr, SC_OP_LIT);
	TS_ASSEMBLER_CELL(&addr, 0x12345678);
	
	TEST_ASSERT_RUN(1, SC_FAULT_OK);
	TEST_ASSERT_EQUAL(1+sizeof(sc_cell_t), IP);	
	TEST_ASSERT_EQUAL(1, u_depth());
	TEST_ASSERT_EQUAL_SC_CELL(0x12345678, u_pop());
}

// NEG -- Unary minus
TT_TEST_CASE(testUnop(SC_OP_NEGATE, 0x12345678, -0x12345678));
TT_TEST_CASE(testUnopFail(SC_OP_NEGATE));

// 0= -- Boolean invert
TT_TEST_CASE(testUnop(SC_OP_ZERO_EQUALS, 0, -1));
TT_TEST_CASE(testUnop(SC_OP_ZERO_EQUALS, -1, 0));
TT_TEST_CASE(testUnop(SC_OP_ZERO_EQUALS, 12345, 0));
TT_TEST_CASE(testUnopFail(SC_OP_ZERO_EQUALS));

// - -- minus
TT_TEST_CASE(testBinop(SC_OP_MINUS, 0x12345678, -1, 0x12345679));
TT_TEST_CASE(testBinopFail(SC_OP_MINUS, 1));
TT_TEST_CASE(testBinopFail(SC_OP_MINUS, 0));

// * -- times
TT_TEST_CASE(testBinop(SC_OP_TIMES, 1234, 5678, 1234*5678));
TT_TEST_CASE(testBinopFail(SC_OP_TIMES, 1));
TT_TEST_CASE(testBinopFail(SC_OP_TIMES, 0));

// /MOD -- get quotient & remainder.
// TODO: check the rounding of the quotient & remainder. If you compile on a pre C99 compiler then the division rounding is weird. 
void testSlashMod(sc_cell_t a1, sc_cell_t a2, sc_cell_t q, sc_cell_t r) {
	TS_ASSEMBLER_CODE(&addr, SC_OP_SLASH_MOD);
	u_push(a1);
	u_push(a2);
	TEST_ASSERT_RUN(1, SC_FAULT_OK);
	TEST_ASSERT_EQUAL(1, IP);	
	TEST_ASSERT_EQUAL(2, u_depth());
	TEST_ASSERT_EQUAL_SC_CELL(r, u_pop());
	TEST_ASSERT_EQUAL_SC_CELL(q, u_pop());
}
TT_TEST_CASE(testSlashMod(38, 5, 38/5, 38%5));
TT_TEST_CASE(testBinopFail(SC_OP_SLASH_MOD, 1));
TT_TEST_CASE(testBinopFail(SC_OP_SLASH_MOD, 0));

TT_IGNORE_FROM_HERE()
/*

static void build_n_incs(int n) {
	memset(code, OP_INC, n);
	code[n] = OP_FAULT; code[n+1] = FAULT_USER;
}
void testInc() {
	build_n_incs(6);
	
	TEST_ASSERT_EQUAL(FAULT_OK, scRun(1));
	TEST_ASSERT_EQUAL(1, IP);	
	TEST_ASSERT_EQUAL(1, val);

	TEST_ASSERT_EQUAL(FAULT_OK, scRun(2));
	TEST_ASSERT_EQUAL(3, IP);	
	TEST_ASSERT_EQUAL(3, val);

	TEST_ASSERT_EQUAL(FAULT_USER, scRun(-1));
	TEST_ASSERT_EQUAL(6+2, IP);	
}

void testInc2() {
	CODE(OP_INC, OP_INC, OP_FAULT, FAULT_USER);
	
	TEST_ASSERT_EQUAL(FAULT_OK, scRun(2));
	TEST_ASSERT_EQUAL(2, IP);	
	TEST_ASSERT_EQUAL(2, val);

	TEST_ASSERT_EQUAL(FAULT_USER, scRun(1));
	TEST_ASSERT_EQUAL(2+2, IP);	
}


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