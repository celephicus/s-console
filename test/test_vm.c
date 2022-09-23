#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "unity.h"

#define SCONSOLE_WANT_INTERNAL_DEFS
#include "sconsole.h"

#include "t_support.h"
void setup_test_vm() {
	ts_SetupStackTestContext();
	TS_ASSEMBLER_LOC = 0;
}
void destroy_test_vm() {
	TEST_ASSERT_EQUAL_MESSAGE(0, u_depth(), "u_depth");
	TEST_ASSERT_EQUAL_MESSAGE(0, r_depth(), "r_depth");
	ts_DestroyStackTestContext();
}

#include <stdio.h>
void dump_test_vm() {
	printf("\n  U stack: ");
	for (int i = u_depth(); i > 0; --i)
		printf("%d ", u_peek(i-1));
	printf("\n  R stack: ");
	for (int i = r_depth(); i > 0; --i)
		printf("%d ", r_peek(i-1));
}
TT_BEGIN_FIXTURE(setup_test_vm, dump_test_vm, destroy_test_vm);

// Test return value of scRun against expected value, FAULT state & IP. 
#define TEST_ASSERT_RUN(n_, fault_, ip_exp_) do {						\
	TEST_ASSERT_EQUAL_MESSAGE((fault_), scRun(n_), "scRun() retval");	\
	TEST_ASSERT_EQUAL_MESSAGE((ip_exp_), IP, "IP");						\
	TEST_ASSERT_EQUAL_MESSAGE((fault_), FAULT, "FAULT");				\
	FAULT = 0;															\
} while (0)

// Verify stack contents.
static void verify_stack(int depth, ...) {
	va_list ap;
	TEST_ASSERT_EQUAL_MESSAGE(depth, u_depth(), "u_depth");
	va_start(ap, depth);
	while (--depth >= 0)
		TEST_ASSERT_EQUAL(va_arg(ap, sc_cell_t), u_peek(depth));
	u_clear();
}
static void verify_r_stack(int depth, ...) {
	va_list ap;
	TEST_ASSERT_EQUAL_MESSAGE(depth, r_depth(), "r_depth");
	va_start(ap, depth);
	while (--depth >= 0)
		TEST_ASSERT_EQUAL(va_arg(ap, sc_cell_t), r_peek(depth));
}

// Tests for sc_Run behaviour, num instructions & bad opcodes.
void testscRunNone() {
	TEST_ASSERT_RUN(0, SC_FAULT_OK, 0);
}
void testBadOpcode() {
	TS_ASSEMBLER_CODE(0x7f); 
	TEST_ASSERT_RUN(1, SC_FAULT_BAD_OPCODE, 1);
}
void testVmRunFault() { // VM does not run if fault set. 
	FAULT = 99;		// Set a fault
	TEST_ASSERT_RUN(-1, 99, 0);
}
void testVmRun() {
	enum { N = 7 };
	for (uint8_t i = 0; i < N; i++)
		TS_ASSEMBLER_CODE(SC_OP_LIT8, i); 
	TS_ASSEMBLER_CODE(SC_OP_LIT8, 99, SC_OP_FAULT); 

	TEST_ASSERT_RUN(2, SC_FAULT_OK, 4);
	TEST_ASSERT_EQUAL(2, u_depth());

	TEST_ASSERT_RUN(-2, 99, N*2+2+1);
	TEST_ASSERT_EQUAL(N, u_depth());
	u_clear();
}

// Check FAULT opcode.
void testVmOpFault() {	
	TS_ASSEMBLER_CODE(SC_OP_FAULT); 
	u_push(113);
	TEST_ASSERT_RUN(1, 113, 1);
}
void testVmOpFaultFail() {	// Check stack underflow on FAULT opcode.
	TS_ASSEMBLER_CODE(SC_OP_FAULT); 
	TEST_ASSERT_RUN(1, SC_FAULT_U_STACK_UFLOW, 1);
}

void testVmIpOverrun() {	// Verify fault on IP going out of range. 
	TS_ASSEMBLER_LOC = SC_HEAP_SIZE-1;
	TS_ASSEMBLER_CODE(SC_OP_SWAP);
	IP = SC_HEAP_SIZE-1;
	u_push(1234);
	u_push(5678);

	TEST_ASSERT_RUN(1, SC_FAULT_OK, SC_HEAP_SIZE);		// Run last instruction OK, but IP now out of range.
	verify_stack(2, 5678, 1234);
	TEST_ASSERT_RUN(1, SC_FAULT_BAD_IP, SC_HEAP_SIZE+1);	// Now should set IP fault. IP goes to next, which is also illegal. 
}

void testVmOpOverrun(uint8_t op, uint8_t sz) {	// Load FIRST byte from IP outside memory.
	TS_ASSEMBLER_LOC = SC_HEAP_SIZE-1;
	TS_ASSEMBLER_CODE(op);
	IP = SC_HEAP_SIZE-1;
	TEST_ASSERT_RUN(1, SC_FAULT_BAD_IP, SC_HEAP_SIZE+sz);	
}
	
void testVmOpOverrun2(uint8_t op, uint8_t sz) {	// Load SECOND byte from IP outside memory.
	TS_ASSEMBLER_LOC = SC_HEAP_SIZE-2;
	TS_ASSEMBLER_CODE(op, 0);
	IP = SC_HEAP_SIZE-2;
	TEST_ASSERT_RUN(1, SC_FAULT_BAD_IP, SC_HEAP_SIZE-1+sz);	
}
	
// Helpers for unary ops.
void testVmUnop(uint8_t op, sc_cell_t a, sc_cell_t r) { // Verify unary op result.
	TS_ASSEMBLER_CODE(op);
	u_push(a);
	TEST_ASSERT_RUN(1, SC_FAULT_OK, 1);
	verify_stack(1, r);
}
void testBinop(uint8_t op, sc_cell_t a1, sc_cell_t a2, sc_cell_t r) {  // Verify binary op result.
	TS_ASSEMBLER_CODE(op);
	u_push(a1);
	u_push(a2);
	TEST_ASSERT_RUN(1, SC_FAULT_OK, 1);
	verify_stack(1, r);
}
void testVmOp_FailUnderflow(uint8_t op, int n_args) { // Verify operator sets fault on underflow with N items on stack
	TS_ASSEMBLER_CODE(op);
	while (n_args-- > 0) u_push(1234);
	TEST_ASSERT_RUN(1, SC_FAULT_U_STACK_UFLOW, 1);
	u_clear();
}

void testVmOp_FailOverflow(uint8_t op, int n_free) { // Verify operator sets fault on underflow with N _FREE_ items on stack
	TS_ASSEMBLER_CODE(op);
	while (u_free() > n_free)
		u_push(1234);
	TEST_ASSERT_RUN(1, SC_FAULT_U_STACK_OFLOW, 1);
	u_clear();
}

// Primitive tests!
//

// LIT8 -- byte literal.
void testByteLiteral(uint8_t val) {
	TS_ASSEMBLER_CODE(SC_OP_LIT8, val);
	TEST_ASSERT_RUN(1, SC_FAULT_OK, 2);
	verify_stack(1, (sc_cell_t)val);
}
TT_TEST_CASE(testByteLiteral(0));
TT_TEST_CASE(testByteLiteral(127));
TT_TEST_CASE(testByteLiteral(0xff));
TT_TEST_CASE(testVmOpOverrun(SC_OP_LIT8, 1));

// LIT -- cell literal. 
void testCellLiteral() {
	TS_ASSEMBLER_CODE(SC_OP_LIT);
	TS_ASSEMBLER_CELL(0x12345678);
	TEST_ASSERT_RUN(1, SC_FAULT_OK, 1+sizeof(sc_cell_t));
	verify_stack(1, 0x12345678);
}
TT_TEST_CASE(testVmOpOverrun(SC_OP_LIT, sizeof(sc_cell_t)));	
TT_TEST_CASE(testVmOpOverrun2(SC_OP_LIT, sizeof(sc_cell_t)));	

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
	TS_ASSEMBLER_CODE(SC_OP_SLASH_MOD);
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
	TS_ASSEMBLER_CODE(SC_OP_DROP);
	u_push(1234);
	TEST_ASSERT_RUN(1, SC_FAULT_OK, 1);
	TEST_ASSERT_EQUAL(0, u_depth());
}
TT_TEST_CASE(testVmOp_FailUnderflow(SC_OP_DROP, 0));

void testVmOpDup() { 	// DUP
	TS_ASSEMBLER_CODE(SC_OP_DUP);
	u_push(1234);
	TEST_ASSERT_RUN(1, SC_FAULT_OK, 1);
	verify_stack(2, 1234, 1234);
}
TT_TEST_CASE(testVmOp_FailUnderflow(SC_OP_DUP, 0));
TT_TEST_CASE(testVmOp_FailOverflow(SC_OP_DUP, 0));

void testVmOpSwap() { 	// SWAP
	TS_ASSEMBLER_CODE(SC_OP_SWAP);
	u_push(1234);
	u_push(5678);
	TEST_ASSERT_RUN(1, SC_FAULT_OK, 1);
	verify_stack(2, 5678, 1234);
}
TT_TEST_CASE(testVmOp_FailUnderflow(SC_OP_SWAP, 0));
TT_TEST_CASE(testVmOp_FailUnderflow(SC_OP_SWAP, 1));

// @ -- Fetch CELL from address. 
void testVmOpFetchCell(uint16_t op_addr, uint16_t val_addr, uint8_t fault) {
	IP = TS_ASSEMBLER_LOC = op_addr; 
	TS_ASSEMBLER_CODE(SC_OP_FETCH); 
	if (!fault) {
		TS_ASSEMBLER_LOC = val_addr; 
		TS_ASSEMBLER_CELL(1234);
	}
	u_push(val_addr);
	TEST_ASSERT_RUN(1, fault, op_addr+1);
	if (!fault) 
		verify_stack(1, 1234);
	else
		u_clear();
}
TT_TEST_CASE(testVmOpFetchCell(0, 1, SC_FAULT_OK));
TT_TEST_CASE(testVmOpFetchCell(SC_HEAP_SIZE-1, 1, SC_FAULT_OK));
TT_TEST_CASE(testVmOpFetchCell(0, SC_HEAP_SIZE-sizeof(sc_cell_t), SC_FAULT_OK));
TT_TEST_CASE(testVmOpFetchCell(0, SC_HEAP_SIZE-sizeof(sc_cell_t)+1, SC_FAULT_MEM));
TT_TEST_CASE(testVmOp_FailUnderflow(SC_OP_FETCH, 0));

// @ -- Fetch BYTE from address. 
void testVmOpFetchByte(uint16_t op_addr, uint16_t val_addr, uint8_t fault) {
	IP = TS_ASSEMBLER_LOC = op_addr; 
	TS_ASSEMBLER_CODE(SC_OP_C_FETCH); 
	if (!fault) {
		TS_ASSEMBLER_LOC = val_addr; 
		TS_ASSEMBLER_CODE(123);
	}
	u_push(val_addr);
	TEST_ASSERT_RUN(1, fault, op_addr+1);
	if (!fault) 
		verify_stack(1, 123);
	else
		u_clear();
}
TT_TEST_CASE(testVmOpFetchByte(0, 1, SC_FAULT_OK));
TT_TEST_CASE(testVmOpFetchByte(SC_HEAP_SIZE-1, 1, SC_FAULT_OK));
TT_TEST_CASE(testVmOpFetchByte(0, SC_HEAP_SIZE-1, SC_FAULT_OK));
TT_TEST_CASE(testVmOpFetchByte(0, SC_HEAP_SIZE, SC_FAULT_MEM));
TT_TEST_CASE(testVmOp_FailUnderflow(SC_OP_C_FETCH, 0));

// ! -- Store CELL to address. 
void testVmOpStoreCell(uint16_t op_addr, uint16_t val_addr, uint8_t fault) {
	IP = TS_ASSEMBLER_LOC = op_addr; 
	TS_ASSEMBLER_CODE(SC_OP_STORE); 
	u_push(1234);
	u_push(val_addr);
	TEST_ASSERT_RUN(1, fault, op_addr+1);
	if (!fault) 
		TEST_ASSERT_EQUAL_SC_CELL(1234, *(sc_cell_t*)&g_sc_state.heap[val_addr]);
	u_clear();
}
TT_TEST_CASE(testVmOpStoreCell(0, 1, SC_FAULT_OK));
TT_TEST_CASE(testVmOpStoreCell(SC_HEAP_SIZE-1, 1, SC_FAULT_OK));
TT_TEST_CASE(testVmOpStoreCell(0, SC_HEAP_SIZE-sizeof(sc_cell_t), SC_FAULT_OK));
TT_TEST_CASE(testVmOpStoreCell(0, SC_HEAP_SIZE-sizeof(sc_cell_t)+1, SC_FAULT_MEM));
TT_TEST_CASE(testVmOp_FailUnderflow(SC_OP_STORE, 0));
TT_TEST_CASE(testVmOp_FailUnderflow(SC_OP_STORE, 1));

// ! -- Store BYTE to address. 
void testVmOpStoreByte(uint16_t op_addr, uint16_t val_addr, uint8_t fault) {
	IP = TS_ASSEMBLER_LOC = op_addr; 
	TS_ASSEMBLER_CODE(SC_OP_C_STORE);
	u_push(123);
	u_push(val_addr);
	TEST_ASSERT_RUN(1, fault, op_addr+1);
	if (!fault) 
		TEST_ASSERT_EQUAL_SC_CELL(123, g_sc_state.heap[val_addr]);
	u_clear();
}
TT_TEST_CASE(testVmOpStoreByte(0, 1, SC_FAULT_OK));
TT_TEST_CASE(testVmOpStoreByte(SC_HEAP_SIZE-1, 1, SC_FAULT_OK));
TT_TEST_CASE(testVmOpStoreByte(0, SC_HEAP_SIZE-1, SC_FAULT_OK));
TT_TEST_CASE(testVmOpStoreByte(0, SC_HEAP_SIZE, SC_FAULT_MEM));
TT_TEST_CASE(testVmOp_FailUnderflow(SC_OP_C_STORE, 0));
TT_TEST_CASE(testVmOp_FailUnderflow(SC_OP_C_STORE, 1));

// Clear stack.
void testVmOpClear() {
	TS_ASSEMBLER_CODE(SC_OP_CLEAR); 
	u_push(123);
	TEST_ASSERT_RUN(1, SC_FAULT_OK, 1);
	TEST_ASSERT_EQUAL(0, u_depth());
}

// EMIT
void testVmEmit() {
	TS_ASSEMBLER_CODE(SC_OP_EMIT, SC_OP_EMIT, ); 
	u_push('i');
	u_push('h');
	TEST_ASSERT_RUN(2, SC_FAULT_OK, 2);
	TEST_ASSERT_EQUAL_STRING("hi", tsGetPutc());
}
TT_TEST_CASE(testVmOp_FailUnderflow(SC_OP_EMIT, 0));

// KEY
void testVmKey(const char* s) {
	TS_ASSEMBLER_CODE(SC_OP_KEY, SC_OP_KEY, SC_OP_KEY, ); 
	tsSetGetc("lo");
	TEST_ASSERT_RUN(3, SC_FAULT_OK, 3);
	verify_stack(3, 'l', 'o', -1);
}
TT_TEST_CASE(testVmOp_FailOverflow(SC_OP_KEY, 0));

// CALL
void testVmCall(uint16_t call_addr) {
	TS_ASSEMBLER_CALL(call_addr); 	// Assemble call ar address zero. Takes care of setting high bit.
	TS_ASSEMBLER_LOC = call_addr; 
	TS_ASSEMBLER_CODE(SC_OP_DUP, SC_OP_RET); // Little sub
	u_push(1234);
	
	TEST_ASSERT_RUN(1, SC_FAULT_OK, call_addr);
	verify_r_stack(1, 2);	// Return address.
	
	TEST_ASSERT_RUN(1, SC_FAULT_OK, call_addr+1);	// Execute DUP.
	TEST_ASSERT_RUN(1, SC_FAULT_OK, 2);				// Execute RET.
	
	verify_stack(2, 1234, 1234);	
}
TT_TEST_CASE(testVmCall(2));
TT_TEST_CASE(testVmCall(SC_HEAP_SIZE-2));
void testVmCallBadReturn() {		// Munge return address.
	TS_ASSEMBLER_CALL(3); 
	TS_ASSEMBLER_LOC = 3; 
	TS_ASSEMBLER_CODE(SC_OP_RET); 
	
	TEST_ASSERT_RUN(1, SC_FAULT_OK, 3);
	verify_r_stack(1, 2);	// Return address.
	r_tos = 0xffff;			
	TEST_ASSERT_EQUAL(SC_FAULT_BAD_IP, scRun(1));	// Who cares what IP ends up.
	r_clear(); FAULT = SC_FAULT_OK;		// Clear things checked in teardown.
}
void testVmCallBadAddress() {		// Call to address out of range.
	TS_ASSEMBLER_CALL(0xffff); 
	TEST_ASSERT_EQUAL(SC_FAULT_BAD_IP, scRun(1));	// Who cares what IP ends up.	
	r_clear(); FAULT = SC_FAULT_OK;		// Clear things checked in teardown.
}
void testVmCallIpOverrun() {		// Call to address out of range.
	IP = TS_ASSEMBLER_LOC = SC_HEAP_SIZE-1; 
	TS_ASSEMBLER_CODE(0x80); 
	TEST_ASSERT_EQUAL(SC_FAULT_BAD_IP, scRun(1));	// Who cares what IP ends up.
	r_clear(); FAULT = SC_FAULT_OK;		// Clear things checked in teardown.
}
	
TT_IGNORE_FROM_HERE()
