#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>      
#include <asm/uaccess.h>   
#include <linux/syscalls.h>

//needed include

void cpuinfo(void)  {

	struct file *f;
    char buf[130]; //buffer for first 4 row of cpuinfo as buf
    mm_segment_t fs;
    int i;
    for(i=0;i<130;i++)
        buf[i] = 0;//buffering for cpu inf
 
    f = filp_open("/proc/cpuinfo", O_RDONLY, 0);
    if(f == NULL)
        printk(KERN_ALERT "filp_open error!!.\n");
    else{
		printk(KERN_INFO "\nCPU Information \n\n"); //cpu inf linebyline
        fs = get_fs();
        set_fs(get_ds());
        f->f_op->read(f, buf, 121, &f->f_pos); // buffer first 121 ch
        set_fs(fs);
        printk(KERN_INFO "%s\n",buf); // kernel print
    }
    filp_close(f,NULL); //fr close


	}


	void loadavg(void) { 
	struct file *f;
    char *puf = (char *) vmalloc(30); //parsing the loadavg as puf
	char *temp;
    mm_segment_t fs;
    int k;
   
 
    f = filp_open("/proc/loadavg", O_RDONLY, 0); //fr loadavg
    if(f == NULL)
        printk(KERN_ALERT "filp_open error!!.\n");
    else{
		printk(KERN_INFO "\nSystem Statistics \n\n");
        fs = get_fs();
        set_fs(get_ds());
        f->f_op->read(f, puf, 128, &f->f_pos);
        set_fs(fs);

		temp = strsep(&puf," ");
        printk("the number of active tasks : %s\n",temp); //first parsed piece for
		temp = strsep(&puf," ");    
		temp = strsep(&puf," ");	//passing not needed pieces
		temp = strsep(&puf," ");
        printk("the total number of processes : %s\n",temp); //fourth piece  prink
    }

    filp_close(f,NULL); //frclose


}

void uptime(void) { 
	struct file *f;
    char *puf = (char *) vmalloc(60); //same as loadavg buffer char as
	char *temp;
    mm_segment_t fs;

 
    f = filp_open("/proc/uptime", O_RDONLY, 0); //uptime 
    if(f == NULL)
        printk(KERN_ALERT "filp_open error!!.\n");
    else{
        fs = get_fs();
        set_fs(get_ds());
        f->f_op->read(f, puf, 60, &f->f_pos); // first 60 ch taken
        set_fs(fs);

		temp = strsep(&puf," "); 
        printk("system was booted since : %s\n",temp); // printing first part as booted
		temp = strsep(&puf," "); //temping next statement
		printk("system has been idle since : %s",temp); // printing second part as idle since
    }
    filp_close(f,NULL); // fr close


}

int simple_init(void) { 
	cpuinfo(); //funccalls
	loadavg();	//funccalls
	uptime();		//funccalls

}


void simple_exit(void) {
	printk(KERN_INFO "Removing Module\n"); //lkm remove

}

module_init(simple_init); 
module_exit(simple_exit); 
MODULE_LICENSE("GPL"); 
MODULE_DESCRIPTION("sys info lkm"); 
MODULE_AUTHOR("glork"); 
