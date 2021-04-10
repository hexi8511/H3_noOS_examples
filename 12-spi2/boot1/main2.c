#include "cache.h"
#include "gpio_reg.h"
#include "basic_reg.h"
#include "uart.h"
#include "stdio.h"
#include "string.h"
#include "led.h"
#include "cp15_v.h"
//#include "ths.h"
#include "ccu_reg.h"
#include <linux/types.h>
#include <linux/io.h>
#include "dram_sun8i_h3.h"
#include "ccu.h"
#include "asm/io.h"
#include "mmu.h"
#include "uart_reg.h"
#include "rtc.h"
#include "timer.h"
#include "intc.h"
#include "spi.h"






typedef void(*pfunc)(int zero,int type,unsigned int addr);

unsigned char *cmdline="earlyprintk=ttyS0,115200 console=ttyS0,115200 root=/dev/mmcblk0p2 rootwait";
//"console=ttyS0,115200 root=/dev/mmcblk0p2 rootwait rootfstype=ext4 cgroup_enable=memory swapaccount=1 panic=10 consoleblank=0 enforcing=0 loglevel=4";

    #define IH_NMLEN        32    /* Image Name Length        */  
      
    /* 
     * Legacy format image header, 
     * all data in network byte order (aka natural aka bigendian). 
     */  
    typedef struct image_header {  
        uint32_t    ih_magic;    /* Image Header Magic Number    */  
        uint32_t    ih_hcrc;    /* Image Header CRC Checksum    */  
        uint32_t    ih_time;    /* Image Creation Timestamp    */  
        uint32_t    ih_size;    /* Image Data Size        */  
        uint32_t    ih_load;    /* Data     Load  Address        */  
        uint32_t    ih_ep;        /* Entry Point Address        */  
        uint32_t    ih_dcrc;    /* Image Data CRC Checksum    */  
        uint8_t        ih_os;        /* Operating System        */  
        uint8_t        ih_arch;    /* CPU architecture        */  
        uint8_t        ih_type;    /* Image Type            */  
        uint8_t        ih_comp;    /* Compression Type        */  
        uint8_t        ih_name[IH_NMLEN];    /* Image Name        */  
    } image_header_t;  
/* example.c
 * example ARM Linux bootloader code
 * this example is distributed under the BSD licence
 */

/* list of possible tags */
#define ATAG_NONE       0x00000000
#define ATAG_CORE       0x54410001
#define ATAG_MEM        0x54410002
#define ATAG_VIDEOTEXT  0x54410003
#define ATAG_RAMDISK    0x54410004
#define ATAG_INITRD2    0x54420005
#define ATAG_SERIAL     0x54410006
#define ATAG_REVISION   0x54410007
#define ATAG_VIDEOLFB   0x54410008
#define ATAG_CMDLINE    0x54410009

/* structures for each atag */
struct atag_header {
        u32 size; /* length of tag in words including this header */
        u32 tag;  /* tag type */
};

struct atag_core {
        u32 flags;
        u32 pagesize;
        u32 rootdev;
};

struct atag_mem {
        u32     size;
        u32     start;
};

struct atag_videotext {
        u8              x;
        u8              y;
        u16             video_page;
        u8              video_mode;
        u8              video_cols;
        u16             video_ega_bx;
        u8              video_lines;
        u8              video_isvga;
        u16             video_points;
};

struct atag_ramdisk {
        u32 flags;
        u32 size;
        u32 start;
};

struct atag_initrd2 {
        u32 start;
        u32 size;
};

struct atag_serialnr {
        u32 low;
        u32 high;
};

struct atag_revision {
        u32 rev;
};



struct atag_videolfb {
        u16             lfb_width;
        u16             lfb_height;
        u16             lfb_depth;
        u16             lfb_linelength;
        u32             lfb_base;
        u32             lfb_size;
        u8              red_size;
        u8              red_pos;
        u8              green_size;
        u8              green_pos;
        u8              blue_size;
        u8              blue_pos;
        u8              rsvd_size;
        u8              rsvd_pos;
};

struct atag_cmdline {
        char    cmdline[1];
};

struct atag {
        struct atag_header hdr;
        union {
                struct atag_core         core;
                struct atag_mem          mem;
                struct atag_videotext    videotext;
                struct atag_ramdisk      ramdisk;
                struct atag_initrd2      initrd2;
                struct atag_serialnr     serialnr;
                struct atag_revision     revision;
                struct atag_videolfb     videolfb;
                struct atag_cmdline      cmdline;
        } u;
};



#define tag_next(t)	((struct atag *)((u32 *)(t) + (t)->hdr.size))
#define tag_size(type)	((sizeof(struct atag_header) + sizeof(struct type)) >> 2)


static struct atag *params; /* used to point at the current tag */

static void
setup_core_tag(u32 address,long pagesize)
{
    params = (struct atag *)address;         /* Initialise parameters to start at given address */

    params->hdr.tag = ATAG_CORE;            /* start with the core tag */
    params->hdr.size = tag_size(atag_core); /* size the tag */

    params->u.core.flags = 0;               /* ensure read-only */
    params->u.core.pagesize =0;// pagesize;     /* systems pagesize (4k) */
    params->u.core.rootdev = 0;             /* zero root device (typicaly overidden from commandline )*/

    params = tag_next(params);              /* move pointer to next tag */
}



static void
setup_ramdisk_tag(u32 size)
{
    params->hdr.tag = ATAG_RAMDISK;         /* Ramdisk tag */
    params->hdr.size = tag_size(atag_ramdisk);  /* size tag */

    params->u.ramdisk.flags = 0;            /* Load the ramdisk */
    params->u.ramdisk.size = size;          /* Decompressed ramdisk size */
    params->u.ramdisk.start = 0;            /* Unused */

    params = tag_next(params);              /* move pointer to next tag */
}

static void
setup_initrd2_tag(u32 start, u32  size)
{
    params->hdr.tag = ATAG_INITRD2;         /* Initrd2 tag */
    params->hdr.size = tag_size(atag_initrd2);  /* size tag */

    params->u.initrd2.start = start;        /* physical start */
    params->u.initrd2.size = size;          /* compressed ramdisk size */

    params = tag_next(params);              /* move pointer to next tag */
}

static void
setup_mem_tag(u32 start, u32 len)
{
    params->hdr.tag = ATAG_MEM;             /* Memory tag */
    params->hdr.size = tag_size(atag_mem);  /* size tag */

    params->u.mem.start = start;            /* Start of memory area (physical address) */
    params->u.mem.size = len;               /* Length of area */

    params = tag_next(params);              /* move pointer to next tag */
}

static void
setup_cmdline_tag(const char * line)
{
    int linelen = strlen(line);

    if(!linelen)
        return;                             /* do not insert a tag for an empty commandline */

    params->hdr.tag = ATAG_CMDLINE;         /* Commandline tag */
    params->hdr.size = (sizeof(struct atag_header) + linelen + 1 + 4) >> 2;

    strcpy(params->u.cmdline.cmdline,line); /* place commandline into tag */

    params = tag_next(params);              /* move pointer to next tag */
}

static void setup_serial_tag(void)
{

	struct atag_serialnr serialnr;

	//get_board_serial(&serialnr);
	params->hdr.tag = ATAG_SERIAL;
	params->hdr.size = tag_size (atag_serialnr);
	params->u.serialnr.low = 0;
	params->u.serialnr.high=0;
	params = tag_next (params);

}

static void setup_start_tag (void)
{
	params = (struct tag *)0x40000100;

	params->hdr.tag = ATAG_CORE;
	params->hdr.size = tag_size (atag_core);

	params->u.core.flags = 0;
	params->u.core.pagesize = 0;
	params->u.core.rootdev = 0;

	params = tag_next (params);
}

static void setup_memory_tags(void)
{
	int i;

	for (i = 0; i < 1; i++) {
		params->hdr.tag = ATAG_MEM;
		params->hdr.size = tag_size (atag_mem);

		params->u.mem.start = 0x40000000;
		params->u.mem.size =0x40000000;

		params = tag_next (params);
	}
}


static void setup_commandline_tag( char *commandline)
{
	char *p;

	if (!commandline)
		return;

	/* eat leading white space */
	for (p = commandline; *p == ' '; p++);

	/* skip non-existent command lines so the kernel will still
	 * use its default command line.
	 */
	if (*p == '\0')
		return;

	params->hdr.tag = ATAG_CMDLINE;
	params->hdr.size =
		(sizeof (struct atag_header) + strlen (p) + 1 + 4) >> 2;

	strcpy (params->u.cmdline.cmdline, p);

	params = tag_next (params);
}


static void setup_revision_tag(void)
{
	u32 rev = 0;

//	rev = get_board_rev();
	params->hdr.tag = ATAG_REVISION;
	params->hdr.size = tag_size (atag_revision);
	params->u.revision.rev = 1;
	params = tag_next (params);
}
static void
setup_end_tag(void)
{
    params->hdr.tag = ATAG_NONE;            /* Empty tag ends list */
    params->hdr.size = 0;                   /* zero length */
}



#define DRAM_BASE 0x40000000
#define ZIMAGE_LOAD_ADDRESS DRAM_BASE +0x2000000
#define INITRD_LOAD_ADDRESS DRAM_BASE + 0x800000

static void
setup_tags(parameters)
{
	setup_start_tag();
    //setup_core_tag(parameters, 4096);       /* standard core tag 4k pagesize */
	setup_cmdline_tag(cmdline);    /* commandline setting root device */
	//setup_commandline_tag(cmdline);
	//setup_revision_tag();
	//setup_serial_tag();
	setup_memory_tags();
    //setup_mem_tag(DRAM_BASE, 0x40000000);    /* 64Mb at 0x10000000 */
    //setup_mem_tag(DRAM_BASE + 0x8000000, 0x4000000); /* 64Mb at 0x18000000 */
    //setup_ramdisk_tag(4096);                /* create 4Mb ramdisk */ 
    //setup_initrd2_tag(INITRD_LOAD_ADDRESS, 0x100000); /* 1Mb of compressed data placed 8Mb into memory */
    
    setup_end_tag();                    /* end of tags */
}

int
start_linux(char *name,char *rdname)
{
    void (*theKernel)(int zero, int arch, u32 params);
    u32 exec_at = (u32)-1;
    u32 parm_at = (u32)-1;
    u32 machine_type;

   	exec_at = ZIMAGE_LOAD_ADDRESS;
    parm_at = DRAM_BASE + 0x100;
	
	//memset(0x40000000,0,0x8000);
    //load_image(name, exec_at);              /* copy image into RAM */

    //load_image(rdname, INITRD_LOAD_ADDRESS);/* copy initial ramdisk image into RAM */
	printf("Setup atag start ...\n");
    setup_tags(parm_at);                    /* sets up parameters */
	printf("Setup atag ... OK!\n");
  //  machine_type = get_mach_type();         /* get machine type */

    //irq_shutdown();                         /* stop irq */
	printf("Close ARM irq/mmu/dcache ...  !\n");
	close_sys_int();
	disable_icache();
	mmu_disable();	
	flush_dcache();
	disable_dcache();

    //cpu_op(CPUOP_MMUCHANGE, NULL);          /* turn MMU off */

    theKernel = (void (*)(int, int, u32))exec_at; /* set the kernel address */
	printf("## Transferring control to Linux (at address %08lx)" \
		"... 0x%lx 0x%lx \n", (ulong) theKernel,4137,parm_at);

    theKernel(0,4137,parm_at);    /* jump to kernel with register set */

    return 0;
}


 void config_pll1_para(void)
{

}


unsigned int sw_int32(unsigned int val)
{
	unsigned int ret=0;

	unsigned char b4,b3,b2,b1;
	b4=(val>>24);
	b3=((val&(0xff<<16))>>16);
	b2=((val&(0xff<<8))>>8);
	b1=((val&(0xff)));

	ret=(b4)|(b3<<8)|(b2<<16)|(b1<<24);

	printf("%x %x %x %x \n",b4,b3,b2,b1);
	return ret;
}

void cpy_kernel_to_ep(unsigned char *dest,unsigned char*src,int size)
{
	int i=0;
	unsigned char *pd,*ps;
	unsigned int sz=1024*1024*16;//sw_int32(size);

	//pd=(unsigned char *)sw_int32((unsigned int )dest);
	pd=(unsigned char *)((unsigned int )dest);
	ps=(unsigned char *)(((unsigned int )src));

	printf("d=%x  s=%x sz=%x\n",pd,ps,sz);

	for(i=0;i<sz;i++)
	{
		*pd=*ps;
		pd++;
		ps++;
	}
}

void dump_kernel(void)
{
	int j;
		unsigned char *ptmp=(unsigned char*)(0x42000000);
		for( j=0;j<512;j++)
		{
		
			printf("%02x ",*ptmp);	
			if((j+1)%16==0)
				printf("\n");
			ptmp++;
		}
		printf("\n");
}


struct sunxi_tzpc {
	u32 r0size;		/* 0x00 Size of secure RAM region */
	u32 decport0_status;	/* 0x04 Status of decode protection port 0 */
	u32 decport0_set;	/* 0x08 Set decode protection port 0 */
	u32 decport0_clear;	/* 0x0c Clear decode protection port 0 */
	/* For A80 and later SoCs */
	u32 decport1_status;	/* 0x10 Status of decode protection port 1 */
	u32 decport1_set;	/* 0x14 Set decode protection port 1 */
	u32 decport1_clear;	/* 0x18 Clear decode protection port 1 */
	u32 decport2_status;	/* 0x1c Status of decode protection port 2 */
	u32 decport2_set;	/* 0x20 Set decode protection port 2 */
	u32 decport2_clear;	/* 0x24 Clear decode protection port 2 */
};

#define SUNXI_TZPC_BASE			0x01c23400


#define SUN8I_H3_TZPC_DECPORT0_ALL  0xbe
#define SUN8I_H3_TZPC_DECPORT1_ALL  0xff
#define SUN8I_H3_TZPC_DECPORT2_ALL  0x7f

/* Configure Trust Zone Protection Controller */
void tzpc_init(void)
{
	struct sunxi_tzpc *tzpc = (struct sunxi_tzpc *)SUNXI_TZPC_BASE;


	/* Enable non-secure access to all peripherals */
	writel(SUN8I_H3_TZPC_DECPORT0_ALL, &tzpc->decport0_set);
	writel(SUN8I_H3_TZPC_DECPORT1_ALL, &tzpc->decport1_set);
	writel(SUN8I_H3_TZPC_DECPORT2_ALL, &tzpc->decport2_set);

}


/**
*This applicant function as follow:
*1.Initization DDR;
*2.Initization Serial port;
*3.Initization LED;
*4.Initization SD/MMC;
*5.Initization System CLK;
*5.Copy u-boot into DDR special address;
*6.Jump to special address and run u-boot.
*
*/

extern unsigned long sunxi_dram_init(void);
extern __s32 load_boot1_from_sdmmc( char *buf);
extern __s32 load_kernel_from_sdmmc( char *buf);
extern __s32 load_kernel2_from_sdmmc( char *buf);
extern __s32 load_fex_from_sdmmc( char *buf);

extern void disable_dcache(void);
extern void close_sys_int(void);
extern void open_sys_int(void);
extern void disable_icache(void);
extern void flush_dcache(void);


extern unsigned int __arm_start;
extern unsigned int __arm_end;

#define int_tbl_sz ((&__arm_end)-(&__arm_start))

void dump_vector_src()
{
	int i=0;
	char *pchar=(char*)&__arm_start;

	printf("__arm_start:0x%08x\n",&__arm_start);
	printf("__arm_end  :0x%08x\n",&__arm_end);
	for(i=0;i<int_tbl_sz*4;i++)
		{
		if(((i+1)%32)==0)
			{
			printf("\n");
			}
	
		printf("0x%x ",*pchar);
		pchar++;
		}
	printf("\n");
}

void cp_int_table()
{
	memset((char*)0,0,int_tbl_sz*4);
	memcpy((char *)0x0,(char *)(&__arm_start),int_tbl_sz*4);
	printf("vector table size %d\n",int_tbl_sz*4);
//	dump_vector_src();
}



void dump_gpio_reg(void)
{
	printf("PC_CFG0=0x%x\n",readl(PC_CFG0));
	printf("PC_CFG0=0x%x\n",readl(PC_CFG0));
}


extern  void rader_task();
int main(void)
{
	int count=0;
	volatile unsigned int tmp_reg=0;
	//image_header_t *kp=(image_header_t *)0x44000000;
	//pfunc uboot=(pfunc)0x44000000;//sw_int32(kp->ih_ep);
	
	//init uart 
//	uart0_init(115200);
	enable_icache();
	flush_dcache();
	enable_dcache();
	
	
	printf("Enable SMP mode for CPU0!\n");
	// Enable SMP mode for CPU0, by setting bit 6 of Auxiliary Ctl reg 
	asm volatile(
		"mrc p15, 0, r0, c1, c0, 1\n"
		"orr r0, r0, #1 << 6\n"
		"mcr p15, 0, r0, c1, c0, 1\n"
		::: "r0");

	//init blue led color
	led_init();
	printf("#######    Allwinner H3 for SPI LCD test!     #######\n");

	//H3 only security switch register config 
	writel(0x07,0x01c20000+0x2f0);

	printf("tzpc init\n");
	tzpc_init();

	printf("Init Intc!\n");
	eGon2_Int_Init();

//	printf("Init ldo crtl!\n");
//	pwr_ldo_en_init();

	//int init
	cp_int_table();
	set_vect_low_addr();
	open_sys_int();

	printf("SYS CLK Init\n");
	sys_clk_init();

	set_apb2_prediv(0);
	set_apb2_div(0);	
	set_apb2_clksrc(1);

	//ccm_clock_enable(PIO_CKID);
	
	//the prediv and div can not be 0 at the same time
	set_ahb1_prediv(0);
	set_ahb1_div(1);//AHB1 CLK=periph0/2 600MHz
	set_apb1_div(0);//APB1 CLK=AHB1 CLK/2 300MHz
	set_ahb1_clksrc(3);//periph0 AHB1=600MHz

	//open pio clk gate
	//ccm_module_enable(PIO_CKID);
	//ccm_clock_enable(PIO_CKID);
	dump_gpio_reg();

	uart0_baud_init3(115200);
	dma_init();
//	dma_main();


	//mmu_system_init(0x40000000,1024,0x44000);//section size=1MB
	//mmu_enable();
	
	//led_blue_flash();
#if 0
	printf("Load kernel!\n");
	load_kernel_from_sdmmc( 0);

	//load_kernel2_from_sdmmc( 0);
	printf("Load system_conf.bin...\n");
	load_fex_from_sdmmc( 0);


	//cpy_kernel_to_ep((unsigned char*)kp->ih_load,(unsigned char*)0x44000000,kp->ih_size);
	//cpy_kernel_to_ep((unsigned char*)0x40008000,(unsigned char*)0x44000000,kp->ih_size);

	
	//dump_kernel();
	//set linux boot parameters
//	memcpy((unsigned char*)0x40000100,cmdline,sizeof(cmdline));
	
	printf("Set kernel boot parameters!\n%s\n",cmdline);
	


	//uboot(0,4137,0x40000000+16*1024);
	start_linux(0,0);
#endif
	//Here never come
	printf("Boot2 Loop!\n");
#if 1
	printf("init Timer0\n");
	timer0_init();

#endif	
	//rtc_test();
	u32 last_tick=sys_get_tick();

	u32 last_tick2=sys_get_tick();

	spi_lcd_test();
//rader_task();

	while(1)
	{
		//printf("loop\n");

		if((sys_get_tick()-last_tick)>500)
		{
		
			last_tick=sys_get_tick();
			led_blue_flash();
		}
	


		if((sys_get_tick()-last_tick2)>100000)
		{
		
			last_tick2=sys_get_tick();
			 	rtc_show();

		}
	
	//add your codes for other function


	count++;
	}

	return 0;
}
