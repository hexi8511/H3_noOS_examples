/*
 * (C) Copyright 2007-2011
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 * Tom Cubie <tangliang@allwinnertech.com>
 *
 * a simple tool to generate bootable image for sunxi platform.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "types.h"
#include <sys/stat.h>

#define BOOT0_MAGIC                     "eGON.BT0"
#define STAMP_VALUE                     0x5F0A6C39

#define BOOT_PUB_HEAD_VERSION           "1100"
#define BOOT0_FILE_HEAD_VERSION         "1230"
#define BOOT0_VERSION                   "1230" 
#define EGON_VERSION                    "1100"  
#define BOOT0_PRVT_HEAD_VERSION         "1230"

#ifdef SUN4I
/* boot head definition from sun4i boot code */
struct boot_file_head {
	uint32_t b_instruction;	/* one intruction jumping to real code */
	uint8_t magic[8];	/* ="eGON.BT0" or "eGON.BT1", not C-style str */
	uint32_t check_sum;	/* generated by PC */
	uint32_t length;	/* generated by PC */
	/*
	 * We use a simplified header, only filling in what is needed
	 * by the boot ROM. To be compatible with Allwinner tools we
	 * would need to implement the proper fields here instead of
	 * padding.
	 */
	uint8_t pad[12];		/* align to 32 bytes */
};
#else
#define BOOT0_MAGIC                     "eGON.BT0"

//通用的，和GPIO相关的数据结构
typedef struct _normal_gpio_cfg
{
    uint8_t      port;                       //端口号
    uint8_t      port_num;                   //端口内编号
    int8_t      mul_sel;                    //功能编号
    int8_t      pull;                       //电阻状态
    int8_t      drv_level;                  //驱动驱动能力
    int8_t      data;                       //输出电平
    uint8_t      reserved[2];                //保留位，保证对齐
} normal_gpio_cfg;

/*
typedef enum __DRAM_TYPE
{
    DRAM_TYPE_DDR   =1,
    DRAM_TYPE_DDR2  =2,
    DRAM_TYPE_DDR3  =3
}__dram_type_e;
*/

typedef struct _boot_dram_para_t
{
    uint32_t           dram_baseaddr;
    uint32_t           dram_clk;
    uint32_t           dram_type;
    uint32_t           dram_rank_num;
    uint32_t           dram_chip_density;
    uint32_t           dram_io_width;
    uint32_t		   dram_bus_width;
    uint32_t           dram_cas;
    uint32_t           dram_zq;
    uint32_t           dram_odt_en;
    uint32_t 		   dram_size;
    uint32_t           dram_tpr0;
    uint32_t           dram_tpr1;
    uint32_t           dram_tpr2;
    uint32_t           dram_tpr3;
    uint32_t           dram_tpr4;
    uint32_t           dram_tpr5;
    uint32_t 		   dram_emr1;
    uint32_t           dram_emr2;
    uint32_t           dram_emr3;

}boot_dram_para_t;

typedef struct _boot_sdram_para_t
{
uint32_t  dram_clk     ;
uint32_t dram_type      ;
uint32_t dram_zq         ;
uint32_t dram_odt_en     ;
uint32_t dram_para1      ;
uint32_t dram_para2     ;
uint32_t dram_mr0        ;
uint32_t dram_mr1        ;
uint32_t dram_mr2        ;
uint32_t dram_mr3        ;
uint32_t dram_tpr0       ;
uint32_t dram_tpr1       ;
uint32_t dram_tpr2       ;
uint32_t dram_tpr3       ;
uint32_t dram_tpr4       ;
uint32_t dram_tpr5       ;
uint32_t dram_tpr6       ;
uint32_t dram_tpr7      ;
uint32_t dram_tpr8      ;
uint32_t dram_tpr9      ;
uint32_t dram_tpr10     ;
uint32_t dram_tpr11     ;
uint32_t dram_tpr12     ;
uint32_t dram_tpr13     ;
}boot_sdram_para_t;
/******************************************************************************/
/*                              file head of Boot                             */
/******************************************************************************/
typedef struct _Boot_file_head
{
	uint32_t  jump_instruction;   // one intruction jumping to real code
	uint8_t   magic[8];           // ="eGON.BT0" or "eGON.BT1",  not C-style string.
	uint32_t  check_sum;          // generated by PC
	uint32_t  length;             // generated by PC
	uint32_t  boot1_length;       // the size of boot_file_head_t
	uint32_t  align_size;         // the align size of boot1
	uint8_t   file_head_vsn[4];   // the version of boot0_file_head_t or boot1_file_head_t
	uint8_t   Boot_vsn[4];        // Boot version
	uint8_t   eGON_vsn[4];        // eGON version
	uint8_t   platform[8];        // platform information
}boot_file_head_t;


typedef struct _boot_para_info_t
{
    uint8_t   blkmagic[16];          // "ePDK-Magic-Block", not C-style string.
    uint8_t   magic[8];
    uint8_t   eGON_vsn[4];           // eGON version
	uint8_t   Boot_vsn[4];           // Boot version
    uint32_t  reserved[20];
}boot_para_info_t;

typedef struct standard_Boot_file_head
{
	uint32_t  jump_instruction;   // one intruction jumping to real code
	uint8_t   magic[8];           // ="eGON.BT0" or "eGON.BT1",  not C-style string.
	uint32_t  check_sum;          // generated by PC
	uint32_t  length;             // generated by PC
	uint32_t  pub_head_size;      // the size of boot_file_head_t
	uint8_t   pub_head_vsn[4];    // the version of boot_file_head_t
	uint8_t   file_head_vsn[4];   // the version of boot0_file_head_t or boot1_file_head_t
	uint8_t   Boot_vsn[4];        // Boot version
	uint8_t   eGON_vsn[4];        // eGON version
	uint8_t   platform[8];        // platform information
}standard_boot_file_head_t;//0x40
/******************************************************************************/
/*                              file head of Boot0                            */
/******************************************************************************/
typedef struct _boot0_private_head_t
{
	uint32_t                       prvt_head_size;	//0x34
	char                        prvt_head_vsn[4];       // the version of boot0_private_head_t//0x38
	unsigned int                dram_para[32];          // DRAM patameters for initialising dram. Original values is arbitrary,//0xB8
	int32_t						uart_port;              // UART控制器编号 Bc
	normal_gpio_cfg             uart_ctrl[2];           // UART控制器(调试打印口)数据信息//0xcc
	int32_t                       enable_jtag;            // 1 : enable,  0 : disable//0xd0
    normal_gpio_cfg	            jtag_gpio[5];           // 保存JTAG的全部GPIO信息//40 //f8
    normal_gpio_cfg             storage_gpio[32];       // 存储设备 GPIO信息//32*8//0x1f8
    char                        storage_data[512 - sizeof(normal_gpio_cfg) * 32];  //0x2f8    // 用户保留数据信息
    //boot_nand_connect_info_t    nand_connect_info;
}boot0_private_head_t;//size 0x2C8  512+8*5+4*1+8*2+4*1+32*4+1*4+1*4


typedef struct _boot0_file_head_t
{
	standard_boot_file_head_t   boot_head;
	boot0_private_head_t  		prvt_head;
}boot_file_head;

#define   DDR3_USED

boot_file_head  BT0_head = {
	                                  {
	      /* jump_instruction */          ( 0xEA000000 | ( ( ( sizeof( boot_file_head ) + sizeof( int ) - 1 ) / sizeof( int ) - 2 ) & 0x00FFFFFF ) ),
							   		      BOOT0_MAGIC,
							   		      STAMP_VALUE,
							   		      0x2000,
							   		      sizeof( standard_boot_file_head_t ),
							   		      BOOT_PUB_HEAD_VERSION,
							   		      BOOT0_FILE_HEAD_VERSION,
							   		      BOOT0_VERSION,
							   		      EGON_VERSION,
							   		      {
							   		      	0, 0, '2','.','4','.','0',0
							   		      },
							 	      },
#ifdef  DDR3_USED
							 	      {
							 	          sizeof( boot0_private_head_t ),
							 	          BOOT0_PRVT_HEAD_VERSION,
							 	          { 0 },
							 	          0,
										  {	{ 8, 20, 2, 1, 1, 0, {0}},
										  	{ 8, 21, 2, 1, 1, 0, {0}}
										  },
							 	          0,
							 	          { 0 },
							 	          { 0 },
							 	          { 0 }
							 	      }
#else
							 	      {
							 	          sizeof( boot0_private_head_t ),
							 	          BOOT0_PRVT_HEAD_VERSION,
							 	          { 0x40000000,
							 	            1024,
							 	            180,
							 	            1,
							 	            1,
							 	            0,
							 	            (__dram_type_e)1,
							 	            16,
							 	            10,
							 	            14,
							 	            4,
							 	            3,
							 	            0,
							 	            16,
							 	            1024
							 	          },
							 	          0,
										  {
										  	{ 8, 20, 2, 1, 1, 0, {0}},
										  	{ 8, 21, 2, 1, 1, 0, {0}}
										  },
							 	          0,
							 	          { 0 },
							 	          { 0 },
							 	          { 0 }
							 	      }
#endif
							 	  };


#endif

#ifdef SUN4I
int gen_check_sum(struct boot_file_head *head_p)
#else
/* check sum functon from sun4i boot code */
int gen_check_sum(boot_file_head *head_p)
#endif
{
	uint32_t length;
	uint32_t *buf;
	uint32_t loop;
	uint32_t i;
	uint32_t sum;
#ifdef SUN4I
	length = head_p->length;
#else
	length = head_p->boot_head.length;
#endif

	if ((length & 0x3) != 0)	/* must 4-byte-aligned */
		return -1;
	buf = (uint32_t *)head_p;
#ifdef SUN4I
	head_p->check_sum = STAMP_VALUE;	/* fill stamp */
#else
	head_p->boot_head.check_sum = STAMP_VALUE;	/* fill stamp */
#endif
	
	loop = length >> 2;

	/* calculate the sum */
	for (i = 0, sum = 0; i < loop; i++)
		sum += buf[i];
#ifdef SUN4I
/* write back check sum */
	head_p->check_sum = sum;
#else
/* write back check sum */
	head_p->boot_head.check_sum = sum;
#endif
	

	return 0;
}

#define ALIGN(x, a) __ALIGN_MASK((x), (typeof(x))(a)-1)
#define __ALIGN_MASK(x, mask) (((x)+(mask))&~(mask))

#ifdef SUN4I
	#define SUN4I_SRAM_SIZE 0x7600	/* 0x7748+ is used by BROM */
	#define SRAM_LOAD_MAX_SIZE (SUN4I_SRAM_SIZE - sizeof(struct boot_file_head))
#else
	#define SUN4I_SRAM_SIZE 0x8000
	#define SRAM_LOAD_MAX_SIZE (0x54000 - sizeof(boot_file_head))
#endif


#define BLOCK_SIZE 512

struct boot_img {
#ifdef SUN4I
	struct boot_file_head header;
#else
	boot_file_head header;
#endif
	char code[SRAM_LOAD_MAX_SIZE];
	char pad[BLOCK_SIZE];
};
/*
*Here define a struct for SDRAM parameters, if your SDRAM is not different and you can change it.
**/
boot_sdram_para_t sun6i_sdram={
.dram_clk        = 432,
.dram_type       = 3,
.dram_zq         = 0xfb,
.dram_odt_en     = 0,
.dram_para1      = 0x10F40400,
.dram_para2      = 0x1101,
.dram_mr0        = 0x1A50,
.dram_mr1        = 0x4,
.dram_mr2        = 0x18,
.dram_mr3        = 0,
.dram_tpr0       = 0,
.dram_tpr1       = 0x80000800,
.dram_tpr2       = 0x39a70140,
.dram_tpr3       = 0xa092e74c,
.dram_tpr4       = 0x2948c209,
.dram_tpr5       = 0x8944422c,
.dram_tpr6       = 0x30028480,
.dram_tpr7       = 0x2a3297,
.dram_tpr8       = 0x5034fa8,
.dram_tpr9       = 0x36353d8,
.dram_tpr10      = 0,
.dram_tpr11      = 0,
.dram_tpr12      = 0,
.dram_tpr13      = 0x10000,
};

static normal_gpio_cfg uart_pin_cfg[2]={
	{
	.port=8,                    //端口号
    .port_num=20,                  //端口内编号
    .mul_sel=2,                    //功能编号
    .pull=1,                    //电阻状态
    .drv_level=0xff,                  //驱动驱动能力
    .data=0xff,                    //输出电平
    .reserved={0},              //保留位，保证对齐
	},
	{
	.port=8,                    //端口号
    .port_num=21,                  //端口内编号
    .mul_sel=2,                    //功能编号
    .pull=1,                    //电阻状态
    .drv_level=0xff,                  //驱动驱动能力
    .data=0xff,                    //输出电平
    .reserved={0},              //保留位，保证对齐
	}
};

static normal_gpio_cfg jtag_pin_cfg[5]={
	{0x08,0x09,0x03,0xFF,0xFF,0xFF,{0}}, 
{0x08,0x0a,0x03,0xFF,0xFF,0xFF,{0}}, 
{0x08,0x0b,0x03,0xFF,0xFF,0xFF,{0}}, 
{0x08,0x0c,0x03,0xFF,0xFF,0xFF,{0}}, 
{0}
};

static normal_gpio_cfg sd0_pin_cfg[6]={
{0x06,0x02,0x02,0x01,0xFF,0xFF,{0}},
{0x06,0x03,0x02,0x01,0xFF,0xFF,{0}},
{0x06,0x01,0x02,0x01,0xFF,0xFF,{0}},
{0x06,0x00,0x02,0x01,0xFF,0xFF,{0}},
{0x06,0x05,0x02,0x01,0xFF,0xFF,{0}},
{0x06,0x04,0x02,0x01,0xFF,0xFF,{0}}
};

static normal_gpio_cfg sd2_pin_cfg[6]={
{0x03,0x07,0x03,0x01,0xFF,0xFF,{0}},
{0x03,0x06,0x03,0x01,0xFF,0xFF,{0}},
{0x03,0x08,0x03,0x01,0xFF,0xFF,{0}},
{0x03,0x09,0x03,0x01,0xFF,0xFF,{0}},
{0x03,0x0a,0x03,0x01,0xFF,0xFF,{0}},
{0x03,0x0b,0x03,0x01,0xFF,0xFF,{0}}
};

static char storage_data[256]={
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,
 0x02,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,
 0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};


int main(int argc, char *argv[])
{
	int fd_in, fd_out;
	struct boot_img img;
	unsigned file_size;
	int count;

	if (argc < 2) {
		printf("\tThis program makes an input bin file to sun4i " \
		       "bootable image.\n" \
		       "\tUsage: %s input_file out_putfile\n", argv[0]);
		return EXIT_FAILURE;
	}

	fd_in = open(argv[1], O_RDONLY);
	if (fd_in < 0) {
		perror("Open input file");
		return EXIT_FAILURE;
	}

	memset(img.pad, 0, BLOCK_SIZE);

	/* get input file size */
	file_size = lseek(fd_in, 0, SEEK_END);

	if (file_size > SRAM_LOAD_MAX_SIZE) {
		fprintf(stderr, "ERROR: File too large!\n");
		return EXIT_FAILURE;
	}

	fd_out = open(argv[2], O_WRONLY | O_CREAT, 0666);
	if (fd_out < 0) {
		perror("Open output file");
		return EXIT_FAILURE;
	}

	/* read file to buffer to calculate checksum */
	lseek(fd_in, 0, SEEK_SET);
	count = read(fd_in, img.code, file_size);
	if (count != file_size) {
		perror("Reading input image");
		return EXIT_FAILURE;
	}

	/* fill the header */
#ifdef SUN4I
	img.header.b_instruction =	/* b instruction */
		0xEA000000 |	/* jump to the first instr after the header */
		((sizeof(struct boot_file_head) / sizeof(int) - 2)
		 & 0x00FFFFFF);
	memcpy(img.header.magic, BOOT0_MAGIC, 8);	/* no '0' termination */
	img.header.length =
		ALIGN(file_size + sizeof(struct boot_file_head), BLOCK_SIZE);
	gen_check_sum(&img.header);

	count = write(fd_out, &img, img.header.length);
	if (count != img.header.length) {
		perror("Writing output");
		return EXIT_FAILURE;
	}
#else
	memcpy(&img.header,&BT0_head,sizeof(boot_file_head));
	
	//copy sdram data
	memcpy(&img.header.prvt_head.dram_para,&sun6i_sdram,sizeof(boot_sdram_para_t));
	
	//copy uart pin, JTAG pin, sd0 and sd2 pin config information
	img.header.prvt_head.uart_port=0;
	img.header.prvt_head.enable_jtag=1;     

	memcpy(&img.header.prvt_head.uart_ctrl,&uart_pin_cfg,sizeof(uart_pin_cfg));  
	memcpy(&img.header.prvt_head.jtag_gpio,&jtag_pin_cfg,sizeof(jtag_pin_cfg));
	
	memcpy(&img.header.prvt_head.storage_gpio,&sd0_pin_cfg,sizeof(sd0_pin_cfg));          
	memcpy(&img.header.prvt_head.storage_gpio+8*(6+10),&sd2_pin_cfg,sizeof(sd2_pin_cfg));  
	memcpy(&img.header.prvt_head.storage_data,&storage_data,sizeof(storage_data));     
	
	
	img.header.boot_head.jump_instruction=0xEA000000 |	/* jump to the first instr after the header */
		((sizeof(boot_file_head) / sizeof(int))
		 & 0x00FFFFFF);

	img.header.boot_head.length =
		ALIGN(file_size + sizeof(boot_file_head), BLOCK_SIZE);
	gen_check_sum(&img.header);

	count = write(fd_out, &img, img.header.boot_head.length);
	if (count != img.header.boot_head.length) {
		perror("Writing output");
		return EXIT_FAILURE;
	}
#endif

	

	close(fd_in);
	close(fd_out);

	return EXIT_SUCCESS;
}