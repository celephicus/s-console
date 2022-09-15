#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "unity.h"

// A play VM for trying out ideas.
#define VM_CODE_SIZE 10
static uint8_t val; 
static uint8_t code[VM_CODE_SIZE];
static uint8_t IP;			// Instruction pointer, indexes into code.
static uint8_t FAULT;		// Error indicator.

TT_BEGIN_INCLUDE()
enum { // Error codes...
	FAULT_OK, 		// No fault.
	FAULT_MEM,		// Access out of memory space. 
	FAULT_BAD_OPCODE,	// Illegal opcode.
	FAULT_BAD_IP,		// IP outside of memory space.
	FAULT_USER		// User fault codes from here.
};

enum { // Instructions...
	OP_FAULT,	// Sets fault from next value in instruction stream.
	OP_INC,		// Increments val.
	OP_LIT,		// Loads next value in instruction stream into val.
	OP_LOAD,	// Load val from address in val.
};
TT_END_INCLUDE()

// Used in the VM to check if an fault has been set, and jump to fault handler if it has. 
#define CHECK_FAULT(op_) do { op_; if (FAULT) goto fault; } while (0)

// Used in the VM to set an fault, and jump to fault handler. 
#define SET_FAULT(err_) do { FAULT = (err_); goto fault; } while (0)

// Read the VM's tiny little memory.
	static uint8_t mem8rd(uint8_t addr) {
	if (addr >= sizeof(code)) { FAULT = FAULT_MEM; return -1; }
	return code[addr];
}	

// Read the VM's memory pointed to be IP. On fault this sets a different fault of IP out of range.
static uint8_t mem8rd_ip() {
	uint8_t addr = IP++;
	if (addr >= sizeof(code)) { FAULT = FAULT_BAD_IP; return -1; }
	return code[addr];
}	

#define ELEMENT_COUNT(x_) (sizeof(x_) / sizeof(x_[0]))

// Run instructions through the VM until either  n instructions (if n positive) or an fault is set or HALT is executed.
static uint8_t vmRun(int n) {
	static void* OPS[] = { &&op_fault, &&op_inc, &&op_lit, &&op_load, };
	uint8_t c;		// Scratchpad
	
	if (FAULT) 			// Cannot run in fault state.
		return FAULT;
		
next: 
	if (0 == n) 	// Zero instructions to do so exit.
		return FAULT_OK;
	if (n > 0)	// Only for non-negative n: count instructions then exit when we have done our quota. 
		n -= 1;
    
    uint8_t op;
	// last_ip = IP;	// Store last IP to restore it on fault.
	CHECK_FAULT(op = mem8rd_ip());	// Load opcode, increment IP,  maybe flag IP out of range. 
	
	if (op >= ELEMENT_COUNT(OPS))	// Illegal opcode.
		SET_FAULT(FAULT_BAD_OPCODE);
		
	goto *OPS[op];				// Jump to code snippets to do words. 
	
fault:	
	// IP = last_ip;	// Restore IP on fault.
	return FAULT;

// Words in snippet form.	
op_fault: CHECK_FAULT(c = mem8rd_ip()); SET_FAULT(c);
op_inc: val += 1; goto next;
op_lit: CHECK_FAULT(val = mem8rd_ip()); goto next;
op_load: CHECK_FAULT(val = mem8rd(val)); goto next;
}

void vmInit() {
	memset(code, 0xee, sizeof(code));
	IP = 0;
	val = 0;
	FAULT = FAULT_OK;		// Clear fault. 
}

TT_BEGIN_FIXTURE(vmInit, NULL, NULL);

#define CODE(...)	do {									\
	uint8_t x[] = { __VA_ARGS__ };							\
	memcpy(code, x, ELEMENT_COUNT(x));						\
} while (0)
	
void testVmRunNone() {
	TEST_ASSERT_EQUAL(FAULT_OK, vmRun(0));
	TEST_ASSERT_EQUAL(0, IP);
	TEST_ASSERT_EQUAL(0, val);
}

void testVmFaultOp() {
	CODE(OP_FAULT, FAULT_USER, OP_INC);
	
	TEST_ASSERT_EQUAL(FAULT_USER, vmRun(10));
	TEST_ASSERT_EQUAL(2, IP);
	TEST_ASSERT_EQUAL(0, val);
	
	// Nothing should happen as VM has fault set. 
	TEST_ASSERT_EQUAL(FAULT_USER, vmRun(10));
	TEST_ASSERT_EQUAL(2, IP);
	TEST_ASSERT_EQUAL(0, val);
}

static void build_n_incs(int n) {
	memset(code, OP_INC, n);
	code[n] = OP_FAULT; code[n+1] = FAULT_USER;
}
void testInc() {
	build_n_incs(6);
	
	TEST_ASSERT_EQUAL(FAULT_OK, vmRun(1));
	TEST_ASSERT_EQUAL(1, IP);	
	TEST_ASSERT_EQUAL(1, val);

	TEST_ASSERT_EQUAL(FAULT_OK, vmRun(2));
	TEST_ASSERT_EQUAL(3, IP);	
	TEST_ASSERT_EQUAL(3, val);

	TEST_ASSERT_EQUAL(FAULT_USER, vmRun(-1));
	TEST_ASSERT_EQUAL(6+2, IP);	
}

void testInc2() {
	CODE(OP_INC, OP_INC, OP_FAULT, FAULT_USER);
	
	TEST_ASSERT_EQUAL(FAULT_OK, vmRun(2));
	TEST_ASSERT_EQUAL(2, IP);	
	TEST_ASSERT_EQUAL(2, val);

	TEST_ASSERT_EQUAL(FAULT_USER, vmRun(1));
	TEST_ASSERT_EQUAL(2+2, IP);	
}

void testLiteral() {
	CODE(OP_LIT, 122, OP_FAULT, FAULT_USER);
	
	TEST_ASSERT_EQUAL(FAULT_OK, vmRun(1));	// Load literal. 
	TEST_ASSERT_EQUAL(2, IP);	
	TEST_ASSERT_EQUAL(122, val);
	
	TEST_ASSERT_EQUAL(FAULT_USER, vmRun(-1));	// Fault.
	TEST_ASSERT_EQUAL(2+2, IP);	
	TEST_ASSERT_EQUAL(122, val);
}

void testLoad() {
	CODE(OP_LIT, 6, OP_LOAD, OP_FAULT, FAULT_USER, 0, 122);
	
	TEST_ASSERT_EQUAL(FAULT_USER, vmRun(-1));
	TEST_ASSERT_EQUAL(5, IP);	
	TEST_ASSERT_EQUAL(122, val);
}

void testLoadBad() {
	CODE(OP_LIT, 66, OP_LOAD, OP_FAULT, FAULT_USER, 0, 122);
	TEST_ASSERT_EQUAL(FAULT_MEM, vmRun(-1));
	TEST_ASSERT_EQUAL(3, IP);	
}

void testOverrun1() {
	code[VM_CODE_SIZE-1] = OP_INC;
	IP = VM_CODE_SIZE-1;
	
	TEST_ASSERT_EQUAL(FAULT_OK, vmRun(1));		// Run last instruction OK.
	TEST_ASSERT_EQUAL(VM_CODE_SIZE, IP);	
	TEST_ASSERT_EQUAL(1, val);
	
	TEST_ASSERT_EQUAL(FAULT_BAD_IP, vmRun(1));		// Now should fault as outside memory space.
	TEST_ASSERT_EQUAL(VM_CODE_SIZE+1, IP);			// IP goes to next, which is also illegal. 
	TEST_ASSERT_EQUAL(1, val);
}

void testOverrun2() {
	code[VM_CODE_SIZE-1] = OP_LIT; 
	IP = VM_CODE_SIZE-1;
	
	TEST_ASSERT_EQUAL(FAULT_BAD_IP, vmRun(1));	// Opcode OK, but loads from IP which is now out of memory.
	TEST_ASSERT_EQUAL(VM_CODE_SIZE+1, IP);		// IP goes to next, which is also illegal, so does not inrement again as is normal for LITx. 
	// Value of val is now undefined. 
}

void testBadOpcode() {
	CODE(0xff); 
	
	TEST_ASSERT_EQUAL(FAULT_BAD_OPCODE, vmRun(1));	// Opcode OK, but loads from IP which is now out of memory.
	TEST_ASSERT_EQUAL(1, IP);	
	TEST_ASSERT_EQUAL(0, val);
}

#if 0
TT_IGNORE_FROM_HERE()


#define r_pop() (*--ctx->rp)
#define SET_IP(ip_) do { if (!isIpValid(ip_)) { err = FAULT_IP_INVALID; goto err; } else IP = (ip_)} while (0)
#define CHECK_FAULT(op_) do { op_; if (ctx->err) goto err; } while (0)

#define r_pop() (r_can_pop(1) ? (*--ctx->rp) : (ctx->err = FAULT_R_UND, -1)
// Rule: IP always points to the next instruction if no fault. Else it points to the word that caused the fault. 

op_halt:  SET_FAULT(HALT); /*  goto next; not required */
op_ret: CHECK_FAULT(a = r_pop()); SET_IP(a); goto next;
op_FAULT: SET_FAULT(u_pop());		.
		.
		.
	static void* OPS[] = { op_halt, op_ret, ... };
	
	uint8_t op;
	
	 842 1352 1862
	 
next: 
	if ((n > 0) && (0 == --n))
		return 0;
	CHECK_FAULT(op = mem8rd(*IP++));
	if (op & 0x80) {
		CHECK_FAULT(uint16_t a = (uint16_t)(op & ~0x80) | (uint16_t) mem8rd(*IP++));
		r_push(IP);
		SET_IP(a);
		goto next;
	}
	if (op >= ELEMENTS(OPS))
		SET_FAULT(BAD_OPCODE);
		
	goto *OPS[op];
	
err:	
	return err;
# Source file thst defines the commands for sconsole.

# We only have two commands, PRIMOP & COMMAND.
# Primops are defined as a fragment of "C" code, commands are composed from primops and other words.

typedef int16_t sc_cell_t;

typedef struct {
	sc_cell_t ustack[CONSOLE_USER_STACK_SIZE];
	sc_cell_t* sp;
	sc_cell_t err;		// Set non-zero on fault.
}

#define u_pop() (*(CONTEXT.sp++))
#define u_push(x_) *--CONTEXT.sp = (sc_cell_t)(x_)
#define u_stackbase (&CONTEXT.ustack[CONSOLE_USER_STACK_SIZE])
#define u_clear() CONTEXT.sp = &ustack[CONSOLE_USER_STACK_SIZE]
#define u_tos() *CONTEXT.sp
#define u_depth() (&ustack[CONSOLE_USER_STACK_SIZE] - g_consoleContext.sp)

void execute(sc_context_t* ctx, int count) {
	while (1) {
	}
}
>R: "Execution: ( x -- ) ( R: -- x )"
# Functions available to primops:
#  User stack: u_clear(), u_pop(), u_push(), u_tos()
#  Return stack: r_clear(), r_pop(), p_push(), r_tos()
#  IP: instruction pointer, points to byte,

PRIMOP (ret) [r_pop(1)] "R( addr - ) Pop return address off r-stack and load IP." { IP = r_pop(); }
PRIMOP (lit8)	[push(1)]	"( - v8) Pushes next byte in instruction stream." [1, hide] { uint8_t b = memget8(IP++); push(); }

PRIMOP	event	"( u16: payload u8:id - ) Send event up to PC containing top stack value"  [-2] {
        int8_t id = pop();
        int16_t val = pop();
        event(id, val);
    }
PRIMOP 	lit16	"( - v8) Pushes next word in instruction stream." [1, hide] { push(memget16(IP)); IP += 2; }
PRIMOP 	quote	"( - addr) Reads length from IP, pushes address of next instruction, increments IP by length." [-1, hide] {
        uint8_t len = memget(IP++);
        push(IP); // address of quotation
        IP += len; // jump over
    }
PRIMOP 	choice	"(f addr:t addr:f - ) Reads two addresses, likely quotations, then executes one depending on the next stack value." [-3] {
        int16_t f = pop(), t = pop();
        rpush(IP);
        IP = (pop() == 0) ? f : t;
    }
PRIMOP 	if	"(p addr - ) Reads address, likely a quotation, then executes if the next stack value is non-zero." [-2] {
        int16_t t = pop();
        if (pop() != 0) {
            rpush(IP);
            IP = t;
        }
    }

PRIMOP 	c@		"{addr - v8) Fetch 8 bit value from address." [-1] { tos() = memget8(tos()); }
PRIMOP 	c!		"{v addr - ) Store 8 bit value at address." [-2] { uint16_t addr = pop(); memput8(addr, pop()); }
PRIMOP 	@		"{addr - v) Fetch  bit value from address." [-1] { tos() = memget16(tos()); }
PRIMOP 	!		"{v addr - ) Store 16 bit value at address." [-2] { uint16_t addr = pop(); memput16(addr, pop()); }
PRIMOP  +		"(v:arg1 v:arg2 - v:result) Add 2 values, push result." [-2] { BINOP(+); }
PRIMOP  -		"(v:arg1 v:arg2 - v:result) Add 2 values, push result." [-2] { BINOP(-); }
PRIMOP  *		"(v:arg1 v:arg2 - v:result) Add 2 values, push result." [-2] { BINOP(*); }
PRIMOP  /		"(v:arg1 v:arg2 - v:result) Add 2 values, push result." [-2] { BINOP(/); }
PRIMOP  mod		"(v:arg1 v:arg2 - v:result) Add 2 values, push result." [-2] { BINOP(%); }
PRIMOP  and		"(v:arg1 v:arg2 - v:result) Add 2 values, push result." [-2] { BINOP(&); }
PRIMOP  or		"(v:arg1 v:arg2 - v:result) Add 2 values, push result." [-2] { BINOP(|); }
PRIMOP  xor		"(v:arg1 v:arg2 - v:result) Add 2 values, push result." [-2] { BINOP(^); }
PRIMOP  shift	"(v:arg1 v:arg2 - v:result) Shift arg1 by |arg2|, left if arg2 negative, right otherwise, push result." [-2] {
        int16_t x = pop(); // negative values shift left, positive right
        tos() = (x < 0) ? tos() << -x : tos() >> x;
	}
PRIMOP  =		"(v:arg1 v:arg2 - v:result) Add 2 values, push result." [-2] { BINOP(==); }
PRIMOP  <		"(v:arg1 v:arg2 - v:result) Add 2 values, push result." [-2] { BINOP(<); }
PRIMOP  u<		"(v:arg1 v:arg2 - v:result) Add 2 values, push result." [-2] { UBINOP(<); }
PRIMOP  negate	"(v:arg - v:result) Negate value" [-1] { UNOP(-); }
PRIMOP  not		"(v:arg - v:result) Invert value" [-1] { UNOP(~); }
PRIMOP  1+		"(v:arg - v:result) Negate value" [-1] { tos() = tos() + 1; }
PRIMOP  1-		"(v:arg - v:result) Negate value" [-1] { tos() = tos() - 1; }

PRIMOP 	drop	"(v*n - n*(n-1)) Discard top value." [-1] { u_drop(); }
PRIMOP 	dup 	"(... n - ... n n) Duplicate top value." [-1, 1] { u_push(u_tos()); }
PRIMOP 	swap 	"(n1 n2 - n2 n1) Swap two top value.s" [-2] {
        int16_t t = u_tos(), n = u_nos();
        u_tos = u_nos(); u_nos = t;
    }
PRIMOP pick 	"( n -- v)	Copy nth item to top of stack." [] {
        int16_t n = pop();
		verify_stack(n);
        u_push(u_pick(n));
    }
PRIMOP roll		"(n -- ) Rotate stack so that top item slipped into nth position." [] {
        int16_t n = pop();
		verify_stack(n);
        int16_t n = pop(), t = *(s - n);
        int16_t* i;
        for (i = s - n; i < s; i++)
        {
            *i = *(i + 1);
        }
        *s = t;
    }
PRIMOP clear "( - <empty>) Clear stack." [] { u_clear(); }

VAR here "( -- addr) Address of next free dictionary byte."
int16_t here; // dictionary 'here' pointer
    int16_t last; // last definition address


        //bind(0,  ret);				[Return,                "(return)",              0   //             -  (from return)
        //bind(1,  lit8);
        //bind(2,  lit16);
        //bind(3,  quote);
        bind(4,  eventHeader);               EventHeader,           "event{",                4   // id          -
        //bind(5,  eventBody8);              EventBody8,            "cdata",                 5   // val         -
        bind(6,  eventBody16);               EventBody16,           "data",                  6   // val         -
        bind(7,  eventFooter);               EventFooter,           "}event",                7   //             -
        bind(8,  eventOp);                     Event,                 "event",                 8   // val id      -
        //bind(9,  fetch8);                    Fetch8,                "c@",                    9   // addr        - val
        //bind(10, store8);                    Store8,                "c!",                    10  // val addr    -
        //bind(11, fetch16);                   Fetch16,               "@",                     11  // addr        - val
        //bind(12, store16);                   Store16,               "!",                     12  // val addr    -
        //bind(13, add);                       Add,                   "+",                     13  // y x         - sum
        //bind(14, sub);                       Subtract,              "-",                     14  // y x         - diff
        //bind(15, mul);                       Multiply,              "*",                     15  // y x         - prod
        //bind(16, div);                       Divide,                "/",                     16  // y x         - quot
        //bind(17, mod);                       Modulus,               "mod",                   17  // y x         - rem
        //bind(18, andb);                      And,                   "and",                   18  // y x         - result
        //bind(19, orb);                       Or,                    "or",                    19  // y x         - result
        //bind(20, xorb);                      ExclusiveOr,           "xor",                   20  // y x         - result
        //bind(21, shift);                     Shift,                 "shift",                 21  // x bits      - result
        //ind(22, eq);                        Equal,                 "=",                     22  // y x         - pred
        bind(23, neq);                       NotEqual,              "<>",                    23  // y x         - pred
        bind(24, gt);                        Greater,               ">",                     24  // y x         - pred
        bind(25, geq);                       GreaterOrEqual,        ">=",                    25  // y x         - pred
        bind(26, lt);                        Less,                  "<",                     26  // y x         - pred
        bind(27, leq);                       LessOrEqual,           "<=",                    27  // y x         - pred
        bind(28, notb);                      Not,                   "not",                   28  // x           - result
        bind(29, neg);                       Negate,                "neg",                   29  // x           - -x
        bind(30, inc);                       Increment,             "1+",                    30  // x           - x+1
        bind(31, dec);                       Decrement,             "1-",                    31  // x           - x-1
        bind(32, drop);                      Drop,                  "drop",                  32  // x           -
        bind(33, dup);                       Duplicate,             "dup",                   33  // x           - x x
        bind(34, swap);                      Swap,                  "swap",                  34  // y x         - x y
        bind(35, pick);                      Pick,                  "pick",                  35  // n           - val
        bind(36, roll);                      Roll,                  "roll",                  36  // n           -
        bind(37, clr);                       Clear,                 "clear",                 37  //             -
        bind(38, pushr);                     Push,                  "push",                  38  // x           -   (to return)
        bind(39, popr);                      Pop,                   "pop",                   39  //             - x (from return)
        bind(40, peekr);                     Peek,                  "peek",                  40  //             - x (from return)
        bind(41, forget);                    Forget,                "forget",                41  // addr        -
        bind(42, call);                      Call,                  "call",                  42  // addr        -
        bind(43, choice);                    Choice,                "choice",                43  // q p         -
        bind(44, chooseIf);                  If,                    "if",                    44  // q           -
        bind(45, loopTicks);                 LoopTicks,             "loopTicks",             45  //             -
        bind(46, setLoop);                   SetLoop,               "setLoop",               46  // addr        -
        bind(47, stopLoop);                  StopLoop,              "stopLoop",              47  //             -
        bind(48, resetBoard);                Reset,                 "(reset)",               48  //             -
        bind(49, pinMode);                   PinMode,               "pinMode",               49  // mode pin    -
        bind(50, digitalRead);               DigitalRead,           "digitalRead",           50  // pin         - val
        bind(51, digitalWrite);              DigitalWrite,          "digitalWrite",          51  // val pin     -
        bind(52, analogRead);                AnalogRead,            "analogRead",            52  // pin         - val
        bind(53, analogWrite);               AnalogWrite,           "analogWrite",           53  // val pin     -
        bind(54, attachISR);                 AttachISR,             "attachISR",             54  // addr i mode -
        bind(55, detachISR);                 DetachISR,             "detachISR",             55  // i           -
        bind(56, milliseconds);              Milliseconds,          "milliseconds",          56  //             - millis
        bind(57, pulseIn);                   PulseIn,               "pulseIn",               57] // val pin     - duration
        bind(58, next);
        bind(59, nop);




    inline int16_t boolval(int16_t b) // helper (not Brief instruction)
    {
        // true is all bits on (works for bitwise and logical operations alike)
        return b ? 0xFFFF : 0;
    }

    /* Moving items between data and return stack. The return stack is commonly also used to store data
    that is local to a subroutine. It is safe to push data here to be recovered after a subroutine
    call. It is not safe to use it for passing data between subroutines. That is what the data stack
    is for. Think of arguments vs. locals. The normal way of handling locals in Brief that need to
    survive a call and return from another word is to store them on the return stack. */

    void pushr()
    {
        rpush(pop());
    }

    void popr()
    {
        push(rpop());
    }

    void peekr()
    {
        push(*r);
    }

    /* Dictionary manipulation instructions:

    The `forget` function is a Forthism for reverting to the address of a previously defined word;
    essentially forgetting it and any (potentially dependent words) defined thereafter. */

    void forget() // revert dictionary pointer to TOS
    {
        int16_t i = pop();
        if (i < here) here = i; // don't "remember" random memory!
    }

    /* A `call` instruction pops an address and calls it; pushing the current `p` as to return. */

    void call()
    {
        rpush(p);
        p = pop();
    }


    void next()
    {
        int16_t count = rpop() - 1;
        int16_t rel = memget(p++);
        if (count > 0)
        {
            rpush(count);
            p -= (rel + 2);
        }
    }

    void nop()
    {
    }

    /* A Brief word (address) may be set to run in the main loop. Also, a loop counter is
    maintained for use by conditional logic (throttling for example). */

    int16_t loopword = -1; // address of loop word

    int16_t loopIterations = 0; // number of iterations since 'setup' (wraps)

    void loopTicks()
    {
        push(loopIterations & 0x7FFF);
    }

    void setLoop()
    {
        loopIterations = 0;
        loopword = pop();
    }

    void stopLoop()
    {
        loopword = -1;
    }

    /* Upon first connecting to a board, the PC will execute a reset so that assumptions about
    dictionary contents and such hold true. */

    void resetBoard() // likely called initialy upon connecting from PC
    {
        clr();
        here = last = 0;
        loopword = -1;
        loopIterations = 0;
    }

    /* Here begins all of the Arduino-specific instructions.

    Starting with basic setup and reading/write to GPIO pins. Note we treat `HIGH`/`LOW` values as
    Brief-style booleans (-1 or 0) to play well with the logical and conditional operations. */

    void pinMode()
    {
        ::pinMode(pop(), pop());
    }

    void digitalRead()
    {
        push(::digitalRead(pop()) ? -1 : 0);
    }

    void digitalWrite()
    {
        ::digitalWrite(pop(), pop() == 0 ? LOW : HIGH);
    }

    void analogRead()
    {
        push(::analogRead(pop()));
    }

    void analogWrite()
    {
        ::analogWrite(pop(), pop());
    }

    /* I2C support comes from several instructions, essentially mapping composable, zero-operand
    instructions to functions in the Arduino library:

    http://arduino.cc/en/Reference/Wire

    Brief words (addresses/quotations) may be hooked to respond to Wire events. */

    void wireBegin()
    {
        Wire.begin(); // join bus as master (slave not supported)
    }

    void wireRequestFrom()
    {
        Wire.requestFrom(pop(), pop());
    }

    void wireAvailable()
    {
        push(Wire.available());
    }

    void wireRead()
    {
        while (Wire.available() < 1);
        push(Wire.read());
    }

    void wireBeginTransmission()
    {
        Wire.beginTransmission((uint8_t)pop());
    }

    void wireWrite()
    {
        Wire.write((uint8_t)pop());
    }

    void wireEndTransmission()
    {
        Wire.endTransmission();
    }

    int16_t onReceiveWord = -1;

    void wireOnReceive(int16_t count)
    {
        if (onReceiveWord != -1)
        {
            push(count);
            exec(onReceiveWord);
        }
    }

    void wireSetOnReceive()
    {
        onReceiveWord = pop();
        Wire.onReceive(wireOnReceive);
    }

    int16_t onRequestWord = -1;

    void wireOnRequest()
    {
        if (onRequestWord != -1)
        {
            exec(onRequestWord);
        }
    }

    void wireSetOnRequest()
    {
        onRequestWord = pop();
        Wire.onRequest(wireOnRequest);
    }

    /* Brief word addresses (or quotations) may be set to run upon interrupts. For more info on
    the argument values and behavior, see:

    http://arduino.cc/en/Reference/AttachInterrupt

    We keep a mapping of up to MAX_INTFAULTUPTS (6) words. */

    int16_t isrs[MAX_INTFAULTUPTS];

    void interrupt(int16_t n) // helper (not Brief instruction)
    {
        int16_t w = isrs[n];
        if (w != -1) exec(w);
    }

    void interrupt0() // helper (not Brief instruction)
    {
        interrupt(0);
    }

    void interrupt1() // helper (not Brief instruction)
    {
        interrupt(1);
    }

    void interrupt2() // helper (not Brief instruction)
    {
        interrupt(2);
    }

    void interrupt3() // helper (not Brief instruction)
    {
        interrupt(3);
    }

    void interrupt4() // helper (not Brief instruction)
    {
        interrupt(4);
    }

    void interrupt5() // helper (not Brief instruction)
    {
        interrupt(5);
    }

    void interrupt6() // helper (not Brief instruction)
    {
        interrupt(6);
    }

    void attachISR()
    {
        uint8_t mode = pop();
        uint8_t interrupt = pop();
        isrs[interrupt] = pop();
        switch (interrupt)
        {
            case 0 : attachInterrupt(0, interrupt0, mode);
            case 1 : attachInterrupt(1, interrupt1, mode);
            case 2 : attachInterrupt(2, interrupt2, mode);
            case 3 : attachInterrupt(3, interrupt3, mode);
            case 4 : attachInterrupt(4, interrupt4, mode);
            case 5 : attachInterrupt(5, interrupt5, mode);
            case 6 : attachInterrupt(6, interrupt6, mode);
        }
    }

    void detachISR()
    {
        int interrupt = pop();
        isrs[interrupt] = -1;
        detachInterrupt(interrupt);
    }

    /* A couple of stragglers... */

    void milliseconds()
    {
        push(millis());
    }

    void pulseIn()
    {
        push(::pulseIn(pop(), pop()));
    }

    /* The Brief VM needs to be hooked into the main setup and loop on the hosting project.
    A minimal *.ino would contain something like:

    #include <Brief.h>

    void setup()
    {
        brief::setup();
    }

    void loop()
    {
        brief::loop();
    }

    Brief setup binds all of the instruction functions from above. After setup, the hosting
    project is free to bind its own custom functions as well!

    An example of this could be to add a `delayMillis` instruction. Such an instruction is not
    included in the VM to discourage blocking code, but you're free to add whatever you like:

    void delayMillis()
    {
        delay((int)brief::pop());
    }

    void setup()
    {
        brief::setup(19200);
        brief::bind(100, delayMillis);
    }

    This adds the new instruction as opcode 100. You can then give it a name and tell the compiler
    about it with `compiler.Instruction("delay", 100)` in PC-side code or can tell the Brief
    interactive about it with `100 'delay instruction`. This is the extensibility story for Brief.

    Notice that custom instruction function may retrieve and return values via the
    `brief::pop()` and `brief::push()` functions, as well as raise faults with
    `brief::fault(uint8_t code)`. */

    void setup()
    {
        Serial.begin(19200); // assumed by interactive
        resetBoard();


        for (int16_t i = 0; i < MAX_INTFAULTUPTS; i++)
        {
            isrs[i] = -1;
        }

        event(BOOT_EVENT_ID, 0); // boot event
    }

    /* The payload from the PC to the MCU is in the form of Brief code. A header byte indicates
    the length and whether the code is to be executed immediately (0x00) or appended to the
    dictionary as a new definition (0x01).

    A dictionary pointer is maintained at the MCU. This pointer always references the first
    available free byte of dictionary space (beginning at address 0x0000). Each definition sent to
    the MCU is appended to the end of the dictionary and advances the pointer. The bottom end of the
    dictionary space is used for arguments (mainly for IL, not idiomatic Brief).

    If code is a definition then it is expected to already be terminated by a `return` instruction (if
    appropriate) and so we do nothing at all; just leave it in place and leave the `here` pointer
    alone.

    If code is to be executed immediately then a return instruction is appended and exec(...) is
    called on it. The dictionary pointer (`here`) is restored; reclaiming this memory. */

    void loop()
    {
        if (Serial.available())
        {
            int8_t b = Serial.read();
            bool isExec = (b & 0x80) == 0x80;
            int8_t len = b & 0x7f;
            for (; len > 0; len--)
            {
                while(!Serial.available());
                memset(here++, Serial.read());
            }

            if (isExec)
            {
                memset(here++, 0); // ensure return
                here = last;
                exec(here);
            }
            else
            {
                last = here;
            }
        }

        if (loopword >= 0)
        {
            exec(loopword);
            loopIterations++;
        }
    }
}
#endif