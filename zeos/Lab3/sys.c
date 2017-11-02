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

int sys_fork() {
    int PID = -1;
    //Search physical pages
    int i;
    int data_pos = NUM_PAG_KERNEL + NUM_PAG_CODE;
    int free_pos = NUM_PAG_KERNEL + NUM_PAG_CODE + NUM_PAG_DATA;
    int error = 0;
    int pos_fail;
    int frames[NUM_PAG_DATA];
    for (i = 0; !error && i < NUM_PAG_DATA; ++i) {
        int fframe = alloc_frame();
        if (fframe == -1) {
            error = 1;
            pos_fail = i;
        }
        else {
            frames[i] = fframe;
        }
    }
    if (error) {
        for (i = 0; i < pos_fail; ++i) {
            free_frame(frames[i]);
        }
        //no és necessari alliberar el directori
//         list_add(p, &freequeue);
        return -ENOMEM;
    }
    
        
    
    //Get a free task_struct
    struct list_head *node;
    if (list_empty(&freequeue)) {
        return -ENOMEM;
    }
        //No hi ha error
    //////////////////////////////////////
    
    t = list_first(&freequeue);
    list_del(node);
    
    //Inherit system data
    struct task_struct * p = list_head_to_task_struct(node);
    copy_data(current(), p, KERNEL_STACK_SIZE);
    
    //Initialize field dir_pages_baseAddr
    allocate_DIR(p);
    

    
    //Kernel and code pages
//     copy_data(current()->dir_pages_baseAddr, p->dir_pages_baseAddr, NUM_PAG_KERNEL + NUM_PAG_CODE);
    copy_data(get_PT(current()), get_PT(p), data_pos);
    
    //Data Pages
    for (i = 0; i < NUM_PAG_DATA; ++i) {
        set_ss_pag(get_PT(current()), free_pos + i, frame[i]);
        set_ss_pag(get_PT(p), data_pos + i, frame[i]);
    }
    copy_data(data_pos << 12,
                free_pos << 12, 
                NUM_PAG_DATA*PAGE_SIZE);
    
    
    //Inherit user data
    
    //Assign a new PID
    
    //Initialize the fields of the task struct non common
    
    //h)
    
    //Insert to the readyqueue
    
    
    //return pid
    return PID;
}

int sys_fork2()
{
    int PID=-1;

    // creates the child process
    //TODO
    struct list_head *node;
    if (list_empty(&freequeue)) {
        return ENOMEM;
    }
    t = list_first(&freequeue);
    list_del(node);
    
    //Copiar del pare al fill
//     struct list_head copia = *node;
    struct task_struct * p = list_head_to_task_struct(node);
    copy_data(current(), p, KERNEL_STACK_SIZE);
    allocate_DIR(p);
    page_table_entry * parent_PT =  get_PT (current());
    page_table_entry * child_PT =  get_PT (p);
    int i;
    for (i = 0; i < TOTAL_PAGES; ++i) {
        int frame_fill = alloc_frame();
        if (frame_fill == -1) {
            return ENOMEM;
        }
        
        //modificar una entrada de la PT del fill
        //(fer que apunti al frame que ens han donat)
        child_PT[i].bits.pbase_addr = &phys_mem[frame_fill];
        
        //copiar el frame del pare al nou frame del fill
        copy_data(parent_PT[i].bits.pbase_addr,
                  child_PT[i].bits.pbase_addr,
                  PAGE_SIZE);
 
    }
    p->list = copia;

    //Pendent: modificar el pointer al directori X
    //          modificar el pid
    //          fer còpies de les págines del pare
    
    
    return PID;
    
    
    
}

void sys_exit()
{  
    struct task_struct * t = current();
    page_table_entry * PT =  get_PT (current());
    unsigned int i;
    for (i = 0; i < TOTAL_PAGES; ++i) {
        free_frame(get_frame(PT,i));
        //No cal esborrar la taula de págines: està condemnat a morir
    }
    list_add(&(t->list), &freequeue);    
    sched_next_rr();
}


//Custom 2/10/2017
int sys_write(int fd, char * buffer, int size) {
    //escriptura = 1;
    //lectura = 0;
    int fd_status = check_fd(fd, 1);
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
