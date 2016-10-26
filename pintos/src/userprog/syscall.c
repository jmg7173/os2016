#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

// XXX : Modify f's eax, esp and so on?? Reference intr_handler
// TODO : implement write, read system call first!
/* Interrupt handle function */
static void
syscall_handler (struct intr_frame *f) 
{
  int syscallnum = *((int*)f->esp);
  // XXX : How can I handle system call? make base structure.
  // use switch-case?
  // Interrupt vector no is known as 0x30
  switch(syscallnum)
    {
      /* implement later */
      /* Project 2 */
    case SYS_HALT:
    case SYS_EXIT:
    case SYS_EXEC:
    case SYS_WAIT:
    case SYS_CREATE:
    case SYS_REMOVE:
    case SYS_OPEN:
    case SYS_FILESIZE:
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
    case SYS_READ:
    case SYS_WRITE:
      break;
    }
  thread_exit ();
}
