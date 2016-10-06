#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "testlib.h"

// create, delete, dumpdata, quit, command
int
main()
{
  char command[51];
  char *type;
  fgets(command,100,stdin);
  strtok(command," ");
  type = strtok(NULL, " ");
  if(!strcmp(type,"list"))
    test_list();
  else if(!strcmp(type, "hashtable"))
    test_hash();
  else if(!strcmp(type, "bitmap"))
    test_bitmap();

  return 0;
}

void
test_list()
{
  struct list lists[10];
  char *name, *command;
  char tmp[51];

  name = strtok(NULL, " ");
  create_list(&lists[name[4]-'0']);
  while(true){
      fgets(tmp,50,stdin);
      tmp[strlen(tmp)-1] = '\0';
      command = strtok(tmp," ");
      
      if(!strcmp(command,"create")){
	  name = strtok(NULL, " ");
	  name = strtok(NULL, " ");
	  create_list(&lists[name[4]-'0']);
      }
      else if(!strcmp(command,"dumpdata"))
	dumpdata_list(lists);
      else if(!strcmp(command,"delete"))
	delete_list(lists);
      else if(!strcmp(command,"list_push_back"))
	list_command_push_back(lists);
      else if(!strcmp(command,"list_push_front"))
	list_command_push_front(lists);
      else if(!strcmp(command,"list_pop_back"))
	list_command_pop_back(lists);
      else if(!strcmp(command,"list_pop_front"))
	list_command_pop_front(lists);
      else if(!strcmp(command,"list_insert"))
	list_command_insert(lists);
      else if(!strcmp(command,"list_insert_ordered"))
	list_command_insert_ordered(lists);
      else if(!strcmp(command,"list_unique"))
	list_command_unique(lists);
      else if(!strcmp(command,"list_splice"))
	list_command_splice(lists);
      else if(!strcmp(command,"list_remove"))
	list_command_remove(lists);
      else if(!strcmp(command,"list_swap"))
	list_command_swap(lists);
      else if(!strcmp(command,"list_size"))
	list_command_size(lists);
      else if(!strcmp(command,"list_empty"))
	list_command_empty(lists);
      else if(!strcmp(command,"list_max"))
	list_command_max(lists);
      else if(!strcmp(command,"list_min"))
	list_command_min(lists);
      else if(!strcmp(command,"list_front"))
	list_command_front(lists);
      else if(!strcmp(command,"list_back"))
	list_command_back(lists);
      else if(!strcmp(command,"list_sort"))
	list_command_sort(lists);
      else if(!strcmp(command,"list_reverse"))
	list_command_reverse(lists);


      else if(!strcmp(command,"quit"))
	return;
  }
}

bool
list_less(const struct list_elem *a,
	       const struct list_elem *b,
	       void *aux)
{
  return (((list_entry(a, struct list_body, point))->data) <
	  ((list_entry(b, struct list_body, point))->data));
}

void
create_list(struct list* elem)
{
  list_init(elem);
}

void
dumpdata_list(struct list lists[10])
{
  char *name;
  struct list_elem *e;
  struct list *curr;
  int idx;	  
  name = strtok(NULL, " ");
  idx = name[4] - '0';
  curr = &lists[idx];
  for(e = list_begin(curr); e != list_end(curr);
      e = list_next(e))
    {
      struct list_body *lb = list_entry(e, struct list_body, point);
      printf("%d ", lb->data);
    }
  printf("\n");
}

void
delete_list(struct list lists[10])
{
  char *name;
  struct list *curr;
  int idx;

  name = strtok(NULL, " ");
  idx = name[4] - '0';
  curr = &lists[idx];

  while(!list_empty(curr))
    {
      struct list_elem *e = list_pop_front(curr);
      struct list_body *lb = list_entry(e, struct list_body, point);
      free(lb);
    }
}

void
list_command_push_back(struct list lists[10])
{  
  char *name;
  struct list *curr;
  struct list_body* new = (struct list_body*)malloc(sizeof(struct list_body));
  int data, idx;

  name = strtok(NULL, " ");
  idx = name[4] - '0';
  curr = &lists[idx];
  data = atoi(strtok(NULL, " "));
  new->data = data;
  
  list_push_back(curr,&(new->point));
}

void
list_command_push_front(struct list lists[10])
{  
  char *name;
  struct list *curr;
  struct list_body* new = (struct list_body*)malloc(sizeof(struct list_body));
  int data, idx;

  name = strtok(NULL, " ");
  idx = name[4] - '0';
  curr = &lists[idx];
  data = atoi(strtok(NULL, " "));
  new->data = data;
  
  list_push_front(curr,&(new->point));
}

void
list_command_insert(struct list lists[10])
{  
  char *name;
  struct list *curr;
  struct list_body* new = (struct list_body*)malloc(sizeof(struct list_body));
  struct list_elem *e;
  int pos, data, idx;
  int i;
  
  name = strtok(NULL, " ");
  idx = name[4] - '0';
  curr = &lists[idx];
  
  pos = atoi(strtok(NULL, " "));
  data = atoi(strtok(NULL, " "));
  
  new->data = data;

  for(i = 0, e = list_begin(curr); (e != list_end(curr)) && i < pos;
      e = list_next(e), i++);

  list_insert(e, &(new->point));
}

void
list_command_insert_ordered(struct list lists[10])
{
  char *name;
  struct list *curr;
  struct list_body* new = (struct list_body*)malloc(sizeof(struct list_body));
  int data, idx;

  name = strtok(NULL, " ");
  idx = name[4] - '0';
  curr = &lists[idx];
  data = atoi(strtok(NULL, " "));
  new->data = data;
  
  list_insert_ordered(curr, &(new->point), list_less, NULL);
}

void
list_command_pop_back(struct list lists[10])
{
  char *name;
  struct list *curr;
  struct list_body* old;
  struct list_elem* e;
  int idx;
  
  name = strtok(NULL, " ");
  idx = name[4] - '0';
  curr = &lists[idx];

  e = list_pop_back(curr);
  old = list_entry(e, struct list_body, point);
  free(old);
}

void
list_command_pop_front(struct list lists[10])
{
  char *name;
  struct list *curr;
  struct list_body* old;
  struct list_elem* e;
  int idx;

  name = strtok(NULL, " ");
  idx = name[4] - '0';
  curr = &lists[idx];
  
  e = list_pop_front(curr);
  old = list_entry(e, struct list_body, point);
  free(old);
}

void
list_command_unique(struct list lists[10])
{
  char *name;
  struct list *first, *second=NULL;
  int idx1, idx2 = -1;
  
  name = strtok(NULL, " ");
  idx1 = name[4] - '0';
  name = strtok(NULL, " ");
  if(name){
      idx2 = name[4] - '0';
      second = &lists[idx2];
  }
  first = &lists[idx1];
  list_unique(first,second,list_less,NULL);
}

void
list_command_splice(struct list lists[10])
{
  char* name;
  int idx1, idx2;
  int dest, from, to;
  int i;
  struct list_elem *before, *start, *end;
  struct list *first, *second;
  
  name = strtok(NULL, " ");
  idx1 = name[4] - '0';
  first = &lists[idx1];

  dest = atoi(strtok(NULL, " "));
  
  name = strtok(NULL, " ");
  idx2 = name[4] - '0';
  second = &lists[idx2];

  from = atoi(strtok(NULL, " "));
  to = atoi(strtok(NULL, " "));
  for(i = 0, before = list_begin(first); (before != list_end(first)) && i < dest;
      before = list_next(before), i++);

  for(i = 0, start = list_begin(second); (start != list_end(second)) && i < from;
      start = list_next(start), i++);
  
  for(i = 0, end = list_begin(second); (end != list_end(second)) && i < to;
      end = list_next(end), i++);

  list_splice(before, start, end);
}

void
list_command_remove(struct list lists[10])
{
  char *name;
  struct list *curr;
  struct list_body* old;
  struct list_elem *e;
  int pos, idx;
  int i;
  
  name = strtok(NULL, " ");
  idx = name[4] - '0';
  curr = &lists[idx];
  
  pos = atoi(strtok(NULL, " "));
  
  for(i = 0, e = list_begin(curr); (e != list_end(curr)) && i < pos;
      e = list_next(e), i++);
  
  old = list_entry(e, struct list_body, point);
  list_remove(e);
  free(old);
}

void
list_command_swap(struct list lists[10])
{
  char *name;
  struct list *curr;
  struct list_elem *e1, *e2;
  int pos1, pos2, idx;
  int i;
  name = strtok(NULL, " ");
  idx = name[4] - '0';
  curr = &lists[idx];
  
  pos1 = atoi(strtok(NULL, " "));
  pos2 = atoi(strtok(NULL, " "));
  for(i = 0, e1 = list_begin(curr); (e1 != list_end(curr)) && i < pos1;
      e1 = list_next(e1), i++);
  for(i = 0, e2 = list_begin(curr); (e2 != list_end(curr)) && i < pos2;
      e2 = list_next(e2), i++);
  list_swap(e1, e2);
}

void
list_command_size(struct list lists[10])
{
  char *name;
  struct list *curr;
  int idx;
  
  name = strtok(NULL, " ");
  idx = name[4] - '0';
  curr = &lists[idx];
  
  printf("%d\n",list_size(curr));
}

void
list_command_empty(struct list lists[10])
{
  char *name;
  struct list *curr;
  int idx;
  
  name = strtok(NULL, " ");
  idx = name[4] - '0';
  curr = &lists[idx];

  if(list_empty(curr))
    printf("true\n");
  else
    printf("false\n");
}

void
list_command_max(struct list lists[10])
{
  char *name;
  struct list *curr;
  struct list_elem *e;
  struct list_body *body;
  int idx;
  
  name = strtok(NULL, " ");
  idx = name[4] - '0';
  curr = &lists[idx];

  e = list_max(curr, list_less, NULL);
  body = list_entry(e, struct list_body, point);
  printf("%d\n",body->data);
}

void
list_command_min(struct list lists[10])
{
  char *name;
  struct list *curr;
  struct list_elem *e;
  struct list_body *body;
  int idx;
  
  name = strtok(NULL, " ");
  idx = name[4] - '0';
  curr = &lists[idx];

  e = list_min(curr, list_less, NULL);
  body = list_entry(e, struct list_body, point);
  printf("%d\n",body->data);
}

void
list_command_front(struct list lists[10])
{
  char *name;
  struct list *curr;
  struct list_elem *e;
  struct list_body *body;
  int idx;
  
  name = strtok(NULL, " ");
  idx = name[4] - '0';
  curr = &lists[idx];

  e = list_front(curr);
  body = list_entry(e, struct list_body, point);
  printf("%d\n",body->data);
}

void
list_command_back(struct list lists[10])
{
  char *name;
  struct list *curr;
  struct list_elem *e;
  struct list_body *body;
  int idx;
  
  name = strtok(NULL, " ");
  idx = name[4] - '0';
  curr = &lists[idx];

  e = list_back(curr);
  body = list_entry(e, struct list_body, point);
  printf("%d\n",body->data);
}

void
list_command_reverse(struct list lists[10])
{
  char *name;
  struct list *curr;
  int idx;
  
  name = strtok(NULL, " ");
  idx = name[4] - '0';
  curr = &lists[idx];

  list_reverse(curr);
}

void
list_command_sort(struct list lists[10])
{
  char *name;
  struct list *curr;
  int idx;
  
  name = strtok(NULL, " ");
  idx = name[4] - '0';
  curr = &lists[idx];

  list_sort(curr, list_less, NULL);
}

void
test_hash()
{
  struct hash hashs[10];
  char *name, *command;
  char tmp[51];

  name = strtok(NULL, " ");
  create_hash(&hashs[name[4]-'0']);
  while(true){
      fgets(tmp,50,stdin);
      tmp[strlen(tmp)-1] = '\0';
      command = strtok(tmp," ");
      
      if(!strcmp(command,"create")){
	  name = strtok(NULL, " ");
	  name = strtok(NULL, " ");
	  create_hash(&hashs[name[4]-'0']);
      }
      else if(!strcmp(command,"dumpdata"))
	dumpdata_hash(hashs);
      else if(!strcmp(command,"delete"))
	delete_hash(hashs);
      else if(!strcmp(command,"hash_insert"))
	hash_command_insert(hashs);
      else if(!strcmp(command,"hash_apply"))
	hash_command_apply(hashs);
      else if(!strcmp(command,"hash_empty"))
	hash_command_empty(hashs);
      else if(!strcmp(command,"hash_size"))
	hash_command_size(hashs);
      else if(!strcmp(command,"hash_clear"))
	hash_command_clear(hashs);
      else if(!strcmp(command,"hash_find"))
	hash_command_find(hashs);
      else if(!strcmp(command,"hash_replace"))
	hash_command_replace(hashs);
      else if(!strcmp(command,"hash_delete"))
	hash_command_delete(hashs);
      else if(!strcmp(command,"quit"))
	break;
  }
}

unsigned
hash_func(const struct hash_elem *e, void *aux)
{
  return hash_int((hash_entry(e, struct hash_elem_body, point))->data);
}

bool
hash_less(const struct hash_elem *a,
	       const struct hash_elem *b,
	       void *aux)
{
  return (((hash_entry(a, struct hash_elem_body, point))->data) <
	  ((hash_entry(b, struct hash_elem_body, point))->data));
}

void
hash_action_destruct(struct hash_elem *e, void *aux)
{
  struct hash_elem_body *old = 
    hash_entry(e, struct hash_elem_body, point);
  free(old);
}

void
hash_action_square(struct hash_elem *e, void *aux)
{
  struct hash_elem_body *heb = 
    hash_entry(e, struct hash_elem_body, point);
  int data = heb->data;
  data = data * data;
  heb->data = data;
}

void
hash_action_triple(struct hash_elem *e, void *aux)
{
  struct hash_elem_body *heb = 
    hash_entry(e, struct hash_elem_body, point);
  int data = heb->data;
  data = data * data * data;
  heb->data = data;
}

void
create_hash(struct hash* elem)
{
  hash_init(elem, hash_func, hash_less, NULL);
}

void
dumpdata_hash(struct hash hashs[10])
{
  char *name;
  struct hash *curr;
  struct hash_iterator i;
  int idx;	  
  name = strtok(NULL, " ");
  idx = name[4] - '0';
  curr = &hashs[idx];

  hash_first(&i, curr);
  while(hash_next(&i)){
      struct hash_elem_body *heb 
	= hash_entry(hash_cur(&i), struct hash_elem_body, point);
      printf("%d ", heb->data);
  }
  printf("\n");
}

void
delete_hash(struct hash hashs[10])
{
  char *name;
  struct hash *curr;
  int idx;

  name = strtok(NULL, " ");
  idx = name[4] - '0';
  curr = &hashs[idx];
  hash_destroy(curr, hash_action_destruct);
}

void
hash_command_insert(struct hash hashs[10])
{
  char *name;
  struct hash *curr;
  struct hash_elem_body* new 
    = (struct hash_elem_body*)malloc(sizeof(struct hash_elem_body));
  int data, idx;

  name = strtok(NULL, " ");
  idx = name[4] - '0';
  curr = &hashs[idx];
  data = atoi(strtok(NULL, " "));
  new->data = data;

  hash_insert(curr,&(new->point));
}

void
hash_command_apply(struct hash hashs[10])
{
  char *name;
  struct hash *curr;
  int idx;

  name = strtok(NULL, " ");
  idx = name[4] - '0';
  curr = &hashs[idx];

  name = strtok(NULL, " ");
  if(!strcmp(name,"square"))
    hash_apply(curr,hash_action_square);
  else if(!strcmp(name, "triple"))
    hash_apply(curr,hash_action_triple);
}

void
hash_command_empty(struct hash hashs[10])
{
  char *name;
  struct hash *curr;
  int idx;
 
  name = strtok(NULL, " ");
  idx = name[4] - '0';
  curr = &hashs[idx];

  if(hash_empty(curr))
    printf("true\n");
  else
    printf("false\n");
}

void
hash_command_size(struct hash hashs[10])
{
  char *name;
  struct hash *curr;
  int idx;

  name = strtok(NULL," ");
  idx = name[4] - '0';
  curr = &hashs[idx];
  printf("%d\n",hash_size(curr));
}

void
hash_command_clear(struct hash hashs[10])
{
  char *name;
  struct hash *curr;
  int idx;

  name = strtok(NULL, " ");
  idx = name[4] - '0';
  curr = &hashs[idx];

  hash_clear(curr, hash_action_destruct);
}

void
hash_command_find(struct hash hashs[10])
{
  char *name;
  struct hash *curr;
  struct hash_elem_body tmp;
  struct hash_elem* find;
  int data_find, idx;

  name = strtok(NULL," ");
  idx = name[4] - '0';
  curr = &hashs[idx];

  data_find = atoi(strtok(NULL, " "));
  tmp.data = data_find;
  find = hash_find(curr,&(tmp.point));
  if(find){
      printf("%d\n",hash_entry(find, struct hash_elem_body, point)->data);
  }
}

void
hash_command_replace(struct hash hashs[10])
{
  char *name;
  struct hash *curr;
  struct hash_elem *old_elem;
  struct hash_elem_body *new = 
    (struct hash_elem_body*)malloc(sizeof(struct hash_elem_body));
  struct hash_elem_body *old;
  int idx, data;

  name = strtok(NULL, " ");
  idx = name[4] - '0';
  curr = &hashs[idx];

  data = atoi(strtok(NULL, " "));
  new->data = data;
  old_elem = hash_replace(curr,&(new->point));
  if(old_elem){
      old = hash_entry(old_elem, struct hash_elem_body, point);
      free(old);
  }
}

void
hash_command_delete(struct hash hashs[10])
{
  char *name;
  struct hash *curr;
  struct hash_elem *old_elem;
  struct hash_elem_body *old;
  struct hash_elem_body find;
  int idx;

  name = strtok(NULL, " ");
  idx = name[4] - '0';
  curr = &hashs[idx];

  find.data = atoi(strtok(NULL, " "));
  old_elem = hash_delete(curr,&(find.point));
  if(old_elem){
      old = hash_entry(old_elem, struct hash_elem_body, point);
      free(old);
  }
}

void
test_bitmap()
{
  struct bitmap **bitmaps = (struct bitmap**)malloc(sizeof(struct bitmap*)*10);
  char *name, *command;
  char tmp[51];

  name = strtok(NULL, " ");
  create_bitmap(&bitmaps[name[2]-'0']);
  while(true){
      fgets(tmp,50,stdin);
      tmp[strlen(tmp)-1] = '\0';
      command = strtok(tmp," ");
      
      if(!strcmp(command,"create")){
	  name = strtok(NULL, " ");
	  name = strtok(NULL, " ");
	  create_bitmap(&bitmaps[name[2]-'0']);
      }
      else if(!strcmp(command,"dumpdata"))
	dumpdata_bitmap(bitmaps);
      else if(!strcmp(command,"delete"))
	delete_bitmap(bitmaps);
      else if(!strcmp(command,"bitmap_mark"))
	bitmap_command_mark(bitmaps);
      else if(!strcmp(command,"bitmap_all"))
	bitmap_command_all(bitmaps);
      else if(!strcmp(command,"bitmap_any"))
	bitmap_command_any(bitmaps);
      else if(!strcmp(command,"bitmap_contains"))
	bitmap_command_contains(bitmaps);
      else if(!strcmp(command,"bitmap_none"))
	bitmap_command_none(bitmaps);
      else if(!strcmp(command,"bitmap_count"))
	bitmap_command_count(bitmaps);
      else if(!strcmp(command,"bitmap_expand"))
	bitmap_command_expand(bitmaps);
      else if(!strcmp(command,"bitmap_set_all"))
	bitmap_command_set_all(bitmaps);
      else if(!strcmp(command,"bitmap_flip"))
	bitmap_command_flip(bitmaps);
      else if(!strcmp(command,"bitmap_reset"))
	bitmap_command_reset(bitmaps);
      else if(!strcmp(command,"bitmap_scan"))
	bitmap_command_scan(bitmaps);
      else if(!strcmp(command,"bitmap_scan_and_flip"))
	bitmap_command_scan_and_flip(bitmaps);
      else if(!strcmp(command,"quit"))
	break;
  }
  free(bitmaps);
}

void
create_bitmap(struct bitmap** elem)
{
  int size;
  size = atoi(strtok(NULL, " "));
  *elem = bitmap_create(size);
}

void
dumpdata_bitmap(struct bitmap **bitmaps)
{
  char *name;
  int idx;
  struct bitmap *curr;
  size_t i;

  name = strtok(NULL, " ");
  idx = name[2]-'0';
  curr = bitmaps[idx];
  for(i = 0; i < bitmap_size(curr); i++){
      printf("%d",bitmap_test(curr,i));
  }
  printf("\n");
}

void
delete_bitmap(struct bitmap **bitmaps)
{
  char *name;
  int idx;

  name = strtok(NULL, " ");
  idx = name[2] - '0';
  bitmap_destroy(bitmaps[idx]);
}

void
bitmap_command_mark(struct bitmap **bitmaps)
{
  char *name;
  int idx, bm_idx;
  struct bitmap *curr;

  name = strtok(NULL, " ");
  idx = name[2] - '0';
  curr = bitmaps[idx];
  bm_idx = atoi(strtok(NULL, " "));
  bitmap_mark(curr, bm_idx);
}

void
bitmap_command_all(struct bitmap **bitmaps)
{
  char *name;
  int idx, start, cnt;
  struct bitmap *curr;

  name = strtok(NULL, " ");
  idx = name[2] - '0';
  curr = bitmaps[idx];
  
  start = atoi(strtok(NULL, " "));
  cnt = atoi(strtok(NULL, " "));

  if(bitmap_all(curr, start, cnt))
    printf("true\n");
  else
    printf("false\n");
}

void
bitmap_command_any(struct bitmap **bitmaps)
{
  char *name;
  int idx, start, cnt;
  struct bitmap *curr;

  name = strtok(NULL, " ");
  idx = name[2] - '0';
  curr = bitmaps[idx];
 
  start = atoi(strtok(NULL, " "));
  cnt = atoi(strtok(NULL, " "));

  if(bitmap_any(curr, start, cnt))
    printf("true\n");
  else
    printf("false\n");
}

void
bitmap_command_contains(struct bitmap **bitmaps)
{
  char *name;
  int idx, start, cnt;
  struct bitmap *curr;

  name = strtok(NULL, " ");
  idx = name[2] - '0';
  curr = bitmaps[idx];

  start = atoi(strtok(NULL, " "));
  cnt = atoi(strtok(NULL, " "));

  name = strtok(NULL, " ");
  
  if(!strcmp(name,"true")){
      if(bitmap_contains(curr, start, cnt, true))
	printf("true\n");
      else
	printf("false\n");
  }
  else if(!strcmp(name, "false")){
      if(bitmap_contains(curr, start, cnt, false))
	printf("true\n");
      else
	printf("false\n");
  }
}

void
bitmap_command_none(struct bitmap **bitmaps)
{
  char *name;
  int idx, start, cnt;
  struct bitmap *curr;

  name = strtok(NULL, " ");
  idx = name[2] - '0';
  curr = bitmaps[idx];

  start = atoi(strtok(NULL, " "));
  cnt = atoi(strtok(NULL, " "));

  if(bitmap_none(curr, start, cnt))
    printf("true\n");
  else
    printf("false\n");
}
void
bitmap_command_count(struct bitmap **bitmaps)
{
  char *name;
  int idx, start, cnt;
  struct bitmap *curr;

  name = strtok(NULL, " ");
  idx = name[2] - '0';
  curr = bitmaps[idx];

  start = atoi(strtok(NULL, " "));
  cnt = atoi(strtok(NULL, " "));

  name = strtok(NULL, " ");
  
  if(!strcmp(name,"true")){
      printf("%d\n",bitmap_count(curr,start,cnt,true));
  }
  else if(!strcmp(name, "false")){
      printf("%d\n",bitmap_count(curr,start,cnt,false));
  }
}

void
bitmap_command_expand(struct bitmap **bitmaps)
{
  char *name;
  int idx, size_up;
  struct bitmap *curr;

  name = strtok(NULL, " ");
  idx = name[2] - '0';
  curr = bitmaps[idx];

  size_up = atoi(strtok(NULL, " "));
  curr = bitmap_expand(curr, size_up);
}

void
bitmap_command_flip(struct bitmap **bitmaps)
{
  char *name;
  int idx, bm_idx;
  struct bitmap *curr;

  name = strtok(NULL, " ");
  idx = name[2] - '0';
  curr = bitmaps[idx];

  bm_idx = atoi(strtok(NULL, " "));
  bitmap_flip(curr,bm_idx);
}

void
bitmap_command_set_all(struct bitmap **bitmaps)
{
  char *name;
  int idx;
  struct bitmap *curr;

  name = strtok(NULL, " ");
  idx = name[2] - '0';
  curr = bitmaps[idx];

  name = strtok(NULL, " ");
  if(!strcmp(name,"true"))
    bitmap_set_all(curr,true);
  else
    bitmap_set_all(curr,false);
}

void
bitmap_command_reset(struct bitmap **bitmaps)
{
  char *name;
  int idx, bm_idx;
  struct bitmap *curr;
  name = strtok(NULL, " ");
  idx = name[2] - '0';
  curr = bitmaps[idx];

  bm_idx = atoi(strtok(NULL, " "));
  bitmap_reset(curr,bm_idx);
}

void
bitmap_command_scan(struct bitmap **bitmaps)
{
  char *name;
  int idx, start, cnt;
  struct bitmap *curr;

  name = strtok(NULL, " ");
  idx = name[2] - '0';
  curr = bitmaps[idx];

  start = atoi(strtok(NULL, " "));
  cnt = atoi(strtok(NULL, " "));
  name = strtok(NULL, " ");
  if(!strcmp(name,"true"))
    printf("%u\n",bitmap_scan(curr,start,cnt,true));
  else if(!strcmp(name,"false"))
    printf("%u\n",bitmap_scan(curr,start,cnt,false));
}

void
bitmap_command_scan_and_flip(struct bitmap **bitmaps)
{
  char *name;
  int idx, start, cnt;
  struct bitmap *curr;

  name = strtok(NULL, " ");
  idx = name[2] - '0';
  curr = bitmaps[idx];

  start = atoi(strtok(NULL, " "));
  cnt = atoi(strtok(NULL, " "));
  name = strtok(NULL, " ");
  if(!strcmp(name,"true"))
    printf("%u\n",bitmap_scan_and_flip(curr,start,cnt,true));
  else if(!strcmp(name,"false"))
    printf("%u\n",bitmap_scan_and_flip(curr,start,cnt,false));
}
