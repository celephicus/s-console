/* This file is autogenerated by grm -- do not edit. */

#ifndef UNITY_INCLUDE_CONFIG_H
#error "Must define UNITY_INCLUDE_CONFIG_H."
#endif

/*** Standard includes. ***/
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "unity.h"


/*** Stuff copied from test files (should be #include's, declarations & macros only) ***/
#define SCONSOLE_WANT_INTERNAL_DEFS
#include "sconsole.h"
enum { // Error codes...
	FAULT_OK, 		// No fault.
	FAULT_MEM,		// Access out of memory space. 
	FAULT_BAD_OPCODE,	// Illegal opcode.
	FAULT_BAD_tvIP,		// tvIP outside of memory space.
	FAULT_USER		// User fault codes from here.
};

enum { // Instructions...
	OP_FAULT,	// Sets fault from next value in instruction stream.
	OP_INC,		// Increments val.
	OP_LIT,		// Loads next value in instruction stream into val.
	OP_LOAD,	// Load val from address in val.
};

/*** External test functions scraped from test files. ***/
void testHeapRamByte(sc_cell_t addr);
void testHeapRamByteBadRead();
void testHeapRamByteBadWrite();
void testHeapRamCell(sc_cell_t addr);
void testHeapRamCellBadRead();
void testHeapRamCellBadWrite();
void testHelpersPutc(const char* s);
void testHelpersGetc(const char* s);
void testHelpersHash();
void testHelpersFindPrimitive();
void testInitContext(void);
void testRStackInit(void);
void testRStackSize(void);
void testRStackPopPush(void);
void testRStackReset(void);
void testRStackPeek(void);
void testRStackTosNos(void);
void testRStackCanPop(void);
void testRStackCanPush(void);
void testToyVmRunNone();
void testToyVmFaultOp();
void testToyVmInc();
void testToyVmInc2();
void testToyVmLiteral();
void testToyVmLoad();
void testToyVmLoadBad();
void testToyVmOverrun1();
void testToyVmOverrun2();
void testToyVmBadOpcode();
void testUStackInit(void);
void testUStackSize(void);
void testUStackPopPush(void);
void testUStackReset(void);
void testUStackPeek(void);
void testUStackTosNos(void);
void testUStackCanPop(void);
void testUStackCanPush(void);
void testscRunNone();
void testBadOpcode();
void testVmRunFault();
void testVmRun();
void testVmOpFault();
void testVmOpFaultFail();
void testVmIpOverrun();
void testVmOpOverrun(uint8_t op, uint8_t sz);
void testVmOpOverrun2(uint8_t op, uint8_t sz);
void testVmUnop(uint8_t op, sc_cell_t a, sc_cell_t r);
void testBinop(uint8_t op, sc_cell_t a1, sc_cell_t a2, sc_cell_t r);
void testVmOp_FailUnderflow(uint8_t op, int n_args);
void testVmOp_FailOverflow(uint8_t op, int n_free);
void testByteLiteral(uint8_t val);
void testCellLiteral();
void testSlashMod(sc_cell_t a1, sc_cell_t a2, sc_cell_t q, sc_cell_t r);
void testVmOpDrop();
void testVmOpDup();
void testVmOpSwap();
void testVmOpFetchCell(uint16_t op_addr, uint16_t val_addr, uint8_t fault);
void testVmOpFetchByte(uint16_t op_addr, uint16_t val_addr, uint8_t fault);
void testVmOpStoreCell(uint16_t op_addr, uint16_t val_addr, uint8_t fault);
void testVmOpStoreByte(uint16_t op_addr, uint16_t val_addr, uint8_t fault);
void testVmOpClear();
void testVmEmit();
void testVmKey(const char* s);
void testVmCall(uint16_t call_addr);
void testVmCallBadReturn();
void testVmCallBadAddress();
void testVmCallIpOverrun();

/*** Fixture & dump functions from test files. ***/
void ts_SetupStackTestContext(void);
void ts_DestroyStackTestContext(void);
void setupTestInitFixture(void);
void destroyTestInitFixture(void);
void vmInit(void);
void tsSetupVmFixture(void);
void tsDumpVmFixture(void);
void tsDestroyVmFixture(void);

/* Declare test stubs. */
static void testHeapRamByte_stub_0(void) { testHeapRamByte(0); }
static void testHeapRamByte_stub_1(void) { testHeapRamByte(SC_HEAP_SIZE-1); }
static void testHeapRamCell_stub_2(void) { testHeapRamCell(0); }
static void testHeapRamCell_stub_3(void) { testHeapRamCell(SC_HEAP_SIZE-sizeof(sc_cell_t)); }
static void testHelpersPutc_stub_4(void) { testHelpersPutc(""); }
static void testHelpersPutc_stub_5(void) { testHelpersPutc("x"); }
static void testHelpersPutc_stub_6(void) { testHelpersPutc("Oh, freddled gruntbuggly, thy micturations are to me..."); }
static void testHelpersGetc_stub_7(void) { testHelpersGetc(""); }
static void testHelpersGetc_stub_8(void) { testHelpersGetc("x"); }
static void testHelpersGetc_stub_9(void) { testHelpersGetc("Oh, freddled gruntbuggly, thy micturations are to me..."); }
static void testByteLiteral_stub_10(void) { testByteLiteral(0); }
static void testByteLiteral_stub_11(void) { testByteLiteral(127); }
static void testByteLiteral_stub_12(void) { testByteLiteral(0xff); }
static void testVmOpOverrun_stub_13(void) { testVmOpOverrun(SC_OP_LIT8, 1); }
static void testVmOpOverrun_stub_14(void) { testVmOpOverrun(SC_OP_LIT, sizeof(sc_cell_t)); }
static void testVmOpOverrun2_stub_15(void) { testVmOpOverrun2(SC_OP_LIT, sizeof(sc_cell_t)); }
static void testVmUnop_stub_16(void) { testVmUnop(SC_OP_NEGATE, 0x12345678, -0x12345678); }
static void testVmOp_FailUnderflow_stub_17(void) { testVmOp_FailUnderflow(SC_OP_NEGATE, 0); }
static void testVmUnop_stub_18(void) { testVmUnop(SC_OP_ZERO_EQUALS, 0, -1); }
static void testVmUnop_stub_19(void) { testVmUnop(SC_OP_ZERO_EQUALS, -1, 0); }
static void testVmUnop_stub_20(void) { testVmUnop(SC_OP_ZERO_EQUALS, 12345, 0); }
static void testVmOp_FailUnderflow_stub_21(void) { testVmOp_FailUnderflow(SC_OP_ZERO_EQUALS, 0); }
static void testBinop_stub_22(void) { testBinop(SC_OP_MINUS, 0x12345678, -1, 0x12345679); }
static void testVmOp_FailUnderflow_stub_23(void) { testVmOp_FailUnderflow(SC_OP_MINUS, 1); }
static void testVmOp_FailUnderflow_stub_24(void) { testVmOp_FailUnderflow(SC_OP_MINUS, 0); }
static void testBinop_stub_25(void) { testBinop(SC_OP_TIMES, 1234, 5678, 1234*5678); }
static void testVmOp_FailUnderflow_stub_26(void) { testVmOp_FailUnderflow(SC_OP_TIMES, 1); }
static void testVmOp_FailUnderflow_stub_27(void) { testVmOp_FailUnderflow(SC_OP_TIMES, 0); }
static void testSlashMod_stub_28(void) { testSlashMod(38, 5, 38/5, 38%5); }
static void testVmOp_FailUnderflow_stub_29(void) { testVmOp_FailUnderflow(SC_OP_SLASH_MOD, 1); }
static void testVmOp_FailUnderflow_stub_30(void) { testVmOp_FailUnderflow(SC_OP_SLASH_MOD, 0); }
static void testVmOp_FailUnderflow_stub_31(void) { testVmOp_FailUnderflow(SC_OP_DROP, 0); }
static void testVmOp_FailUnderflow_stub_32(void) { testVmOp_FailUnderflow(SC_OP_DUP, 0); }
static void testVmOp_FailOverflow_stub_33(void) { testVmOp_FailOverflow(SC_OP_DUP, 0); }
static void testVmOp_FailUnderflow_stub_34(void) { testVmOp_FailUnderflow(SC_OP_SWAP, 0); }
static void testVmOp_FailUnderflow_stub_35(void) { testVmOp_FailUnderflow(SC_OP_SWAP, 1); }
static void testVmOpFetchCell_stub_36(void) { testVmOpFetchCell(0, 1, SC_FAULT_OK); }
static void testVmOpFetchCell_stub_37(void) { testVmOpFetchCell(SC_HEAP_SIZE-1, 1, SC_FAULT_OK); }
static void testVmOpFetchCell_stub_38(void) { testVmOpFetchCell(0, SC_HEAP_SIZE-sizeof(sc_cell_t), SC_FAULT_OK); }
static void testVmOpFetchCell_stub_39(void) { testVmOpFetchCell(0, SC_HEAP_SIZE-sizeof(sc_cell_t)+1, SC_FAULT_MEM); }
static void testVmOp_FailUnderflow_stub_40(void) { testVmOp_FailUnderflow(SC_OP_FETCH, 0); }
static void testVmOpFetchByte_stub_41(void) { testVmOpFetchByte(0, 1, SC_FAULT_OK); }
static void testVmOpFetchByte_stub_42(void) { testVmOpFetchByte(SC_HEAP_SIZE-1, 1, SC_FAULT_OK); }
static void testVmOpFetchByte_stub_43(void) { testVmOpFetchByte(0, SC_HEAP_SIZE-1, SC_FAULT_OK); }
static void testVmOpFetchByte_stub_44(void) { testVmOpFetchByte(0, SC_HEAP_SIZE, SC_FAULT_MEM); }
static void testVmOp_FailUnderflow_stub_45(void) { testVmOp_FailUnderflow(SC_OP_C_FETCH, 0); }
static void testVmOpStoreCell_stub_46(void) { testVmOpStoreCell(0, 1, SC_FAULT_OK); }
static void testVmOpStoreCell_stub_47(void) { testVmOpStoreCell(SC_HEAP_SIZE-1, 1, SC_FAULT_OK); }
static void testVmOpStoreCell_stub_48(void) { testVmOpStoreCell(0, SC_HEAP_SIZE-sizeof(sc_cell_t), SC_FAULT_OK); }
static void testVmOpStoreCell_stub_49(void) { testVmOpStoreCell(0, SC_HEAP_SIZE-sizeof(sc_cell_t)+1, SC_FAULT_MEM); }
static void testVmOp_FailUnderflow_stub_50(void) { testVmOp_FailUnderflow(SC_OP_STORE, 0); }
static void testVmOp_FailUnderflow_stub_51(void) { testVmOp_FailUnderflow(SC_OP_STORE, 1); }
static void testVmOpStoreByte_stub_52(void) { testVmOpStoreByte(0, 1, SC_FAULT_OK); }
static void testVmOpStoreByte_stub_53(void) { testVmOpStoreByte(SC_HEAP_SIZE-1, 1, SC_FAULT_OK); }
static void testVmOpStoreByte_stub_54(void) { testVmOpStoreByte(0, SC_HEAP_SIZE-1, SC_FAULT_OK); }
static void testVmOpStoreByte_stub_55(void) { testVmOpStoreByte(0, SC_HEAP_SIZE, SC_FAULT_MEM); }
static void testVmOp_FailUnderflow_stub_56(void) { testVmOp_FailUnderflow(SC_OP_C_STORE, 0); }
static void testVmOp_FailUnderflow_stub_57(void) { testVmOp_FailUnderflow(SC_OP_C_STORE, 1); }
static void testVmOp_FailUnderflow_stub_58(void) { testVmOp_FailUnderflow(SC_OP_EMIT, 0); }
static void testVmOp_FailOverflow_stub_59(void) { testVmOp_FailOverflow(SC_OP_KEY, 0); }
static void testVmCall_stub_60(void) { testVmCall(2); }
static void testVmCall_stub_61(void) { testVmCall(SC_HEAP_SIZE-2); }

/*** Extra Unity support. ***/

/* Functions for setup, diagnostics dump on a test failure & teardown. */
typedef void (*fixture_func_t)(void);
static fixture_func_t setUp_func, dump_func, tearDown_func;
void registerFixture(fixture_func_t setup, fixture_func_t dumper, fixture_func_t teardown) {
	setUp_func = setup;
	dump_func = dumper;
	tearDown_func = teardown;
}

void setUp() { if (setUp_func) setUp_func(); }
void dumpTestContext() { if (dump_func) dump_func(); }
void tearDown() { if (tearDown_func) tearDown_func(); }

static void do_run_test(UnityTestFunction func, const char* name, int line_num) {
#ifdef UNITY_USE_COMMAND_LINE_ARGS
    if (!UnityTestMatches())
        return;
#endif
	UnityDefaultTestRun(func, name, line_num);
}

int main(int argc, char** argv) {
/*
	int rc_parse = UnityParseOptions(argc,argv);
	if (rc_parse != 0)
		return rc_parse;
*/
  UnityBegin("");
  
  UnitySetTestFile("..\test_helpers.c");
  registerFixture(ts_SetupStackTestContext, NULL, ts_DestroyStackTestContext);
  do_run_test(testHeapRamByte_stub_0, "testHeapRamByte(0)", 23);
  do_run_test(testHeapRamByte_stub_1, "testHeapRamByte(SC_HEAP_SIZE-1)", 24);
  do_run_test(testHeapRamByteBadRead, "testHeapRamByteBadRead", 26);
  do_run_test(testHeapRamByteBadWrite, "testHeapRamByteBadWrite", 31);
  do_run_test(testHeapRamCell_stub_2, "testHeapRamCell(0)", 44);
  do_run_test(testHeapRamCell_stub_3, "testHeapRamCell(SC_HEAP_SIZE-sizeof(sc_cell_t))", 46);
  do_run_test(testHeapRamCellBadRead, "testHeapRamCellBadRead", 48);
  do_run_test(testHeapRamCellBadWrite, "testHeapRamCellBadWrite", 53);
  do_run_test(testHelpersPutc_stub_4, "testHelpersPutc(\"\")", 65);
  do_run_test(testHelpersPutc_stub_5, "testHelpersPutc(\"x\")", 66);
  do_run_test(testHelpersPutc_stub_6, "testHelpersPutc(\"Oh, freddled gruntbuggly, thy micturations are to me...\")", 67);
  do_run_test(testHelpersGetc_stub_7, "testHelpersGetc(\"\")", 79);
  do_run_test(testHelpersGetc_stub_8, "testHelpersGetc(\"x\")", 80);
  do_run_test(testHelpersGetc_stub_9, "testHelpersGetc(\"Oh, freddled gruntbuggly, thy micturations are to me...\")", 81);
  do_run_test(testHelpersHash, "testHelpersHash", 84);
  do_run_test(testHelpersFindPrimitive, "testHelpersFindPrimitive", 90);
  registerFixture(NULL, NULL, NULL);
  
  UnitySetTestFile("..\test_init.c");
  registerFixture(setupTestInitFixture, NULL, destroyTestInitFixture);
  do_run_test(testInitContext, "testInitContext", 25);
  registerFixture(NULL, NULL, NULL);
  
  UnitySetTestFile("..\test_r_stack.c");
  registerFixture(ts_SetupStackTestContext, NULL, ts_DestroyStackTestContext);
  do_run_test(testRStackInit, "testRStackInit", 14);
  do_run_test(testRStackSize, "testRStackSize", 24);
  do_run_test(testRStackPopPush, "testRStackPopPush", 28);
  do_run_test(testRStackReset, "testRStackReset", 41);
  do_run_test(testRStackPeek, "testRStackPeek", 55);
  do_run_test(testRStackTosNos, "testRStackTosNos", 63);
  do_run_test(testRStackCanPop, "testRStackCanPop", 74);
  do_run_test(testRStackCanPush, "testRStackCanPush", 98);
  registerFixture(NULL, NULL, NULL);
  
  UnitySetTestFile("..\test_toy_vm.c");
  registerFixture(vmInit, NULL, NULL);
  do_run_test(testToyVmRunNone, "testToyVmRunNone", 100);
  do_run_test(testToyVmFaultOp, "testToyVmFaultOp", 106);
  do_run_test(testToyVmInc, "testToyVmInc", 123);
  do_run_test(testToyVmInc2, "testToyVmInc2", 138);
  do_run_test(testToyVmLiteral, "testToyVmLiteral", 149);
  do_run_test(testToyVmLoad, "testToyVmLoad", 161);
  do_run_test(testToyVmLoadBad, "testToyVmLoadBad", 169);
  do_run_test(testToyVmOverrun1, "testToyVmOverrun1", 175);
  do_run_test(testToyVmOverrun2, "testToyVmOverrun2", 188);
  do_run_test(testToyVmBadOpcode, "testToyVmBadOpcode", 197);
  registerFixture(NULL, NULL, NULL);
  
  UnitySetTestFile("..\test_u_stack.c");
  registerFixture(ts_SetupStackTestContext, NULL, ts_DestroyStackTestContext);
  do_run_test(testUStackInit, "testUStackInit", 22);
  do_run_test(testUStackSize, "testUStackSize", 32);
  do_run_test(testUStackPopPush, "testUStackPopPush", 36);
  do_run_test(testUStackReset, "testUStackReset", 49);
  do_run_test(testUStackPeek, "testUStackPeek", 63);
  do_run_test(testUStackTosNos, "testUStackTosNos", 71);
  do_run_test(testUStackCanPop, "testUStackCanPop", 82);
  do_run_test(testUStackCanPush, "testUStackCanPush", 106);
  registerFixture(NULL, NULL, NULL);
  
  UnitySetTestFile("..\test_vm.c");
  registerFixture(tsSetupVmFixture, tsDumpVmFixture, tsDestroyVmFixture);
  do_run_test(testscRunNone, "testscRunNone", 14);
  do_run_test(testBadOpcode, "testBadOpcode", 17);
  do_run_test(testVmRunFault, "testVmRunFault", 21);
  do_run_test(testVmRun, "testVmRun", 25);
  do_run_test(testVmOpFault, "testVmOpFault", 39);
  do_run_test(testVmOpFaultFail, "testVmOpFaultFail", 44);
  do_run_test(testVmIpOverrun, "testVmIpOverrun", 49);
  do_run_test(testByteLiteral_stub_10, "testByteLiteral(0)", 112);
  do_run_test(testByteLiteral_stub_11, "testByteLiteral(127)", 113);
  do_run_test(testByteLiteral_stub_12, "testByteLiteral(0xff)", 114);
  do_run_test(testVmOpOverrun_stub_13, "testVmOpOverrun(SC_OP_LIT8, 1)", 115);
  do_run_test(testCellLiteral, "testCellLiteral", 118);
  do_run_test(testVmOpOverrun_stub_14, "testVmOpOverrun(SC_OP_LIT, sizeof(sc_cell_t))", 124);
  do_run_test(testVmOpOverrun2_stub_15, "testVmOpOverrun2(SC_OP_LIT, sizeof(sc_cell_t))", 125);
  do_run_test(testVmUnop_stub_16, "testVmUnop(SC_OP_NEGATE, 0x12345678, -0x12345678)", 128);
  do_run_test(testVmOp_FailUnderflow_stub_17, "testVmOp_FailUnderflow(SC_OP_NEGATE, 0)", 129);
  do_run_test(testVmUnop_stub_18, "testVmUnop(SC_OP_ZERO_EQUALS, 0, -1)", 132);
  do_run_test(testVmUnop_stub_19, "testVmUnop(SC_OP_ZERO_EQUALS, -1, 0)", 133);
  do_run_test(testVmUnop_stub_20, "testVmUnop(SC_OP_ZERO_EQUALS, 12345, 0)", 134);
  do_run_test(testVmOp_FailUnderflow_stub_21, "testVmOp_FailUnderflow(SC_OP_ZERO_EQUALS, 0)", 135);
  do_run_test(testBinop_stub_22, "testBinop(SC_OP_MINUS, 0x12345678, -1, 0x12345679)", 138);
  do_run_test(testVmOp_FailUnderflow_stub_23, "testVmOp_FailUnderflow(SC_OP_MINUS, 1)", 139);
  do_run_test(testVmOp_FailUnderflow_stub_24, "testVmOp_FailUnderflow(SC_OP_MINUS, 0)", 140);
  do_run_test(testBinop_stub_25, "testBinop(SC_OP_TIMES, 1234, 5678, 1234*5678)", 143);
  do_run_test(testVmOp_FailUnderflow_stub_26, "testVmOp_FailUnderflow(SC_OP_TIMES, 1)", 144);
  do_run_test(testVmOp_FailUnderflow_stub_27, "testVmOp_FailUnderflow(SC_OP_TIMES, 0)", 145);
  do_run_test(testSlashMod_stub_28, "testSlashMod(38, 5, 38/5, 38%5)", 156);
  do_run_test(testVmOp_FailUnderflow_stub_29, "testVmOp_FailUnderflow(SC_OP_SLASH_MOD, 1)", 157);
  do_run_test(testVmOp_FailUnderflow_stub_30, "testVmOp_FailUnderflow(SC_OP_SLASH_MOD, 0)", 158);
  do_run_test(testVmOpDrop, "testVmOpDrop", 161);
  do_run_test(testVmOp_FailUnderflow_stub_31, "testVmOp_FailUnderflow(SC_OP_DROP, 0)", 167);
  do_run_test(testVmOpDup, "testVmOpDup", 169);
  do_run_test(testVmOp_FailUnderflow_stub_32, "testVmOp_FailUnderflow(SC_OP_DUP, 0)", 175);
  do_run_test(testVmOp_FailOverflow_stub_33, "testVmOp_FailOverflow(SC_OP_DUP, 0)", 176);
  do_run_test(testVmOpSwap, "testVmOpSwap", 178);
  do_run_test(testVmOp_FailUnderflow_stub_34, "testVmOp_FailUnderflow(SC_OP_SWAP, 0)", 185);
  do_run_test(testVmOp_FailUnderflow_stub_35, "testVmOp_FailUnderflow(SC_OP_SWAP, 1)", 186);
  do_run_test(testVmOpFetchCell_stub_36, "testVmOpFetchCell(0, 1, SC_FAULT_OK)", 201);
  do_run_test(testVmOpFetchCell_stub_37, "testVmOpFetchCell(SC_HEAP_SIZE-1, 1, SC_FAULT_OK)", 202);
  do_run_test(testVmOpFetchCell_stub_38, "testVmOpFetchCell(0, SC_HEAP_SIZE-sizeof(sc_cell_t), SC_FAULT_OK)", 203);
  do_run_test(testVmOpFetchCell_stub_39, "testVmOpFetchCell(0, SC_HEAP_SIZE-sizeof(sc_cell_t)+1, SC_FAULT_MEM)", 204);
  do_run_test(testVmOp_FailUnderflow_stub_40, "testVmOp_FailUnderflow(SC_OP_FETCH, 0)", 205);
  do_run_test(testVmOpFetchByte_stub_41, "testVmOpFetchByte(0, 1, SC_FAULT_OK)", 220);
  do_run_test(testVmOpFetchByte_stub_42, "testVmOpFetchByte(SC_HEAP_SIZE-1, 1, SC_FAULT_OK)", 221);
  do_run_test(testVmOpFetchByte_stub_43, "testVmOpFetchByte(0, SC_HEAP_SIZE-1, SC_FAULT_OK)", 222);
  do_run_test(testVmOpFetchByte_stub_44, "testVmOpFetchByte(0, SC_HEAP_SIZE, SC_FAULT_MEM)", 223);
  do_run_test(testVmOp_FailUnderflow_stub_45, "testVmOp_FailUnderflow(SC_OP_C_FETCH, 0)", 224);
  do_run_test(testVmOpStoreCell_stub_46, "testVmOpStoreCell(0, 1, SC_FAULT_OK)", 236);
  do_run_test(testVmOpStoreCell_stub_47, "testVmOpStoreCell(SC_HEAP_SIZE-1, 1, SC_FAULT_OK)", 237);
  do_run_test(testVmOpStoreCell_stub_48, "testVmOpStoreCell(0, SC_HEAP_SIZE-sizeof(sc_cell_t), SC_FAULT_OK)", 238);
  do_run_test(testVmOpStoreCell_stub_49, "testVmOpStoreCell(0, SC_HEAP_SIZE-sizeof(sc_cell_t)+1, SC_FAULT_MEM)", 239);
  do_run_test(testVmOp_FailUnderflow_stub_50, "testVmOp_FailUnderflow(SC_OP_STORE, 0)", 240);
  do_run_test(testVmOp_FailUnderflow_stub_51, "testVmOp_FailUnderflow(SC_OP_STORE, 1)", 241);
  do_run_test(testVmOpStoreByte_stub_52, "testVmOpStoreByte(0, 1, SC_FAULT_OK)", 253);
  do_run_test(testVmOpStoreByte_stub_53, "testVmOpStoreByte(SC_HEAP_SIZE-1, 1, SC_FAULT_OK)", 254);
  do_run_test(testVmOpStoreByte_stub_54, "testVmOpStoreByte(0, SC_HEAP_SIZE-1, SC_FAULT_OK)", 255);
  do_run_test(testVmOpStoreByte_stub_55, "testVmOpStoreByte(0, SC_HEAP_SIZE, SC_FAULT_MEM)", 256);
  do_run_test(testVmOp_FailUnderflow_stub_56, "testVmOp_FailUnderflow(SC_OP_C_STORE, 0)", 257);
  do_run_test(testVmOp_FailUnderflow_stub_57, "testVmOp_FailUnderflow(SC_OP_C_STORE, 1)", 258);
  do_run_test(testVmOpClear, "testVmOpClear", 261);
  do_run_test(testVmEmit, "testVmEmit", 269);
  do_run_test(testVmOp_FailUnderflow_stub_58, "testVmOp_FailUnderflow(SC_OP_EMIT, 0)", 276);
  do_run_test(testVmOp_FailOverflow_stub_59, "testVmOp_FailOverflow(SC_OP_KEY, 0)", 285);
  do_run_test(testVmCall_stub_60, "testVmCall(2)", 302);
  do_run_test(testVmCall_stub_61, "testVmCall(SC_HEAP_SIZE-2)", 303);
  do_run_test(testVmCallBadReturn, "testVmCallBadReturn", 304);
  do_run_test(testVmCallBadAddress, "testVmCallBadAddress", 314);
  do_run_test(testVmCallIpOverrun, "testVmCallIpOverrun", 318);
  registerFixture(NULL, NULL, NULL);

  return UnityEnd();
}

/*

*/
