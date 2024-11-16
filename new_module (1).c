#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#define MAX_LEN       4096
static struct proc_dir_entry *proc_entry;

static char *buffer;

ssize_t read_proc(struct file *f, char __user *user_buf, size_t count, loff_t *off)
{
    ssize_t len = 0;
    len = strlen(buffer);

    if (*off > 0) {
        return 0;   
    }

    copy_to_user(user_buf, buffer, len);

    *off += len;

    return len;  
}

ssize_t write_proc(struct file *f, const char __user *user_buf, size_t count, loff_t *off)
{

    copy_from_user(buffer, user_buf, count);

    return count;
}

static const struct file_operations proc_fops = {
    .read = read_proc,
    .write = write_proc,
};

int init_module(void)
{
    int ret = 0;
	

    buffer = vmalloc(MAX_LEN);

    proc_entry = proc_create("myproc", 0666, NULL, &proc_fops);

    return ret;
}

void cleanup_module(void)
{
    remove_proc_entry("myproc", NULL);
    vfree(buffer);
}
