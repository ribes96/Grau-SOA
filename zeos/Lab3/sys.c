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
int global_pid = 1000;

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

int ret_from_fork() {
    return 0;
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

    node = list_first(&freequeue);
    list_del(node);

    //Inherit system data
    struct task_struct * p = list_head_to_task_struct(node);
    copy_data((void *)current(), (void *)p, KERNEL_STACK_SIZE);

    //Initialize field dir_pages_baseAddr
    allocate_DIR(p);



    //Kernel and code pages
//     copy_data(current()->dir_pages_baseAddr, p->dir_pages_baseAddr, NUM_PAG_KERNEL + NUM_PAG_CODE);
    copy_data((void *)get_PT(current()), (void *)get_PT(p), data_pos);

    //Data Pages //Inherit user data
    for (i = 0; i < NUM_PAG_DATA; ++i) {
        set_ss_pag(get_PT(current()), free_pos + i, frames[i]);
        set_ss_pag(get_PT(p), data_pos + i, frames[i]);
    }
    copy_data((void *)(data_pos << 12),
                (void *)(free_pos << 12),
                NUM_PAG_DATA*PAGE_SIZE);

    for (i = 0; i < NUM_PAG_DATA; ++i) {
        del_ss_pag(get_PT(current()), free_pos + i);
    }

            //Flush del TLB
    set_cr3(current()->dir_pages_baseAddr);



    //Assign a new PID
    p->PID = global_pid;
    PID = global_pid;
    ++global_pid;

    //Initialize the fields of the task struct non common
    p->state = ST_READY;


    int * child_esp;
    __asm__ __volatile__(
        "movl %%esp, %0"
        :"=g" (child_esp)
    );
    //El desfasament en el pare
    child_esp = (int *)((int)child_esp - (int)current());

    child_esp = (int *)((int)child_esp + (int)p);
    //h)
    __asm__ __volatile__(
        "movl $ret_from_fork, (%0)\n\t"
        "movl $0, -1(%0)\n\t"
        :
        :"g" (child_esp)
            );

    //A causa dels dos pushl
    --child_esp;
    --child_esp;

    p->kernel_esp = (unsigned long)child_esp;

    p->statistics.user_ticks = 0;
    p->statistics.system_ticks = 0;
    p->statistics.blocked_ticks = 0;
    p->statistics.ready_ticks = 0;
    p->statistics.elapsed_total_ticks = get_ticks();
    p->statistics.total_trans = 0;

    //No ha executat cap ==> els té tots
    p->statistics.remaining_ticks = p->quantum_ticks;


    //Insert to the readyqueue
    list_add_tail(&(p->list), &readyqueue);

    //return pid
    return PID;
}

/*
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
*/

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


int sys_get_stats(int pid, struct stats * st) {
  if (pid<0) return -EINVAL; //Invalid argument
  struct task_struct * p = current();
  if (p->PID == pid) {
    copy_data((void *)&(p->statistics), (void *)st, sizeof(p->statistics));
    return 0;
  }
  struct list_head * i;
  list_for_each(i, &readyqueue) {
    struct task_struct * p = list_head_to_task_struct(i);
    if (p->PID == pid) {
      copy_data((void *)&(p->statistics), (void *)st, sizeof(p->statistics));
      return 0;
    }
  }
  //No s'ha trobat
  //TODO Quan tinguem blockejats s'haurà de mirar també a la llista de blocked
  //idem amb zombies
  return -ESRCH; //No such process
}

void user_to_system(void)
{
  current()->statistics.user_ticks +=get_ticks() - current()->statistics.elapsed_total_ticks;
  current()->statistics.elapsed_total_ticks = get_ticks();
}

void system_to_user(void)
{
  current()->statistics.system_ticks +=get_ticks() - current()->statistics.elapsed_total_ticks;
  current()->statistics.elapsed_total_ticks = get_ticks();
}
