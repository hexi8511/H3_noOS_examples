#include <stdio.h>
#include <errno.h>



/**
  * @brief  Definition
  */
#undef errno
extern int errno;
#define STACK_BUFFER 65536 /* Reserved stack space in bytes. */

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
void *_sbrk (int nbytes)
{
    /* Symbol defined by linker map */
    extern int end; /* start of free memory (as symbol) */
    extern int HeapLimit; /* end of free memory */

    /* The statically held previous end of the heap, with its initialization. */
    static void *heap_ptr = (void *)&end; /* Previous end */
    static void *limit = (void *)&HeapLimit;

    if ((limit - (heap_ptr + nbytes)) > STACK_BUFFER )
    {
        void *base = heap_ptr;
        heap_ptr += nbytes;
        return base;
    }
    else
    {
        errno = ENOMEM;
        printf("=> ENOMEM\r\n");
        return (void *) - 1;
    }
} /* _sbrk () */

/*-------------------- SYSCALL DEFINITION ------------------*/
int _write (int file,
            char *buf,
            int nbytes)
{
//	uart_write(0,0,buf, nbytes);
    SerialPortWrite((uint8_t *)buf, nbytes);
    return nbytes;
} /* _write () */
int _exit(int file) { return -1; }
int _read(int file) { return -1; }
int _close(int file) { return -1; }
int _fstat(int file) { return -1; }
int _isatty(int file) { return 1; }
int _lseek(int file, int ptr, int dir) { return 0; }
