#ifndef ___PROCTIMES___
#define ___PROCTIMES___
struct proc_time{
        pid_t pid;
	char name[16];
	unsigned long start_time;
	unsigned long real_time;
	unsigned long user_time;
	unsigned long sys_time;
};
struct proctimes{
	struct proc_time proc;
	struct proc_time parent_proc;
	struct proc_time oldest_child_proc;
};
#endif
