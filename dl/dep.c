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

#include <rho/layout.h>
#include <rho/arch.h>
#include <rho/exec.h>

#include "dl.h"

char *_dep(void *object, uint32_t index, int loaded) {
	struct elf_cache cache;
	const char *name;
	size_t i;

	if (loaded) {
		elf_gencache(&cache, object);
	}
	else {
		elf_gencache_img(&cache, object);
	}

	if (!cache.dynamic) {
		return NULL;
	}

	name = NULL;
	for (i = 0; cache.dynamic[i].tag != DT_NULL; i++) {
		if (cache.dynamic[i].tag == DT_NEEDED) {
			if (!index--) {
				name = &cache.strtab[cache.dynamic[i].val];
			}
		}
	}

	return (char*) name;
}
