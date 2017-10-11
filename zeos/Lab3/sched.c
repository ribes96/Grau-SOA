/*
 * sched.c - initializes struct for task 0 anda task 1
 */

#include <sched.h>
#include <mm.h>
#include <io.h>

/**
 * Container for the Task array and 2 additional pages (the first and the last one)
 * to protect against out of bound accesses.
 */
union task_union protected_tasks[NR_TASKS+2]
  __attribute__((__section__(".data.task")));

union task_union *task = &protected_tasks[1]; /* == union task_union task[NR_TASKS] */


//Custom 11/10/2017
//Ho hem activat
#if 1
struct task_struct *list_head_to_task_struct(struct list_head *l)
{
  return list_entry( l, struct task_struct, list);
}
#endif

extern struct list_head blocked;


/* get_DIR - Returns the Page Directory address for task 't' */
page_table_entry * get_DIR (struct task_struct *t) 
{
	return t->dir_pages_baseAddr;
}

/* get_PT - Returns the Page Table address for task 't' */
page_table_entry * get_PT (struct task_struct *t) 
{
	return (page_table_entry *)(((unsigned int)(t->dir_pages_baseAddr->bits.pbase_addr))<<12);
}


int allocate_DIR(struct task_struct *t) 
{
	int pos;

	pos = ((int)t-(int)task)/sizeof(union task_union);

	t->dir_pages_baseAddr = (page_table_entry*) &dir_pages[pos]; 

	return 1;
}

void cpu_idle(void)
{
	__asm__ __volatile__("sti": : :"memory");

	while(1)
	{
	;
	}
}

//Custom 11/10/2017
//Conté espais disponibles per posar un procés
struct list_head freequeue;

//Conté els "processos" que estan preparats per fer servir CPU
struct list_head readyqueue;

void init_idle (void)
{
    struct list_head * node = freequeue->next;
    list_del(node);
    
    struct task_struct * PCB = list_head_to_task_struct(node);
    PCB->PID = 0;
    
    allocate_DIR(PCB);
}

void init_task1(void)
{
}






void init_sched(){
    //Inicialització de freequeue
    INIT_LIST_HEAD(&freequeue);
    int i;
    struct list_head * p = &freequeue;
    for (i = 0; i < NR_TASKS; ++i) {
        //Consultar: és veritat?
        
        //El tamany d'un element de freequeue
        int taman_task_union = sizeof(unsigned long)*KERNEL_STACK_SIZE;
        
        //La dirección del següent
        list_add((struct task_struct * )(*p + taman_task_union), p);
        p = p->next;
    }
    //////////////////////////////
    
    //Inicialització de readyqueue
    INIT_LIST_HEAD(&readyqueue);
    
    //////////////////////////////
    
}

//Feta per nosaltres
//TODO
void inner_task_switch(union task_union*t) {
        __asm__ __volatile__(
        "pushl %esi"
        :
        :
            );
}


void task_switch(union task_union*t) {
    __asm__ __volatile__(
        "pushl %esi"
        "pushl %edi"
        "pushl %ebx"
            );
    inner_task_switch(t);
    __asm__ __volatile__(
        "popl %ebx"
        "popl %edi"
        "popl %esi"
            );
}




//Not done by us
struct task_struct* current()
{
  int ret_value;
  
  __asm__ __volatile__(
  	"movl %%esp, %0"
	: "=g" (ret_value)
  );
  return (struct task_struct*)(ret_value&0xfffff000);
}

