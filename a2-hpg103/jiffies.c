#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/jiffies.h>

#define procfs_name "jiffies"

struct proc_dir_entry *proc_file;

int procfile_read(char *buffer, char **buffer_location, off_t offset, int buffer_length, int *eof, void *data) {
	
	int ret;
	
	printk(KERN_INFO "procfile_read (/proc/%s) called\n", procfs_name);

        //disallow the use of the 'offset' parameter..
	if (offset >0)
	{
		ret = 0;
	}
	else
	{
		//TODO: write the value of the jiffies global into the specified
		//buffer
		ret = sprintf(buffer, "jiffies=%lu\n", jiffies);
	}

	return ret;
}

int init_module() {

	// Using create_proc_entry() to create a /proc entry...
	
	proc_file = create_proc_entry(procfs_name, 0644, NULL);

	if(proc_file == NULL) {
		remove_proc_entry(procfs_name, &proc_root);
		printk(KERN_ALERT "ERROR: could not initialize /proc/%s\n",
				procfs_name);
		return -ENOMEM;
	}

	//TODO:
	//utilize proc_dir_entry pointer to setup the interface/filelength;
	//utilize the THIS_MODULE object, which is generated automatically

	proc_file->read_proc = procfile_read;
	proc_file->owner = THIS_MODULE;
	proc_file->mode = S_IFREG | S_IRUGO;
	proc_file->uid = 0;
	proc_file->gid = 0;
	proc_file->size = 100;

	printk(KERN_INFO "/proc/%s created\n", procfs_name);
	return 0;
}

void cleanup_module() {
	remove_proc_entry(procfs_name, &proc_root);
	//here I simply print out to user that the module was removed
	printk(KERN_INFO "/proc/%s removed\n", procfs_name);
}
