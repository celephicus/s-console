Overview

Acknowledgement

Inspaired by the Brief project from AshleyF https://github.com/AshleyF/BriefEmbedded.git. Many ideas are his, but his language requires an IDE, I need a freestanding implementation.

Initial Requirements

I needed a console for my embedded development work, something that I can type number into and call "C" functions. I found that I needed quite good error checking for stack & number overflow errors. I also don't like having to explicitly set BASE to change number bases. I also really have never been comfortable with Forth's interpreter/compiled modes and the hoops that you have to jump through sometimes. Christophe Laverenne's FreeForth http://christophe.lavarenne.free.fr/ff/ which is always compiling seemed much more natural to me. 

Code size is an issue, so apart from the VM which implements the primitives and is written in "C", the bulk of the code is written in the VM bytecode. Efficiency was not considered a priority at all. 

Numbers

We type a lot of numbers. We do distinguish between signed & unsigned, which is unusual for Forths.

<none> or - Signed decimal.
+           Unsigned decimal.
$           Unsigned hex.
%           Unsigned binary.
&           Hex string, eg `&00ff00' is stored as exactly 4 bytes 0300ff00
"           String, with escapes, null terminated eg `"f00' is stored as C format "f00", `"\xfe_\_' becomes "\xfe _".

Printing
Numbers or strings are printed with a prefix and a trailing space by default.
There is primitive formatting where the number can be printed to a set width field, either right justified with leading zeroes or spaces, or left justified. I have always found this sufficient.

Memory
S-console operates on a small logical memory area, comprised of a read only section containing the dictionary and definitions for the primitives and words compiled in at build time, and a RAM section for variables and words defined by the user. For an AVR target the read only section can be placed into Flash.

Examples
  
> 1234 .-> 1234
> $1234 $.-> $1234
> [2 *] "double define ->
> 23 double . -> 46
> ["no "yes choice $.] "yeahnah define ->
> 0 yeahnah 1 yeahnah -> no yes
  
  
