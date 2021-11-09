#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <linux/syscalls.h>
#include <proctimes.h>
#include <asm/current.h>
#include <linux/sched.h>
#include <asm/string.h>

#define ONE_BILLION 1000000000

extern unsigned long volatile jiffies;
void populate_proc_time_struct(struct proc_time *proc_time, struct task_struct *process);

asmlinkage int sys_getproctimes(pid_t pid, struct proctimes *time) {
	struct task_struct * process;
	printk(KERN_INFO "getproctimes designed by Eric Huddleston is invoked!\n");

	// get the specified process
	if(pid == 0) {
		process = get_current();
	} else {
		process = find_task_by_pid(pid);
		// if specified process does not exist, return EINVAL
		if(process == NULL) {
			printk(KERN_INFO "getproctimes exiting with error\n");
			return EINVAL;
		}
	}

//	if(!access_ok(VERIFY_WRITE, time, sizeof(time))) {
//		printk(KERN_INFO "getproctimes invalid pointer passed in\n");
//		return EINVAL;
//	}

	struct proctimes proctimes;
	struct proc_time current_proc_time, parent_proc_time, oldchild_proc_time;

	// Find oldest child process
	struct list_head *child_list = process->parent->children.next; // list of children
      //struct list_head *first_child_list; // first child on that list
	struct task_struct *oldest_child = container_of(child_list, struct task_struct, sibling);
//	if(!first_child_list = NULL) {
		// Get the first child from the list
//		struct task_struct *first_child = list_entry(first_child_list, struct task_struct, sibling);
//		printk(KERN_INFO "getproctimes: child detected for pid %d\n", process->pid);
//		printk(KERN_INFO "getproctimes: first child list @ %p\n", first_child_list);
//		printk(KERN_INFO "getproctimes: first child @ %p\n", first_child);
//		printk(KERN_INFO "getproctimes: first child pid %d\n", first_child->pid);
//		oldest_child = first_child;
//	}

	populate_proc_time_struct(&current_proc_time, process);
	populate_proc_time_struct(&parent_proc_time, process->parent);
	populate_proc_time_struct(&oldchild_proc_time, oldest_child);
	//if(&oldest_child > 1) {
	//	populate_proc_time_struct(&oldchild_proc_time, oldest_child);
	//} else {
	//	memset(&oldchild_proc_time, 0, sizeof(struct proc_time));
	//}
	
	// Populate the fields of our local structure
	proctimes.proc = current_proc_time;
	proctimes.parent_proc = parent_proc_time;
	proctimes.oldest_child_proc = oldchild_proc_time;

	copy_to_user(time, &proctimes, sizeof(struct proctimes));

	printk(KERN_INFO "getproctimes found pid of %d\n", process->pid);
	return(0);	// return 0 on success
}

void populate_proc_time_struct(struct proc_time *proc_time, struct task_struct *process) {
	proc_time->pid = process->pid;
	

	struct timespec ts_start_time = process->start_time;
	proc_time->start_time = ts_start_time.tv_sec * ONE_BILLION + ts_start_time.tv_nsec;
	proc_time->real_time = jiffies_to_usecs(jiffies) - (proc_time->start_time); 
	proc_time->user_time = get_current()->utime; 
	proc_time->sys_time = get_current()->stime;

	// Copy the name field over from the task struct to our local structure
	strcpy((char *)&proc_time->name, (char *)&process->comm);
}
