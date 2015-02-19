/*------------------------------------------------------------------------*/
/* Sample code of OS dependent controls for FatFs R0.08b                  */
/* (C)ChaN, 2011                                                          */
/*------------------------------------------------------------------------*/

#include <stdlib.h>		/* ANSI memory controls */
#include <malloc.h>		/* ANSI memory controls */

#include "global_includes.h"
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>


#if _FS_REENTRANT
/*-----------------------------------------------------------------------
 Create a Synchronization Object
------------------------------------------------------------------------
 This function is called in f_mount function to create a new
 synchronization object, such as semaphore and mutex. When a zero is
 returned, the f_mount function fails with FR_INT_ERR.
*/

int ff_cre_syncobj (	/* TRUE:Function succeeded, FALSE:Could not create due to any error */
	BYTE vol,			/* Corresponding logical drive being processed */
	_SYNC_t *sobj		/* Pointer to return the created sync object */
)
{
	int ret;

	*sobj = xSemaphoreCreateMutex();		/* FreeRTOS */
	ret = (*sobj != NULL);

	return ret;
}



/*------------------------------------------------------------------------*/
/* Delete a Synchronization Object                                        */
/*------------------------------------------------------------------------*/
/* This function is called in f_mount function to delete a synchronization
/  object that created with ff_cre_syncobj function. When a zero is
/  returned, the f_mount function fails with FR_INT_ERR.
*/

int ff_del_syncobj (	/* TRUE:Function succeeded, FALSE:Could not delete due to any error */
	_SYNC_t sobj		/* Sync object tied to the logical drive to be deleted */
)
{
 
  vQueueDelete (sobj);
  return 1;
}



/*------------------------------------------------------------------------*/
/* Request Grant to Access the Volume                                     */
/*------------------------------------------------------------------------*/
/* This function is called on entering file functions to lock the volume.
/  When a zero is returned, the file function fails with FR_TIMEOUT.
*/

int ff_req_grant (	/* TRUE:Got a grant to access the volume, FALSE:Could not get a grant */
	_SYNC_t sobj	/* Sync object to wait */
)
{
	int ret;

	ret = (xSemaphoreTake(sobj, _FS_TIMEOUT) == pdTRUE);	/* FreeRTOS */

	return ret;
}



/*------------------------------------------------------------------------*/
/* Release Grant to Access the Volume                                     */
/*------------------------------------------------------------------------*/
/* This function is called on leaving file functions to unlock the volume.
*/

void ff_rel_grant (
	_SYNC_t sobj	/* Sync object to be signaled */
)
{
	xSemaphoreGive(sobj);	/* FreeRTOS */
}

#endif




#if _USE_LFN == 3	/* LFN with a working buffer on the heap */
/*------------------------------------------------------------------------*/
/* Allocate a memory block                                                */
/*------------------------------------------------------------------------*/
/* If a NULL is returned, the file function fails with FR_NOT_ENOUGH_CORE.
*/

void* ff_memalloc (	/* Returns pointer to the allocated memory block */
	UINT size		/* Number of bytes to allocate */
)
{
	return malloc(size);
}


/*------------------------------------------------------------------------*/
/* Free a memory block                                                    */
/*------------------------------------------------------------------------*/

void ff_memfree(
	void* mblock	/* Pointer to the memory block to free */
)
{
	free(mblock);
}

#endif
extern int errno;

register char * stack_ptr asm("sp");

char *__env[1] = { 0 };
char **environ = __env;

void initialise_monitor_handles()
{
}

int _getpid(void)
{
        return 1;
}

int _kill(int pid, int sig)
{
        errno = EINVAL;
        return -1;
}

void _exit (int status)
{
        _kill(status, -1);
        while (1) {}                /* Make sure we hang here */
}

//int _write(int file, char *ptr, int len)
//{
//           int todo;
//
//           for (todo = 0; todo < len; todo++)
//           {
//              __io_putchar( *ptr++ );
//           }
//
//        /* Implement your write code here, this is used by puts and printf for example */
//        return len;
//}

caddr_t _sbrk(int incr)
{
        extern char end asm("end");
        static char *heap_end;
        char *prev_heap_end;

        if (heap_end == 0)
                heap_end = &end;

        prev_heap_end = heap_end;
        if (heap_end + incr > stack_ptr)
        {
//                write(1, "Heap and stack collision\n", 25);
//                abort();
                errno = ENOMEM;
                return (caddr_t) -1;
        }

        heap_end += incr;

        return (caddr_t) prev_heap_end;
}

int _close(int file)
{
        return -1;
}


int _fstat(int file, struct stat *st)
{
        st->st_mode = S_IFCHR;
        return 0;
}

int _isatty(int file)
{
        return 1;
}

int _lseek(int file, int ptr, int dir)
{
        return 0;
}

int _read(int file, char *ptr, int len)
{
        return 0;
}

int _open(char *path, int flags, ...)
{
        /* Pretend like we always fail */
        return -1;
}

int _wait(int *status)
{
        errno = ECHILD;
        return -1;
}

int _unlink(char *name)
{
        errno = ENOENT;
        return -1;
}

int _times(struct tms *buf)
{
        return -1;
}

int _stat(char *file, struct stat *st)
{
        st->st_mode = S_IFCHR;
        return 0;
}

int _link(char *old, char *new)
{
        errno = EMLINK;
        return -1;
}

int _fork(void)
{
        errno = EAGAIN;
        return -1;
}

int _execve(char *name, char **argv, char **env)
{
        errno = ENOMEM;
        return -1;
}
