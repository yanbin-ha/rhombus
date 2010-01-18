/* Copyright 2009, 2010 Nick Johnson */

#include <stdint.h>
#include <string.h>
#include <signal.h>
#include <flux.h>
#include <driver.h>

static volatile signal_handler_t sighandlers[MAXSIGNAL];
static volatile uint8_t sigcount[MAXSIGNAL]; /* Used for waiting */
static volatile uint8_t block_count = 0;

void siginit(void) {
	extern void sighand(void);

	_hand((uint32_t) sighand);
}

void sigblock(void) {
	_ctrl(CTRL_SIGNAL, CTRL_SIGNAL);
	block_count++;

	if (block_count != 1) {
		_ctrl(CTRL_NONE, CTRL_SIGNAL); 
	}
}

void sigunblock(void) {
	if (block_count == 0) return;
	
	_ctrl(CTRL_SIGNAL, CTRL_SIGNAL);
	block_count--;

	if (block_count == 0) {
		_ctrl(CTRL_NONE, CTRL_SIGNAL);
	}
}

int fire(uint32_t target, uint16_t signal, void *grant) {
	int ret = _fire(target, signal, grant, 0);
	if (grant) req_free(grant);
	return ret;
}

void tail(uint32_t target, uint16_t signal, void *grant) {
	if (grant) req_free(grant);
	while (_fire(target, signal, grant, FIRE_TAIL));
}

void sigregister(uint16_t signal, signal_handler_t handler) {
	sighandlers[signal] = handler;
}

void sigredirect(uint32_t source, uint32_t signal, void *grant) {
	if (signal > MAXSIGNAL) return;
	sigcount[signal]++;

	if (sighandlers[signal]) {
		sighandlers[signal](source, grant);
	}
	else if (signal != SIG_ERROR && source != info(0)) {
		fire(source, SIG_ERROR, NULL);
	}
}

void wreset(uint16_t signal) {
	sigcount[signal] = 0;
}

void wait(uint16_t signal) {
	while (!sigcount[signal]);
	sigcount[signal]--;
}

void rirq(uint8_t irq) {
	_ctrl(CTRL_IRQRD | CTRL_IRQ(irq), CTRL_IRQRD | CTRL_IRQMASK);
}