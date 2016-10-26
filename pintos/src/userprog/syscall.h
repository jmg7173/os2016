#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

#include "lib/user/syscall.h"

void syscall_init (void);

void usercall_halt(void);
void usercall_exit(int status);
pid_t usercall_exec(const char *file);
int usercall_wait(pid_t pid);
bool usercall_remove(const char *file);
int usercall_open(const char *file);
int usercall_filesize(int fd);
int usercall_read(int fd, void *buffer, unsigned size);
int usercall_write(int fd, const void *buffer, unsigned size);
void usercall_seek(int fd, unsigned position);
unsigned usercall_tell(int fd);
void usercall_close(int fd);
#endif /* userprog/syscall.h */
