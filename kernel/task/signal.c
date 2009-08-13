// Copyright 2009 Nick Johnson

#include <lib.h>
#include <task.h>
#include <trap.h>

uint32_t *signal_table = (void*) SIG_TBL;
uint32_t *signal_map = (void*) SIG_MAP;

image_t *signal(pid_t task, uint8_t sig, 
	uint32_t arg0, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint8_t flags) {
	task_t *t, *src_t;
	pid_t caller = curr_pid;

	// Check existence of target
	t = get_task(task);
	src_t = get_task(caller);
	if (t->magic != 0x4224) {
		if (flags & TF_NOERR) return src_t->image;
		else ret(src_t->image, ENOTASK);
	}

	// Switch to target task
	if (t->pid != curr_pid) task_switch(t);

	// Check existence of signal handler
	if (signal_map[sig] == SF_NIL) {
		if (flags & TF_EKILL) return exit_call(t->image);
		task_switch(src_t);
		if (flags & TF_NOERR) return src_t->image;
		else ret(src_t->image, ENOSIG);
	}

	// Block if set to block
	if (flags & TF_BLOCK) src_t->flags |= TF_BLOCK;
	if (flags & TF_UNBLK) t->flags &= ~TF_BLOCK;

	// Save caller
	t->image->caller = t->caller;
	t->caller = caller;

	// Create new image structure
	memcpy((uint8_t*) ((uint32_t) t->image - sizeof(image_t)), t->image, sizeof(image_t));
	t->tss_esp = (uint32_t) t->image; // Will now create image below old image
	t->image = (void*) ((uint32_t) t->image - sizeof(image_t));
	if ((uint32_t) t->image < SSTACK_BSE + sizeof(image_t)) 
		panic("task state stack overflow");	

	// Set registers to describe signal
	t->image->eax = arg0;
	t->image->ebx = arg1;
	t->image->ecx = arg2;
	t->image->edx = arg3;
	t->image->esi = caller;
	t->image->edi = sig;

	// Set reentry point
	tss_set_esp(t->tss_esp);
	t->image->eip = signal_table[sig];
	return t->image;
}

image_t *sret(image_t *image) {
	task_t *t, *src_t;

	// Unblock the caller
	t = get_task(curr_pid);
	src_t = get_task(t->caller);
	if (image->eax & TF_UNBLK) 
		src_t->flags &= ~TF_BLOCK;

	// Reset image stack
	t->image = (void*) t->tss_esp;
	t->tss_esp += sizeof(image_t);
	t->caller = t->image->caller;

	// Bounds check image
	if ((uint32_t) t->tss_esp >= SSTACK_TOP) 
		panic("task state stack underflow");

	tss_set_esp(t->tss_esp);
	return t->image;
}