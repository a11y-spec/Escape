/**
 * $Id$
 * Copyright (C) 2008 - 2009 Nils Asmussen
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef PROC_H_
#define PROC_H_

#include "common.h"
#include "intrpt.h"
#include "fpu.h"
#include "text.h"

/* max number of processes */
#define PROC_COUNT			1024
#define MAX_FD_COUNT		32
#define MAX_PROC_NAME_LEN	30

/* use an invalid pid to identify the kernel */
#define KERNEL_PID			(PROC_COUNT + 1)
/* for marking unused */
#define INVALID_PID			(PROC_COUNT + 2)

/* the events we can wait for */
#define EV_NOEVENT			0
#define EV_CLIENT			1
#define EV_RECEIVED_MSG		2
#define EV_CHILD_DIED		4
#define EV_UNLOCK			8	/* kernel-intern */

/* the process-state which will be saved for context-switching */
typedef struct {
	u32 esp;
	u32 edi;
	u32 esi;
	u32 ebp;
	u32 eflags;
} sProcSave;

/* the process states */
typedef enum {ST_UNUSED = 0,ST_RUNNING = 1,ST_READY = 2,ST_BLOCKED = 3,ST_ZOMBIE = 4} eProcState;

/* represents a process */
/* TODO move stuff for existing processes to the kernel-stack-page */
typedef struct {
	/* process state. see eProcState */
	u8 state;
	/* the events the process waits for (if waiting) */
	u8 events;
	/* the signal that the process is currently handling (if > 0) */
	tSig signal;
	/* process id (2^16 processes should be enough :)) */
	tPid pid;
	/* parent process id */
	tPid parentPid;
	/* the physical address for the page-directory of this process */
	u32 physPDirAddr;
	/* the text of this process. NULL if it has no text */
	sTextUsage *text;
	/* the number of pages per segment */
	u32 textPages;
	u32 dataPages;
	u32 stackPages;
	/* TODO just for debugging atm */
	u32 ueip;
	sProcSave save;
	/* FPU-state; initially NULL */
	sFPUState *fpuState;
	/* file descriptors: indices of the global file table */
	tFileNo fileDescs[MAX_FD_COUNT];
	/* the io-map (NULL by default) */
	u8 *ioMap;
	/* number of cpu-cycles the process has got so far; TODO: should be cpu-time later */
	u64 ucycleStart;
	u64 ucycleCount;
	u64 kcycleStart;
	u64 kcycleCount;
	u64 cycleCount;
	/* start-command */
	char command[MAX_PROC_NAME_LEN + 1];

	sSLList *threads;
} sProc;

/* the area for proc_changeSize() */
typedef enum {CHG_DATA,CHG_STACK} eChgArea;

/**
 * Initializes the process-management
 */
void proc_init(void);

/**
 * Searches for a free pid and returns it or 0 if there is no free process-slot
 *
 * @return the pid or 0
 */
tPid proc_getFreePid(void);

/**
 * @return the running process
 */
sProc *proc_getRunning(void);

/**
 * @param pid the pid of the process
 * @return the process with given pid
 */
sProc *proc_getByPid(tPid pid);

/**
 * Checks wether the process with given id exists
 *
 * @param pid the process-id
 * @return true if so
 */
bool proc_exists(tPid pid);

/**
 * Determines wether the given process has a child
 *
 * @param pid the process-id
 * @return true if it has a child
 */
bool proc_hasChild(tPid pid);

/**
 * Destroys zombies
 */
void proc_cleanup(void);

/**
 * Switches to another process
 */
void proc_switch(void);

/**
 * Switches to the process with given pid
 *
 * @param pid the process-id
 */
void proc_switchTo(tPid pid);

/**
 * Puts the given process to sleep with given wake-up-events
 *
 * @param pid the process to put to sleep
 * @param events the events on which the process should wakeup
 */
void proc_wait(tPid pid,u8 events);

/**
 * Wakes up all blocked processes that wait for the given event
 *
 * @param event the event
 */
void proc_wakeupAll(u8 event);

/**
 * Wakes up the given process with the given event. If the process is not waiting for it
 * the event is ignored
 *
 * @param pid the process to wakeup
 * @param event the event to send
 */
void proc_wakeup(tPid pid,u8 event);

/**
 * Returns the file-number for the given file-descriptor
 *
 * @param fd the file-descriptor
 * @return the file-number or < 0 if the fd is invalid
 */
tFileNo proc_fdToFile(tFD fd);

/**
 * Searches for a free file-descriptor
 *
 * @return the file-descriptor or the error-code (< 0)
 */
tFD proc_getFreeFd(void);

/**
 * Associates the given file-descriptor with the given file-number
 *
 * @param fd the file-descriptor
 * @param fileNo the file-number
 * @return 0 on success
 */
s32 proc_assocFd(tFD fd,tFileNo fileNo);

/**
 * Duplicates the given file-descriptor
 *
 * @param fd the file-descriptor
 * @return the error-code or the new file-descriptor
 */
tFD proc_dupFd(tFD fd);

/**
 * Redirects <src> to <dst>. <src> will be closed. Note that both fds have to exist!
 *
 * @param src the source-file-descriptor
 * @param dst the destination-file-descriptor
 * @return the error-code or 0 if successfull
 */
s32 proc_redirFd(tFD src,tFD dst);

/**
 * Releases the given file-descriptor (marks it unused)
 *
 * @param fd the file-descriptor
 * @return the file-number that was associated with the fd (or ERR_INVALID_FD)
 */
tFileNo proc_unassocFD(tFD fd);

/**
 * Clones the current process into the given one, saves all new threads in proc_clone() so that
 * they will start there on thread_resume(). The function returns -1 if there is
 * not enough memory.
 *
 * @param newPid the target-pid
 * @return -1 if an error occurred, 0 for parent, 1 for child
 */
s32 proc_clone(tPid newPid);

/**
 * Starts a new thread at given entry-point. Will clone the kernel-stack from the current thread
 *
 * @return < 0 if an error occurred, new tid for current thread, 0 for new thread
 */
s32 proc_startThread(u32 entryPoint);

/**
 * Destroys the current thread. If it's the only thread in the process, the complete process will
 * destroyed.
 */
void proc_destroyThread(void);

/**
 * Destroyes the given process. That means the process-slot will be marked as "unused" and the
 * paging-structure will be freed.
 *
 * @param p the process
 */
void proc_destroy(sProc *p);

/**
 * Setups the user-stack for given interrupt-stack of the current process
 *
 * @param frame the interrupt-stack-frame
 * @param argc the argument-count
 * @param args the arguments on after another, allocated on the heap; may be NULL
 * @param argsSize the total number of bytes for the arguments (just the data)
 */
void proc_setupUserStack(sIntrptStackFrame *frame,u32 argc,char *args,u32 argsSize);

/**
 * Setups the start of execution in user-mode for given interrupt-stack
 *
 * @param frame the interrupt-stack-frame
 */
void proc_setupStart(sIntrptStackFrame *frame);

/**
 * Checks wether the given segment-sizes are valid
 *
 * @param textPages the number of text-pages
 * @param dataPages the number of data-pages
 * @param stackPages the number of stack-pages
 * @return true if so
 */
bool proc_segSizesValid(u32 textPages,u32 dataPages,u32 stackPages);

/**
 * Changes the size of either the data-segment of the process or the stack-segment
 * If <change> is positive pages will be added and otherwise removed. Added pages
 * will always be cleared.
 * If there is not enough memory the function returns false.
 * Note that the size of the current process (dataPages / stackPages) will be adjusted!
 *
 * @param change the number of pages to add or remove
 * @param area the data or stack? (CHG_*)
 * @return true if successfull
 */
bool proc_changeSize(s32 change,eChgArea area);

#if DEBUGGING

/**
 * Prints all existing processes
 */
void proc_dbg_printAll(void);

/**
 * Prints the given process
 *
 * @param p the pointer to the process
 */
void proc_dbg_print(sProc *p);

/**
 * Prints the given process-state
 *
 * @param state the pointer to the state-struct
 */
void proc_dbg_printState(sProcSave *state);

#endif

#endif /*PROC_H_*/
