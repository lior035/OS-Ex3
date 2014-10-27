#include "types.h"
#include "x86.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"


int
sys_gs_create(void)
{
	int size;
	char* name;
	if(argptr(0, &name, 4))
	{
		cprintf("1!!!!!!!!!\n");
		return -1;
	}
	
	if (argint(1, &size) < 0)
	{
		cprintf("2!!!!!!!!!\n");
		return -1;
	}	
	return gs_create(name, size);
}

int
sys_gs_close(void)
{
	int id;
	if (argint(0, &id) < 0)
		return -1;
	return gs_close(id);
}

int
sys_gs_open(void)
{
	char* name;
	if(argptr(0, &name, 4))
	{
		return -1;
	}
	return gs_open(name);
}

int
sys_map(void)
{
	char* addr;
	int id;
	if(argint (0, &id)<0)
		return -1;
	if(argptr(1, (void*)&addr, sizeof(char*))<0)
		return -1;
	return map (id, addr);
}

int
sys_unmap(void)
{
	int id;
	if(argint (0, &id)<0)
		return -1;
	return unmap(id);
}

int
sys_fork(void)
{
	return fork();
}

int
sys_exit(void)
{
	exit();
	return 0;  // not reached
}

int
sys_wait(void)
{
	return wait();
}

int
sys_kill(void)
{
	int pid;

	if (argint(0, &pid) < 0)
		return -1;
	return kill(pid);
}

int
sys_getpid(void)
{
	return proc->pid;
}

int
sys_sbrk(void)
{
	int addr;
	int n, i, j;
	int last_active_area;

	if (argint(0, &n) < 0 || n < 0)
		return -1;

	last_active_area = 0;
	for (i = 0; i < NOAREAS; i++) {
		if (proc->area[i].flag != AREA_NOT_ACTIVE)
			last_active_area = i;

		if (!(proc->area[i].flag == AREA_ACTIVE || proc->area[i].flag == AREA_NOT_ACTIVE))
			continue;

		for (j = i + 1; j < NOAREAS; j++)	// find next active area
		{
			if (proc->area[j].flag != AREA_NOT_ACTIVE)
				break;
		}

		if (j == NOAREAS)
			break;

		if ((proc->area[j].start - (proc->area[last_active_area].start + proc->area[last_active_area].len)) >= n)
			break;
	}

	if (i == NOAREAS)
		panic("NO FREE AREA");

	if (proc->area[i].flag == AREA_NOT_ACTIVE) {
		proc->area[i].start = proc->area[last_active_area].start + proc->area[last_active_area].len;
		proc->area[i].len = 0;
		proc->area[i].flag = AREA_ACTIVE;
	}

	addr = (int)proc->area[i].start + proc->area[i].len;
	//cprintf("selected area: %d, addr: %d\n", i, addr);

	if (growproc(n, i) < 0)
		return -1;
	return addr;
}

int
sys_sleep(void)
{
	int n;
	uint ticks0;

	if (argint(0, &n) < 0)
		return -1;
	acquire(&tickslock);
	ticks0 = ticks;
	while (ticks - ticks0 < n){
		if (proc->killed){
			release(&tickslock);
			return -1;
		}
		sleep(&ticks, &tickslock);
	}
	release(&tickslock);
	return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
	uint xticks;

	acquire(&tickslock);
	xticks = ticks;
	release(&tickslock);
	return xticks;
}
