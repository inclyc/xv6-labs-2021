#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "date.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;

  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;


  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}


#ifdef LAB_PGTBL
int
sys_pgaccess(void)
{
  // lab pgtbl: your code here.
  uint64 va_start;
  int n_pages; // how many pages should we check
  uint64 abits; // pointer to result buffer;
  uint32 result = 0;
  argaddr(0, &va_start);
  argint(1, &n_pages);
  argaddr(2, &abits);
  if(va_start >= MAXVA || va_start < 0) return -1;
  if(n_pages > 32 || n_pages < 0) return -1;
  if(abits >= MAXVA || abits < 0) return -1;

  pagetable_t pgtbl = myproc()->pagetable;
  for(int i = 0;i < n_pages; i++){
    pte_t *pte = walk(pgtbl, va_start + PGSIZE * i, 0);
    if(!pte) return -1;
    if(*pte & PTE_A){
      result |= (1 << i);
      *pte ^= PTE_A; // set access bit to zero
    }
  }
  copyout(pgtbl, (uint64)abits, (char *)&result, sizeof(int));
  return 0;
}
#endif

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
