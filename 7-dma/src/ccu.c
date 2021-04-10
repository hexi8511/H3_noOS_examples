#include "ccu_reg.h"
#include "ccu.h"
#include "gpio_reg.h"
#include "basic_reg.h"
#include "uart.h"

#include <stdio.h>
#include <linux/io.h>

//clock struct

static void delay()  
{  
    volatile int i = 0x10000;  
    while (i--);  
}  

#if 0
int get_periph0_clk(void)
{
	unsigned int reg_val;
	int factor_n, factor_k, pll6;

	reg_val = readl(PLL_PERIPH0_CTRL_REG);
	factor_n = ((reg_val >> 8) & 0x1f) + 1;
	factor_k = ((reg_val >> 4) & 0x03) + 1;
	pll6 = 24 * factor_n * factor_k/2;
	return pll6;
}

int get_cpux_clk(void)
{
	unsigned int reg_val;
	int 	div_m, div_p;
	int 	factor_k, factor_n;
	int 	clock;

	reg_val  = readl(PLL_CPUX_CTRL_REG);
	div_p    = ((reg_val >>16) & 0x3);
	factor_n = ((reg_val >> 8) & 0x1f) + 1;
	factor_k = ((reg_val >> 4) & 0x3) + 1;
	div_m    = ((reg_val >> 0) & 0x3) + 1;

	clock = 24 * factor_n * factor_k/div_m/(1<<div_p);

	return clock;
}

int get_axi_clk(void)
{
	int clock;
	unsigned int reg_val;
	int clock_src, factor;

	reg_val   = readl(CPUX_AXI_CFG_REG);
	clock_src = (reg_val >> 16) & 0x03;
	factor    = (reg_val >> 0) & 0x03;

	if(factor >= 3)
	{
		factor = 4;
	}
	else
	{
		factor ++;
	}

	switch(clock_src)
	{
		case 0:
			clock = 32000;
			break;
		case 1:
			clock = 24;
			break;
		case 2:
			clock =  get_cpux_clk();
			break;
		default:
			return 0;
	}

	return clock/factor;
}

int get_ahb_clk(void)
{
	unsigned int reg_val;
	int factor;
	int clock;
    int src = 0;

	reg_val = readl(AHB1_APB1_CFG_REG);
	
    src = (reg_val >> 12)&0x3;
    clock = 0;
    switch(src)
    {
        case 2://src is axi
            factor  = (reg_val >> 4) & 0x03;
            clock   = get_axi_clk()>>factor;
            break;
        case 3://src is pll6
            factor  = (reg_val >> 6) & 0x03;
            clock   = sunxi_clock_get_pll6()/(factor+1);
        break;
    }

	return clock;
}

int get_apb1_clk(void)
{
	unsigned int reg_val;
	int          clock, factor;

	reg_val = readl(AHB1_APB1_CFG_REG);
	factor  = (reg_val >> 8) & 0x03;
	clock   = get_ahb_clk();

	if(factor)
	{
		clock >>= factor;
	}
	else
	{
		clock >>= 1;
	}

	return clock;
}

int get_apb2_clk(void)
{
	return 24;
}

#endif


int pll_enable(enum PLL_TYPE type)
{
	unsigned int reg_val=0;
	unsigned int reg_addr=0;
	//printf("%s()\n",__func__);

	switch(type)
		{
		case PLL_TYPE_CPUX:reg_addr=PLL_CPUX_CTRL_REG;break;
		case PLL_TYPE_AUDIO:reg_addr=PLL_AUDIO_CTRL_REG;break;
		case PLL_TYPE_VIDEO:reg_addr=PLL_VIDEO_CTRL_REG;break;
		case PLL_TYPE_VE:reg_addr=PLL_VE_CTRL_REG;break;
		case PLL_TYPE_DDR:reg_addr=PLL_DDR_CTRL_REG;break;
		case PLL_TYPE_PERIPH0:reg_addr=PLL_PERIPH0_CTRL_REG;break;
		case PLL_TYPE_GPU:reg_addr=PLL_GPU_CTRL_REG;break;
		case PLL_TYPE_PERIPH1:reg_addr=PLL_PERIPH1_CTRL_REG;break;
		case PLL_TYPE_DE:reg_addr=PLL_DE_CTRL_REG;break;
		default:return -1;
		}

	reg_val=readl(reg_addr);
	reg_val|=(1<<31);
	writel(reg_val,reg_addr);

	//printf("%s() waitting addr 0x%08x\n",__func__,reg_addr);

	if(type==PLL_TYPE_DDR)//ddr do not wait
		{
		//printf("set uncpu_pll ok\n");
		return 0;
		}
	
	//wait pll locked
	reg_val=readl(reg_addr);
	while(!(reg_val&( 1<<PLL_LOCKED_BIT)))
		{
		//printf("set pll%d error\n",type);
		reg_val=readl(reg_addr);
		}

	//printf("set pll%d ok\n",type);
	return 0;
}

int pll_disable(enum PLL_TYPE type)
{
	unsigned int reg_val=0;
	unsigned int reg_addr=0;

	switch(type)
		{
		case PLL_TYPE_CPUX:reg_addr=PLL_CPUX_CTRL_REG;break;
		case PLL_TYPE_AUDIO:reg_addr=PLL_AUDIO_CTRL_REG;break;
		case PLL_TYPE_VIDEO:reg_addr=PLL_VIDEO_CTRL_REG;break;
		case PLL_TYPE_VE:reg_addr=PLL_VE_CTRL_REG;break;
		case PLL_TYPE_DDR:reg_addr=PLL_DDR_CTRL_REG;break;
		case PLL_TYPE_PERIPH0:reg_addr=PLL_PERIPH0_CTRL_REG;break;
		case PLL_TYPE_GPU:reg_addr=PLL_GPU_CTRL_REG;break;
		case PLL_TYPE_PERIPH1:reg_addr=PLL_PERIPH1_CTRL_REG;break;
		case PLL_TYPE_DE:reg_addr=PLL_DE_CTRL_REG;break;
		default:return -1;
		}

	reg_val=readl(reg_addr);
	reg_val&=~(1<<31);
	writel(reg_addr,reg_val);
	
	//wait pll locked
	reg_val=readl(reg_addr);
	while(!(reg_val&( 1<<PLL_LOCKED_BIT)))
		{
		reg_val=readl(reg_addr);
		}
	return 0;
}

//extern struct audio_pll_table_t *audio_pll_table;
//extern struct cpux_pll_table_t *cpux_pll_table;
//extern struct ve_pll_table_t *ve_pll_table;
//extern struct ddr_pll_table_t *ddr_pll_table;
//extern struct periphx_pll_table_t *periphx_pll_table;
//#include "ccu.h"

struct cpux_pll_table_t cpux_pll_table[]={

{ 1008000000     ,14,3,1,1},
{ 900000000      ,25,3,2,1},
{ 800000000      ,25,4,3,1},
{ 704000000      ,22,4,3,1},
{ 600000000      ,25,1,1,1},
{ 540000000      ,15,3,2,1},
{ 400000000      ,25,2,3,1},
{ 300000000      ,25,1,2,1},

};

struct periphx_pll_table_t periphx_pll_table[]={
{ 1008000000     ,21,4},
{ 600000000      ,25,2},
{ 300000000      ,25,1},
};

struct ddr_pll_table_t ddr_pll_table[]={
{ 1008000000     ,14,3,1},
{ 900000000      ,25,3,2},
{ 800000000      ,25,4,3},
{ 704000000      ,22,4,3},
{ 696000000      ,29,1,1},
{ 684000000      ,19,3,2},
{ 672000000      ,7,4,1},
{ 648000000      ,9,3,1},
{ 640000000      ,20,4,3},
{ 624000000      ,13,2,1},
{ 600000000      ,25,3,3},
{ 540000000      ,15,3,2},
{ 480000000      ,5,4,1},
{ 408000000      ,17,3,3},
{ 400000000      ,25,2,3},
{ 300000000      ,25,1,2},
{ 200000000      ,25,1,3},
};

struct ve_pll_table_t ve_pll_table[]={
{ 594000000      ,99,4},
{ 592000000      ,74,3},
{ 500000000      ,125,6},
{ 400000000      ,50,3},
{ 300000000      ,25,2},
{ 297000000      ,99,7},
{ 200000000      ,25,3},
{ 100000000      ,25,6},

};


struct audio_pll_table_t audio_pll_table[]={

{ 70000000       ,35,12,1},

{ 69000000       ,23,8,1},

{ 68000000       ,17,6,1},

{ 67000000       ,67,24,1},

{ 66000000       ,11,4,1},

{ 65000000       ,65,24,1},

{ 64000000       ,8,3,1},

{ 63000000       ,21,8,1},

{ 62000000       ,31,12,1},

{ 61000000       ,61,24,1},

{ 60000000       ,5,2,1},

{ 59000000       ,59,24,1},

{ 58000000       ,29,12,1},

{ 57000000       ,19,8,1},

{ 56000000       ,7,3,1},

{ 55000000       ,55,24,1},

{ 54000000       ,9,4,1},

{ 53000000       ,53,24,1},

{ 52000000       ,13,6,1},

{ 51000000       ,17,8,1},

{ 50000000       ,25,12,1},

{ 49000000       ,49,24,1},

{ 48000000       ,2,1,1},

{ 47000000       ,47,24,1},

{ 46000000       ,23,12,1},

{ 45000000       ,15,8,1},

{ 44000000       ,11,6,1},

{ 43000000       ,43,24,1},

{ 42000000       ,7,4,1},

{ 41000000       ,41,24,1},

{ 40000000       ,5,3,1},

{ 39000000       ,13,8,1},

{ 38000000       ,19,12,1},

{ 37000000       ,37,24,1},

{ 36000000       ,3,2,1},


{ 34000000       ,17,12,1},

{ 33000000       ,11,8,1},

{ 32000000       ,4,3,1},

{ 31000000       ,31,24,1},

{ 30000000       ,5,4,1},

{ 29000000       ,29,24,1},

{ 28000000       ,7,6,1},

{ 27000000       ,9,8,1},

{ 26000000       ,13,12,1},

{ 25000000       ,25,24,1},

{ 24200000       ,121,30,4},

{ 23000000       ,23,24,1},
{ 22000000       ,11,12,1},
{ 21000000       ,7,8,1},
{ 20000000       ,5,6,1}
};
/**
*test code 
*/
void calc_reg_val(u8 type, unsigned long long int pll,unsigned int* p,unsigned int* m,unsigned int* n,unsigned int* k)
{
	int size,i;
	switch(type)
		{
		case PLL_TYPE_CPUX:{
			size=sizeof(cpux_pll_table)/sizeof(struct cpux_pll_table_t);
	
				//printf("array size %d pll:%lld\n",size,pll);
				for(i=0;i<size-1;i++)
					{
					if((pll<=cpux_pll_table[i].pll)&&(pll>cpux_pll_table[i+1].pll))
						{
							*p=cpux_pll_table[i].p;
							*m=cpux_pll_table[i].m;
							*n=cpux_pll_table[i].n;
							*k=cpux_pll_table[i].k;
						}
					}
				break;
			}
		case PLL_TYPE_AUDIO:
			{	
				size=sizeof(audio_pll_table)/sizeof(struct audio_pll_table_t);
				
				//printf("array size %d\n",size);
				for(i=0;i<size-1;i++)
					{
					if((pll<=audio_pll_table[i].pll)&&(pll>audio_pll_table[i+1].pll))
						{
							*p=audio_pll_table[i].p;
							*m=audio_pll_table[i].m;
							*n=audio_pll_table[i].n;
						}
					}
			break;
			}
		case PLL_TYPE_VIDEO:
		case PLL_TYPE_VE:
		case PLL_TYPE_GPU:
		case PLL_TYPE_DE:
			{	
				size=sizeof(ve_pll_table)/sizeof(struct ve_pll_table_t);
				
				//printf("array size %d\n",size);
				for(i=0;i<size-1;i++)
					{
					if((pll<=ve_pll_table[i].pll)&&(pll>ve_pll_table[i+1].pll))
						{
							
							*m=ve_pll_table[i].m;
							*n=ve_pll_table[i].n;
						}
					}
			break;
			}
		case PLL_TYPE_DDR:
			{
			size=sizeof(ddr_pll_table)/sizeof(struct ddr_pll_table_t);
	
				//printf("array size %d\n",size);
				for(i=0;i<size-1;i++)
					{
					if((pll<=ddr_pll_table[i].pll)&&(pll>ddr_pll_table[i+1].pll))
						{
							//*p=cpux_pll_table[i].p;
							*m=ddr_pll_table[i].m;
							*n=ddr_pll_table[i].n;
							*k=ddr_pll_table[i].k;
						}
					}
				break;
			}

		case PLL_TYPE_PERIPH0:
		case PLL_TYPE_PERIPH1:
			{	
				size=sizeof(periphx_pll_table)/sizeof(struct periphx_pll_table_t);
				
				//printf("array size %d\n",size);
				for(i=0;i<size-1;i++)
					{
					if((pll<=periphx_pll_table[i].pll)&&(pll>periphx_pll_table[i+1].pll))
						{
							*k=periphx_pll_table[i].k;
							//*m=periphx_pll_table[i].m;
							*n=periphx_pll_table[i].n;
						}
					}
			break;
			}
	}
	
}

int pll_set_out_freq(u8 type,unsigned long long int pll)
{
	unsigned int reg_val=0;
	unsigned int reg_addr=0;

	unsigned int reg_p_val=0;
	unsigned int reg_m_val=0;
	unsigned int p,m,n,k;
	
	pll_enable(type);

	switch(type)
		{
		case PLL_TYPE_CPUX://default 408M
			{
			//calc m k n p val
			//24000000*n*k/(m*p)   n=1~32;k=1~4;m=1!4
			calc_reg_val(type,pll,&p,&m,&n,&k);
			
			//printf("p=%d,m=%d,k=%d,n=%d\n",p,m,k,n);
			//check args is valiabled
			if((p<1)||(p>4)||(n>32)||(n<1)||(k>4)||(k<1)||(m>4)||(m<1))
				{
				return -1;
				}
			reg_addr=PLL_CPUX_CTRL_REG;
			reg_val=readl(reg_addr);
			
			reg_p_val=(reg_val>>16)&0x3;
			reg_m_val=(reg_val>>0)&0x3;

			//if p > oldp set it first
			if(reg_p_val<(p-1))
				{
				reg_val|=(((p-1)<<16)|((n-1)<<8)|((k-1)<<4)|(m-1));
				writel(reg_p_val,reg_addr);
				delay();
				}
			//if m>oldp set it first
			if(reg_m_val<(m-1))
				{
					reg_val|=(((p-1)<<16)|((n-1)<<8)|((k-1)<<4)|(m-1));
					writel(reg_m_val,reg_addr);
					delay();
				}
			
			//k,n
			reg_val&=(~((0x03<<16)|(0x1f<<8)|(0x03<<4)|0x03));
			reg_val|=(((p-1)<<16)|((n-1)<<8)|((k-1)<<4)|(m-1));

			//freq=(24*n*k)/(m*p); pll cpux default frequency is 408MHz
			writel(reg_val,reg_addr);

			//if m>oldp set it first
			if(reg_m_val>(m-1))
				{
				
				reg_val|=(((p-1)<<16)|((n-1)<<8)|((k-1)<<4)|(m-1));
					writel(reg_val,reg_addr);
					delay();
				}

			reg_val=readl(reg_addr);
	
			while(!(reg_val&( 1<<PLL_LOCKED_BIT)))
				{
				reg_val=readl(reg_addr);
				}

			if(reg_p_val>(p-1))
				{
				
				reg_val|=(((p-1)<<16)|((n-1)<<8)|((k-1)<<4)|(m-1));
					writel(reg_val,reg_addr);
					delay();
				}
			
				goto out;
				
			break;
			}
		case PLL_TYPE_AUDIO://default 24.571M
			{
			
			calc_reg_val(type,pll,&p,&m,&n,&k);
			if((p<1)||(p>16)||(n>128)||(n<1)||(m>32)||(m<1))
				{
				return -1;
				}
			reg_addr=PLL_AUDIO_CTRL_REG;
			reg_val=readl(reg_addr);
			reg_val&=(~((0xf<<16)|(0x7f<<8)|0x1f));
			reg_val|=(((p-1)<<16)|((n-1)<<8)|(m-1));	
			writel(reg_val,reg_addr);	
	
		/*
		PLL _AUDIO = (24MHz*N)/(M*P)
		PLL_AUDIO(8X)= (24MHz*N*2)/M
		PLL_AUDIO(4X)=PLL_AUDIO(8X)/2
		PLL_AUDIO(2X)=PLL_AUDIO(4X)/2*/
			break;
			}
		case PLL_TYPE_VIDEO://default 297M
			{
				calc_reg_val(type,pll,&p,&m,&n,&k);
				if((n>128)||(n<1)||(m>16)||(m<1))
				{
				return -1;
				}
				
				reg_addr=PLL_VIDEO_CTRL_REG;
				reg_val=readl(reg_addr);
				reg_val&=(~((0x1<<24)|(0x7f<<8)|0xf));
				reg_val|=((1<<24)|((n-1)<<8)|(m-1));//integer mode	
				writel(reg_val,reg_addr);	
	
				break;
			//pll=24M*n/m
			}

		case PLL_TYPE_VE://default 297M
			{
				calc_reg_val(type,pll,&p,&m,&n,&k);
				if((n>128)||(n<1)||(m>16)||(m<1))
				{
				return -1;
				}
				
				reg_addr=PLL_VE_CTRL_REG;
				reg_val=readl(reg_addr);
				reg_val&=(~((0x1<<24)|(0x7f<<8)|0xf));
				reg_val|=((1<<24)|((n-1)<<8)|(m-1));//integer mode	
				writel(reg_val,reg_addr);	
	
				break;
			    //pll=24M*n/m
			}
		case PLL_TYPE_DDR:{ //ok below default 408M

				reg_addr=PLL_DDR_TUN_REG;
				reg_val=readl(reg_addr);
				reg_val&=~((7<<24)|(128<<16));
				reg_val|=((2<<24)|(16<<16));
				writel(reg_val,reg_addr);

				calc_reg_val(type,pll,&p,&m,&n,&k);

				//printf("p=%d,m=%d,k=%d,n=%d\n",p,m,k,n);

				if((n>32)||(n<1)||(k>4)||(k<1)||(m>4)||(m<1))
				{
				return -1;
				}
				reg_addr=PLL_DDR_CTRL_REG;
				reg_val=readl(reg_addr);
				reg_val&=(~((0x1f<<8)|(0x3<<4)|0x3));
				reg_val|=((0<<20)|((n-1)<<8)|((k-1)<<4)|(m-1));//validating the pll-ddr
				writel(reg_val,reg_addr);

				reg_val|=((1<<20)|((n-1)<<8)|((k-1)<<4)|(m-1));//validating the pll-ddr
				writel(reg_val,reg_addr);
		
				while(readl(PLL_DDR_CTRL_REG)&(1<<20));
				
				break;
			//pll=24M*n*k/m
			}

		case PLL_TYPE_PERIPH0://default 600M
			{
				calc_reg_val(type,pll,&p,&m,&n,&k);
				if((n>32)||(n<1)||(k>4)||(k<1)||(m>4)||(m<1))
				{
				return -1;
				}
				reg_addr=PLL_PERIPH0_CTRL_REG;
				reg_val=readl(reg_addr);
				reg_val&=(~((1<<25)|(1<<24)|(0x1f<<8)|(0x3<<4)|0x3));
				reg_val|=((0<<25)|(0<<24)|(1<<18)|(0<<16)|((n-1)<<8)|((k-1)<<4)|(m-1));//enable 
				writel(reg_val,reg_addr);	
				//pll=24M*n*k/2		
				break;
			}
		case PLL_TYPE_GPU://default 297M
			{
				calc_reg_val(type,pll,&p,&m,&n,&k);
				
				if((n>128)||(n<1)||(m>16)||(m<1))
				{
				return -1;
				}
				
				reg_addr=PLL_GPU_CTRL_REG;
				reg_val=readl(reg_addr);
				reg_val&=(~((0x1<<24)|(0x7f<<8)|0x0f));
				reg_val|=((1<<24)|((n-1)<<8)|(m-1));//integer mode	
				writel(reg_val,reg_addr);	
				//pll=24M*n*k/2
				break;
			}
		case PLL_TYPE_PERIPH1://default 600M
			{
				calc_reg_val(type,pll,&p,&m,&n,&k);
				if((n>32)||(n<1)||(k>4)||(k<1)||(m>4)||(m<1))
				{
				return -1;
				}
				reg_addr=PLL_PERIPH1_CTRL_REG;
				reg_val=readl(reg_addr);
				reg_val&=(~((1<<25)|(1<<24)|(0x1f<<8)|(0x3<<4)|0x3));
				reg_val|=((0<<25)|(1<<24)|(1<<18)|(0<<16)|((n-1)<<8)|((k-1)<<4)|(m-1));//enable 
				writel(reg_val,reg_addr);	
				//pll=24M*n/m		
				break;
			}
		case PLL_TYPE_DE://default 297M
			{
				calc_reg_val(type,pll,&p,&m,&n,&k);
				
				if((n>128)||(n<1)||(m>16)||(m<1))
				{
				return -1;
				}
				
				reg_addr=PLL_DE_CTRL_REG;
				reg_val=readl(reg_addr);
				reg_val&=(~((0x1<<24)|(0x7f<<8)|0x0f));
				reg_val|=((1<<24)|((n-1)<<8)|(m-1));//integer mode	
				writel(reg_val,reg_addr);	
				//pll=24M*n*k/2
				break;
			}
		default:return -1;
		}

	//wait pll locked
	reg_val=readl(reg_addr);
	while(!(reg_val&( 1<<PLL_LOCKED_BIT)))
		{
		reg_val=readl(reg_addr);
		}

out:

	return 0;
}





//bus clk
/**********************
*CPUX,AXI,APB0
**********************/
/*
*0 	lsoc
*1 	24Mhz
*2/3 pll_cpux
*/

void set_cpux_clksrc(char src)
{
	unsigned int reg_val=readl(CPUX_AXI_CFG_REG);
	
	reg_val=reg_val&(~(0x3<<16));
	reg_val=reg_val|(src<<16);
	writel(reg_val,CPUX_AXI_CFG_REG);
}


/*
*0 	/1
*1 	/2
*2/3 /4
*/
void set_cpux_apb_div(char div)
{
	unsigned int reg_val=readl(CPUX_AXI_CFG_REG);
	reg_val=reg_val&(~(0x3<<8));
	reg_val=reg_val|(div<<8);
	writel(reg_val,CPUX_AXI_CFG_REG);
}

/*
*0 	/1
*1 	/2
*2	/3
*3  /4
*/
void set_cpux_axi_div(char div)
{
	unsigned int reg_val=readl(CPUX_AXI_CFG_REG);
	reg_val=reg_val&(~(0x3<<0));
	reg_val=reg_val|(div<<0);
	writel(reg_val,CPUX_AXI_CFG_REG);
}

/**********************
*AHB1 APB1
**********************/
/*
*0 	lsoc
*1 	24Mhz
*2	axi
*3 	pll_periph0
*/

void set_ahb1_clksrc(char src)
{
	unsigned int reg_val=readl(AHB1_APB1_CFG_REG);
	reg_val=reg_val&(~(0x3<<12));
	reg_val=reg_val|(src<<12);
	writel(reg_val,AHB1_APB1_CFG_REG);
}
/*
*0 	/1
*1 	/2
*2	/3
*3  /4
*/
void set_ahb1_prediv(char div)
{
	unsigned int reg_val=readl(AHB1_APB1_CFG_REG);
	reg_val=reg_val&(~(0x3<<6));
	reg_val=reg_val|(div<<6);
	writel(reg_val,AHB1_APB1_CFG_REG);
}

/*
*0 	/1
*1 	/2
*2	/4
*3  /8
*/
void set_ahb1_div(char div)
{
	unsigned int reg_val=readl(AHB1_APB1_CFG_REG);
	reg_val=reg_val&(~(0x3<<4));
	reg_val=reg_val|(div<<4);
	writel(reg_val,AHB1_APB1_CFG_REG);
}
/*
*0 	/2
*1 	/2
*2	/4
*3  /8
*/
void set_apb1_div(char div)
{
	unsigned int reg_val=readl(AHB1_APB1_CFG_REG);
	reg_val=reg_val&(~(0x3<<8));
	reg_val=reg_val|(div<<8);
	writel(reg_val,AHB1_APB1_CFG_REG);
}


/**********************
*APB2
**********************/
/*
*0 	lsoc
*1 	24Mhz
*2/3 pll_periph0
*/
void set_apb2_clksrc(char src)
{
	unsigned int reg_val=readl(APB2_CFG_REG);
	reg_val=reg_val&(~(0x3<<24));
	reg_val=reg_val|(src<<24);
	writel(reg_val,APB2_CFG_REG);
}


/*
*0 	/1
*1 	/2
*2	/4
*3  /8
*/
void set_apb2_prediv(char div)
{
	unsigned int reg_val=readl(APB2_CFG_REG);
	reg_val=reg_val&(~(0x3<<16));
	reg_val=reg_val|(div<<16);
	writel(reg_val,APB2_CFG_REG);
}

/*
*div =1 ~32
*/
void set_apb2_div(char div)
{
	unsigned int reg_val=readl(APB2_CFG_REG);
	reg_val=reg_val&(~(0x1f<<0));
	reg_val=reg_val|(div<<0);
	writel(reg_val,APB2_CFG_REG);
}

/**********************
*AHB2
**********************/

/*
*0 	ahb1
*1 	pll_periph0/2
*/
void set_ahb2_clksrc(char src)
{
	unsigned int reg_val=readl(AHB2_CFG_REG);
	reg_val=reg_val&(~(0x3<<0));
	reg_val=reg_val|(src<<0);
	writel(reg_val,AHB2_CFG_REG);
}


/********************
*BUS gate
***********************/

void ccm_clock_enable(u32 clk_id)
{
	switch(clk_id>>8) {
		case AXI_BUS:
			set_wbit(AHB2_CFG_REG, 0x1U<<(clk_id&0xff));
			break;
		case AHB1_BUS0:
			set_wbit(BUS_CLK_GATING_REG0, 0x1U<<(clk_id&0xff));
			break;
		case AHB1_BUS1:
			set_wbit(BUS_CLK_GATING_REG1, 0x1U<<(clk_id&0xff));
			break;
		case APB1_BUS0:
			set_wbit(BUS_CLK_GATING_REG2, 0x1U<<(clk_id&0xff));
			break;
		case APB2_BUS0:
			set_wbit(BUS_CLK_GATING_REG3, 0x1U<<(clk_id&0xff));
		case AHB2_BUS:
			set_wbit(BUS_CLK_GATING_REG4, 0x1U<<(clk_id&0xff));
			break;
	}
}


void ccm_clock_disable(u32 clk_id)
{
	switch(clk_id>>8) {
		case AXI_BUS:
			clr_wbit(AHB2_CFG_REG, 0x1U<<(clk_id&0xff));
			break;
		case AHB1_BUS0:
			clr_wbit(BUS_CLK_GATING_REG0, 0x1U<<(clk_id&0xff));
			break;
		case AHB1_BUS1:
			clr_wbit(BUS_CLK_GATING_REG1, 0x1U<<(clk_id&0xff));
			break;
		case APB1_BUS0:
			clr_wbit(BUS_CLK_GATING_REG2, 0x1U<<(clk_id&0xff));
			break;
		case APB2_BUS0:
			clr_wbit(BUS_CLK_GATING_REG3, 0x1U<<(clk_id&0xff));
		case AHB2_BUS:
			clr_wbit(BUS_CLK_GATING_REG4, 0x1U<<(clk_id&0xff));
			break;
	}
}

void ccm_module_enable(u32 clk_id)
{
	switch(clk_id>>8) {
		case AHB1_BUS0:
			set_wbit(BUS_SOFT_RST_REG0, 0x1U<<(clk_id&0xff));
			break;
		case AHB1_BUS1:
			set_wbit(BUS_SOFT_RST_REG1, 0x1U<<(clk_id&0xff));
			break;
		case AHB1_LVDS:
			set_wbit(BUS_SOFT_RST_REG2, 0x1U<<(clk_id&0xff));
			break;
		case APB1_BUS0:
			set_wbit(BUS_SOFT_RST_REG3, 0x1U<<(clk_id&0xff));
			break;
		case AHB2_BUS:
			set_wbit(BUS_SOFT_RST_REG4, 0x1U<<(clk_id&0xff));
			break;
	}
}

void ccm_module_disable(u32 clk_id)
{
	switch(clk_id>>8) {
		case AHB1_BUS0:
			clr_wbit(BUS_SOFT_RST_REG0, 0x1U<<(clk_id&0xff));
			break;
		case AHB1_BUS1:
			clr_wbit(BUS_SOFT_RST_REG1, 0x1U<<(clk_id&0xff));
			break;
		case AHB1_LVDS:
			clr_wbit(BUS_SOFT_RST_REG2, 0x1U<<(clk_id&0xff));
			break;
		case APB1_BUS0:
			clr_wbit(BUS_SOFT_RST_REG3, 0x1U<<(clk_id&0xff));
			break;
		case APB2_BUS0:
			clr_wbit(BUS_SOFT_RST_REG4, 0x1U<<(clk_id&0xff));
			break;
	}
}

void ccm_module_reset(u32 clk_id)
{
	ccm_module_disable(clk_id);
	ccm_module_disable(clk_id);
	ccm_module_enable(clk_id);
}


void ccm_clock_disable_all(void)
{
	//writel(0, AHB2_CFG_REG);
	writel(0, BUS_CLK_GATING_REG0);
	writel(0, BUS_CLK_GATING_REG1);
	writel(0, BUS_CLK_GATING_REG2);
	writel(0, BUS_CLK_GATING_REG3);
	writel(0, BUS_CLK_GATING_REG4);
//	clr_wbit(CCM_AXI_GATE_CTRL, 0xffffffff);
//	clr_wbit(CCM_AHB1_GATE0_CTRL, 0xffffffff);
//	clr_wbit(CCM_AHB1_GATE1_CTRL, 0xffffffff);
//	clr_wbit(CCM_APB1_GATE0_CTRL, 0xffffffff);
//	clr_wbit(CCM_APB2_GATE0_CTRL, 0xffffffff);
}

void ccm_reset_all_module(void)
{
	writel(0, BUS_SOFT_RST_REG0);
	writel(0, BUS_SOFT_RST_REG1);
	writel(0, BUS_SOFT_RST_REG2);
	writel(0, BUS_SOFT_RST_REG3);
	writel(0, BUS_SOFT_RST_REG4);
//	clr_wbit(CCM_AHB1_RST_REG0, 0xffffffff);
//	clr_wbit(CCM_AHB1_RST_REG1, 0xffffffff);
//	clr_wbit(CCM_AHB1_RST_REG2, 0xffffffff);
//	clr_wbit(CCM_APB1_RST_REG, 0xffffffff);
//	clr_wbit(CCM_APB2_RST_REG, 0xffffffff);
}

void ccmu_cpus_clksrc(u32 src)
{
	unsigned int reg_val=readl(SUNXI_RPRCM_BASE);
	reg_val=reg_val&(~(0x3<<16));
	reg_val=reg_val|(src<<16);
	writel(reg_val,SUNXI_RPRCM_BASE);
}

void ccmu_cpus_postdiv(u32 div)
{
	unsigned int reg_val=readl(SUNXI_RPRCM_BASE);
	reg_val=reg_val&(~(0x1f<<8));
	reg_val=reg_val|(div<<8);
	writel(reg_val,SUNXI_RPRCM_BASE);
}


void ccmu_cpus_div(u32 div)
{
	unsigned int reg_val=readl(SUNXI_RPRCM_BASE);
	reg_val=reg_val&(~(0x3<<4));
	reg_val=reg_val|(div<<4);
	writel(reg_val,SUNXI_RPRCM_BASE);
}

static void init_cpus(void)
{

	volatile unsigned long value;

	value = readl(0x01F01C00 + 0x0); //R_CPUS_CFG_BASE
	value &= ~1;
	writel(value, 0x01F01C00 + 0x0);
	value = readl(0x01F01C00 + 0x0);
	value |= 1;
	writel(value, 0x01F01C00 + 0x0);
}

void open_apb0_clk()
{
 //msg("[%s] APB0 frequency setting\n",__func__);
	ccmu_cpus_clksrc(1);//24M

	//delay();
	ccmu_cpus_postdiv(0);//24M
	ccmu_cpus_div(0);
	//ccm_module_enable(PIO_CKID);
	//ccm_clock_enable(PIO_CKID);

	writel(readl(SUNXI_RPRCM_BASE + 0x0c)	&		0x00, SUNXI_RPRCM_BASE + 0x0c);//apb0 div
	//init_cpus();

	writel(readl(SUNXI_RPRCM_BASE + 0x28)	|		0x01, SUNXI_RPRCM_BASE + 0x28);//apb0 get 0.mask  1.pass
	//writel(readl(SUNXI_RPRCM_BASE + 0xb0)	&		(~0x01), SUNXI_RPRCM_BASE + 0xb0);//apb0 1.de-assert 0.reset
	writel(readl(SUNXI_RPRCM_BASE + 0xb0)	|		(0x01), SUNXI_RPRCM_BASE + 0xb0);//apb0 1.de-assert 0.reset
}
#if 0
void open_apb0_clk()
{
	//set_cpux_clksrc(1);//24M

	//delay();
	//set_cpux_apb_div(0);//24M
	//ccm_module_enable(PIO_CKID);
	//ccm_clock_enable(PIO_CKID);

	//writel(readl(SUNXI_RPRCM_BASE + 0x0c)	|		0x00, SUNXI_RPRCM_BASE + 0x0c);//apb0 div
	init_cpus();
	writel(readl(SUNXI_RPRCM_BASE + 0xb0)	&		(~0x01), SUNXI_RPRCM_BASE + 0xb0);//apb0 1.de-assert 0.reset
	writel(readl(SUNXI_RPRCM_BASE + 0xb0)	|		(0x01), SUNXI_RPRCM_BASE + 0xb0);//apb0 1.de-assert 0.reset
	writel(readl(SUNXI_RPRCM_BASE + 0x28)	|		0x01, SUNXI_RPRCM_BASE + 0x28);//apb0 get 0.mask  1.pass

}
#endif

void pll_ddr_init()
{
	if(0!=pll_set_out_freq(PLL_TYPE_DDR,624000000))//800MHz
		{
		printf("set ddr pll error!\n");
		return ;
		}
}

void sys_clk_init()
{
	printf("[%s] Init all system pll\n",__func__);

	//set pll frequency
	if(0!=pll_set_out_freq(PLL_TYPE_CPUX,1008000000))//1.008GHz
		{
		printf("set cpux pll error!\n");
		return ;
		}

	if(0!=pll_set_out_freq(PLL_TYPE_PERIPH0,600000000))//600MHz
		{
		printf("set periph0 pll error!\n");
		return ;
		}

	if(0!=pll_set_out_freq(PLL_TYPE_DDR,624000000))//800MHz
		{
		printf("set ddr pll error!\n");//25*24*3/2
		return ;
		}

	if(0!=pll_set_out_freq(PLL_TYPE_VE,594000000))
		{
		printf("set ve pll error!\n");
		return ;
		}
	
	if(0!=pll_set_out_freq(PLL_TYPE_VIDEO,297000000))
		{
		printf("set video pll error!\n");
		return ;
		}

	if(0!=pll_set_out_freq(PLL_TYPE_GPU,594000000))
		{
		printf("set ddr gpu error!\n");
		return ;
		}

	if(0!=pll_set_out_freq(PLL_TYPE_DE,594000000))
			{
			printf("set de pll error!\n");
			return ;
			}

	printf("[%s] CPUX APB AXI frequency setting\n",__func__);

	//clk src setting
	set_cpux_axi_div(0);//AXI CLK=CPUX CLK/1=1.008G
	set_cpux_apb_div(0);//APB CLK=CPUX CLK/1=1.008G
	set_cpux_clksrc(2);//pll_cpux 1.008G

	printf("[%s] APB2 frequency setting\n",__func__);

	//set_apb2_prediv(0);	//APB2 CLK=periph0/1 
	//set_apb2_div(0);	//APB2 CLK=periph0/1/1
	//set_apb2_clksrc(2);	//pll_periph0 600M


	printf("[%s] APB2 frequency setting ok\n",__func__);
	//Need resetting uart0 baud
	//uart0_baud_init2(115200);

	printf("\b[%s]AHB1 APB1 frequency setting\n",__func__);

	//the prediv and div can not be 0 at the same time
	set_ahb1_prediv(0);
	set_ahb1_div(1);//AHB1 CLK=periph0/2 600MHz
	set_apb1_div(0);//APB1 CLK=AHB1 CLK/2 300MHz
	set_ahb1_clksrc(3);//periph0 AHB1=600MHz

	//打开DMA
	(*(volatile unsigned int *)(0x01c20000 + 0x60) ) &= 1 << 6;//AHB DMA clk
	(*(volatile unsigned int *)(0x01c20000 + 0x2C0)) &= ~(1 << 6);//DMA rst assert
#if 0
	//close uart clk gate 
	ccm_clock_disable(UART0_CKID);
	int kk=1000;	
	while(kk--)
		{
		delay();
		}
	//open uart clk gate
	ccm_clock_enable(UART0_CKID);
	
	//close pio clk gate
	ccm_clock_disable(PIO_CKID);
	kk=1000;
	while(kk--)
		{
		delay();
		}
	//open pio clk gate
	ccm_clock_enable(PIO_CKID);
#endif

unsigned int myval=readl(PLL_DDR_CTRL_REG);
printf("ddr_ctrl=%x\n",myval);

}
