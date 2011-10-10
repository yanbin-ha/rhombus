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

#include <robject.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <proc.h>

#include <rdi/core.h>

// XXX SEC - does not check for read access
static char *_find(struct robject *r, rp_t src, int argc, char **argv) {
	return rtoa(RP_CONS(getpid(), r->index));
}

static char *_open(struct robject *r, rp_t src, int argc, char **argv) {
	return strdup("T");
}

// XXX SEC - does not check for read access
static char *_get_access(struct robject *r, rp_t src, int argc, char **argv) {
	char *lookup;
	uint32_t bitmap;
	uint32_t user;

	if (argc == 1) {
		user = getuser(RP_PID(src));
	}
	else if (argc == 2) {
		user = atoi(argv[1]);
	}
	else {
		return strdup("! arg");
	}

	lookup = saprintf("access-%X", user);
	bitmap = (uint32_t) robject_data(r, lookup);
	free(lookup);

	if (!bitmap) {
		bitmap = (uint32_t) robject_data(r, "access-default");
	}

	return saprintf("%X", bitmap);
}

// XXX SEC - does not check for alter access
static char *_set_access(struct robject *r, rp_t src, int argc, char **argv) {
	char *lookup;
	uint32_t bitmap;
	uint32_t user;

	if (argc == 2) {
		user = getuser(RP_PID(src));
		sscanf(argv[1], "%X", &bitmap);
	}
	else if (argc == 3) {
		user = atoi(argv[1]);
		sscanf(argv[2], "%X", &bitmap);
	}
	else {
		return strdup("! arg");
	}

	lookup = saprintf("access-%X", user);
	robject_set_data(r, lookup, (void*) bitmap);
	free(lookup);

	return strdup("T");
}

static char *_sync(struct robject *r, rp_t src, int argc, char **argv) {
	return strdup("T");
}

struct robject *rdi_class_core;

void __rdi_class_core_setup() {
	
	rdi_class_core = robject_cons(0, robject_class_basic);

	robject_set_call(rdi_class_core, "find", _find);
	robject_set_call(rdi_class_core, "get-access", _get_access);
	robject_set_call(rdi_class_core, "set-access", _set_access);
	robject_set_call(rdi_class_core, "sync", _sync);
	robject_set_call(rdi_class_core, "open", _open);

	robject_set_data(rdi_class_core, "type", (void*) "driver");
	robject_set_data(rdi_class_core, "name", (void*) "RDI-class-core");
}

struct robject *rdi_core_cons(uint32_t index, uint32_t access) {
	struct robject *r;

	r = robject_cons(index, rdi_class_core);
	robject_set_data(r, "access-default", (void*) access);

	return r;
}

void rdi_core_free(struct robject *r) {
	robject_free(r);
}
