#include "types.h"
#include "user.h"

void test1();
void test2();
void test3();
void test4();
void test5();
void test6();

int main(void){
	
	test1();
	test2();
	test3();
	test4();
	test5();
	wait();
	test6();
	
	exit();
	return 0;
}



//Test1: simple test  - check return values. 
void test1()
{
	int id;
	int result;
	printf(1,"\n*****************************\nTest 1 =  ");
	id=gs_create("pg1",200);
	if(id < 0){	
		printf(1,"FAIL1\n");return;}
	result=gs_open("pg1");
	result=gs_close(result);
	if(result < 0){	
		printf(1,"FAIL2\n");return;}
	result = gs_close(id);
	if(result < 0){	
		printf(1,"FAIL3\n");return;}
	printf(1,"SUCCESS\n");
}

//Test2: simple test  - open without create, and close without create. 
void test2()
{
	int result;
	int id;
	printf(1,"\n*****************************\nTest 2 =  ");
	id=gs_create("pg2",200);
	if(id < 0){	
		printf(1,"1FAIL1\n");return;}
	result=gs_open("pg0");
	if(result >=0){	
		printf(1,"F2AIL\n");return;}
	result=gs_close(id+5);
	if(result >=0){	
		printf(1,"3FAIL\n");return;}
	result=gs_open("pg2");
	result=gs_close(result);
	if(result <0){	
		printf(1,"4FAIL\n %d",result);return;}
	result=gs_close(id);
	if(result <0){	
		printf(1,"4FAIL\n %d",result);return;}
	printf(1,"SUCCESS\n");
}

//Test3:  create and open page, mapping , write and read and unmapping. and close.
void test3()
{
	int result;
	int id,mid;
	unsigned int ptr=0;
	printf(1,"\n*****************************\nTest 3 =  ");
	id=gs_create("pg3",200);
	if(id < 0){	
		printf(1,"FAIL1\n");return;}
	result=gs_open("pg3");
	mid=map(result,&ptr);
	if(mid <0){	
		printf(1,"FAIL3\n");return;}
	((unsigned *)ptr)[0]='a';
	if(((unsigned *)ptr)[0] !='a')
	{	
		printf(1,"FAIL4\n");return;}
	unmap(mid);
	gs_close(result);
	gs_close(id);
	printf(1,"SUCCESS\n");
}

//Test4:  make more than one mapping to the same section and check the updating one will update the other
void test4()
{
int result,result1,result2;
	int id,mid1,mid2,mid3;
	unsigned int ptr1,ptr2,ptr3;
	printf(1,"\n*****************************\nTest 4 =  ");
	id=gs_create("pg4",200);
	if(id < 0){	
		printf(1,"FAIL1\n");return;}
	result=gs_open("pg4");
	result1=gs_open("pg4");
	result2=gs_open("pg4");
	if(result == id){	
		printf(1,"FAIL\n");return;}
	mid1=map(result,&ptr1);
	mid2=map(result1,&ptr2);
	mid3=map(result2,&ptr3);
	if(mid1 <0){	
		printf(1,"FAIL2\n");return;}
	((unsigned *)ptr1)[0]='a';
	if(((unsigned *)ptr1)[0] !='a'){	
	printf(1,"FAIL3\n");return;}
	if(((unsigned *)ptr2)[0] !='a' || ((unsigned *)ptr3)[0] !='a'){	
	printf(1,"FAIL4\n");return;}
	unmap(mid1);
	unmap(mid2);
	unmap(mid3);
	gs_close(result);
	gs_close(result1);
	gs_close(result2);
	gs_close(id);
	printf(1,"SUCCESS\n");

}
//Test5:  two process - write and read from the same area.
void test5()
{

	int result;
	int id,mid;
	int status;
	unsigned int ptr=0;
	printf(1,"\n*****************************\nTest 5 =  ");
	id=gs_create("pg5",200);
	if(id < 0){	
		printf(1,"FAIL1\n");return;}
	result=gs_open("pg5");
	if(result == id){	
		printf(1,"FAIL2\n");return;}
	
	status = fork();
	if(status==0)
	{
		mid=map(result,&ptr);
		if(mid <0){	
		printf(1,"1FAIL3\n");return;}
		sleep(1);
		if(((unsigned *)ptr)[0] !='a'){
		printf(1,"2FAIL4\n");return;}
		unmap(mid);
		exit();
		return;
	}
	else{
		mid=map(result,&ptr);
		if(mid <0){	
		printf(1,"1FAIL5\n");return;}
		((unsigned *)ptr)[0]='a';
	}
	while(wait()!=-1);
	gs_close(result);
	gs_close(id);
	printf(1,"SUCCESS\n");
}
//Test6:  two process - both join the same group at the same time and the first leave before the wait of the second
void test6()
{
	int result;
	int id,mid;
	unsigned int ptr=0;
	printf(1,"\n*****************************\nTest 6 =  ");
	id=gs_create("pg6",200);
	if(id < 0){	
		printf(1,"FAIL1\n");return;}
	result=gs_open("pg6");
	mid=map(result,&ptr);
	if(mid <0){	
		printf(1,"FAIL3\n");return;}
	((unsigned *)ptr)[0]='a';
	if(((unsigned *)ptr)[0] !='a')
	{	
		printf(1,"FAIL4\n");return;}
	unmap(mid);
	
	if(((unsigned *)ptr)[0] =='a')
	{	
		printf(1,"FAIL5\n");return;}
	gs_close(result);
	gs_close(id);
	printf(1,"SUCCESS\n");
}
