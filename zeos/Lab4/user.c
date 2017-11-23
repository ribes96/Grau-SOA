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
        for (j = 0; j <= i; ++j) {
            cont2 += j;
        }
        cont += cont2;
    }
    return cont;
}

int feinaIO(int n) {
    return read(0,0,n);
}


//Escriu les estadístiques en 5 línies
void write_stats_big(struct stats * t) {
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


//Escriu les estadístiques en una sola linia
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
    
    char * desc_user = "   User: ";
    char * desc_system = "   Sys: ";
    char * desc_blocked = "   Block: ";
    char * desc_ready = "   Ready: ";
    char * desc_total = "   Total: ";
    
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

//Escriu per pantalla n
void custom_write_int(int n) {
    char nC[10];
    itoa(n, nC);
    write(1, nC, strlen(nC));
}

int burstCPU() {
    
    int n = 3000;
    
    
     int pid1, pid2, pid3;
     pid1 = fork();
     if (!pid1) {
         //codi del fill1
         int a = feinaCPU(n);
         char * st = "Aquest el el P1\n";
         
        write(1,st, strlen(st));
        custom_write_int(a);
        struct stats t;
        get_stats(getpid(), &t);
        write_stats(&t);
        exit();
     }
     else {
        pid2 = fork();
        if (!pid2) {
            //codi del fill1
            int a = feinaCPU(n);
            char * st = "\nAquest el el P2\n";
            
            write(1,st, strlen(st));
            custom_write_int(a);
            struct stats t;
            get_stats(getpid(), &t);
            write_stats(&t);
            exit();
        }
        else {
        pid3 = fork();
            if (!pid3) {
                //codi del fill1
                int a = feinaCPU(n);
                char * st = "\nAquest el el P3\n";
                
                write(1,st, strlen(st));
                custom_write_int(a);
                struct stats t;
                get_stats(getpid(), &t);
                write_stats(&t);
                exit();
            }
            else {
                //Només el pare
                read(0,0,10);
            }

        }
     }

     
    return 0;
}

int burstIO() {
    
    int n = 50;
    
    
     int pid1, pid2, pid3;
     pid1 = fork();
     if (!pid1) {
         //codi del fill1
         int a = feinaIO(n);
         char * st = "Aquest el el P1\n";
         
        write(1,st, strlen(st));
        custom_write_int(a);
        struct stats t;
        get_stats(getpid(), &t);
        write_stats(&t);
        exit();
     }
     else {
        pid2 = fork();
        if (!pid2) {
            //codi del fill2
            int a = feinaIO(n);
            char * st = "\nAquest el el P2\n";
            
            write(1,st, strlen(st));
            custom_write_int(a);
            struct stats t;
            get_stats(getpid(), &t);
            write_stats(&t);
            exit();
        }
        else {
        pid3 = fork();
            if (!pid3) {
                //codi del fill3
                int a = feinaIO(n);
                char * st = "\nAquest el el P3\n";
                
                write(1,st, strlen(st));
                custom_write_int(a);
                struct stats t;
                get_stats(getpid(), &t);
                write_stats(&t);
                exit();
            }
            else {
            //Només ho fa el pare
            read(0,0,500);
            read(0,0,500);
//          read(0,0,10);
            
            }
        }
     }
     
    return 0;
}

int burstMix() {
    
    int n = 1000;
    
    
     int pid1, pid2, pid3;
     pid1 = fork();
     if (!pid1) {
         //codi del fill1
         int a = feinaCPU(n);
         char * st = "Aquest el el P1\n";
         
        write(1,st, strlen(st));
        custom_write_int(a);
        struct stats t;
        get_stats(getpid(), &t);
        write_stats(&t);
        exit();
     }
     else {
        pid2 = fork();
        if (!pid2) {
            //codi del fill2
            int a = feinaIO(n);
            char * st = "\nAquest el el P2\n";
            
            write(1,st, strlen(st));
            custom_write_int(a);
            struct stats t;
            get_stats(getpid(), &t);
            write_stats(&t);
            exit();
        }
        else {
        pid3 = fork();
            if (!pid3) {
                //codi del fill3
                int a = feinaCPU(n/2);
                int b = feinaIO(n/2);
                char * st = "\nAquest el el P3\n";
                
                write(1,st, strlen(st));
                custom_write_int(a);
                struct stats t;
                get_stats(getpid(), &t);
                write_stats(&t);
                exit();
            }
            else {
                //Només el pare
                read(0,0,10);
                read(0,0,10);
                read(0,0,10);
            }

        }
     }

     
    return 0;
}





int __attribute__ ((__section__(".text.main")))
  main(void)
{
    /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
     /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */

     
     //0: Round Robin
     //1: FIFO
    set_sched_policy(1);
    
    
    
//      burstCPU();
//      burstIO();
     burstMix();
     char * st = "\nEl process idle:\n";
     write(1,st, strlen(st));
     struct stats t;
     get_stats(0, &t);
     write_stats(&t);
     
  while(1) { }
}
