struct global_section{
	struct spinlock lock;
	unsigned int cnt;
	unsigned int size;
	pde_t* pgdir;
	char name[7];
};
struct global_sec_table {
	struct spinlock lock;
	struct global_section gs[NGlobalSections];
};