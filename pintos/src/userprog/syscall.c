#include "userprog/syscall.h"
#include "userprog/process.h"
#include <stdio.h>
#include <syscall-nr.h>
#include <console.h>
#include <list.h>
#include "threads/interrupt.h"
#include "threads/malloc.h"
#include "threads/synch.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "devices/input.h"
#include "devices/shutdown.h"
#include "filesys/filesys.h"
#include "filesys/file.h"

static struct lock syslock;

static void syscall_handler (struct intr_frame *);
static struct file_elem *find_file_from_fd(int fd);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
  lock_init(&syslock);
}

/* Interrupt handle function */
static void
syscall_handler (struct intr_frame *f) 
{
  int syscallnum;
  
  if(!f->esp || !is_user_vaddr(f->esp) || f->esp < (void*)0x0)
    usercall_exit(-1);
  //hex_dump(f->esp,f->esp,212,true);
  syscallnum = *(int*)f->esp;
  //printf("syscallnum : %d thread : %s\n",syscallnum,thread_current()->name);
  switch(syscallnum)
    {
      /* Project 2 */
    case SYS_HALT:
      usercall_halt();
      break;
    case SYS_EXIT:
      if(!is_user_vaddr(f->esp+4))
	{
	  usercall_exit(-1);
	  break;
	}
      usercall_exit(*(int*)(f->esp+4));
      break;
    case SYS_EXEC:
      if(!is_user_vaddr(f->esp+4))
	{
	  usercall_exit(-1);
	  break;
	}
      f->eax = usercall_exec(*(const char**)(f->esp+4));
      break;
    case SYS_WAIT:
      if(!is_user_vaddr(f->esp+4))
	{
	  usercall_exit(-1);
	  break;
	}
      f->eax = usercall_wait(*(pid_t *)(f->esp+4));
      break;
      /* Not implement */
    case SYS_CREATE:
      if(!is_user_vaddr(f->esp+4) ||
	 !is_user_vaddr(f->esp+8))
	{
	  usercall_exit(-1);
	  break;
	}
      f->eax = usercall_create(*(const char**)(f->esp+4),
			       *(unsigned*)(f->esp+8));
      break;
    case SYS_REMOVE:
       if(!is_user_vaddr(f->esp+4))
	{
	  usercall_exit(-1);
	  break;
	}
       f->eax = usercall_remove(*(const char**)(f->esp+4));
       break;
    case SYS_OPEN:
      if(!is_user_vaddr(f->esp+4))
	{
	  usercall_exit(-1);
	  break;
	}
      f->eax = usercall_open(*(const char**)(f->esp+4));
      break;
    case SYS_FILESIZE:
      if(!is_user_vaddr(f->esp+4))
	{
	  usercall_exit(-1);
	  break;
	}
      f->eax = usercall_filesize(*(int*)(f->esp+4));

      break;
    case SYS_READ:
      if(!is_user_vaddr(f->esp+12))
	{
	  usercall_exit(-1);
	  break;
	}
      f->eax = usercall_read(*(int*)(f->esp+4),
			     *(void**)(f->esp+8),
			     *(unsigned*)(f->esp+12));

      break;
    case SYS_WRITE:
      if(!is_user_vaddr(f->esp+12))
	{
	  usercall_exit(-1);
	  break;
	}
      f->eax = usercall_write(*(int*)(f->esp+4),
			      *(const void**)(f->esp+8),
			      *(unsigned*)(f->esp+12));
      break;
    case SYS_SEEK:
      if(!is_user_vaddr(f->esp+8))
	{
	  usercall_exit(-1);
	  break;
	}
      usercall_seek(*(int*)(f->esp+4),
		    *(unsigned*)(f->esp+8));
      break;
    case SYS_TELL:
      if(!is_user_vaddr(f->esp+4))
	{
	  usercall_exit(-1);
	  break;
	}
      f->eax = usercall_tell(*(int*)(f->esp+4));
      break;
    case SYS_CLOSE:
      if(!is_user_vaddr(f->esp+4))
	{
	  usercall_exit(-1);
	  break;
	}
      usercall_close(*(int*)(f->esp+4));
      break;
      /* Newly defined system call */
    case SYS_FIBO:
      if(!is_user_vaddr(f->esp+4))
	{
	  usercall_exit(-1);
	  break;
	}
      f->eax = usercall_pibo(*(int*)(f->esp+4));
      break;
    case SYS_SUM4:
      if(!is_user_vaddr(f->esp+16))
	{
	  usercall_exit(-1);
	  break;
	}
      f->eax = usercall_sum4(*(int*)(f->esp+4),
			      *(int*)(f->esp+8),
			      *(int*)(f->esp+12),
			      *(int*)(f->esp+16));
      break;
      /* Not implement */
      /* Project 3 */
    case SYS_MMAP:
    case SYS_MUNMAP:
      /* Project 4 */
    case SYS_CHDIR:
    case SYS_MKDIR:
    case SYS_READDIR:
    case SYS_ISDIR:
    case SYS_INUMBER:
    default:
      break;
      /* implement first */
    

    }
}

void
usercall_halt(void)
{
  shutdown_power_off();
}

void
usercall_exit(int status)
{
  struct thread *curr = thread_current();
  curr->wait_exec = true;
  while(!list_empty(&curr->list_child))
    {
      struct list_elem *e 
	= list_pop_front(&curr->list_child);
      struct thread *child
	= list_entry(e,struct thread, child_elem);
      process_wait(child->tid);
    }
  while(!list_empty(&curr->files))
    {
      struct list_elem *e
	= list_pop_front(&curr->files);
      struct file_elem *fe
	= list_entry(e, struct file_elem, elem);
      lock_acquire(&syslock);
      file_close(fe->f);
      lock_release(&syslock);
      free(fe);
    }
  curr->return_status = status;
  printf("%s: exit(%d)\n",curr->name,status);
 
  /* Wait for parent to process_wait me */
  curr->parent->wait_exec = false;
  sema_up(&curr->parent->sema);
  sema_down(&curr->sema);
  if(curr->parent)
    list_remove(&curr->child_elem);
  thread_exit();
}

pid_t
usercall_exec(const char *file)
{
  return process_execute(file);
}

int
usercall_wait(pid_t pid)
{
  return process_wait(pid);
}

bool
usercall_create(const char *file, unsigned initial_size)
{
  bool result = false;
  if(!is_user_vaddr(file) || file == NULL)
    {
      usercall_exit(-1);
      return result;
    }
  else
    {
      lock_acquire(&syslock);
      result = filesys_create(file, initial_size);
      lock_release(&syslock);
    }
  return result;
}

bool
usercall_remove(const char *file)
{
  bool result = false;

  if(file == NULL || !is_user_vaddr(file))
    {
      usercall_exit(-1);
      return result;
    }

  lock_acquire(&syslock);
  result = filesys_remove(file);
  lock_release(&syslock);

  return result;
}

int
usercall_open(const char *file)
{
  struct thread *curr = thread_current();
  struct file_elem *f_elem;
  
  if(file == NULL || !is_user_vaddr(file))
    {
      usercall_exit(-1);
      return -1;
    }

  f_elem = malloc(sizeof(struct file_elem));

  lock_acquire(&syslock);
  f_elem->f = filesys_open(file);
  lock_release(&syslock);
  
  if(f_elem->f == NULL)
    {
      free(f_elem);
      return -1;
    }

  f_elem->fd = curr->newfd++;
  if(f_elem->fd >= MAX_FILE_NO)
    {
      file_close(f_elem->f);
      free(f_elem);
      return -1;
    }

  list_push_back(&curr->files,&f_elem->elem);

  return f_elem->fd;
}

int
usercall_filesize(int fd)
{
  struct file_elem *fe = find_file_from_fd(fd);
  int size = -1;
  if(fe == NULL)
    usercall_exit(-1);
  else size = file_length(fe->f);
  return size;
}

int
usercall_read(int fd, void *buffer, unsigned size)
{
  int retval = -1;
  /* For Standard Input */
  if(fd == 0)
    {
      unsigned i;
      for(i = 0; i<size; i++)
	{
	  *((uint8_t*)buffer + i) = input_getc();
	}
      retval = size;
    }
  /* For Invalid fd */
  else if(fd == 1 || fd < 0)
    usercall_exit(-1);
  /* For general file read */
  else
    {
      struct file_elem *fe = find_file_from_fd(fd);
      if(fe == NULL)
	usercall_exit(-1);
      else retval = file_read(fe->f, buffer, size);
    }
  return retval;
}

int
usercall_write(int fd, const void *buffer, unsigned size)
{
  int retval = -1;
  /* For Invalid fd */
  if(fd <= 0)
    usercall_exit(-1);
  /* For Standard Output */
  else if(fd == 1)
    {
      putbuf(buffer, size);
      retval = size;
    }
  /* For general file write */
  else
    {
      struct file_elem *fe = find_file_from_fd(fd);
      if(fe == NULL)
	usercall_exit(-1);
      else retval = file_write(fe->f, buffer, size);
    }
  return retval;
}

void
usercall_seek(int fd, unsigned position)
{
  struct file_elem *fe = find_file_from_fd(fd);
  if(fe == NULL)
    usercall_exit(-1);
  else
    file_seek(fe->f, position);
}

unsigned
usercall_tell(int fd)
{
  int result = -1;
  struct file_elem *fe = find_file_from_fd(fd);
  if(fe == NULL)
    usercall_exit(-1);
  else
    result = file_tell(fe->f);
  return result;
}

void
usercall_close(int fd)
{
  struct file_elem *fe = find_file_from_fd(fd);

  if(fe == NULL)
    usercall_exit(-1);
  else
    {
      struct thread *curr = thread_current();
      list_remove(&fe->elem);
      if(fe->fd + 1 == curr->newfd)
	curr->newfd--;

      lock_acquire(&syslock);
      file_close(fe->f);
      lock_release(&syslock);

      free(fe);
    }
}

void
usercall_seek(int fd, unsigned position)
{
}

unsigned
usercall_tell(int fd)
{
}

void
usercall_close(int fd)
{
}

int
usercall_pibo(int n)
{
  int i;
  int a = 0, b = 1, c = 0;
  for(i = 1; i<= n; i++)
    {
      a = b;
      b = c;
      c = a+b;
    }
  return c;
}

int
usercall_sum4(int a, int b, int c, int d)
{
  return a+b+c+d;
}

struct file_elem *
find_file_from_fd(int fd)
{
  struct thread *curr = thread_current();
  struct list_elem *e;
  for(e  = list_begin(&curr->files);
      e != list_end(&curr->files);
      e  = list_next(e))
    {
      struct file_elem *fe =
	list_entry(e, struct file_elem, elem);
      if(fe->fd == fd)
	return fe;
    }
  return NULL;
}
