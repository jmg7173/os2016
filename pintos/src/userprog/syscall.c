#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include <console.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "devices/input.h"
#include "devices/shutdown.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

// XXX : Modify f's eax, esp and so on?? Reference intr_handler
// XXX : To Pass test, implement exec system call.
// TODO : implement write, read system call first!
/* Interrupt handle function */
static void
syscall_handler (struct intr_frame *f) 
{
  int syscallnum;
  // XXX : How can I handle system call? make base structure.
  // use switch-case?
  // Interrupt vector no is known as 0x30
  if(!f->esp || !is_user_vaddr(f->esp) || f->esp < (void*)0x0)
    usercall_exit(-1);
  //hex_dump(f->esp,f->esp,212,true);
  syscallnum = *(int*)f->esp;
  switch(syscallnum)
    {
      /* implement later */
      /* Project 2 */
    case SYS_HALT:
      usercall_halt();
      break;
    case SYS_EXIT:
      if(!is_user_vaddr(f->esp+4))
	usercall_exit(-1);
      usercall_exit(*(int*)(f->esp+4));
      break;
    case SYS_EXEC:
      if(!is_user_vaddr(f->esp+4))
	usercall_exit(-1);
      usercall_exec(*(const char**)(f->esp+4));
      break;
    case SYS_WAIT:
      break;
      /* Not implement */
    case SYS_CREATE:
    case SYS_REMOVE:
    case SYS_OPEN:
    case SYS_FILESIZE:
      break;
    case SYS_READ:
      if(!is_user_vaddr(f->esp+4) ||
	 !is_user_vaddr(f->esp+8) ||
	 !is_user_vaddr(f->esp+12))
	usercall_exit(-1);
      f->eax = usercall_read(*(int*)(f->esp+4),
			     *(void**)(f->esp+8),
			     *(unsigned*)(f->esp+12));

      break;
    case SYS_WRITE:
      if(!is_user_vaddr(f->esp+4) ||
	 !is_user_vaddr(f->esp+8) ||
	 !is_user_vaddr(f->esp+12))
	usercall_exit(-1);
      f->eax = usercall_write(*(int*)(f->esp+4),
			      *(const void**)(f->esp+8),
			      *(unsigned*)(f->esp+12));
      break;
      /* Newly defined system call */
    case SYS_FIBO:
      if(!is_user_vaddr(f->esp+4))
	usercall_exit(-1);
      f->eax = usercall_pibo(*(int*)(f->esp+4));
      break;
    case SYS_SUM4:
      if(!is_user_vaddr(f->esp+4) ||
	 !is_user_vaddr(f->esp+8) ||
	 !is_user_vaddr(f->esp+12)||
	 !is_user_vaddr(f->esp+16))
	usercall_exit(-1);
      f->eax = usercall_sum4(*(int*)(f->esp+4),
			      *(int*)(f->esp+8),
			      *(int*)(f->esp+12),
			      *(int*)(f->esp+16));
      break;
      /* Not implement */
    case SYS_SEEK:
    case SYS_TELL:
    case SYS_CLOSE:
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

// TODO : detach thread from thread list
void
usercall_exit(int status)
{
  struct thread *curr = thread_current();
  
  printf("%s: exit(%d)\n",curr->name,status);
  thread_exit();
}

pid_t
usercall_exec(const char *file)
{
  tid_t child_tid = process_execute(file);

  return child_tid;
}

int
usercall_wait(pid_t pid UNUSED)
{
  struct thread *child;
  return pid;
}

bool
usercall_remove(const char *file UNUSED)
{
}

int
usercall_open(const char *file UNUSED)
{
}

int
usercall_filesize(int fd UNUSED)
{
}

// TODO
int
usercall_read(int fd, void *buffer, unsigned size)
{
  int retval = -1;
  /* For Standard Input */
  if(fd == 0)
    {
      int i;
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
      return size;
    }
  /* For general file write */
  else
    {
    }
}

void
usercall_seek(int fd UNUSED, unsigned position UNUSED)
{
}

unsigned
usercall_tell(int fd UNUSED)
{
}

void
usercall_close(int fd UNUSED)
{
}

int
usercall_pibo(int n)
{
  int i;
  int a = 0, b = 1, c = 0;
  printf("in pibo(not fibo) n is %d\n",n);
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
