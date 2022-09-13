#ifndef SCONSOLE_H__
#define SCONSOLE_H__


#include "sconsole-config.h"


enum {
	//EXC_RUN_PENDING = 0, 	// Only returnd from run when all requested instructons were run.
	//EXC_RUN_YIELDED = -1,	// Returned from run() after executing a YIELD.
	//EXC_RUN_HALTED = -2,	// Returned from run() after executing a HALT.

	SC_EXC_OK = 0,
	SC_EXC_BAD_OPCODE ,
	SC_EXC_BAD_MEM,		// Bad memory access to VM.
	SC_EXC_USER_STACK_UFLOW, SC_EXC_USER_STACK_OFLOW, SC_EXC_USER_STACK_ACCESS,
	SC_EXC_CALL_STACK_UFLOW, SC_EXC_CALL_STACK_OFLOW, SC_EXC_CALL_STACK_ACCESS,
};

// Used to include internal stuff for implementation and testing, not for normal usage.
#ifdef SCONSOLE_WANT_INTERNAL_DEFS

// The context, holds the current state of a sconsole process. 
typedef struct {
#ifdef TEST
	// TODO: Use __attribute__((packed))
	sc_cell_t t_ustack_pre;
#endif
	sc_cell_t ustack[SC_DATA_STACK_SIZE];
#ifdef TEST
	sc_cell_t t_ustack_post;
#endif
	sc_cell_t* usp;

	sc_cell_t rstack[SC_RETURN_STACK_SIZE];
	sc_cell_t* rp;

	sc_cell_t err;		// Set non-zero on error.
	sc_cell_t ip;		// Index of next instruction to be executed. 
} sc_context_t;

// Heap and some global variables used by all sconsole processes.
typedef struct {
#ifdef TEST
	// TODO: Use __attribute__((packed))
	uint8_t t_heap_pre;
#endif
	uint8_t heap[SC_HEAP_SIZE];
#ifdef TEST
	uint8_t t_heap_post;
#endif
	sc_cell_t here;		// Next free byte in heap.
	sc_cell_t latest;	// Free byte following last definition. 
	sc_context_t* current;	// Currenty executing sconsole process.
} sc_globals_t;

extern sc_globals_t g_sc;

// Hack to allow access by macros.
extern sc_context_t* sc_ctx;

#define ELEMENT_COUNT(a_) (sizeof(a_) / sizeof (a_[0]))

/* User stack operations. Designed to be called from a function where `ctx' points to a sc_contect_t. 
	Basic functions are pop, push, clear; tos & nos are top & next item on stack.
	stackbase is a factor for a few macros.
	depth is the number of items on the stack, and free() is the number of free spaces before it overflows.
	There is no checking of the stack, this must be done before using these functions. */
#define u_pop() u_can_pop(1) ? (*(sc_ctx->usp++)) : (sc_ctx->err = SC_EXC_USER_STACK_UFLOW, 0)
#define u_push(x_) if (u_can_push(1)) *--sc_ctx->usp = (sc_cell_t)(x_); else sc_ctx->err = SC_EXC_USER_STACK_OFLOW
#define u_size() ELEMENT_COUNT(sc_ctx->ustack)
#define u_stackbase (sc_ctx->ustack + u_size())
#define u_reset() sc_ctx->usp = u_stackbase;
#define u_peek(n_) *( ((n_) < u_depth()) ? &sc_ctx->usp[n_] : (sc_ctx->err = SC_EXC_USER_STACK_ACCESS, &sc_ctx->ustack[0]))
#define u_tos u_peek(0)
#define u_nos u_peek(1)
#define u_depth() (u_stackbase - sc_ctx->usp)		
#define u_free() (sc_ctx->usp - sc_ctx->ustack)

// Checks if can push/pop the user stack.
#define u_can_pop(n_)  (u_depth() >= (n_))
#define u_can_push(n_) (u_depth() <= (SC_DATA_STACK_SIZE - (n_)))

// Return stack has more limited operations. 
#define r_pop() (*(sc_ctx->rp++))
#define r_push(x_) *--sc_ctx->rp = (sc_cell_t)(x_)
#define r_stackbase (sc_ctx->rstack + sizeof(sc_ctx->rstack))
#define r_reset() sc_ctx->rp = r_stackbase;
#define r_peek(n_) sc_ctx->rp[n_]
#define r_tos r_peek(0)
#define r_nos r_peek(1)
#define r_depth() (r_stackbase - sc_ctx->rp)		
#define r_free() (SC_RETURN_STACK_SIZE - r_depth())

// Checks if can push/pop the user stack.
#define r_can_pop(n_)  (r_depth() >= (n_))
#define r_can_push(n_) (r_depth() <= (SC_RETURN_STACK_SIZE - (n_)))

// Instruction pointer.
#define IP (sc_ctx->ip)

// Initialise the console.
void scInitContext(sc_context_t* ctx);

/* Sconsole's only needs to access its "virtual memory", a subset of physical memory. You can define custom words to access physical memory.
	This is comprised of;
	* the RAM heap in low memory from address zero containing words that you define.
	* a read only area directly above it containing the dictionary of standard words. 
	* there may be memory above this to implement multiple processes or similar. 
	Byte and cell accesses are allowed, but the byte access (red or write) may not cross the boundary.
	Writes are obviously not allowed to the read only area.
	Read/write werrors generate distinct errors
*/
// Set IP as index into heap.
void scSetIp(sc_context_t* ctx, sc_ucell_t offs);

// Write byte to heap. No action if outside heap. 
void scHeapWriteByte(sc_context_t* ctx, sc_ucell_t offs, uint8_t v);

// Write cell to heap. No action if outside heap. 
void scHeapWriteCell(sc_context_t* ctx, sc_ucell_t offs, sc_ucell_t v);

// Read byte from heap.
uint8_t scHeapReadByte(sc_context_t* ctx, sc_ucell_t offs);

// Read cell from heap.
sc_ucell_t scHeapReadCell(sc_context_t* ctx, sc_ucell_t offs);


#endif // SCONSOLE_WANT_INTERNAL_DEFS

#endif // SCONSOLE_H__
