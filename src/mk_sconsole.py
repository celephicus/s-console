import re, sys

primops = []

NON_ALNUMS = {x[0]: '_'+x[1:]+'_' for x in '''\
!pling "dquote #hash $dollar %percent &amp 'quote (lparen )rparen *star +plus ,comma -dash .dot /slash 
:colon ;semi <lt =eq >gt ?query @at [lbracket \bslash ]rbracket ^ _usqore `backtick {lbrace |pipe }rbrace ~tilde'''.split()}
def c_name(s):
	return ''.join([NON_ALNUMS.get(x, x) for x in s]).strip('_').replace('__', '_').lower()
	
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
		primops.append((word_name, code_snippet, ))
	else:
		print(f"Line `{ln}' not understood.", file=sys.stderr)
		sys.exit()
	
for p in primops:
	print(p)
