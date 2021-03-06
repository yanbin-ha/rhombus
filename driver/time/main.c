/*
 * Copyright (C) 2009-2011 Nick Johnson <nickbjohnson4224 at gmail.com>
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

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <rho/mutex.h>
#include <rho/natio.h>
#include <rho/proc.h>

#include <rdi/core.h>
#include <rdi/vfs.h>
#include <rdi/arch.h>
#include <rdi/io.h>

#include "time.h"

size_t time_read(struct robject *self, rp_t source, uint8_t *buffer, size_t size, off_t offset) {
	char *data;

	data = saprintf("%d", get_time());
	
	if (size > strlen(data)) {
		size = strlen(data);
	}

	memcpy(buffer, data, size);
	free(data);

	return size;
}

int main(int argc, char **argv) {
	struct robject *file;

	rdi_init();

	// create device file
	file = rdi_file_cons(1, ACCS_READ);

	// set interface functions
	rdi_global_read_hook = time_read;

	// daemonize
	msendb(RP_CONS(getppid(), 0), ACTION_CHILD);
	_done();

	return 0;
}
