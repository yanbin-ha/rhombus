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
#include <stdio.h>
#include <errno.h>
#include <natio.h>

/****************************************************************************
 * freopen
 *
 * Same as fopen(), but result is stored in <stream>.
 */

FILE *freopen(const char *path, const char *mode, FILE *stream) {
	uint64_t fd;

	if (stream) {
		free(stream);
	}

	fd = fs_find(0, path);

	if (!fd) {
		if (mode[0] == 'w' || mode[0] == 'a') {
			fd = fs_cons(fs_find(0, path_parent(path)), path_name(path), FOBJ_FILE);
			if (!fd) return NULL;
		}
		if (mode[0] == 'w') {
			reset(fd);
		}
	}

	return fdopen(fs_find(0, path), mode);
}
