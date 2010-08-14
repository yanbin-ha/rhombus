/*
 * Copyright (C) 2009, 2010 Nick Johnson <nickbjohnson4224 at gmail.com>
 * 
 * Permission to use, copy, modify, and/or distribute this software for any
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
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <natio.h>
#include <errno.h>
#include <dict.h>

/****************************************************************************
 * fstat
 *
 * XXX - doc
 */

int fstat(const char *path, const char *field, const char *fmt, ...) {
	va_list ap;
	char *value;
	char *fullpath;

	fullpath = strvcat("vfs:", path, ":", field, NULL);
	value = dread(fullpath);
	free(fullpath);

	if (value) {
		va_start(ap, fmt);
		vsscanf(value, fmt, ap);
		va_end(ap);
		return 0;
	}
	else {
		errno = ENOFILE;
		return -1;
	}
}
