#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "unity.h"

#define SCONSOLE_WANT_INTERNAL_DEFS
#include "sconsole.h"

#include "t_support.h"
static sc_cell_t addr;
void test_vm_setup() {
	ts_SetupStackTestContext();
	addr = 0;
}
TT_BEGIN_FIXTURE(test_vm_setup, NULL, ts_DestroyStackTestContext);

void testscRunNone() {
	TEST_ASSERT_EQUAL(SC_FAULT_OK, scRun(0));
	TEST_ASSERT_EQUAL(0, IP);
}

void testBadOpcode() {
	TS_ASSEMBLER_CODE(&addr, 0xff); 
	
	TEST_ASSERT_EQUAL(SC_FAULT_BAD_OPCODE, scRun(1));	
	FAULT = SC_FAULT_OK;
	TEST_ASSERT_EQUAL(1, IP);	
}

void testByteLiteral() {
	TS_ASSEMBLER_CODE(&addr, SC_OP_LIT8, 122);
	
	TEST_ASSERT_EQUAL(SC_FAULT_OK, scRun(1));	// Load literal. 
	TEST_ASSERT_EQUAL(2, IP);	
	TEST_ASSERT_EQUAL(1, u_depth());
	TEST_ASSERT_EQUAL_SC_CELL(122, u_pop());
}
void testCellLiteral() {
	TS_ASSEMBLER_CODE(&addr, SC_OP_LIT);
	TS_ASSEMBLER_CELL(&addr, 0x12345678);
	
	TEST_ASSERT_EQUAL(SC_FAULT_OK, scRun(1));	
	TEST_ASSERT_EQUAL(1+sizeof(sc_cell_t), IP);	
	TEST_ASSERT_EQUAL(1, u_depth());
	TEST_ASSERT_EQUAL_SC_CELL(0x12345678, u_pop());
}

TT_IGNORE_FROM_HERE()
/*
void testVmFaultOp() {
	CODE(OP_FAULT, FAULT_USER, OP_INC);
	
	TEST_ASSERT_EQUAL(FAULT_USER, scRun(10));
	TEST_ASSERT_EQUAL(2, IP);
	TEST_ASSERT_EQUAL(0, val);
	
	// Nothing should happen as VM has fault set. 
	TEST_ASSERT_EQUAL(FAULT_USER, scRun(10));
	TEST_ASSERT_EQUAL(2, IP);
	TEST_ASSERT_EQUAL(0, val);
}

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