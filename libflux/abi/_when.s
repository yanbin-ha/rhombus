; Copyright 2010 Nick Johnson
; ISC Licensed, see LICENSE for details

[bits 32]

global _when

_when:

	mov ecx, [esp+4]
	mov edx, [esp+8]
	int 0x42

	ret