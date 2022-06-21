#include "types.h"
#include "param.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "sysinfo.h"
#include "defs.h"

uint64 sys_sysinfo(void)
{
  uint64 va_info;
  if(argaddr(0, &va_info)){
    return -1;
  }
  struct sysinfo info;
  struct proc *p = myproc();
  info.freemem = kfreepages_count() * PGSIZE;
  info.nproc = nproc();
  if(copyout(p->pagetable, va_info, (void*)&info, sizeof(info))){
    return -1;
  }
  return 0;
}
