#include <libc.h>
#include <schedperf.h>

int read(int fd, char * buf, int nbytes);
int set_sched_policy(int p);

char buff[24];

int pid;

int feinaCPU(int n) {
    int cont = 0;
    int i, j;
    for (i = 0; i <= n; ++i) {
        j = 0;
        int cont2 = 0;
        for (j = 0; j <= n; ++j) {
            cont2 += j;
        }
        cont += cont2;
    }
    return cont;
}

int feinaIO(int n) {
    return read(0,0,n);
}


// struct stats
// {
//   unsigned long user_ticks;
//   unsigned long system_ticks;
//   unsigned long blocked_ticks;
//   unsigned long ready_ticks;
//   unsigned long elapsed_total_ticks;
//   unsigned long total_trans; /* Number of times the process has got the CPU: READY->RUN transitions */
//   unsigned long remaining_ticks;
// };

void write_stats(struct stats * t) {
    char user_ticksC[10];
    char system_ticksC[10];
    char blocked_ticksC[10];
    char ready_ticksC[10];
    char total_ticksC[10];
    
    itoa(t->user_ticks, user_ticksC);
    itoa(t->system_ticks, system_ticksC);
    itoa(t->blocked_ticks, blocked_ticksC);
    itoa(t->ready_ticks, ready_ticksC);
    itoa(t->user_ticks + 
         t->system_ticks + 
         t->blocked_ticks + 
         t->ready_ticks, 
         
         total_ticksC);
    
    char * desc_user = "\nEl temps de usuari es ";
    char * desc_system = "\nEl temps de sistema es ";
    char * desc_blocked = "\nEl temps de blocked es ";
    char * desc_ready = "\nEl temps de ready es ";
    char * desc_total = "\nEl temps de total es ";
    
    write(1, desc_user, strlen(desc_user));
    write(1, user_ticksC, strlen(user_ticksC));
    
    write(1, desc_system, strlen(desc_system));
    write(1, system_ticksC, strlen(system_ticksC));
    
    write(1, desc_blocked, strlen(desc_blocked));
    write(1, blocked_ticksC, strlen(blocked_ticksC));
    
    write(1, desc_ready, strlen(desc_ready));
    write(1, ready_ticksC, strlen(ready_ticksC));
    
    write(1, desc_total, strlen(desc_total));
    write(1, total_ticksC, strlen(total_ticksC));
    
}

void custom_write_int(int n) {
    char nC[10];
    itoa(n, nC);
    write(1, nC, strlen(nC));
}

int burstCPU() {
    
    //consultar amb 45000 és un instant, amb 48000 és bucle (casi) infinit
    int n = 48000;
    
    int a = feinaCPU(n);
    custom_write_int(a);
    struct stats t;
    get_stats(getpid(), &t);
    
    write_stats(&t);
    return 0;
}

int burstIO() {
    int n = 1000;
    feinaIO(n);
    struct stats t;
    get_stats(getpid(), &t);
    
    write_stats(&t);
    return 0;
}

int burstMix() {
    int n = 500;
    feinaCPU(n);
    feinaIO(n);
    struct stats t;
    get_stats(getpid(), &t);
    
    write_stats(&t);
    return 0;
}



int __attribute__ ((__section__(".text.main")))
  main(void)
{
    /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
     /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */

     
     //0: Round Robin
     //1: FIFO
    set_sched_policy(0);
     char * ss = "\nTot va be\n";
     write(1,ss,strlen(ss));

     
     
     burstCPU();
//      burstIO();
     
     /* TODO
      * - Que les dades estadístiques estiguin bé
      * - Veure com ho fem en diferents processos
      * - Posar les estadístiques a la taula
      * - Descripció dels workloads
      */
  while(1) { }
}
