#include "probes.h"

#include <linux/module.h>
#include <linux/printk.h>
#include <linux/fprobe.h>
#include <linux/sched.h>

int trace_read(struct fprobe *fp, unsigned long entry_ip, unsigned long ret_ip, struct pt_regs *regs, void *entry_data){
    unsigned long first = regs->di;
    unsigned long third = regs->dx;

    struct file *file = (struct file *)first;
    char path_buf[256];
    char *tmp;

    tmp = d_path(&file->f_path, path_buf, sizeof(path_buf));

    if ((strcmp(tmp, "/dev/kmsg") == 0) || (strcmp(tmp, "/proc/kmsg") == 0)){
        return 0;
    }

    pr_info("[vfs_trace] READ %s %lu %s\n", tmp, third, current->comm);
    return 0;
}

int trace_write(struct fprobe *fp, unsigned long entry_ip, unsigned long ret_ip, struct pt_regs *regs, void *entry_data){ 
    unsigned long first = regs->di;
    unsigned long third = regs->dx;

    struct file *file = (struct file *)first;
    char path_buf[256];
    char *tmp;

    tmp = d_path(&file->f_path, path_buf, sizeof(path_buf));

    if ((strcmp(tmp, "/dev/kmsg") == 0) || (strcmp(tmp, "/proc/kmsg") == 0) || (strncmp(tmp, "/var/log/", 9) == 0)){
        return 0;
    }

    pr_info("[vfs_trace] WRITE %s %lu %s\n", tmp, third, current->comm);
    return 0;
};

void trace_open(struct fprobe *fp, unsigned long entry_ip, unsigned long ret_ip, struct pt_regs *regs, void *entry_data){ 
    unsigned long ret = regs->ax;
    struct file* file = (struct file *)ret;

    if (IS_ERR(file)) {
        long err = PTR_ERR(file);
        pr_info("[vfs_trace] do_filp_open failed: error = %ld (PID: %d, comm: %s)\n",
                err, current->pid, current->comm);
        return;
    }

    if (!file || !file->f_path.dentry)
        return;

    char path_buf[256];
    char *path = d_path(&file->f_path, path_buf, sizeof(path_buf));

    pr_info("[vfs_trace] OPEN %s %s", path, current->comm);
};

int trace_vfs_close(struct fprobe *fp, unsigned long entry_ip, unsigned long ret_ip, struct pt_regs *regs, void *entry_data){ 
    unsigned long first = regs->di;
    struct file *filp = (struct file *)first;

    // if (!filp || !filp->f_path.dentry)
    //     return 0;

    char path_buf[256];
    char *path = d_path(&filp->f_path, path_buf, sizeof(path_buf));

    pr_info("[vfs_trace] CLOSE %s %s\n", path, current->comm);
    return 0;
};

struct fprobe read_probe = {
    .entry_handler = trace_read,
};

struct fprobe write_probe = {
    .entry_handler = trace_write,
};

struct fprobe open_probe = {
    .exit_handler = trace_open
};

struct fprobe close_probe = {
    .entry_handler = trace_vfs_close,
};

