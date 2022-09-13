
/* A Sconsole process is defined by the state of its context, which contains the user & call stacks, an instruction pointer to the memory space and an error indicator.
	Running a Sconsole process involves initialising a context object, then setting it as current with scSetContext(), then calling scRun().
	You can implement multitasking by switching between several contexts concurrently, ideally by having each call YIELD periodically. Note that since all processes
	share the heap, only have one process allocating memory else you will get corruption. 
	
	Error handling is simple: all primitives can set an error indicator to a non-zero value, in which case run() will stop and return the error, with IP set to the instruction
	that caused the error. 
typedef struct {
	cell_t 
	cell_t ip;  	// A pointer to an internal memory space.
	cell_t error; 	// Set to non-zero on error.
	stack_t user;
	stack_t call;
} context_t;

// Set to current context.
context_t* ctx;

// Checked primitove operations
void u_drop() {
	if (!ctx->user.can_pop(1) )
		ctx->error = EXC_USER_STACK_UFLOW;
	else 
		ctx->user.pop();
}

#define SET_ERROR(_exc) return ctx->error = (_exc)
#define CHECK_ERROR() if (error) return ctx->error

/* Stack notation
	f - Boolean flag, either all 1's or all 0's.
	c - 8 bit character.
    w - word, signed/unsigned integer.
	n - signed integer.
	u - unsigned integer.
    d - double sized signed integer
    ud - double sized unsigned integer
	a - address
*/

// Runs at most `n' instructions then return PENDING, or continuously if n is negative. HALT, YIELD or an error will return corresponding status.
void run(context_t* ctx, int n) {
	while (1) {
		op = mem_get_byte(IP); CHECK_ERROR();
		switch (op) {
			case PRIM_LAMBDA: u_push(mem_get_byte(IP)); CHECK_ERROR(); IP += u_tos + 1; break;
			case PRIM_LIT: c = mem_get_cell(IP + 1); CHECK_ERROR(); u_push(c); IP += sizeof(cell_t); break;
			case PRIM_DROP: u_drop(); CHECK_ERROR(); break;
			case PRIM_YIELD: SET_ERROR(EXC_RUN_YIELDED);
			case PRIM_HALT: SET_ERROR(EXC_RUN_HALTED);
			case PRIM_ERROR: c = u_pop(); CHECK_ERROR(); SET_ERROR(c); break;
			case PRIM_CALL: r_push(IP + 1); CHECK_ERROR(); c = u_pop(); CHECK_ERROR(); IP = mem_get_cell(c); CHECK_ERROR(); 
			default: 
			if (op & 0x80) {
				IP += 1;
				r_push(IP + 2); CHECK_ERROR(); 
				IP = (cell_t)op | mem_get_byte(IP + 1); CHECK_ERROR();
			}
			else
				SET_ERROR(EXC_BAD_OPCODE);
		}
		IP += 1; 
		if (n > 0) {
			if (0 == --n)
				SET_ERROR(EXC_RUN_PENDING);
		}
	}
}