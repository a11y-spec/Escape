#
# $Id$
# Copyright (C) 2008 - 2009 Nils Asmussen
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
#

#include <esc/syscalls.h>

.section .text

.extern errno

#ifdef __i386__
#include <esc/arch/i586/syscalls.s>
#endif
#ifdef __eco32__
#include <esc/arch/eco32/syscalls.s>
#endif

#ifdef __mmix__
#include <esc/arch/mmix/syscalls.s>
#else

# other
SYSC_1ARGS sysconf,SYSCALL_GETCONF
SYSC_1ARGS debugChar,SYSCALL_DEBUGCHAR
SYSC_0ARGS debug,SYSCALL_DEBUG

# driver
SYSC_3ARGS createdev,SYSCALL_CRTDEV
SYSC_1ARGS getclientid,SYSCALL_GETCLIENTID
SYSC_2ARGS getclient,SYSCALL_GETCLIENTPROC
SYSC_7ARGS getwork,SYSCALL_GETWORK

# I/O
SYSC_2ARGS open,SYSCALL_OPEN
SYSC_2ARGS pipe,SYSCALL_PIPE
SYSC_2ARGS stat,SYSCALL_STAT
SYSC_2ARGS fstat,SYSCALL_FSTAT
SYSC_2ARGS chmod,SYSCALL_CHMOD
SYSC_3ARGS chown,SYSCALL_CHOWN
SYSC_2ARGS tell,SYSCALL_TELL
SYSC_3ARGS fcntl,SYSCALL_FCNTL
SYSC_3ARGS seek,SYSCALL_SEEK
SYSC_3ARGS read,SYSCALL_READ
SYSC_3ARGS write,SYSCALL_WRITE
SYSC_4ARGS send,SYSCALL_SEND
SYSC_4ARGS receive,SYSCALL_RECEIVE
SYSC_1ARGS dup,SYSCALL_DUPFD
SYSC_2ARGS redirect,SYSCALL_REDIRFD
SYSC_2ARGS link,SYSCALL_LINK
SYSC_1ARGS unlink,SYSCALL_UNLINK
SYSC_1ARGS mkdir,SYSCALL_MKDIR
SYSC_1ARGS rmdir,SYSCALL_RMDIR
SYSC_3ARGS mount,SYSCALL_MOUNT
SYSC_1ARGS unmount,SYSCALL_UNMOUNT
SYSC_0ARGS sync,SYSCALL_SYNC
SYSC_1ARGS close,SYSCALL_CLOSE

# memory
SYSC_1ARGS _chgsize,SYSCALL_CHGSIZE
SYSC_5ARGS _regadd,SYSCALL_ADDREGION
SYSC_2ARGS regctrl,SYSCALL_SETREGPROT
SYSC_3ARGS _mapphys,SYSCALL_MAPPHYS
SYSC_3ARGS _mapmod,SYSCALL_MAPMOD
SYSC_2ARGS _shmcrt,SYSCALL_CREATESHMEM
SYSC_1ARGS _shmjoin,SYSCALL_JOINSHMEM
SYSC_1ARGS shmleave,SYSCALL_LEAVESHMEM
SYSC_1ARGS shmdel,SYSCALL_DESTROYSHMEM

# process
SYSC_0ARGS getpid,SYSCALL_PID
SYSC_1ARGS getppidof,SYSCALL_PPID
SYSC_0ARGS getuid,SYSCALL_GETUID
SYSC_1ARGS setuid,SYSCALL_SETUID
SYSC_0ARGS geteuid,SYSCALL_GETEUID
SYSC_1ARGS seteuid,SYSCALL_SETEUID
SYSC_0ARGS getgid,SYSCALL_GETGID
SYSC_1ARGS setgid,SYSCALL_SETGID
SYSC_0ARGS getegid,SYSCALL_GETEGID
SYSC_1ARGS setegid,SYSCALL_SETEGID
SYSC_2ARGS getgroups,SYSCALL_GETGROUPS
SYSC_2ARGS setgroups,SYSCALL_SETGROUPS
SYSC_2ARGS isingroup,SYSCALL_ISINGROUP
SYSC_0ARGS fork,SYSCALL_FORK
SYSC_2ARGS exec,SYSCALL_EXEC
SYSC_1ARGS waitchild,SYSCALL_WAITCHILD
SYSC_3ARGS getenvito,SYSCALL_GETENVITO
SYSC_3ARGS getenvto,SYSCALL_GETENVTO
SYSC_2ARGS setenv,SYSCALL_SETENV

# signals
SYSC_2ARGS signal,SYSCALL_SETSIGH
SYSC_3ARGS kill,SYSCALL_SENDSIG

# thread
SYSC_0ARGS gettid,SYSCALL_GETTID
SYSC_0ARGS getthreadcnt,SYSCALL_GETTHREADCNT
SYSC_2ARGS _startthread,SYSCALL_STARTTHREAD
SYSC_1ARGS _exit,SYSCALL_EXIT
SYSC_0ARGS getcycles,SYSCALL_GETCYCLES
SYSC_1ARGS alarm,SYSCALL_ALARM
SYSC_0ARGS yield,SYSCALL_YIELD
SYSC_1ARGS sleep,SYSCALL_SLEEP
SYSC_3ARGS waitmuntil,SYSCALL_WAIT
SYSC_4ARGS _waitunlock,SYSCALL_WAITUNLOCK
SYSC_2ARGS notify,SYSCALL_NOTIFY
SYSC_3ARGS _lock,SYSCALL_LOCK
SYSC_2ARGS _unlock,SYSCALL_UNLOCK
SYSC_1ARGS join,SYSCALL_JOIN
SYSC_1ARGS suspend,SYSCALL_SUSPEND
SYSC_1ARGS resume,SYSCALL_RESUME
#endif
