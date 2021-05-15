
#include "basic_reg.h"
#include "ccu_reg.h"
#include "stdio.h"
#include "ccu.h"
#include "delay.h"
#include "spi.h"
#include "gpio.h"
#include "led.h"
#include <linux/io.h>
#include "spi.h"
#include "uart.h"

#define SPI_LCD_NOP		0x00
#define SPI_LCD_RESET		0x01
#define SPI_LCD_READ_DISP_ID_INFO	0x04
#define SPI_LCD_READ_DISP_STATUS		0x09
#define SPI_LCD_READ_DISP_PWR_MODE	0x0a
#define SPI_LCD_READ_MADCTRL				0x0b
#define SPI_LCD_READ_DISP_PIXEL_FMT	0x0c
#define SPI_LCD_READ_DISP_IMG_FMT		0x0d
#define SPI_LCD_READ_SIGNAL_MODE		0x0e
#define SPI_LCD_READ_SELF_DIAGNOSTIC_RESULT	0x0f
#define SPI_LCD_ENTER_SLEEP_MODE					0x10
#define SPI_LCD_SLEEP_OUT					0x11
#define SPI_LCD_PARTIAL_MODE_ON					0x12
#define SPI_LCD_NORMAL_DISP_MODE_ON					0x13
#define SPI_LCD_DISP_INVERSION_OFF					0x20
#define SPI_LCD_DISP_INVERSION_ON					0x21
#define SPI_LCD_GAMMA_SET								0x26
#define SPI_LCD_DISP_OFF										0x28
#define SPI_LCD_DISP_ON										0x29
#define SPI_LCD_COLUMN_ADDR_SET					0x2a
#define SPI_LCD_PAGE_ADDR_SET					0x2b
#define SPI_LCD_MEM_WRITE							0x2c
#define SPI_LCD_COLOR_SET							0x2d
#define SPI_LCD_MEM_READ							0x2e
#define SPI_LCD_PARTIAL_AREA						0x30
#define SPI_LCD_VERTICAL_SCROLL_DEF		0x33
#define SPI_LCD_TEAR_EFFECT_LINE_OFF		0x34
#define SPI_LCD_TEAR_EFFECT_LINE_ON		0x35
#define SPI_LCD_MEM_ACCE_CTRL					0x36
#define SPI_LCD_VERTICAL_SCROLL_ADDR	0x37
#define SPI_LCD_IDLE_MODE_OFF					0x38
#define SPI_LCD_IDLE_MODE_ON					0x39
#define SPI_LCD_PIXEL_FMT_SET					0x3a
#define SPI_LCD_WRITE_MEM_CONTINUE		0x3c
#define SPI_LCD_READ_MEM_CONTINUE		0x3e
#define SPI_LCD_SET_TEAR_SCANLINE			0x44
#define SPI_LCD_GET_SCANLINE					0x45
#define SPI_LCD_WRITE_DISP_BRIGHTNESS	0x51
#define SPI_LCD_READ_DISP_BRIGHTNESS	0x52
#define SPI_LCD_WRITE_CTRL_DISP				0x53
#define SPI_LCD_READ_CTRL_DISP					0x54
#define SPI_LCD_WRITE_CONTENT_ADAPTIVE_BC	 0x55
#define SPI_LCD_READ_CONTENT_ADAPTIVE_BC	 0x56
#define SPI_LCD_WRITE_CABC_MINI_BRIGHTNESS	 0x5e
#define SPI_LCD_READ_CABC_MINI_BRIGHTNESS	 0x5f
#define SPI_LCD_READ_ID1	 0xda
#define SPI_LCD_READ_ID2	 0xdb
#define SPI_LCD_READ_ID3	 0xdc
#define SPI_LCD_RGB_INTERFACE_SIGNAL_CTRL	 0xb0
#define SPI_LCD_FRAME_CTRL_NORM	 0xb1
#define SPI_LCD_FRAME_CTRL_IDLE	 0xb2
#define SPI_LCD_FRAME_CTRL_PARTIAL	 0xb3
#define SPI_LCD_DISP_INVERSION_CTRL	 0xb4
#define SPI_LCD_BLANK_PORCH_CTRL	 0xb5
#define SPI_LCD_DISP_FUNCTION_CTRL 0xb6
#define SPI_LCD_ENTRY_MODE_SET	 0xb7
#define SPI_LCD_BACKLIGHT_CTRL1	 0xb8
#define SPI_LCD_BACKLIGHT_CTRL2	 0xb9
#define SPI_LCD_BACKLIGHT_CTRL3	 0xba
#define SPI_LCD_BACKLIGHT_CTRL4	 0xbb
#define SPI_LCD_BACKLIGHT_CTRL5	 0xbc

#define SPI_LCD_BACKLIGHT_CTRL7	 0xbe
#define SPI_LCD_BACKLIGHT_CTRL8	 0xbf

#define SPI_LCD_PWR_CTRL1	 				0xc0
#define SPI_LCD_PWR_CTRL2	 				0xc1
#define SPI_LCD_VCOM_CTRL1	 			0xc5
#define SPI_LCD_VCOM_CTRL2	 			0xc7

#define SPI_LCD_NV_MEM_WRITE	 		0xd0
#define SPI_LCD_NV_MEM_PROTECTION_KEY 				0xd1
#define SPI_LCD_NV_MEM_STATUS_READ	 				0xd2
#define SPI_LCD_READ_ID4	 				0xd3
#define SPI_LCD_POS_GAMMA_CORRECTION 				0xe0
#define SPI_LCD_NEG_GAMMA_CORRECTION 				0xe1
#define SPI_LCD_DIGITAL_GAMMA_CTRL1 				0xe2
#define SPI_LCD_DIGITAL_GAMMA_CTRL2 				0xe3
#define SPI_LCD_INTERFACE_CTRL 				0xf6

enum
{
	SPI_READ=0,
	SPI_WRITE
};

struct spi_lcd_reg_ctrl
{
	u8 addr;
	u8 read_write;
	u8 argc;
};

struct spi_lcd_reg_ctrl spi_reg_tbl[]={
	{SPI_LCD_NOP,									SPI_WRITE,0},
	{SPI_LCD_RESET,									SPI_WRITE,0},
	{SPI_LCD_READ_DISP_ID_INFO,			SPI_READ,3},
	{SPI_LCD_READ_DISP_STATUS,			SPI_READ,4},
	{SPI_LCD_READ_DISP_PWR_MODE,		SPI_READ,1},
	{SPI_LCD_READ_MADCTRL,					SPI_READ,1},
	{SPI_LCD_READ_DISP_PIXEL_FMT,		SPI_READ,1},
	{SPI_LCD_READ_DISP_IMG_FMT	,		SPI_READ,1},
	{SPI_LCD_READ_SIGNAL_MODE	,		SPI_READ,1},
	{SPI_LCD_READ_SELF_DIAGNOSTIC_RESULT	,SPI_READ,1},
	{SPI_LCD_ENTER_SLEEP_MODE					,SPI_WRITE,0},
	{SPI_LCD_SLEEP_OUT					,SPI_WRITE,0},
	{SPI_LCD_PARTIAL_MODE_ON					,SPI_WRITE,0},
	{SPI_LCD_NORMAL_DISP_MODE_ON			,SPI_WRITE,0},
	{SPI_LCD_DISP_INVERSION_OFF				,SPI_WRITE,0},
	{SPI_LCD_DISP_INVERSION_ON				,SPI_WRITE,0},
	{SPI_LCD_GAMMA_SET								,SPI_WRITE,1},
	{SPI_LCD_DISP_OFF									,SPI_WRITE,0},
	{SPI_LCD_DISP_ON									,SPI_WRITE,0},
	{SPI_LCD_COLUMN_ADDR_SET					,SPI_WRITE,4},
	{SPI_LCD_PAGE_ADDR_SET					,SPI_WRITE,4},
	//{SPI_LCD_MEM_WRITE							,SPI_WRITE,0},//special
	{SPI_LCD_COLOR_SET							,SPI_WRITE,9},
	//{SPI_LCD_MEM_READ							,SPI_READ,0},//special
	{SPI_LCD_PARTIAL_AREA						,SPI_WRITE,4},
	{SPI_LCD_VERTICAL_SCROLL_DEF		,SPI_WRITE,6},
	{SPI_LCD_TEAR_EFFECT_LINE_OFF		,SPI_WRITE,0},
	{SPI_LCD_TEAR_EFFECT_LINE_ON		,SPI_WRITE,1},
	{SPI_LCD_MEM_ACCE_CTRL					,SPI_WRITE,1},
	{SPI_LCD_VERTICAL_SCROLL_ADDR	,SPI_WRITE,2},
	{SPI_LCD_IDLE_MODE_OFF					,SPI_WRITE,0},
	{SPI_LCD_IDLE_MODE_ON					,SPI_WRITE,0},
	{SPI_LCD_PIXEL_FMT_SET					,SPI_WRITE,1},
	//{SPI_LCD_WRITE_MEM_CONTINUE	,SPI_WRITE,0},//
	//{SPI_LCD_READ_MEM_CONTINUE		,SPI_READ,0},//
	{SPI_LCD_SET_TEAR_SCANLINE			,SPI_WRITE,2},
	{SPI_LCD_GET_SCANLINE					,SPI_READ,2},
	{SPI_LCD_WRITE_DISP_BRIGHTNESS	,SPI_WRITE,1},
	{SPI_LCD_READ_DISP_BRIGHTNESS	,SPI_READ,1},
	{SPI_LCD_WRITE_CTRL_DISP				,SPI_WRITE,1},
	{SPI_LCD_READ_CTRL_DISP					,SPI_READ,1},
	{SPI_LCD_WRITE_CONTENT_ADAPTIVE_BC	 ,SPI_WRITE,1},
	{SPI_LCD_READ_CONTENT_ADAPTIVE_BC	 ,SPI_READ,1},
	{SPI_LCD_WRITE_CABC_MINI_BRIGHTNESS	 ,SPI_WRITE,1},
	{SPI_LCD_READ_CABC_MINI_BRIGHTNESS	 ,SPI_READ,1},
	{SPI_LCD_READ_ID1	 ,SPI_READ,1},
	{SPI_LCD_READ_ID2	 ,SPI_READ,1},
	{SPI_LCD_READ_ID3	 ,SPI_READ,1},
	{SPI_LCD_RGB_INTERFACE_SIGNAL_CTRL	 ,SPI_WRITE,1},
	{SPI_LCD_FRAME_CTRL_NORM	 ,SPI_WRITE,2},
	{SPI_LCD_FRAME_CTRL_IDLE	 ,SPI_WRITE,2},
	{SPI_LCD_FRAME_CTRL_PARTIAL	 ,SPI_WRITE,2},
	{SPI_LCD_DISP_INVERSION_CTRL	 ,SPI_WRITE,1},
	{SPI_LCD_BLANK_PORCH_CTRL	 ,SPI_WRITE,4},
	{SPI_LCD_DISP_FUNCTION_CTRL ,SPI_WRITE,4},
	{SPI_LCD_ENTRY_MODE_SET	 ,SPI_WRITE,1},
	{SPI_LCD_BACKLIGHT_CTRL1	 ,SPI_WRITE,2},
	{SPI_LCD_BACKLIGHT_CTRL2	 ,SPI_WRITE,2},
	{SPI_LCD_BACKLIGHT_CTRL3	 ,SPI_WRITE,2},
	{SPI_LCD_BACKLIGHT_CTRL4	 ,SPI_WRITE,2},
	{SPI_LCD_BACKLIGHT_CTRL5	 ,SPI_WRITE,2},
	{SPI_LCD_BACKLIGHT_CTRL7	 ,SPI_WRITE,1},
	{SPI_LCD_BACKLIGHT_CTRL8	 ,SPI_WRITE,1},

	{SPI_LCD_PWR_CTRL1	 				,SPI_WRITE,1},
	{SPI_LCD_PWR_CTRL2	 				,SPI_WRITE,1},
	{SPI_LCD_VCOM_CTRL1	 			,SPI_WRITE,2},
	{SPI_LCD_VCOM_CTRL2	 			,SPI_WRITE,1},

	{SPI_LCD_NV_MEM_WRITE	 		,SPI_WRITE,2},
	{SPI_LCD_NV_MEM_PROTECTION_KEY 				,SPI_WRITE,3},
	{SPI_LCD_NV_MEM_STATUS_READ	 				,SPI_READ,2},
	{SPI_LCD_READ_ID4	 				,SPI_READ,3},
	{SPI_LCD_POS_GAMMA_CORRECTION 				,SPI_WRITE,15},
	{SPI_LCD_NEG_GAMMA_CORRECTION 				,SPI_WRITE,15},
	{SPI_LCD_DIGITAL_GAMMA_CTRL1 				,SPI_WRITE,16},
	{SPI_LCD_DIGITAL_GAMMA_CTRL2 				,SPI_WRITE,16},
	{SPI_LCD_INTERFACE_CTRL 				,SPI_WRITE,3},
};


u32 spi_lcd_get_reg_tbl_size()
{
	return (sizeof(spi_reg_tbl)/sizeof(struct spi_lcd_reg_ctrl));
}

void spi_lcd_init_master(u8 spi_n)
{
	struct spi_controller spi_ctrler;
	//u32 read_id=0;

	printf("%s\n",__func__);

	spi_ctrler.work_mode=SPI_WORK_MODE_MASTER;//0 slave mode, 1 master mode
	spi_ctrler.transmit_pause_enable=1;			//0 irgnore rxfifo full, 1	stop transmit rxfifo full

	spi_ctrler.module_enable=1;						//0 disable,1 enable	
	spi_ctrler.master_sample_data_mode=1;	//0 delay,1 normal
	spi_ctrler.first_transmit_bit=MSB_FIRST;					//0 msb, 1lsb
	spi_ctrler.master_sample_data_opt=1;		//0 normal operate, 1delay
	spi_ctrler.rapid_mode=1;							//0 normal write,1 repid writel	
	spi_ctrler.dummy_burst_type=BRUST_BIT_ZERO;				//0 bit value is 0,1 bit value is 1
	spi_ctrler.discard_hash_burst=0;				//0 receive all spi brust,1 discard unused burst
	spi_ctrler.ss_level=LOW_LEVEL;									//0 set ss to low, 1 set ss to high
	spi_ctrler.ss_owner=1;								//0 spi hardware, 1 softerware
	spi_ctrler.ss_select=0;								//0 1 2 3=>ss0 ss1 ss2 ss3 
	spi_ctrler.ss_ctrl=1;									//0 ss still asserted between brusts,1 ss negate between brusts
	spi_ctrler.chip_select_pol=1;						//0 high polarity, 1 low polarity
	spi_ctrler.spi_clk_pol=0;								//0 high ,1 low
	spi_ctrler.spi_clk_phase=0;							//0 leading edge for sample data,1 leading edge for setup
	spi_ctrler.int_ctrl_flag=0;
	
	//spi fifo dma
	spi_ctrler.tx_fifo_dma_req=0;						//0 disable dma req, 1 enable dma req
	spi_ctrler.tx_fifo_empty_trig_level=32;			//256-0
	spi_ctrler.rx_fifo_dma_req=0;						//0 disable , 1 enable
	spi_ctrler.rx_fifo_dma_mode=0;					//0 normal, 1 dedicate mode
	spi_ctrler.rx_fifo_ready_req_trig_level=32;	//0-256

	//spi wait clock
	spi_ctrler.dual_wait_clk_cnt=0;				//daul mode wait clk count between datas
	spi_ctrler.signle_wait_clk_cnt=0;			//signle mode wait clk count between datas
	
	//below just for master 
	spi_ctrler.clk_div_select=1;								//0 div rate 1,1 div rate 2
	spi_ctrler.div1=1;									//spi clk=src/2^n     n=0~16
	spi_ctrler.div2=0;									//spi clk=src/(2*(n+1))  n=0~255


	spi_ctrler.recv_s_or_d=0;						//0 signle mode, 1 daul modes
	spi_ctrler.recv_dummy_brust_cnt_daul=0;//0-16
	spi_ctrler.transmit_cnt_signle=1;//0~2^24

	spi_init(spi_n,&spi_ctrler);	
}


void spi_lcd_write_reg(u8 addr,u8* buf, u32 size)
{
		int i;
		u8 send_buf[32]={0,};

		send_buf[0]=addr;

		//send cmd
		spi_cs_soft_low(0);
		p8_low();//command cs low	

		spi_one_transmit(0,&send_buf[0],1,0,0);
		//get data
		p8_high();	
		if(size!=0)
		{
		//printf("size:%d %x\n",size,*buf);
		spi_one_transmit(0,buf,size,0,0);
		}
		//delay_ms(10);
		spi_cs_soft_high(0);	
}

void spi_lcd_read_reg(u8 addr,u8* buf, u32 *size)
{
		int i;
		u8 send_buf[32]={0,};
		struct spi_lcd_reg_ctrl  *pspi_lrc=spi_reg_tbl;
#if 1
		for(i=0;i<spi_lcd_get_reg_tbl_size();i++)
		{
			if(pspi_lrc->addr==addr)
				{
				*size=pspi_lrc->argc;
		
				break;
				}
			else
				*size=0;
			pspi_lrc++;
		}

		send_buf[0]=addr;

		if(pspi_lrc->read_write==SPI_READ)
		{

		//send cmd
		spi_cs_soft_low(0);
		p8_low();//command cs low	
		//spi_one_transmit(0,send_buf,2,val,2);
	
		spi_one_transmit(0,&send_buf[0],1,0,0);

		//get data
		p8_high();
		if(*size>=3)
			{
				spi_send_wait_clk(0,1,1);
			}
		spi_one_transmit(0,0,0,buf,*size);	
		spi_cs_soft_high(0);	
		}

#endif

#if 0
	send_buf[0]=0xd3;
	*size=3;
	spi_cs_soft_low(0);
	p8_low();//command cs low	
	spi_one_transmit(0,&send_buf[0],4,buf,0);
	p8_high();
	spi_cs_soft_high(0);	
#endif

}

void spi_lcd_col_addr_set(u16 col_addr_start,u16 col_addr_end)
{
	u8 buf[4]={0,};

	buf[0]=(col_addr_start>>8);
	buf[1]=(col_addr_start>>0);
	buf[2]=(col_addr_end>>8);
	buf[3]=(col_addr_end>>0);

	spi_lcd_write_reg(SPI_LCD_COLUMN_ADDR_SET,buf,4);
}

void spi_lcd_page_addr_set(u16 addr_start,u16 addr_end)
{
	u8 buf[4]={0,};

	buf[0]=(addr_start>>8);
	buf[1]=(addr_start>>0);
	buf[2]=(addr_end>>8);
	buf[3]=(addr_end>>0);

	spi_lcd_write_reg(SPI_LCD_PAGE_ADDR_SET,buf,4);
}

void spi_lcd_write_data(u16 page_saddr,u16 page_eaddr,u16 col_saddr,u16 col_eaddr,u16* dbuf, u32 size)
{
		int i;
		
		struct spi_lcd_reg_ctrl  *pspi_lrc=spi_reg_tbl;

		spi_lcd_col_addr_set(col_saddr,col_eaddr);
		spi_lcd_page_addr_set(page_saddr,page_eaddr);
		spi_lcd_write_reg(0x2c,0,0);
	
		//send cmd
		spi_cs_soft_low(0);
		//p8_low();//command cs low	
		
		//send command
		//spi_one_transmit(0,SPI_LCD_MEM_WRITE,1,0,0);

		//send data
		p8_high();
		if(size!=0)
			{
			spi_one_transmit(0,(u8*)dbuf,size*2,0,0);
			}
			
		delay_ms(10);
		spi_cs_soft_high(0);	

		spi_lcd_write_reg(SPI_LCD_SLEEP_OUT,0,0);
}


u16 disp_data[320*240]={0,};


void lcd_init()
{
printf("%s\n",__func__);
u8 val[32]={0,};
#if 1
        val[0]=(0x39); 
         val[1]=(0x2C); 
         val[2]=(0x00); 
         val[3]=(0x34); 
         val[4]=(0x02); 

	spi_lcd_write_reg(0xCB,val,5);
        
         val[0]=(0x00); 
         val[1]=(0XC1); 
         val[2]=(0X30); 
 spi_lcd_write_reg(0xCF,val,3);


         val[0]=(0x85); 
         val[1]=(0x00); 
         val[2]=(0x78); 
 spi_lcd_write_reg(0xe8,val,3);
      
         val[0]=(0x00); 
         val[1]=(0x00); 
 spi_lcd_write_reg(0xea,val,2);
          
         val[0]=(0x64); 
         val[1]=(0x03); 
         val[2]=(0X12); 
         val[3]=(0X81); 
spi_lcd_write_reg(0xed,val,4);
       
         val[0]=(0x20); 
  spi_lcd_write_reg(0xf7,val,1);
        //Power control 
         val[0]=(0x23);   //VRH[5:0] 
 spi_lcd_write_reg(0xC0,val,1);
            //Power control 
         val[0]=(0x10);   //SAP[2:0];BT[3:0] 
 spi_lcd_write_reg(0xC1,val,1);
            //VCM control 
         val[0]=(0x3e); //¶Ô±È¶Èµ÷œÚ
         val[1]=(0x28); 
 spi_lcd_write_reg(0xC5,val,2);
       //VCM control2 
         val[0]=(0x86);  //--
 spi_lcd_write_reg(0xC7,val,1);
           // Memory Access Control 
#ifdef USE_HORIZONTAL
         val[0]=(0xE8); //C8	   //48 68ÊúÆÁ//28 E8 ºáÆÁ
#else
		 val[0]=(0x48); 
#endif
spi_lcd_write_reg(0x36,val,1);
      
         val[0]=(0x55); 
spi_lcd_write_reg(0x3a,val,1);
    
         val[0]=(0x00);  
         val[1]=(0x18); 
 spi_lcd_write_reg(0xb1,val,2);
        // Display Function Control 
         val[0]=(0x08); 
         val[1]=(0x82);
         val[2]=(0x27);  
 spi_lcd_write_reg(0xb6,val,3);
      // 3Gamma Function Disable 
         val[0]=(0x00); 
 spi_lcd_write_reg(0xf2,val,1);
         //Gamma curve selected 
         val[0]=(0x01); 
 spi_lcd_write_reg(0x26,val,1);
        //Set Gamma 
         val[0]=(0x0F); 
         val[1]=(0x31); 
         val[2]=(0x2B); 
         val[3]=(0x0C); 
         val[4]=(0x0E); 
         val[5]=(0x08); 
         val[6]=(0x4E); 
         val[7]=(0xF1); 
         val[8]=(0x37); 
         val[9]=(0x07); 
         val[10]=(0x10); 
         val[11]=(0x03); 
         val[12]=(0x0E); 
         val[13]=(0x09); 
         val[14]=(0x00); 
spi_lcd_write_reg(0xe0,val,15);
   //Set Gamma 
         val[0]=(0x00); 
         val[1]=(0x0E); 
         val[2]=(0x14); 
         val[3]=(0x03); 
         val[4]=(0x11); 
         val[5]=(0x07); 
         val[6]=(0x31); 
         val[7]=(0xC1); 
         val[8]=(0x48); 
         val[9]=(0x08); 
         val[10]=(0x0F); 
         val[11]=(0x0C); 
         val[12]=(0x31); 
         val[13]=(0x36); 
         val[14]=(0x0F); 
 spi_lcd_write_reg(0xe1,val,15);
#endif
			val[0]=(0x48); 
			spi_lcd_write_reg(0x36,val,1);
			delay_ms(50);
	        val[0]=(0x55); 
 			spi_lcd_write_reg(0x3a,val,1);
			delay_ms(50);

			 val[0]=(0x12); 
		spi_lcd_write_reg(0x51,val,1);
	 val[0]=(0x2c); 
		spi_lcd_write_reg(0x53,val,1);
	

val[0]=(0x02); 
		spi_lcd_write_reg(0x55,val,1);
 			spi_lcd_write_reg(0x11,0,0);

	 delay_ms(120); 
 			spi_lcd_write_reg(0x29,0,0);

 			spi_lcd_write_reg(0x2c,0,0);
printf("%s 1\n",__func__);
	
}

void spi_init_test()
{
u8 val[32]={0,};

	spi_lcd_write_reg(SPI_LCD_DISP_INVERSION_ON,0,0);

	delay_ms(3000);
	spi_lcd_write_reg(SPI_LCD_RESET,0,0);
	delay_ms(60);

	spi_lcd_write_reg(SPI_LCD_SLEEP_OUT,0,0);
	delay_ms(60);
	val[0]=0x06;
	spi_lcd_write_reg(SPI_LCD_DISP_INVERSION_CTRL,val,1);
	spi_lcd_write_reg(SPI_LCD_DISP_ON,0,0);
	//spi_lcd_write_reg(SPI_LCD_DISP_OFF,0,0);
	
	delay_ms(1000);
	spi_lcd_write_reg(SPI_LCD_DISP_INVERSION_ON,0,0);

	delay_ms(1000);
	printf("SPI_LCD_DISP_INVERSION_OFF\n");
	spi_lcd_write_reg(SPI_LCD_DISP_INVERSION_OFF,0,0);
	spi_lcd_write_reg(SPI_LCD_DISP_INVERSION_OFF,0,0);
	//spi_lcd_write_reg(SPI_LCD_DISP_INVERSION_ON,0,0);
	spi_lcd_write_reg(SPI_LCD_RESET,0,0);
	delay_ms(60);
	spi_lcd_write_reg(SPI_LCD_SLEEP_OUT,0,0);
	delay_ms(60);
	spi_lcd_write_reg(SPI_LCD_DISP_INVERSION_OFF,0,0);
	spi_lcd_write_reg(SPI_LCD_DISP_OFF,0,0);
	spi_lcd_write_reg(SPI_LCD_DISP_ON,0,0);

	//spi_lcd_write_reg(SPI_LCD_DISP_ON,0,0);
}

//extern  unsigned char gImage_family[86408] ;
//extern  unsigned char gImage_family2[86408] ;
extern  unsigned char gImage_family3[86408] ;
//extern  unsigned char gImage_family4[86408] ;
//extern  unsigned char gImage_family5[86408] ;

void spi_lcd_start()
{
u32 i;
#if 1
	//for(i=0;i<320*240;i++)
	{
		//if(i>140*240)
		//	disp_data[i]=(0x1f<<11);
		//else if(i>50*240)
		//	disp_data[i]=(0x1f<<0);
		//else
	//		disp_data[i]=gImage_family;;
	}
#endif
	printf("start to display\n");
	//spi_lcd_write_data(0,319,0,240-1,disp_data,320*240);
while(1)
{
//	spi_lcd_write_data(0,319,0,239,gImage_family,320*240);
delay_ms(2000);
//spi_lcd_write_data(0,319,0,239,gImage_family2,320*240);
//delay_ms(2000);
spi_lcd_write_data(0,319,0,239,gImage_family3,320*240);delay_ms(2000);
//spi_lcd_write_data(0,319,0,239,gImage_family4,320*240);delay_ms(2000);
//spi_lcd_write_data(0,319,0,239,gImage_family5,320*240);delay_ms(2000);
}
}

void spi_lcd_read_gram()
{
		int i;
		u8 send_buf[32]={0,};
		u8 recv_buf[32]={0,};

		send_buf[0]=SPI_LCD_MEM_READ;
		spi_lcd_col_addr_set(0,0);
		spi_lcd_page_addr_set(0,0);
		//send cmd
		spi_cs_soft_low(0);
		p8_low();//command cs low	
		//spi_one_transmit(0,send_buf,2,val,2);
	
		spi_one_transmit(0,&send_buf[0],1,0,0);

		//get data
		p8_high();
	
		spi_send_wait_clk(0,1,8);

		spi_one_transmit(0,0,0,recv_buf,32);
		delay_ms(10);	
		spi_cs_soft_high(0);	
	
		printf("recv_buf:\n");
		for(i=0;i<32;i++)
		{
			printf("%02x ",recv_buf[i]);
			if(((i+1)%16)==0)
				printf("\n");
		}
		printf("\n");
	
}

void spi_lcd_draw_point(u32 x,u32 y,u16 color)
{
	u16 pval=color;
	spi_lcd_write_data(y,y,x,x,&pval,1);
}

struct point
{
u32 x;
u32 y;
};

float msqrt( float number )
{
    long i;
    float x2, y;
    const float threehalfs = 1.5F;

    x2 = number * 0.5F;
    y  = number;
//    i  = * ( long * ) &y;                       // evil floating point bit level hacking
 //   i  = 0x5f3759df - ( i >> 1 );               // what the fuck? 
   // y  = * ( float * ) &i;
    //y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
//    y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

    return y;
}
void spi_lcd_draw_line(u32 x1,u32 y1,u32 x2,u32 y2,u16 color)
{
	struct point pp;
	u32 i;
	s32 dx=(x2-x1);
	s32 dy=(y2-y1);
	u32 k=dy>>dx;
	
	for(i=0;i<dx;i++)
		{

		pp.y=x1+k*i;
		pp.x=i;
		spi_lcd_draw_point(pp.x,pp.y,color);
		//pp.x=x1+i/dz*dx;
		//pp.y=y1+i/dz*dy;
		//spi_lcd_draw_point(pp.x,pp.y,color);
		}
		

	
}


void spi_lcd_fill(u32 x1,u32 y1,u32 x2, u32 y2,u16 color)
{

	u32 i;
	u32 psize=(y2-y1)*(x2-x1);

	for(i=0;i<psize;i++)
	{
			disp_data[i]=color;
	}

	spi_lcd_write_data(y1,y2-1,x1,x2-1,disp_data,psize);

}

void dump_reg(u8 addr)
{
		u32 i;

		u8 recv_buf[32]={0,};
		u32 recv_sz=0;	
		spi_lcd_read_reg(addr,recv_buf,&recv_sz);

		for(i=0;i<recv_sz;i++)
		{
			printf("0x%x ",recv_buf[i]);
		}
		printf("\n");
}

extern void lcd_ctrl_gpio_init();

/*
*
*/
#include "timer.h"
void spi_lcd_test()
{
	u32 last_tick2=sys_get_tick();
	lcd_ctrl_gpio_init();

	spi_lcd_init_master(0);
	
	//PA9 RST, PA8 CD/
	p9_low();
	delay_ms(50);
	p9_high();

	//open blacklight
	p7_high();
	
	u8 transmit_one_flag=0;
	int i,j=0;
	
	printf("reg tbl 2 size=%d\n",spi_lcd_get_reg_tbl_size());
	
	// dump_reg(0x09);
	// dump_reg(0x0a);
	// dump_reg(0x0b);
	// dump_reg(0x0c);
	lcd_init();
	printf("%s start lcd\n",__func__);
	spi_lcd_start();
	//spi_lcd_read_gram();
	//dump_reg(0x09);
	//dump_reg(0x0a);
	//dump_reg(0x0b);
	//dump_reg(0x0c);
	//dump_reg(0x52);
	//dump_reg(0x54);
	//dump_reg(0x56);	
	//spi_lcd_fill(10,10,100,100,((0x22<<11)|(56<<5)));
	//spi_init_test();

	
	//spi_lcd_draw_line(100,100,50,200,(0x3f<<5));
#if 0
	while(1)
	{
		if((sys_get_tick()-last_tick2)>500)
		{
			last_tick2=sys_get_tick();
			transmit_one_flag=1;
		}
			
	
		//if(transmit_one_flag==1)
		{
			
			u8 recv_buf[32]={0,};
			u32 recv_sz=0;	

			transmit_one_flag=0;



			spi_lcd_read_reg(0x09,recv_buf,&recv_sz);
			if(spi_reg_tbl[j].read_write==SPI_READ)
			{

				spi_lcd_read_reg(spi_reg_tbl[j].addr,recv_buf,&recv_sz);

				printf("[%x] spi_recv:",spi_reg_tbl[j].addr);
			
				for(i=0;i<recv_sz;i++)
				{
					printf("0x%x ",recv_buf[i]);
				}
				printf("\n");
			}	

			j++;
			if(j>=spi_lcd_get_reg_tbl_size())
				break;			
		
		}

	}
#endif
}



