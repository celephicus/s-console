import re, sys

primops = []

# Turn strings of arbitrary printable ASCII chars into valid "C" identifiers, so foo -> foo, c++ -> c_plus_plus, 1go -> __1_go
NON_ALNUMS = {x[0]: '_'+x[1:]+'_' for x in '''\
!pling "dquote #hash $dollar %percent &amp 'quote (lparen )rparen *star +plus ,comma -dash .dot /slash 
:colon ;semi <lt =eq >gt ?query @at [lbracket \\bslash ]rbracket ^ _usqore `backtick {lbrace |pipe }rbrace ~tilde'''.split()}
def c_name(s):
	s = ''.join([NON_ALNUMS.get(x, x) for x in s]).replace('__', '_').lower()
	if s[0].isnumeric():
		s = '__' + s[0] + '_' + s[1:]
	return s
print(c_name('-'))

# Parse out primitive definitions.	
text = open('sconsole.src', 'rt').read()
# PRIMOP 	LIT8	'( - c)' "Pushes next byte in instruction stream." [hide] { c = CHECK_ERR(memget_char_ip()) console_u_push(c); }
for ln in text.splitlines():
	ln = ln.lstrip()
	if not ln:
		continue
	if ln.startswith('#'):
		continue
	if ln.startswith('IGNORE'):
		break
	if m := re.match(r'''
	  PRIMOP\s*
	  ([^\s]+)\s*		# Word. 
	  '(.*)'\s+			# Stack effect,
	  "(.*)"\s*			# Description.
	  \[([^]]*)\]\s*	# Options.
	  \{(.*)\}\s*		# Code.
	  $					# 
	  ''', ln, re.X):
		#print(m.groups())
		word_name = m.group(1)
		code_snippet = [m.group(5).strip()]
		if not code_snippet[0].endswith(';'):
			code_snippet.append(';')
		code_snippet.append(' goto next;')
		code_snippet = "".join(code_snippet)
		primops.append((c_name(word_name), code_snippet, ))
	else:
		print(f"Line `{ln}' not understood.", file=sys.stderr)
		sys.exit()

# Dump info	
for p in primops:
	print(p)

# Generate definitions.
sc_opcodes = '\n'.join([f'\tSC_OP_{p[0].upper()},  \\' for p in primops])
sc_snippets = '\n'.join([f'\t{p[0]}: {p[1]} \\' for p in primops])
sc_jumps = ', '.join([f'&&{p[0]}' for p in primops])

f = open('sconsole.auto.h', 'wt')
f.write(f'''\
// This file is autogenerated, do not edit.

#define SC_OPCODES \\
{sc_opcodes}

#define SC_JUMPS \\
{sc_jumps}

#define SC_SNIPPETS \\
{sc_snippets}

''')