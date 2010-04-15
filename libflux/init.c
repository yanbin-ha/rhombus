/* Copyright 2009, 2010 Nick Johnson */

#include <flux/arch.h>
#include <flux/heap.h>
#include <flux/abi.h>
#include <flux/signal.h>

static void __exit(uint32_t source, struct request *req) {
	_exit(1);
}

void _cini() {
	extern void siginit(void);

	/* Heap */
	heap_init();

	/* Fault handling */
	siginit();
	sigregister(0, __exit);
	sigregister(2, __exit);
	sigregister(5, __exit);
}