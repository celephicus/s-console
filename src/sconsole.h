#ifndef SCONSOLE_H__
#define SCONSOLE_H__

// Include configuration. 
#include "sconsole-config.h"

enum {
	SC_FAULT_OK = 0,
	SC_FAULT_BAD_OPCODE ,
	SC_FAULT_MEM,		// Bad memory access to VM.
	SC_FAULT_BAD_IP,
	SC_FAULT_U_STACK_UFLOW, SC_FAULT_U_STACK_OFLOW, SC_FAULT_U_STACK_ACCESS,
	SC_FAULT_R_STACK_UFLOW, SC_FAULT_R_STACK_OFLOW, SC_FAULT_R_STACK_ACCESS,
};

// Initialise the console.
typedef struct ScContext ScContext;
void scInitContext(ScContext* ctx);
void scInit(ScContext* ctx);
uint8_t scRun(int n);

// Read/write byte on heap. Will set memory fault if out of bounds. 
void scHeapWriteByte(sc_cell_t offs, uint8_t v);
uint8_t scHeapReadByte(sc_cell_t offs);

// Read/write Sconsole cell on heap. Will set memory fault if out of bounds. 
void scHeapWriteCell(sc_cell_t offs, sc_cell_t v);
sc_cell_t scHeapReadCell(sc_cell_t offs);

// Used to include internal stuff for implementation and testing, not for normal usage.
#ifdef SCONSOLE_WANT_INTERNAL_DEFS

#include "sconsole.auto.h"

// Functions to read/write a character to the context's console.
typedef void (*sc_putc_func)(char);
typedef int16_t (*sc_getc_func)();

// The context, holds the current state of a sconsole process. 
typedef struct  __attribute__((packed)) ScContext {
#ifdef TEST
	sc_cell_t t_u_stack_pre;
#endif
	sc_cell_t u_stack[SC_U_STACK_SIZE];
#ifdef TEST
	sc_cell_t t_u_stack_post;
#endif
	sc_cell_t* u_sp;

#ifdef TEST
	sc_cell_t t_r_stack_pre;
#endif
	sc_cell_t r_stack[SC_R_STACK_SIZE];
#ifdef TEST
	sc_cell_t t_r_stack_post;
#endif
	sc_cell_t* r_sp;

	sc_cell_t fault;		// Set non-zero on error.
	sc_cell_t ip;		// Index of next instruction to be executed. 
	sc_putc_func putc;
	sc_getc_func getc;
} ScContext;

// State of Sconsole. Contains heap and some global variables used by all sconsole processes.
typedef struct __attribute__((packed)) {
#ifdef TEST
	uint8_t t_heap_pre;
#endif
	uint8_t heap[SC_HEAP_SIZE];
#ifdef TEST
	uint8_t t_heap_post;
#endif
	sc_cell_t here;		// Next free byte in heap.
	sc_cell_t latest;	// Free byte following last definition. 
	ScContext* ctx;	// Currently executing sconsole process.
} ScState;

extern ScState g_sc_state;

// Some macros to access the current Sconsole instance, makes things easier for macros.
#define CTX (g_sc_state.ctx)
#define HEAP (g_sc_state.heap)
#define LATEST (g_sc_state.latest)
#define HERE (g_sc_state.here)
#define IP (CTX->ip)			
#define FAULT (CTX->fault)			

#define ELEMENT_COUNT(a_) (sizeof(a_) / sizeof (a_[0]))

/* User stack operations. Designed to be called where `CTX' points to a ScContext. 
	Basic functions are pop, push, clear; tos & nos are top & next item on stack.
	stackbase is a factor for a few macros.
	depth is the number of items on the stack, and free() is the number of free spaces before it overflows.
	can_pop/can_push checks if can push/pop the user stack by the specified number of items.
*/
#define u_pop() (*(CTX->u_sp++))
#define u_drop() (CTX->u_sp++)
#define u_push(x_) (*--CTX->u_sp = (sc_cell_t)(x_))
#define u_size() ELEMENT_COUNT(CTX->u_stack)
#define u_stackbase (CTX->u_stack + u_size())
#define u_reset() CTX->u_sp = u_stackbase;
#define u_peek(n_) (CTX->u_sp[n_])
#define u_tos u_peek(0)
#define u_nos u_peek(1)
#define u_depth() (u_stackbase - CTX->u_sp)		
#define u_free() (CTX->u_sp - CTX->u_stack)
#define u_can_pop(n_)  ((n_) <= u_depth())
#define u_can_push(n_) ((n_) <= u_free())

// Return stack operations are a copy of those for the user stack but with `u_' replaced with `r_'
//TODO: Automate R stack macros!
#define r_pop() (*(CTX->r_sp++))
#define r_push(x_) *--CTX->r_sp = (sc_cell_t)(x_); 
#define r_size() ELEMENT_COUNT(CTX->r_stack)
#define r_stackbase (CTX->r_stack + r_size())
#define r_reset() CTX->r_sp = r_stackbase;
#define r_peek(n_) (CTX->r_sp[n_])
#define r_tos r_peek(0)
#define r_nos r_peek(1)
#define r_depth() (r_stackbase - CTX->r_sp)		
#define r_free() (CTX->r_sp - CTX->r_stack)
#define r_can_pop(n_)  ((n_) <= r_depth())
#define r_can_push(n_) ((n_) <= r_free())

/* Sconsole's only needs to access its "virtual memory", a subset of physical memory. You can define custom words to access physical memory.
	This is comprised of;
	* the RAM heap in low memory from address zero containing words that you define.
	* a read only area directly above it containing the dictionary of standard words. 
	* there may be memory above this to implement multiple processes or similar. 
	Byte and cell accesses are allowed, but the byte access (read or write) may not cross the boundary.
	Writes are obviously not allowed to the read only area.
	Illegal read/write generate distinct errors.
*/

// Instructions...
enum {
	SC_OPCODES
};
#endif // SCONSOLE_WANT_INTERNAL_DEFS

#endif // SCONSOLE_H__
