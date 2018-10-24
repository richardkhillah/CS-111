#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <errno.h>

#include "SortedList.h"
#include "common.h"

void SortedList_insert(SortedList_t* list, SortedListElement_t* element) {
	// ensure valid list and element
	if(list == NULL || element == NULL) {
		return;
	}

	SortedListElement_t* ptr = list->next;

	// navigate through list, comparing node elements along the way.
	while(ptr != list) {
		if(strcmp(element->key, ptr->key)) {
			break;
		}
		ptr = ptr->next;
	}

	if(opt_yield & INSERT_YIELD) {
		sched_yield();
	}

	// connect element-to-add to list
	element->next = ptr;
	element->prev = ptr ->prev;

	// change exising list references.
	ptr->prev->next = element;
	ptr->prev = element;
} // end SortedList_insert()


int SortedList_delete(SortedListElement_t* element) {
	

}