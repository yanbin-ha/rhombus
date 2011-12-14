/*
 * Copyright (C) 2011 Nick Johnson <nickbjohnson4224 at gmail.com>
 * 
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <stdlib.h>

#include <sys/types.h>
#include <sys/wait.h>

#include <rho/ipc.h>

pid_t waitpid(pid_t pid, int *status, int options) {
	struct msg *msg;
	
	if (pid == -1) {
		msg = (options & WNOHANG) 
			? mqueue_pull(PORT_CHILD, 0) 
			: mwait(PORT_CHILD, 0);
		pid = RP_PID(msg->source);
	}
	else {
		msg = (options & WNOHANG) 
			? mqueue_pull(PORT_CHILD, RP_CONS(pid, 0)) 
			: mwait(PORT_CHILD, RP_CONS(pid, 0));
	}
	
	if (status) {
		*status = *((uint32_t*) msg->data);
	}

	free(msg);

	return pid;
}
