#include "types.h"
#include "user.h"

int main(void) {
	char *sp;
	asm volatile("movl %%esp, %0" : "=r"(sp));
	printf(1, "legal 1 %d\n", *sp);
	sp = (char *)((int)sp & -4096);
	printf(1, "legal 2 %d\n", *sp);
	sp -= 1;
	
	int ret = write(0, sp, 1);
	printf(1,"ret=%d\n", ret);
	*sp = 0;
	
	exit();
	return 0;
}