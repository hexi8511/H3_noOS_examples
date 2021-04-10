#include "ccu_reg.h"
#include "ccu.h"
#include "gpio_reg.h"

#include "stdio.h"

//clock struct



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

reg_val=__read_reg(reg_addr);
//reg_val|=PLL_EN_BIT_SET;
SET_BIT(reg_val,31);
__write_reg(reg_addr,reg_val);

//wait pll locked
reg_val=__read_reg(reg_addr);
while(!(reg_val&( 1<<PLL_LOCKED_BIT)))
{
;
}

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

reg_val=__read_reg(reg_addr);
//reg_val&=PLL_EN_BIT_CLR;
CLR_BIT(reg_val,31);
__write_reg(reg_addr,reg_val);

//wait pll locked
reg_val=__read_reg(reg_addr);
while(!(reg_val&( 1<<PLL_LOCKED_BIT)))
{
;
}
return 0;
}


/**
*test code
*/
void calc_reg_val(unsigned int* p,unsigned int* m,unsigned int* n,unsigned int* k)
{

}

int pll_set_out_freq(u8 type)
{
unsigned int reg_val=0;
unsigned int reg_addr=0;

unsigned int p,m,n,k;



switch(type)
	{
	case PLL_TYPE_CPUX:{
		//calc m k n p val
		calc_reg_val(&p,&m,&n,&k);
		
		//check args is valiabled
		if((p<1)||(p>4)||(n>32)||(n<1)||(k>4)||(k<1)||(m>4)||(m<1))
			{
			return -1;
			}
		reg_addr=PLL_CPUX_CTRL_REG;
		reg_val=__read_reg(reg_addr);
		reg_val&=(~((0x03<<16)|(0x1f<<8)|(0x03<<4)|0x03));
		reg_val|=((p<<16)|(n<<8)|(k<<4)|m);

		//freq=(24*n*k)/(m*p); pll cpux default frequency is 408MHz

		break;
		}
	case PLL_TYPE_AUDIO:
		{
		reg_addr=PLL_AUDIO_CTRL_REG;
		if((p<1)||(p>16)||(n>128)||(n<1)||(m>32)||(m<1))
			{
			return -1;
			}
		reg_addr=PLL_CPUX_CTRL_REG;
		reg_val=__read_reg(reg_addr);
		reg_val&=(~((0xf<<16)|(0x7f<<8)|0x1f));
		reg_val|=((p<<16)|(n<<8)|m);		
	
	/*
		PLL _AUDIO = (24MHz*N)/(M*P)
	PLL_AUDIO(8X)= (24MHz*N*2)/M
	PLL_AUDIO(4X)=PLL_AUDIO(8X)/2
	PLL_AUDIO(2X)=PLL_AUDIO(4X)/2*/
		break;
		}
	case PLL_TYPE_VIDEO:reg_addr=PLL_VIDEO_CTRL_REG;break;
	case PLL_TYPE_VE:reg_addr=PLL_VE_CTRL_REG;break;
	case PLL_TYPE_DDR:reg_addr=PLL_DDR_CTRL_REG;break;
	case PLL_TYPE_PERIPH0:reg_addr=PLL_PERIPH0_CTRL_REG;break;
	case PLL_TYPE_GPU:reg_addr=PLL_GPU_CTRL_REG;break;
	case PLL_TYPE_PERIPH1:reg_addr=PLL_PERIPH1_CTRL_REG;break;
	case PLL_TYPE_DE:reg_addr=PLL_DE_CTRL_REG;break;
	default:return -1;
	}



__write_reg(reg_addr,reg_val);

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
*0 	/1
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
		case APB2_BUS0:
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
	writel(0, AHB2_CFG_REG);
	writel(0, BUS_CLK_GATING_REG0);
	writel(0, BUS_CLK_GATING_REG1);
	writel(0, BUS_CLK_GATING_REG2);
	writel(0, BUS_CLK_GATING_REG3);
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

static void delay()  
{  
    volatile int i = 0x10000;  
    while (i--);  
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
