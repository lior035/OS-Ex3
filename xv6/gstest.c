#include "types.h"
#include "user.h"

#define PGSIZE 4096 //page in integer

int main(void){
	//int err, id, size = 3*PGSIZE;
	//uint addr[4];
	//char* mapedAddr, *mapedAddr2;

	//err = gscreate(size);
	//printf(1, "I am the parent. error code for creation -> %d\n" ,err);
	////opening second time - second mapping
	//err = gsopen();
	//printf(1, "I am the parent. error code for opening after creation -> %d\n" ,err);
	////err = gs_compare();
	////-1 if there was an error, 0 if no error but doesn't have at least one global section, 1 if found and ok.
	////printf(1, "I am the parent. error code for comparing -> %d (should be 1 if ok)\n" ,err);


	//id = map((char*)addr);
	//printf(1, "I am the parent. maped addr: %x, id: %d\n", *addr, id);
	//mapedAddr = (char*)*addr;
	//*mapedAddr = 100;
	//printf(1, "I am the parent. set section to %d\n", *mapedAddr);

	//int idt = map((char*)addr);
	//printf(1, "I am the parent. second maped addr: %x,id: %d\n", *addr, idt);
	//mapedAddr2 = (char*)*addr;
	//*mapedAddr2 += 20;
	//printf(1, "I am the parent. set section to %d\n", *mapedAddr);
	//printf(1, "I am the parent. set section to %d\n", *mapedAddr2);
	////now we are supposed to get 120 from the child.

	//unmap(id);
	//unmap(idt);

	//err = gsclose();
	//printf(1, "I am the parent. error code for closing -> %d\n" ,err);



	//int err, id, size = 3*PGSIZE;
	//char*addr;
	//int ChildPid = fork();
	//if (ChildPid < 0)
	//	printf(1, "Fork failed %d\n", ChildPid);
	//else if (ChildPid > 0) {
	//	err = gscreate(size);
	//	printf(1, "I am the parent. error code for creation -> %d\n" ,err);
	//	id = map(&addr);
	//	*addr = 100;
	//	unmap(id);
	//	wait();
	//}
	//else{
	//	sleep(1);
	//	err = gsopen();
	//	printf(1, "I am the parent. error code for creation -> %d\n" ,err);
	//	id = map(&addr);
	//	unmap(id);
	//	exit();
	//}


	int err, id, size = 3*PGSIZE;
	char* addr = 0;
	char* mapedAddr, *mapedAddr2;
	
	//printf(1, "new addr: %x\n", &addr);
	//for(i=0; i<2; i++){
		int ChildPid = fork();
		if (ChildPid < 0)
			printf(1, "Fork failed %d\n", ChildPid);
		else if (ChildPid > 0) {
			printf(1, "I am the parent. My pid is %d, child id is %d\n", getpid(), ChildPid);
			

			err = gs_create(size);
			printf(1, "I am the parent. error code for creation -> %d\n" ,err);
			//opening second time - second mapping
			err = gs_open();
			printf(1, "I am the parent. error code for opening after creation -> %d\n" ,err);
			//err = gs_compare();
			//-1 if there was an error, 0 if no error but doesn't have at least one global section, 1 if found and ok.
			//printf(1, "I am the parent. error code for comparing -> %d (should be 1 if ok)\n" ,err);
			
			
			id = map((char*)&addr);
			printf(1, "I am the parent. maped addr: %x, id: %d\n", *addr, id);
			mapedAddr = addr;
			*mapedAddr = 100;
			printf(1, "I am the parent. set section to %d\n", *mapedAddr);
			
			int idt = map((char*)&addr);
			printf(1, "I am the parent. second maped addr: %x,id: %d\n", *addr, idt);
			mapedAddr2 = addr;
			*mapedAddr2 += 20;
			printf(1, "I am the parent. set section to %d\n", *mapedAddr);
			printf(1, "I am the parent. set section to %d\n", *mapedAddr2);
			//now we are supposed to get 120 from the child.
			
			unmap(id);
			unmap(idt);

			err = gs_close();
			printf(1, "I am the parent. error code for closing -> %d\n" ,err);



			wait();	
		} else { //ChildPid = 0
			sleep(2);
			printf(1, "I am the child. My pid is %d\n", getpid());
			//err = gs_create(size);
			err = gs_open();
			printf(1, "I am the child. error code for opening after creation -> %d\n" ,err);
			
			id = map((char*)&addr);
			//printf(1, "gs addr: %x,id: %d\n", addr, id);
			mapedAddr = addr;
			printf(1, "I am the child, what the parent wrote: %d\n", *mapedAddr);
			
			unmap(id);
			//err = gs_compare();
			//printf(1, "I am the child. error code for comparing -> %d (should be 0 if ok - not opened)\n" ,err);

			err = gs_close();
			printf(1, "I am the child. error code for opening after creation -> %d\n" ,err);

			exit();
		}
	//}
	
	printf(1, "I am %d, exiting\n", getpid());
	exit(); //every process will leave barrier here.
}