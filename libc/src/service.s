[BITS 32]

%include "syscalls.s"

[extern lastError]

SYSC_RET_2ARGS_ERR regService,SYSCALL_REG
SYSC_RET_1ARGS_ERR unregService,SYSCALL_UNREG
SYSC_RET_3ARGS_ERR getClient,SYSCALL_GETCLIENT
