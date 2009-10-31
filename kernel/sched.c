/* Copyright 2009 Nick Johnson */

#include <lib.h>
#include <task.h>
#include <int.h>

struct sched_queue queue;

void sched_ins(pid_t pid) {
	task_t *t, *tn;

	if (!queue.next) {
		t = task_get(pid);
		t->next_task = pid;
		queue.next = pid;
	}
	else {
		t = task_get(queue.next);
		tn = task_get(pid);
		tn->next_task = t->next_task;
		t->next_task = pid;
	}
}

void sched_rem(pid_t pid) {
	task_t *t, *t2;

	if (queue.next) {
		t = task_get(queue.next);
		while (t && t->next_task != pid && t->next_task != queue.next) {
			t = task_get(t->next_task);
		}

		if (t) {
			t2 = task_get(t->next_task);
			t->next_task = t2->next_task;
		}
	}
}

task_t *task_next(uint8_t flags) {
	pid_t pid;
	task_t *t;
	flags = 0;

	do {
		pid = queue.next;
		t = task_get(pid);
		queue.next = t->next_task;
		sched_ins(pid);
	} while (t->flags & TF_BLOCK);

	return t;
}