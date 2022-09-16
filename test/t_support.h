#ifndef T_SUPPORT_H_
#define T_SUPPORT_H_

// These functions are for building a usable test context and obviously expect that the underlying API calls work properly.
void ts_SetupScGlobals();
void ts_DestroyScGlobals();
void ts_SetupStackTestContext();
void ts_DestroyStackTestContext();

// Assembler for the heap.
// Write some 8 bit values at address pointed to by first arg.
#define TS_ASSEMBLER_CODE(addr_, ...)	do {				\
	uint8_t x[] = { __VA_ARGS__ };							\
	memcpy(&HEAP[*(addr_)], x, sizeof(x));					\
	*(addr_) += sizeof(x);									\
} while (0)

// Write a single cell at address pointed to by first arg.
#define TS_ASSEMBLER_CELL(addr_, v_)	do {				\
	*(sc_cell_t*)&HEAP[*(addr_)] = (v_);					\
	*(addr_) += sizeof(sc_cell_t);							\
} while (0)
	

#endif 		// T_SUPPORT_H_

