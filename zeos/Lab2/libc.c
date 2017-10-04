/*
 * libc.c 
 */

#include <libc.h>

#include <types.h>

int errno;

void itoa(int a, char *b)
{
  int i, i1;
  char c;
  
  if (a==0) { b[0]='0'; b[1]=0; return ;}
  
  i=0;
  while (a>0)
  {
    b[i]=(a%10)+'0';
    a=a/10;
    i++;
  }
  
  for (i1=0; i1<i/2; i1++)
  {
    c=b[i1];
    b[i1]=b[i-i1-1];
    b[i-i1-1]=c;
  }
  b[i]=0;
}

int strlen(char *a)
{
  int i;
  
  i=0;
  
  while (a[i]!=0) i++;
  
  return i;
}


//CUSTOM 29/9/2017
int write(int fd, char *buffer, int size) {
    // eax, ecx i edx es poden fer servir sense problemes, perque son caller-saved
    //ebx no ho és, i per tant l'hem de salvar
    int retorno = 0;    //No té per què tenir valor
    __asm__ __volatile__(
        "pushl %%ebx;"            //Salvar ebx
        "movl 8(%%ebp), %%ebx;"
        "movl 12(%%ebp), %%ecx;"
        "movl 16(%%ebp), %%edx;"
        "movl $4, %%eax;"
        "INT $0x80;"
        "popl %%ebx;"             //Tornar el valor de ebx
        "movl %%eax, %0;"
        :"=r"(retorno)
        :
        :"%ecx", "%edx", "%eax"
            );
    if (retorno < 0) {
        errno = -retorno;
        return -1;
    }
    return retorno;  //És el bo
}


void perror() {    
    char * missatge = "Syscall failed: ";
    //CONSULTAR si és adequat cridar a write des d'aquí sapiguent que es carrega errno
    char err[10];
    itoa(errno, err);
    write(1, missatge, strlen(missatge));
    write(1, err, strlen(err));
}

//CUSTOM 4/10/2017
int gettime() {
    __asm__ __volatile__(
        "movl $10, %eax;"
        "INT $0x80"     //No cal fer més perque el resultat ja és a eax
            );
    return;
}

