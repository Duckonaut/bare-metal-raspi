#ifndef KERNEL_MEMORY_H
#define KERNEL_MEMORY_H

#include <stdint.h>

#define PAGE_SIZE 4096
#define KERNEL_HEAP_SIZE (1024 * 1024)
#define KERNEL_STACK_SIZE PAGE_SIZE

typedef struct  {
	uint8_t allocated: 1;			// This page is allocated to something
	uint8_t kernel_page: 1;			// This page is a part of the kernel
	uint8_t kernel_heap_page: 1;	// This page is a part of the kernel
	uint32_t reserved: 29;
} page_flags_t;

struct page;
typedef struct {
	struct page* first;
	struct page* last;
	uint32_t size;
} page_list_t;

typedef struct page { 
	uint32_t virtual_address;
	page_flags_t flags;
	struct page* prev;
	struct page* next;
	page_list_t* container;
} page_t;

void push_page(page_list_t* list, page_t* page);
void push_page_at_start(page_list_t* list, page_t* page);
page_t* pop_page(page_list_t* list);
page_t* peek_page(page_list_t* list);
void remove_page(page_list_t* list, page_t* page);

void memory_init();

void* kmalloc(uint32_t size);
void kfree(void* ptr);

void* page_alloc();
void page_free(void* ptr);

#endif
