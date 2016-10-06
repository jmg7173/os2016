#ifndef TESTLIB_H_
#define TESTLIB_H_

#include "list.h"
#include "hash.h"
#include "bitmap.h"
struct list_body
{
  struct list_elem point;
  int data;
};

struct hash_elem_body
{
  struct hash_elem point;
  int data;
};

const char* basic_command[] = {
    "create",
    "delete",
    "dumpdata",
    "quit"
};

/*
 * empty
 * size
 * clear
 * find
 * replace
 */

void test_list();
void test_hash();
void test_bitmap();

// Function for list
void create_list(struct list* elem);
void delete_list(struct list lists[10]);
void dumpdata_list(struct list lists[10]);
void list_command_push_back(struct list lists[10]);
void list_command_push_front(struct list lists[10]);
void list_command_pop_back(struct list list[10]);
void list_command_pop_front(struct list list[10]);
void list_command_insert(struct list lists[10]);
void list_command_insert_ordered(struct list lists[10]);
void list_command_unique(struct list lists[10]);
void list_command_splice(struct list lists[10]);
void list_command_remove(struct list lists[10]);
void list_command_swap(struct list lists[10]);
void list_command_size(struct list lists[10]);
void list_command_empty(struct list lists[10]);
void list_command_max(struct list lists[10]);
void list_command_min(struct list lists[10]);
void list_command_front(struct list lists[10]);
void list_command_back(struct list lists[10]);
void list_command_reverse(struct list lists[10]);
void list_command_sort(struct list lists[10]);
bool list_less(const struct list_elem *a, 
	       const struct list_elem *b,
	       void *aux);

// Function for hash
void create_hash(struct hash* elem);
void dumpdata_hash(struct hash hashs[10]);
void delete_hash(struct hash hashs[10]);
void hash_command_insert(struct hash hashs[10]);
void hash_command_apply(struct hash hashs[10]);
void hash_command_empty(struct hash hashs[10]);
void hash_command_size(struct hash hashs[10]);
void hash_command_clear(struct hash hashs[10]);
void hash_command_find(struct hash hashs[10]);
void hash_action_destruct(struct hash_elem *e, void *aux);
void hash_action_square(struct hash_elem *e, void *aux);
void hash_action_triple(struct hash_elem *e, void *aux);
bool hash_less(const struct hash_elem *a,
	       const struct hash_elem *b,
	       void *aux);

#endif
