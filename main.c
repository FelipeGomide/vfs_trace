#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/fprobe.h>
//#include <linux/ftrace.h>
//#include <linux/ptrace.h>

static int trace_read(struct fprobe *fp, unsigned long entry_ip, unsigned long ret_ip, struct pt_regs *regs, void *entry_data){
    //pr_info("[vfs_trace] A read operation has been detected!");
  
    unsigned long first = regs->di;
    unsigned long second = regs->si;
    unsigned long third = regs->dx;

    struct file *file = (struct file *)regs->di;
    char path_buf[256];
    char *tmp;

    tmp = d_path(&file->f_path, path_buf, sizeof(path_buf));
	

    if ((strcmp(tmp, "/dev/kmsg") == 0) || (strcmp(tmp, "/proc/kmsg") == 0)){
	    return 0;
    }

    pr_info("[vfs_trace] READ: %lu, %s, %lu", first, tmp, third);
    //pr_info("[vfs_trace] Read operation size %llu", third);

    return 0;
}

struct fprobe myprobe = {
    .entry_handler = trace_read,
};

static int create_probes(void){
    int error = register_fprobe(&myprobe, "vfs_read", NULL);

    if (error != 0){
        pr_err("[vfs_trace] Error: Could not create probes! Code: %d", error);
        return 1;
    }
    return 0;
}

static int unregister_probes(void){
    unregister_fprobe(&myprobe);

    return 0;
}

static int __init trace_setup(void){
    pr_info("[vfs_trace] Module starting!");

    const int error = create_probes();

    if (error){
        return 1;
    }

    return 0;
}

static void __exit trace_close(void){
    const int misses = unregister_probes();
    pr_info("[vfs_trace] Exiting module. Identified %d misses. \n", misses);
}

module_init(trace_setup); 
module_exit(trace_close); 

MODULE_LICENSE("GPL");
