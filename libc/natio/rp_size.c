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
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <rho/natio.h>

/*****************************************************************************
 * rp_size
 *
 * Returns the file size of <file> in bytes. If this value is zero, the file
 * may not exist, be the wrong type, or be a character device. checktype can 
 * be used to differentiate between these cases.
 */

off_t rp_size(rp_t file) {
	uint32_t size0, size1;
	uint64_t size;
	char *reply;

	if (!file) {
		return 0;
	}

	reply = rcall(file, 0, "size");

	if (!reply) {
		errno = ENOSYS;
		return 0;
	}

	if (reply[0] == '!') {
		if      (!strcmp(reply, "! nfound")) errno = ENOENT;
		else if (!strcmp(reply, "! nosys"))  errno = ENOSYS;
		else if (!strcmp(reply, "! denied")) errno = EACCES;
		else                                 errno = EUNK;
		free(reply);
		return 0;
	}

	size0 = 0;
	sscanf(reply, "%u:%u", &size0, &size1);
	size = size1 | (uint64_t) size0 << 32;
	free(reply);

	return size;
}
