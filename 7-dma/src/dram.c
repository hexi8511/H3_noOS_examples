
/*
*Here define a struct for SDRAM parameters, if your SDRAM is not different and you can change it.
**/
__dram_para_t sun6i_sdram={
.dram_clk        = 408,
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
