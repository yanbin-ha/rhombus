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

int _exec(void *image, size_t size, int flags) {
	struct slt32_header *slt_hdr;
	struct slt32_entry *slt;
	struct elf32_ehdr *exec;
	void  *temp;
	struct elf_cache cache;
	void  *entry;
	size_t i;

	/* check executable */
	if (elf_check(image)) {
		return 1;
	}

	/*** POINT OF MAYBE RETURNING IF YOU, y'know, have to... ***/

	/* copy executable high */
	temp = exec = (void*) sltalloc("dl.img.exec", size);

	if ((uintptr_t) image % PAGESZ) {
		/* not aligned, copy */
		page_anon(exec, size, PROT_READ | PROT_WRITE);
		memcpy(exec, image, size);
	}
	else {
		/* aligned, use paging */
		page_self(image, exec, size);
	}

	/*** POINT OF NO RETURN ***/

	/* clear lower memory */
	slt     = (void*) SLT_BASE;
	slt_hdr = (void*) SLT_BASE;

	for (i = slt_hdr->first; i; i = slt[i].next) {
		if (slt[i].flags & SLT_FLAG_CLEANUP) {
			page_free((void*) slt[i].base, slt[i].size);
		}
	}

	/* load executable */
	elf_load(exec, 0);
	exec  = (void*) 0x100000;
	entry = (void*) exec->e_entry;

	elf_gencache_exec(&cache, exec);
	elfc_relocate_all_exec(&cache);

	/* remove executable image */
	page_free(temp, size);
	sltfree_name("dl.img.exec");

	/* reset event handler */
	_when(0);

	/* enter executable */
	dl_enter(entry);

	return 1;
}
