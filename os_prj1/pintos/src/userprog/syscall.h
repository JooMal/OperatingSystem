#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

//#include "threads/thread.h"
#include "userprog/process.h"
//#include "lib/user/syscall.h"
#include "threads/interrupt.h"

/////////////////////////NEW////////////////////////
int sum_of_four_int(const char* a, const char* b, const char* c, const char* d);
int fibonacci(const char* n);
///////////////////////////////////////////////////
void syscall_init (void);
int read(int fd, void* buffer, unsigned size);
int write(int fd, const void* buffer, unsigned size);
int exit(int status);
void halt(void);
tid_t exec(const char *cmd_line);
int wait(tid_t pid);
//bool create(const char *file, unsigned intial_size);
//bool remove(const char *file);
//int open(const char *file);
//int filesize(int fd);
//int read(int fd, void *buffer, unsigned size);
//void seek(int fd, unsigned position);
//unsigned tell(int fd);
//void close(int fd);
#endif /* userprog/syscall.h */
