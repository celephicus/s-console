#include <stdbool.h>

#define SCONSOLE_WANT_INTERNAL_DEFS
#include "sconsole.h"

// One instance of global state.
sc_globals_t g_sc;

// Hack to allow access by macros.
sc_context_t* sc_ctx;

void scInitContext(sc_context_t* ctx) {
	sc_ctx = ctx;
	sc_ctx->err = SC_EXC_OK;		// Set no error. 
	u_reset();
//	r_reset();
	// TODO: Set IP to some value to fault if not set to something else. 
}

#if 0
void scInit(sc_context_t* ctxs, int num_ctx) {
	g_sc->contexts = ctxs;
	//g_sc->num_ctx = num_ctx;
	if (1 != num_ctx)
		return 1;
	g_sc->current = &g_sc->contexts[0];
	u_reset();
	scSetIp(ctx , -1);		// Set invalid.
}

void scSetIp(sc_context_t* ctx, sc_ucell_t offs) {
	sc_ctx->ip = offs; // No check, exec will do that.
}

static bool is_byte_within_heap(sc_context_t* ctx, sc_ucell_t offs) {
	return (offs <= SC_HEAP_SIZE - sizeof(uint8_t));
}
static bool is_cell_within_heap(sc_context_t* ctx, sc_ucell_t offs) {
	return (offs <= SC_HEAP_SIZE - sizeof(sc_cell_t));
}
void scHeapWriteByte(sc_context_t* ctx, sc_ucell_t offs, uint8_t v) {
	if(offs <= SC_HEAP_SIZE - sizeof(uint8_t))
		sc_ctx->heap[offs] = v;
	if(offs <= SC_HEAP_SIZE + SC_READ_ONLY_SIZE - sizeof(uint8_t))
		sc_ctx->error = SC_ERR_MEM_WRITE_RO;
	else
		sc_ctx->error = SC_ERR_MEM_WRITE_RANGE;
}
void scHeapWriteCell(sc_context_t* ctx, sc_ucell_t offs, sc_ucell_t v) {
	if(offs <= SC_HEAP_SIZE - sizeof(sc_ucell_t))
		(*sc_ucell_t)&sc_ctx->heap[offs] = v;
	if(offs <= SC_HEAP_SIZE + SC_READ_ONLY_SIZE - sizeof(sc_ucell_t))
		sc_ctx->error = SC_ERR_MEM_WRITE_RO;
	else
		sc_ctx->error = SC_ERR_MEM_WRITE_RANGE;
}

// Read byte from heap.
uint8_t scHeapReadByte(sc_context_t* ctx, sc_ucell_t offs) {
	if(offs <= SC_HEAP_SIZE - sizeof(uint8_t))
		return sc_ctx->heap[offs] = v;
	if(offs <= SC_HEAP_SIZE + SC_READ_ONLY_SIZE - sizeof(uint8_t))
		return PGM_READ_BYTE(offs - SC_HEAP_SIZE);
	else
		sc_ctx->error = SC_ERR_MEM_WRITE_RANGE;
}
// Read cell from heap.
sc_ucell_t scHeapReadCell(sc_context_t* ctx, sc_ucell_t offs) {
	if(offs <= SC_HEAP_SIZE - sizeof(sc_ucell_t))
		return sc_ctx->heap[offs] = v;
	if(offs <= SC_HEAP_SIZE + SC_READ_ONLY_SIZE - sizeof(sc_ucell_t))
		return PGM_READ_DWORD(offs - SC_HEAP_SIZE);
	else
		sc_ctx->error = SC_ERR_MEM_WRITE_RANGE;
}

#endif