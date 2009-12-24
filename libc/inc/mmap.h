#ifndef MMAP_H
#define MMAP_H

#include <config.h>
#include <flux.h>

#define MMAP_PAGESIZE PAGESZ

#define PROT_NONE 0
#define PROT_READ MMAP_READ
#define PROT_WRITE MMAP_WRITE
#define PROT_EXEC MMAP_EXEC

int mmap(void *addr, size_t length, int prot);
int umap(void *addr, size_t length);
int emap(void *addr, uint32_t frame, int prot);

#endif/*MMAP_H*/
