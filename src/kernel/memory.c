#include <kernel/atags.h>
#include <kernel/memory.h>
#include <stdint.h>

void push_page(page_list_t* list, page_t* page)
{
	list->last->next = page;
	page->prev = list->last;
	list->last = page;

	list->size += 1;

	if (list->first == 0)
		list->first = page;
}

void push_page_at_start(page_list_t *list, page_t *page) {
	list->first->prev = page;
	page->next = list->first;
	list->first = page;

	list->size += 1;

	if (list->last == 0)
		list->last = page;
}

page_t* pop_page(page_list_t* list)
{
	if (list->size == 0)
		return 0;

	page_t* page = list->last;
	list->last = page->prev;

	if (list->last != 0)
		list->last->next = 0;

	list->size -= 1;

	return page;
}

page_t* peek_page(page_list_t* list)
{
	if (list->size == 0)
		return 0;

	return list->last;
}

void remove_page(page_list_t *list, page_t *page) {
	if (page->prev != 0)
		page->prev->next = page->next;
	else
		list->first = page->next;

	if (page->next != 0)
		page->next->prev = page->prev;
	else
		list->last = page->prev;

	list->size -= 1;
}
