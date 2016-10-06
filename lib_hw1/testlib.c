#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "testlib.h"

// create, delete, dumpdata, quit, command
int main()
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

void test_list()
{
  struct list* lists = (struct list*)malloc(sizeof(struct list)*10);
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

bool list_less(const struct list_elem *a,
	       const struct list_elem *b,
	       void *aux)
{
  return (((list_entry(a, struct list_body, point))->data) <
	  ((list_entry(b, struct list_body, point))->data));
}

void create_list(struct list* elem)
{
  list_init(elem);
}

void dumpdata_list(struct list lists[10])
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

void delete_list(struct list lists[10])
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

void list_command_push_back(struct list lists[10])
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

void list_command_push_front(struct list lists[10])
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

void list_command_insert(struct list lists[10])
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

void list_command_insert_ordered(struct list lists[10])
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

void list_command_pop_back(struct list lists[10])
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

void list_command_pop_front(struct list lists[10])
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

void list_command_unique(struct list lists[10])
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

void list_command_splice(struct list lists[10])
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

void list_command_remove(struct list lists[10])
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

void list_command_swap(struct list lists[10])
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

void list_command_size(struct list lists[10])
{
  char *name;
  struct list *curr;
  int idx;
  
  name = strtok(NULL, " ");
  idx = name[4] - '0';
  curr = &lists[idx];
  
  printf("%d\n",list_size(curr));
}

void list_command_empty(struct list lists[10])
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

void list_command_max(struct list lists[10])
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

void list_command_min(struct list lists[10])
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

void list_command_front(struct list lists[10])
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

void list_command_back(struct list lists[10])
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

void list_command_reverse(struct list lists[10])
{
  char *name;
  struct list *curr;
  int idx;
  
  name = strtok(NULL, " ");
  idx = name[4] - '0';
  curr = &lists[idx];

  list_reverse(curr);
}

void list_command_sort(struct list lists[10])
{
  char *name;
  struct list *curr;
  int idx;
  
  name = strtok(NULL, " ");
  idx = name[4] - '0';
  curr = &lists[idx];

  list_sort(curr, list_less, NULL);
}

void test_hash()
{

}

void test_bitmap()
{
}
