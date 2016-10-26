#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include <console.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
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
  int syscallnum = *((int*)f->esp);
  // XXX : How can I handle system call? make base structure.
  // use switch-case?
  // Interrupt vector no is known as 0x30
  if(!f->esp || !is_user_vaddr(f->esp))
    usercall_exit(-1);
  printf("system calling! syscall num : %d\n",syscallnum);
  hex_dump(f->esp,f->esp,212,true);
  switch(syscallnum)
    {
      /* implement later */
      /* Project 2 */
    case SYS_HALT:
      usercall_halt();
      break;
    case SYS_EXIT:
      usercall_exit(*(int*)(f->esp+4));
      break;
    case SYS_EXEC:
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
      break;
    case SYS_WRITE:
      f->eax = usercall_write(*(int*)(f->esp+4),
			      *(const void**)(f->esp+8),
			      *(unsigned*)(f->esp+12));
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
  thread_exit ();
}

void
usercall_halt(void)
{
  shutdown_power_off();
}

void
usercall_exit(int status UNUSED)
{
  printf("exit(%d)\n",status);
}

pid_t
usercall_exec(const char *file UNUSED)
{
}

int
usercall_wait(pid_t pid UNUSED)
{
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

int
usercall_read(int fd, void *buffer, unsigned size)
{
  if(fd == 0)
    {
    }
}

int
usercall_write(int fd, const void *buffer, unsigned size)
{
  if(fd == 1)
    {
      putbuf(buffer, size);
      return size;
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
