#include "userprog/syscall.h"
#include <stdio.h>
#include <stdlib.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
//#include "lib/user/syscall.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
//  printf ("system call!\n");
 // printf("now esp is on %x\n",f->esp);
 // hex_dump(f->esp,f-:>esp,500,1);
  //check if f->esp is on user or kernel
 // printf("syscall number : %d\n",*(uint32_t*)(f->esp));
  switch (*(uint32_t*)(f->esp))
	{
		case SYS_HALT:
			halt();
			break;
		case SYS_EXIT:
			exit((int)*(uint32_t*)(f->esp+4));
			break;
		case SYS_EXEC:
			if(!is_user_vaddr(f->esp+4)) exit(-1);
			else f->eax=exec((char*)*(uint32_t*)(f->esp+4));
			break;
		case SYS_WAIT:
			f->eax=wait(*(uint32_t*)(f->esp+4));
			break;
		case SYS_CREATE:
			break;
		case SYS_REMOVE:
			break;
		case SYS_OPEN:
			break;
		case SYS_READ:
			f->eax=read((int)*(uint32_t*)(f->esp+4),(void*)*(uint32_t*)(f->esp+8),(unsigned)*(uint32_t*)(f->esp+12));
			break;
		case SYS_WRITE:
			f->eax=write((int)*(uint32_t*)(f->esp+4),(const void*)*(uint32_t*)(f->esp+8),(unsigned)*(uint32_t*)(f->esp+12));
			break;
		case SYS_SEEK:
			break;
		case SYS_TELL:
			break;
		case SYS_CLOSE:
			break;
		/////////////////////////NEW////////////////////////
		case SYS_SUM:
			f->eax = sum_of_four_int((const char*)*(uint32_t*)(f->esp + 4), (const char*)*(uint32_t*)(f->esp + 8), (const char*)*(uint32_t*)(f->esp + 12), (const char*)*(uint32_t*)(f->esp + 16));
			break;
		case SYS_FIBO:
			f->eax = fibonacci((const char*)*(uint32_t*)(f->esp + 4));
			break;
		///////////////////////////////////////////////////
	}
//  thread_exit ();
}

/////////////////////////NEW////////////////////////
int sum_of_four_int(const char* a, const char* b, const char* c, const char* d)
{
	return atoi(a) + atoi(b) + atoi(c) + atoi(d);
}

int fibonacci(const char* n)
{
	int a, b, num, ans, i;
	a = b = ans = 1;
	num = atoi(n);
	if(num>2) {
		for (i = 0; i < num - 2; i++) {
			ans = a + b;
			a = b;
			b = ans;
		}
	}
	return ans;
}
///////////////////////////////////////////////////


int exit(int status)
{
	//Terminates the current user program, return value is status to kernel
	//if the process's parent waits for it, this is the status that will be returnes.
	//Convectinally, a status of 0 indicates success and nonzero values indicate errors.

	thread_current()->exit_status = status;
	thread_exit();
	return status;
	
}
int write(int fd, const void *buffer, unsigned size)
{
	if(fd==1)
	{
		putbuf(buffer, size);
		return size;
	}
	return -1; //terminated in abnormal way
}
int read(int fd, void *buffer, unsigned size)
{
	int i;
	if(fd==0)
	{
		for(i=0;i<size;i++) buffer=input_getc();
		return size;
	}
	return -1; //terminated in abnormal way
}
void halt(void)
{
	shutdown_power_off();
}

tid_t exec(const char *cmd_line)
{
	/* -------parsing cmd_line------- */
	char* token;
	char tmp[30][10]={};
	char *ptr[2];
	int k=0;
	char temp_file_name[30];
	char tmp_argv[30][10]={};
	int tmp_argc=0;
	strlcpy(temp_file_name, cmd_line, strlen(cmd_line)+1);
	token = strtok_r(temp_file_name," ",&ptr[0]);

	while(token != NULL)
	{
		strlcpy(tmp[k], token, strlen(token)+1);
		token=strtok_r(tmp[k]," ",&ptr[1]);
		while(token != NULL)
		{
			strlcpy(tmp[k], token, strlen(token)+1);
			token=strtok_r(NULL," ",&ptr[1]);
		}
		token=strtok_r(NULL," ",&ptr[0]);
		if(token==NULL) break;
		strlcpy(tmp_argv[tmp_argc++], token, strlen(token)+1);
		k++;
	}
	/* -----parsing cmd_line is done----- */


	struct file* file=NULL;
	file = filesys_open(temp_file_name);
	if(file==NULL) //check file is valid or not (for exec-missing test case)
	{
		printf("load: %s: open failed\n",temp_file_name);
		return -1; //if file is unvalid, return -1
	}
	else return process_execute(cmd_line);
}

int wait(tid_t pid)
{
	return process_wait(pid); 
}
