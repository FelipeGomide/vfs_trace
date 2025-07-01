#ifndef MY_PROBE
#define MY_PROBE

#include <linux/module.h>
#include <linux/printk.h>
#include <linux/fprobe.h>

int trace_read(struct fprobe *fp, unsigned long entry_ip, unsigned long ret_ip, struct pt_regs *regs, void *entry_data);
int trace_write(struct fprobe *fp, unsigned long entry_ip, unsigned long ret_ip, struct pt_regs *regs, void *entry_data);
int trace_filp_close(struct fprobe *fp, unsigned long entry_ip, unsigned long ret_ip, struct pt_regs *regs, void *entry_data);

void trace_open(struct fprobe *fp, unsigned long entry_ip, unsigned long ret_ip, struct pt_regs *regs, void *entry_data);

extern struct fprobe read_probe;
extern struct fprobe write_probe;
extern struct fprobe open_probe;
extern struct fprobe close_probe;

#endif
