#include "vfs_probes.h"

int trace_vfs_read(struct fprobe *fp, unsigned long entry_ip, unsigned long ret_ip, struct pt_regs *regs, void *entry_data){
    unsigned long first = regs->di;
    unsigned long third = regs->dx;

    struct file *file = (struct file *)first;
    char path_buf[256];
    char *tmp;

    tmp = d_path(&file->f_path, path_buf, sizeof(path_buf));

    if ((strcmp(tmp, "/dev/kmsg") == 0) || (strcmp(tmp, "/proc/kmsg") == 0)){
        return 0;
    }

    pr_info("[vfs_trace] READ: %s, %lu", tmp, third);
    return 0;
}

int trace_vfs_write(struct fprobe *fp, unsigned long entry_ip, unsigned long ret_ip, struct pt_regs *regs, void *entry_data){ 
    unsigned long first = regs->di;
    unsigned long third = regs->dx;

    struct file *file = (struct file *)first;
    char path_buf[256];
    char *tmp;

    tmp = d_path(&file->f_path, path_buf, sizeof(path_buf));

    if ((strcmp(tmp, "/dev/kmsg") == 0) || (strcmp(tmp, "/proc/kmsg") == 0) || (strncmp(tmp, "/var/log/", 9) == 0)){
        return 0;
    }

    pr_info("[vfs_trace] WRITE: %s, %lu", tmp, third);
    return 0;
};

int trace_vfs_open(struct fprobe *fp, unsigned long entry_ip, unsigned long ret_ip, struct pt_regs *regs, void *entry_data){ return 0; };

int trace_vfs_close(struct fprobe *fp, unsigned long entry_ip, unsigned long ret_ip, struct pt_regs *regs, void *entry_data){ return 0; };

struct fprobe read_probe = {
    .entry_handler = trace_vfs_read,
};

struct fprobe write_probe = {
    .entry_handler = trace_vfs_write,
};