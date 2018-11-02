#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <string.h>

#include <errno.h>
#include <sched.h>

#include "SortedList.h"
#include "utils.h"

//int opt_yield = 0;

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
	// ensure we have a list
	if(element == NULL) {
		return 0; // done
	}

	// ensure element is properly part of a list before removing
	if(element->next->prev == element->prev->next) {
		if(opt_yield & DELETE_YIELD) {
			sched_yield();
		}
			// remove
			element->prev->next = element->next;
			element->next->prev = element->prev;
			return 0; // success
	}

	return 1; // error with the list
} // end SortedList_delete


SortedListElement_t* SortedList_lookup(SortedList_t* list, const char* key) {
	// ensure valid list and key
	if(list == NULL || key == NULL) {
		return NULL;
	}

	SortedList_t* ptr = list->next;

	while(ptr != list) {
		if(strcmp(key, ptr->key) == 0) {
			return ptr; // found element
		}

		if(opt_yield & LOOKUP_YIELD) {
			sched_yield();
		}

		ptr = ptr->next; // keep looking
	}

	return NULL; // error
} // end SortedList_lookup()


int SortedList_length(SortedList_t* list) {
	// ensure we have a list to work with
	if(list == NULL) {
		return -1; 
	}

	int numElements = 0;

	SortedList_t* ptr = list->next;
	
	while(ptr != list) {
		numElements++;

		if(opt_yield & LOOKUP_YIELD) {
			sched_yield();
		}

		ptr = ptr->next;
	}
	return numElements;
} // end SortedList_length()