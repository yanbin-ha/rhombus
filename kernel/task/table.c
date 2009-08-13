// Copyright 2009 Nick Johnson

#include <lib.h>
#include <trap.h>
#include <task.h>
#include <mem.h>

pool_t tpool[(MAX_TASKS/1024) + 1];	// Pool allocator for task structures
task_t *task = (void*) TASK_TBL; 	// Array of task structures (max 65536)
pid_t curr_pid = 0;

task_t *get_task(pid_t pid) {
	if ((page_get((uint32_t) &task[pid]) & PF_PRES) == 0) {
		p_alloc((uint32_t) &task[pid], (PF_PRES | PF_RW));
		pgclr((void*) &task[pid]);
	}
	return &task[pid];
}

task_t *new_task(task_t *src) {
	if (!src || src->magic != 0x4224) panic("pad parent!\n");
	pid_t new_pid = pool_alloc(tpool);
	task_t *new = get_task(new_pid);

	printk("eyes %d ", new_pid);

	new->map = map_clone();
	printk("%d ", new_pid);
	new->image = src->image;
	new->flags = src->flags &~ TF_BLOCK;
	new->parent = src->pid;
	new->pid = new_pid;
	new->magic = 0x4224;
	new->tss_esp = src->tss_esp;

	insert_sched(new_pid);
	printk("creating task %d\n", new_pid);
	return new;
}

uint32_t rem_task(task_t *t) {
	printk("rem_task %d\n", t->pid);
	t->magic = 0x0000;
	remove_sched(t->pid);
	return pool_free(tpool, t->pid);
}

image_t *task_switch(task_t *t) {
	if (t->magic != 0x4224) panic("invalid task");
	curr_pid = t->pid;

	map_load(t->map);
	tss_set_esp(t->tss_esp);
	return t->image;
}
