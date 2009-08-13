// Copyright 2009 Nick Johnson

#include <lib.h>
#include <mem.h>

#ifndef MAX_PHMEM_MAX
pool_t fpool[MAX_PHMEM/4];
#else
pool_t fpool[0x1000];
#endif

void page_touch(uint32_t page) {
	page &= ~0x3FFFFF;
	uint32_t target = (uint32_t) &ctbl[page >> 12];
	if (cmap[page >> 22] & PF_PRES) return;
	cmap[page >> 22] = frame_new() | (PF_PRES | PF_RW | PF_USER);
	asm volatile ("invlpg %0" :: "m" (target));
	pgclr(&ctbl[page >> 12]);
}

void page_set(uint32_t page, page_t value) {
	page &= ~0xFFF;
	if ((cmap[page >> 22] & PF_PRES) == 0) page_touch(page);
	ctbl[page >> 12] = value;
	asm volatile ("invlpg %0" :: "m" (page));
}

page_t page_get(uint32_t page) {
	return (cmap[page >> 22] & PF_PRES) ? ctbl[page >> 12] : 0;
}

inline void page_flush() {
	register uint32_t cr3;
	asm volatile("mov %%cr3, %0" : "=r" (cr3));
	asm volatile("mov %0, %%cr3" : : "r" (cr3));
}