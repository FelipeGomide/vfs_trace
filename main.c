#include "probes.h"

#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/fprobe.h>
#include <linux/ftrace.h>
#include <linux/ptrace.h>

static int create_probes(void){
    int error = register_fprobe(&read_probe, "vfs_read", NULL);

    if (error != 0){
        pr_err("[vfs_trace] Error: Could not create READ probe! Code: %d", error);
        return 1;
    }

    error = register_fprobe(&write_probe, "vfs_write", NULL);

    if (error != 0){
        pr_err("[vfs_trace] Error: Could not create WRITE probe! Code: %d", error);
        return 1;
    }

    error = register_fprobe(&open_probe, "do_filp_open", NULL);

    if (error != 0){
        pr_err("[vfs_trace] Error: Could not create OPEN probe! Code: %d", error);
        return 1;
    }

    register_fprobe(&close_probe,"filp_close", NULL);

    return 0;
}

static int unregister_probes(void){
    int count = 0;

    count += read_probe.nmissed;
    unregister_fprobe(&read_probe);
    count += write_probe.nmissed;
    unregister_fprobe(&write_probe);
    count += open_probe.nmissed;
    unregister_fprobe(&open_probe);
    count += close_probe.nmissed;
    unregister_fprobe(&close_probe);

    return count;
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