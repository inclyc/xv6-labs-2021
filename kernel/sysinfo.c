#include "sysinfo.h"
#include "types.h"
#include "defs.h"
#include "proc.h"

uint64 sys_sysinfo(void)
{
  uint64 va_info;
  if(argaddr(0, &va_info)){
    return -1;
  }
  struct sysinfo info;
  struct proc *p = myproc();
  // TODO: fill info.freemem and info.nproc here
  copyout(p->pagetable, va_info, &info, sizeof(info));
}
