/*
 * interrupt.c -
 */
#include <types.h>
#include <interrupt.h>
#include <segment.h>
#include <hardware.h>
#include <io.h>

#include <zeos_interrupt.h>

Gate idt[IDT_ENTRIES];
Register    idtR;

//CUSTOM 28/9/2017
//Per que el compilador s'ho tragui
void keyboard_handler();

//Alone Ribes
//2/10/2017
void clock_handler();

void system_call_handler();

//Alone Ribes
//Custom 2/10/2017

#define KERNEL_BUFFER_SPACE 1024

char KERNEL_BUFFER[KERNEL_BUFFER_SPACE]


char char_map[] =
{
  '\0','\0','1','2','3','4','5','6',
  '7','8','9','0','\'','�','\0','\0',
  'q','w','e','r','t','y','u','i',
  'o','p','`','+','\0','\0','a','s',
  'd','f','g','h','j','k','l','�',
  '\0','�','\0','�','z','x','c','v',
  'b','n','m',',','.','-','\0','*',
  '\0','\0','\0','\0','\0','\0','\0','\0',
  '\0','\0','\0','\0','\0','\0','\0','7',
  '8','9','-','4','5','6','+','1',
  '2','3','0','\0','\0','\0','<','\0',
  '\0','\0','\0','\0','\0','\0','\0','\0',
  '\0','\0'
};

void setInterruptHandler(int vector, void (*handler)(), int maxAccessibleFromPL)
{
  /***********************************************************************/
  /* THE INTERRUPTION GATE FLAGS:                          R1: pg. 5-11  */
  /* ***************************                                         */
  /* flags = x xx 0x110 000 ?????                                        */
  /*         |  |  |                                                     */
  /*         |  |   \ D = Size of gate: 1 = 32 bits; 0 = 16 bits         */
  /*         |   \ DPL = Num. higher PL from which it is accessible      */
  /*          \ P = Segment Present bit                                  */
  /***********************************************************************/
  Word flags = (Word)(maxAccessibleFromPL << 13);
  flags |= 0x8E00;    /* P = 1, D = 1, Type = 1110 (Interrupt Gate) */

  idt[vector].lowOffset       = lowWord((DWord)handler);
  idt[vector].segmentSelector = __KERNEL_CS;
  idt[vector].flags           = flags;
  idt[vector].highOffset      = highWord((DWord)handler);
}

void setTrapHandler(int vector, void (*handler)(), int maxAccessibleFromPL)
{
  /***********************************************************************/
  /* THE TRAP GATE FLAGS:                                  R1: pg. 5-11  */
  /* ********************                                                */
  /* flags = x xx 0x111 000 ?????                                        */
  /*         |  |  |                                                     */
  /*         |  |   \ D = Size of gate: 1 = 32 bits; 0 = 16 bits         */
  /*         |   \ DPL = Num. higher PL from which it is accessible      */
  /*          \ P = Segment Present bit                                  */
  /***********************************************************************/
  Word flags = (Word)(maxAccessibleFromPL << 13);

  //flags |= 0x8F00;    /* P = 1, D = 1, Type = 1111 (Trap Gate) */
  /* Changed to 0x8e00 to convert it to an 'interrupt gate' and so
     the system calls will be thread-safe. */
  flags |= 0x8E00;    /* P = 1, D = 1, Type = 1110 (Interrupt Gate) */

  idt[vector].lowOffset       = lowWord((DWord)handler);
  idt[vector].segmentSelector = __KERNEL_CS;
  idt[vector].flags           = flags;
  idt[vector].highOffset      = highWord((DWord)handler);
}


void setIdt()
{
  /* Program interrups/exception service routines */
  idtR.base  = (DWord)idt;
  idtR.limit = IDT_ENTRIES * sizeof(Gate) - 1;
  
  set_handlers();

  /* ADD INITIALIZATION CODE FOR INTERRUPT VECTOR */
  
  //CUSTOM 28/9/20147
  setInterruptHandler(33, keyboard_handler, 0);
  
  //Alone Ribes CUSTOM 2/10/2017
  setInterruptHandler(32, clock_handler, 0);
  
  setInterruptHandler(0x80, system_call_handler, 0);

  set_idt_reg(&idtR);
}

//CUSTOM 28/9/2017
void keyboard_routine() {
    //Es crida des de keyboard_handler
    Byte ch = inb(0x60); // Port
    
    Byte mode = ch & 0x80; //Agafar el bit de m�s pes
    Byte code = ch & 0x7F; //Agafar la resta de bits
    
    
    if (!mode) { //make, (key pressed)
        char char_pressed = char_map[code];
        if (char_pressed == '\0') char_pressed = 'C'; //Si no �s imprimible
        Byte a = 0;
        printc_xy(a,a,char_pressed);
        
    }
    
}

//TODO posar els codis d'error b�n posats en algun altre fitxer
//Alone Ribes
//Custom 2/10/2017
#define EFAULT 14

int sys_write(int fd, char * buffer, int size) {
    int escriptura = 1;
    int lectura = 0;
    int fd_status = check_fd(fd, escriptura);
    if ( fd_status < 0) {
        return fd_status;
    }
    if (!buffer) { //Si el buffer �s NULL
        return -EFAULT;  //NOT_TODO Retornar el codi adequat
    }
    if (size < 0) {
        return -1;  //TODO Retornar el code adequat
                    // Realment write no dona cap error amb un tamany negatiu. No escriu res
    }
    
//     char * KERNEL_SPACE = 0;      //notTODO Mirar quina direcci� �s la real
    //Alone Ribes
    copy_from_user(buffer, &KERNEL_BUFFER, size);
    int bytes_written;
    bytes_written = sys_write_console(KERNEL_SPACE, size);
    return bytes_written;
}

//Alone Ribes
//CUSTOM 2/10/2017
void clock_routine() {
    
}



