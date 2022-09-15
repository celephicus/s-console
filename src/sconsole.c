#include <stdbool.h>

#define SCONSOLE_WANT_INTERNAL_DEFS
#include "sconsole.h"

// One instance of global state.
sc_globals_t g_sc;

// Hack to allow access by macros.
ScContext* g_sc_ctx;

void scInitContext(ScContext* ctx) {
	g_sc_ctx = ctx;
	FAULT = SC_FAULT_OK;		// Set no error. 
	u_reset();
	r_reset();
	// TODO: Set IP to some value to fault if not set to something else. 
}

uint8_t scHeapReadByte(sc_cell_t offs) {
	if(offs <= SC_HEAP_SIZE - sizeof(uint8_t))
		return g_sc.heap[offs];
//	if(offs <= SC_HEAP_SIZE + SC_READ_ONLY_SIZE - sizeof(uint8_t))
//		return PGM_READ_BYTE(offs - SC_HEAP_SIZE);
	else {
		FAULT = SC_FAULT_MEM;
		return (uint8_t)-1;
	}
}
void scHeapWriteByte(sc_cell_t offs, uint8_t v) {
	if(offs <= SC_HEAP_SIZE - sizeof(uint8_t))
		g_sc.heap[offs] = v;
//	if(offs <= SC_HEAP_SIZE + SC_READ_ONLY_SIZE - sizeof(uint8_t))
//		return PGM_READ_BYTE(offs - SC_HEAP_SIZE);
	else
		FAULT = SC_FAULT_MEM;
}

sc_cell_t scHeapReadCell(sc_cell_t offs) {
	if(offs <= SC_HEAP_SIZE - sizeof(sc_cell_t))
		return *(const sc_cell_t*)&g_sc.heap[offs];
//	if(offs <= SC_HEAP_SIZE + SC_READ_ONLY_SIZE - sizeof(uint8_t))
//		return PGM_READ_BYTE(offs - SC_HEAP_SIZE);
	else {
		FAULT = SC_FAULT_MEM;
		return (sc_cell_t)-1;
	}
}
void scHeapWriteCell(sc_cell_t offs, sc_cell_t v) {
	if(offs <= SC_HEAP_SIZE - sizeof(sc_cell_t))
		*(sc_cell_t*)&g_sc.heap[offs] = v;
//	if(offs <= SC_HEAP_SIZE + SC_READ_ONLY_SIZE - sizeof(uint8_t))
//		return PGM_READ_BYTE(offs - SC_HEAP_SIZE);
	else
		FAULT = SC_FAULT_MEM;
}

#if 0
void scInit(ScContext* ctxs, int num_ctx) {
	g_sc->contexts = ctxs;
	//g_sc->num_ctx = num_ctx;
	if (1 != num_ctx)
		return 1;
	g_sc->current = &g_sc->contexts[0];
	u_reset();
	scSetIp(ctx , -1);		// Set invalid.
}

void scSetIp(ScContext* ctx, sc_ucell_t offs) {
	g_sc_ctx->ip = offs; // No check, exec will do that.
}

static bool is_byte_within_heap(ScContext* ctx, sc_ucell_t offs) {
	return (offs <= SC_HEAP_SIZE - sizeof(uint8_t));
}
static bool is_cell_within_heap(ScContext* ctx, sc_ucell_t offs) {
	return (offs <= SC_HEAP_SIZE - sizeof(sc_cell_t));
}
void scHeapWriteByte(ScContext* ctx, sc_ucell_t offs, uint8_t v) {
	if(offs <= SC_HEAP_SIZE - sizeof(uint8_t))
		g_sc_ctx->heap[offs] = v;
	if(offs <= SC_HEAP_SIZE + SC_READ_ONLY_SIZE - sizeof(uint8_t))
		g_sc_ctx->error = SC_ERR_MEM_WRITE_RO;
	else
		g_sc_ctx->error = SC_ERR_MEM_WRITE_RANGE;
}
void scHeapWriteCell(ScContext* ctx, sc_ucell_t offs, sc_ucell_t v) {
	if(offs <= SC_HEAP_SIZE - sizeof(sc_ucell_t))
		(*sc_ucell_t)&g_sc_ctx->heap[offs] = v;
	if(offs <= SC_HEAP_SIZE + SC_READ_ONLY_SIZE - sizeof(sc_ucell_t))
		g_sc_ctx->error = SC_ERR_MEM_WRITE_RO;
	else
		g_sc_ctx->error = SC_ERR_MEM_WRITE_RANGE;
}

// Read cell from heap.
sc_ucell_t scHeapReadCell(ScContext* ctx, sc_ucell_t offs) {
	if(offs <= SC_HEAP_SIZE - sizeof(sc_ucell_t))
		return g_sc_ctx->heap[offs] = v;
	if(offs <= SC_HEAP_SIZE + SC_READ_ONLY_SIZE - sizeof(sc_ucell_t))
		return PGM_READ_DWORD(offs - SC_HEAP_SIZE);
	else
		g_sc_ctx->error = SC_ERR_MEM_WRITE_RANGE;
}

#endif