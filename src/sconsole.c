#include <stdbool.h>
#include <stdlib.h>

#define SCONSOLE_WANT_INTERNAL_DEFS
#include "sconsole.h"

#define SC_STATIC /* empty */
// TODO: implement SC_STATIC properly.

// One instance of global state.
ScState g_sc_state;

void scInitContext(ScContext* ctx) {
	ctx->fault = SC_FAULT_OK;		// Set no error. 
	// TODO: How to clear stacks before CTX is set?
	ctx->u_sp = ctx->u_stack + SC_U_STACK_SIZE;
	ctx->r_sp = ctx->r_stack + SC_R_STACK_SIZE;
	// TODO: Set IP to some value to fault if not set to something else. 
}
void scInit(ScContext* ctx) {
	g_sc_state.ctx = ctx; 
	g_sc_state.here = 0;
	g_sc_state.latest = 0;
}

static uint8_t heap_read_byte_ip() {
	uint8_t v = scHeapReadByte(IP++);
	if (FAULT)
		FAULT = SC_FAULT_BAD_IP;
	return v;
}
static sc_cell_t heap_read_cell_ip() {
	sc_cell_t v = scHeapReadCell(IP);
	IP += sizeof(sc_cell_t);
	if (FAULT)
		FAULT = SC_FAULT_BAD_IP;
	return v;
}

SC_STATIC uint8_t sc_find_primitive_op(uint16_t h) {
	typedef struct { uint16_t h; uint8_t op; } PrimopDef;
	static const PrimopDef SC_PRIMITIVE_HASH[] = {
		SC_CONST_DICT
	};

	const PrimopDef* low = &SC_PRIMITIVE_HASH[0];
	const PrimopDef* high = low + ELEMENT_COUNT(SC_PRIMITIVE_HASH);
	while (low <= high) {
		const PrimopDef* mid = low + (high - low) / 2;  // Yes this is legal pointer arithmetic.

		if (h == mid->h)
			return mid->op;		// Found it!

		if (mid->h < h)
			low = mid + 1;

		else
			high = mid - 1;
	}
	return -1;
 }
 
// Hash function as we store command names as a 16 bit hash. Lower case letters are converted to upper case.
// The values came from Wikipedia and seem to work well, in that collisions between the hash values of different commands are very rare.
// All characters in the string are hashed even non-printable ones.
#define HASH_START (5381)
#define HASH_MULT (33)
SC_STATIC uint16_t sc_hash(const char* str) {
	uint16_t h = HASH_START;
	char c;
	while ('\0' != (c = *str++)) {
		if ((c >= 'a') && (c <= 'z')) 	// Normalise letter case to UPPER CASE. 
			c -= 'a' - 'A';
		h = (h * HASH_MULT) ^ (uint16_t)c;
	}
	return h;
}	 
	
// Used in the VM to check if an fault has been set, and jump to fault handler if it has. 
#define CHECK_FAULT(op_) do { op_; if (FAULT) goto f_a_u_l_t; } while (0)

// Used in the VM to set an fault, and jump to fault handler. 
#define SET_FAULT(err_) do { FAULT = (err_); goto f_a_u_l_t; } while (0)

// Used in VM to verify IP within range. 
#define VERIFY_VAL_FOR_IP(ip_) ((ip_) < (SC_HEAP_SIZE + SC_READ_ONLY_SIZE))
#define VERIFY_IP() do { if (!VERIFY_VAL_FOR_IP(IP)) SET_FAULT(SC_FAULT_BAD_IP); } while (0)

/* Some helper functions & macros for commands. */
#define BINOP(op_) do { VERIFY_U_CAN_POP(2); scratch.cell = u_pop(); u_tos = u_tos op_ scratch.cell; } while (0)
#define UNOP(op_) do { VERIFY_U_CAN_POP(1); u_tos = op_ u_tos; } while (0)

#define VERIFY_U_CAN_POP(n_) if (u_can_pop(n_)) {} else SET_FAULT(SC_FAULT_U_STACK_UFLOW)
#define VERIFY_U_CAN_PUSH(n_) if (u_can_push(n_)) {} else SET_FAULT(SC_FAULT_U_STACK_OFLOW)
#define VERIFY_R_CAN_POP(n_) if (r_can_pop(n_)) {} else SET_FAULT(SC_FAULT_R_STACK_UFLOW)
#define VERIFY_R_CAN_PUSH(n_) if (r_can_push(n_)) {} else SET_FAULT(SC_FAULT_R_STACK_OFLOW)
	
uint8_t scRun(int n) {
	static const void* OPS[] = { SC_JUMPS };
	union Scratch {
		uint8_t u8;		
		int8_t i8;		
		sc_cell_t cell;	
		uint16_t u16;
		div_t rdiv;
	} scratch;
	
next: 
	if (FAULT) 			// Cannot run in fault state.
		return FAULT;
		
	if (0 == n) 	// Zero instructions to do so exit.
		return SC_FAULT_OK;	
	if (n > 0)	// Only for non-negative n: count instructions then exit when we have done our quota. 
		n -= 1;
    
    uint8_t op;
	// last_ip = IP;	// Store last IP to restore it on fault.
	CHECK_FAULT(op = heap_read_byte_ip());	// Load opcode, increment IP,  maybe flag IP out of range. 
	
	if (op & 0x80) {		// Looks like a call...
		CHECK_FAULT(scratch.u16 = ((uint16_t)(op & ~0x80) << 8) | (uint16_t)heap_read_byte_ip()); // Set fault if second byte of address out or range.
		r_push(IP);
		IP = scratch.u16;
		VERIFY_IP();		// Check call addess in range.
		goto next;
	}
	
	if (op >= ELEMENT_COUNT(OPS))	// Illegal opcode.
		SET_FAULT(SC_FAULT_BAD_OPCODE);
		
	goto *OPS[op];				// Jump to code snippets to do words. 
	
f_a_u_l_t:	
	// IP = last_ip;	// Restore IP on fault.
	return FAULT;

// Words in snippet form.	
SC_SNIPPETS
}

uint8_t scHeapReadByte(sc_cell_t offs) {
	if(offs <= SC_HEAP_SIZE - sizeof(uint8_t))
		return HEAP[offs];
//	if(offs <= SC_HEAP_SIZE + SC_READ_ONLY_SIZE - sizeof(uint8_t))
//		return PGM_READ_BYTE(offs - SC_HEAP_SIZE);
	else {
		FAULT = SC_FAULT_MEM;
		return (uint8_t)-1;
	}
}
void scHeapWriteByte(sc_cell_t offs, uint8_t v) {
	if(offs <= SC_HEAP_SIZE - sizeof(uint8_t))
		HEAP[offs] = v;
//	if(offs <= SC_HEAP_SIZE + SC_READ_ONLY_SIZE - sizeof(uint8_t))
//		return PGM_READ_BYTE(offs - SC_HEAP_SIZE);
	else
		FAULT = SC_FAULT_MEM;
}

sc_cell_t scHeapReadCell(sc_cell_t offs) {
	if(offs <= SC_HEAP_SIZE - sizeof(sc_cell_t))
		return *(const sc_cell_t*)&HEAP[offs];
//	if(offs <= SC_HEAP_SIZE + SC_READ_ONLY_SIZE - sizeof(uint8_t))
//		return PGM_READ_BYTE(offs - SC_HEAP_SIZE);
	else {
		FAULT = SC_FAULT_MEM;
		return (sc_cell_t)-1;
	}
}
void scHeapWriteCell(sc_cell_t offs, sc_cell_t v) {
	if(offs <= SC_HEAP_SIZE - sizeof(sc_cell_t))
		*(sc_cell_t*)&HEAP[offs] = v;
//	if(offs <= SC_HEAP_SIZE + SC_READ_ONLY_SIZE - sizeof(uint8_t))
//		return PGM_READ_BYTE(offs - SC_HEAP_SIZE);
	else
		FAULT = SC_FAULT_MEM;
}
