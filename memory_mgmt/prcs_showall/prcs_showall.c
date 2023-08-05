#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>

#include <linux/version.h>
#if LINUX_VERSION_CODE > KERNEL_VERSION(4, 10, 0)
#include <linux/sched/signal.h>	/* for_each_xxx(), ... */
#endif
#include <linux/fs.h>		/* no_llseek() */
#include <linux/slab.h>
#include <linux/uaccess.h>	/* copy_to_user() */
#include <linux/kallsyms.h>

#define OURMODNAME	"prcs_showall"

MODULE_AUTHOR("Opensource");
MODULE_DESCRIPTION("Show all processes by iterating over the task list");
MODULE_LICENSE("Dual MIT/GPL");
MODULE_VERSION("0.1");

static int show_prcs_in_tasklist(void)
{
	struct task_struct *p;
#define MAXLEN   128
	char tmp[MAXLEN];
	int numread = 0, n = 0, total = 0;
	char hdr[] = "     Name       |  TGID  |   PID  |  RUID |  EUID";

	pr_info("%s\n", &hdr[0]);
	/*
	 * The for_each_process() is a macro that iterates over the task structures in memory.
	 * The task structs are global of course; this implies we should hold a lock of some
	 * sort while working on them (even if only reading!). So, doing
	 *  read_lock(&tasklist_lock);
	 *  [...]
	 *  read_unlock(&tasklist_lock);
	 * BUT, this lock - tasklist_lock - isn't exported and thus unavailable to modules.
	 * So, using an RCU read lock is indicated here (this has been added later to this code).
	 */
	rcu_read_lock();
	for_each_process(p) {
		memset(tmp, 0, 128);
		n = snprintf(tmp, 128, "%-16s|%8d|%8d|%7u|%7u\n", p->comm, p->tgid, p->pid,
			     /* (old way to disp credentials): p->uid, p->euid -or-
			      *	current_uid().val, current_euid().val
				  * better way using kernel helper __kuid_val():
			      */
			     __kuid_val(p->cred->uid), __kuid_val(p->cred->euid)
		    );
		numread += n;
		pr_info("%s", tmp);
		//pr_debug("n=%d numread=%d tmp=%s\n", n, numread, tmp);

		cond_resched();
		total++;
	}			// for_each_process()
	rcu_read_unlock();

	return total;
}

static int __init prcs_showall_init(void)
{
	int total;

	pr_info("%s: inserted\n", OURMODNAME);
	total = show_prcs_in_tasklist();
	pr_info("%s: total # of processes on system: %d\n", OURMODNAME, total);

	return 0;		/* success */
}

static void __exit prcs_showall_exit(void)
{
	pr_info("%s: removed\n", OURMODNAME);
}

module_init(prcs_showall_init);
module_exit(prcs_showall_exit);
