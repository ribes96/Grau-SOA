/*
 * sys.c - Syscalls implementation
 */
#include <devices.h>

#include <utils.h>

#include <io.h>

#include <mm.h>

#include <mm_address.h>

#include <sched.h>

#include <errno.h>

#define LECTURA 0
#define ESCRIPTURA 1

unsigned int zeos_ticks;

#define KERNEL_BUFFER_SPACE 1024

char KERNEL_BUFFER[KERNEL_BUFFER_SPACE];

int check_fd(int fd, int permissions)
{
  if (fd!=1) return -9; /*EBADF*/
  if (permissions!=ESCRIPTURA) return -13; /*EACCES*/
  return 0;
}

int sys_ni_syscall()
{
	return -38; /*ENOSYS*/
}

int sys_getpid()
{
	return current()->PID;
}

int sys_fork()
{
  int PID=-1;

  // creates the child process
  
  return PID;
}

void sys_exit()
{  
}


//Custom 2/10/2017
int sys_write(int fd, char * buffer, int size) {
    int escriptura = 1;
    int lectura = 0;
    int fd_status = check_fd(fd, escriptura);
    if ( fd_status < 0) {
        return fd_status;
    }
    if (!buffer) { //Si el buffer és NULL
        return -EFAULT;

    }
    if (size < 0) {
        return -EINVAL;
    }
    
    
    //Per si buffer no hi cap a KERNEL_BUFFER
    int s = size;
    char *b = buffer;
    int written_count = 0;
    while (s > KERNEL_BUFFER_SPACE) {
        copy_from_user(b, KERNEL_BUFFER, KERNEL_BUFFER_SPACE);
        written_count += sys_write_console(KERNEL_BUFFER, KERNEL_BUFFER_SPACE);
        b += KERNEL_BUFFER_SPACE;
        s -= KERNEL_BUFFER_SPACE;
        
    }
    if (s) { //Si encara en queda un
        copy_from_user(b, KERNEL_BUFFER, s);
        written_count += sys_write_console(KERNEL_BUFFER, s);
    }
    
    return written_count;
}
