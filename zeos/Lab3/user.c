#include <libc.h>


char buff[24];

int pid;

long inner(long n) {
    int i;
    long suma;
    suma = 0;
    for (i=0;i<n; i++) suma = suma + i;
    return suma;
}

long outer(long n) {
    int i;
    long acum;
    acum = 0;
    for (i=0;i<n; i++) acum = acum + inner(i);
    return acum;
}

int add(int par1,int par2) {
    __asm__(
        "mov 0x8(%ebp),%eax;"
        "add 0xc(%ebp),%eax;"
    );
    return;
}

int __attribute__ ((__section__(".text.main")))
  main(void)
{
    
    /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
     /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */
     
//      long count, acum;
//      count = 75;
//      acum = 0;
//      acum = outer(count);

  while(1) {}
}
