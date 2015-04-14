/*
 * virt_kmem - /dev/virt_mem device support
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * Copyright (C) 2013 Tom Zanussi <tom.zanussi@linux.intel.com>
 */

#include <linux/miscdevice.h>
#include <linux/seq_file.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/fs.h>

#include <asm/pgtable.h>

#ifdef CONFIG_VIRT_KMEM
void __weak mem_init_print_layout(struct seq_file *m) {}
#else
void __init __weak mem_init_print_layout(struct seq_file *m) {}
#endif

/* virt kmem info */
static int virt_kmem_show(struct seq_file *m, void *v)
{
	mem_init_print_info(m, NULL);
	mem_init_print_layout(m);

	return 0;
}

static void *virt_kmem_start(struct seq_file *m, loff_t *pos)
{
	return NULL + (*pos == 0);
}

static void virt_kmem_stop(struct seq_file *m, void *v)
{
}

static void *virt_kmem_next(struct seq_file *m, void *v, loff_t *pos)
{
	++*pos;

	return NULL;
}

static const struct seq_operations virt_kmem_seq_ops = {
	.start = virt_kmem_start,
	.next  = virt_kmem_next,
	.stop  = virt_kmem_stop,
	.show  = virt_kmem_show
};

static int virt_kmem_open(struct inode *inode, struct file *file)
{
	int ret;

	ret = seq_open(file, &virt_kmem_seq_ops);

	return ret;
}

static int virt_kmem_release(struct inode *inode, struct file *file)
{
	return 0;
}

static const struct file_operations virt_kmem_fops = {
	.open		= virt_kmem_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= virt_kmem_release,
};

static struct miscdevice virt_kmem_device= {
	MISC_DYNAMIC_MINOR,
	"virt_kmem",
	&virt_kmem_fops
};

static int __init virt_kmem_init(void)
{
	int ret;

	ret = misc_register(&virt_kmem_device);
	if (ret) {
		printk(KERN_ERR "virt_kmem: misc_register failed, ret = %d\n", ret);
		return ret;
	}

	return 0;
}

static void __exit virt_kmem_exit(void)
{
	misc_deregister(&virt_kmem_device);
}

module_init(virt_kmem_init);
module_exit(virt_kmem_exit);

MODULE_AUTHOR("Tom Zanussi <tom.zanussi@linux.intel.com>");
MODULE_LICENSE("GPL");
