#ifndef T_SUPPORT_H_
#define T_SUPPORT_H_

// These functions are for building a usable test context and obviously expect that the underlying API calls work properly.
void ts_SetupScGlobals();
void ts_DestroyScGlobals();
void ts_SetupStackTestContext();
void ts_DestroyStackTestContext();

void tsSetGetc(const char* s);
char* tsGetPutc();

// Assembler for the heap.
extern uint16_t g_ts_assembler_loc;
#define TS_ASSEMBLER_LOC (g_ts_assembler_loc)

// Write some 8 bit values at address pointed to by first arg.
#define TS_ASSEMBLER_CODE(...)	do {				\
	uint8_t x[] = { __VA_ARGS__ };							\
	memcpy(&HEAP[g_ts_assembler_loc], x, sizeof(x));					\
	g_ts_assembler_loc += sizeof(x);									\
} while (0)

// Write a single cell at address pointed to by first arg.
#define TS_ASSEMBLER_CELL(v_)	do {				\
	*(sc_cell_t*)&HEAP[g_ts_assembler_loc] = (v_);					\
	g_ts_assembler_loc += sizeof(sc_cell_t);							\
} while (0)
	
// Assemble a CALL to the given 16 bit address.
#define TS_ASSEMBLER_CALL(call_)						\
	TS_ASSEMBLER_CODE(((uint8_t)((call_) >> 8)) | 0x80, (uint8_t)(call_))

#endif 		// T_SUPPORT_H_

