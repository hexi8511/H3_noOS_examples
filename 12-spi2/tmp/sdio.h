#ifndef __SDIO_H__
#define __SDIO_H__


#define SDIO_IDMAC_DIS_INT_ONCMP    1
#define SDIO_IDMAC_LAST_DESC        2
#define SDIO_IDMAC_FIRST_DESC       3
#define SDIO_IDMAC_CHAIN_FLAG       4   
#define SDIO_IDMAC_ERROR            30  //when some error happened in transfer, this bit will be set
#define SDIO_IDMAC_HOLD             31  //transfer over this bit will be clear,1=desc own is idmac

typedef struct sdio_idmac_desc
{
    unsigned int idmac_desc;
    unsigned int buffer_size;//data buffer size must be a multiple of 4 byte,max value 2^16=65536
    unsigned int* p_buffer;//4 byte align address
    sdio_idmac_desc* p_next_desc;
}sdio_idmac_desc_t;


#define SDIO0_BASE_ADDR 0x01c0f000
#define SDIO1_BASE_ADDR 0x01c10000
#define SDIO2_BASE_ADDR 0x01c11000

#define SD_GCTL_OFFSET  0x00    //control register
#define SD_CKCR_OFFSET  0x04    //clock control register
#define SD_TMOR_OFFSET  0x08    //timeout register
#define SD_BWDR_OFFSET  0x0c    //bus width register
#define SD_BKSR_OFFSET  0x10    //block size register
#define SD_BYCR_OFFSET  0x14    //byte count register
#define SD_CMDR_OFFSET  0x18    //command register
#define SD_CAGR_OFFSET  0x1c    //command argument register
#define SD_RESP0_OFFSET 0x20    //response 0 register
#define SD_RESP1_OFFSET 0x24    //response 1 register
#define SD_RESP2_OFFSET 0x28    //response 2 register
#define SD_RESP3_OFFSET 0x2c    //response 3 register
#define SD_IMKR_OFFSET  0x30    //interrupt mask register
#define SD_MISR_OFFSET  0x34    //masked interrupt status register
#define SD_RISR_OFFSET  0x38    //Raw interrupt status register
#define SD_STAR_OFFSET  0x3c    //status register
#define SD_FWLR_OFFSET  0x40    //fifo water level register
#define SD_FUNS_OFFSET  0x44    //fifo function select register
#define SD_A12A_OFFSET  0x58    //auto command 12 argument
#define SD_NTSR_OFFSET  0x5c    //SD newtiming set register
#define SD_SDBG_OFFSET  0x60    //SD newtiming set debug register
#define SD_HWRST_OFFSET 0x78    //hardware reset register
#define SD_DMAC_OFFSET  0x80    //bus mode control register
#define SD_DLBA_OFFSET  0x84    //descriptor list base register
#define SD_IDST_OFFSET  0x88    //DMAC state register
#define SD_IDIE_OFFSET  0x8c    //DMAC interrupt enable register
#define SD_THLDC_OFFSET 0x100   //card threshold control register
#define SD_DSBD_OFFSET  0x10c   //eMMC 4.41 DDR start bit detection control
#define SD_RES_CRC_OFFSET   0x110   //CRC status  from card/eMMC in write operation
#define SD_DATA7_CRC_OFFSET   0x114 //CRC data7 from card/eMMC
#define SD_DATA6_CRC_OFFSET   0x118 //CRC data6 from card/eMMC
#define SD_DATA5_CRC_OFFSET   0x11c //CRC data5 from card/eMMC
#define SD_DATA4_CRC_OFFSET   0x120 //CRC data4 from card/eMMC
#define SD_DATA3_CRC_OFFSET   0x124 //CRC data3 from card/eMMC
#define SD_DATA2_CRC_OFFSET   0x128 //CRC data2 from card/eMMC
#define SD_DATA1_CRC_OFFSET   0x12c //CRC data1 from card/eMMC
#define SD_DATA0_CRC_OFFSET   0x130 //CRC data0 from card/eMMC
#define SD_CRC_STA_OFFSET   0x134   //response CRC from card/eMMC
#define SD_FIFO_OFFSET      0x200   //Read/Write FIFO


/*Get sdio 0,1,2 reg addr
@index, sdio interface index number
@offset, interface register offset
ret phy address of register*/
#define SDIO_REG(index,offset) (SDIO##index##_BASE_ADDR+offset)


#define SDIO_FIFO_ACCESS_MODE_BIT   (1<<31)     //1 AHB bus,0 DMA bus
#define SDIO_DDR_MOD_SELECT_BIT     (1<<10)     //1 DDR mode,0 SDR mode    
#define SDIO_CD_DBC_ENB_BIT         (1<<8)      //1 enable de-bounce, 0 disable 
#define SDIO_DMA_ENB_BIT            (1<<5)      //1 enable DMA to transfer data,0 AHB    
#define SDIO_INT_ENB_BIT            (1<<4)      //1 enable interrupt,0 disable interrupt
#define SDIO_DMA_RST_BIT            (1<<2)      //1 reset IDMAC
#define SDIO_FIFO_RST_BIT           (1<<1)      //1 reset fifo  
#define SDIO_SOFT_RST_BIT           (1<<0)      //1 reset SD/MMC controller


#define SD_CLKDIV_MASKDATA0_BIT     (1<<31)     //1 mask data0 when update clock;0 don't mask data0
#define SD_CLKDIV_CCLK_CTRL_BIT     (1<<17)     //1 turn off card clock when FSM in IDLE state
                                                //0 card clock always on
#define SD_CLKDIV_CCLK_ENB_BIT      (1<<16)     //0 card clock off,1 card clock on
#define SD_CLKDIV_CCLK_DIV_BIT      (0xff<<0)   //clk=2^n (1=0~255)

#define SD_TIMOUT_DTO_LMT           (0xfffffff<<8)
#define SD_TIMOUT_RTO_LMT           (0xff<<0)

#define SD_CTYPE                    (0x03<<0)   //2'b00 1bit; 2'b01 4bit;2'b1x 8bit
#define SD_BLKSIZ                   (0xffff<<0) //default 0x200
#define SD_BYTCNT                   (0xffffffff)


#define SD_CMD_CMD_LOAD_BIT         (1<<31)     //sdio start cmd
#define SD_CMD_USE_HOLD_REG_BIT     (1<<29)     //1,use hold register to send CMD and data to card
#define SD_CMD_VOL_SW_BIT           (1<<28)     //1 voltage switch command,set for cmd11 only,0 normal cmd
#define SD_CMD_BOOT_ABT_BIT         (1<<27)     //1 terminate the boot operation
#define SD_CMD_EXP_BOOT_ACK_BIT     (1<<26)     //expect one boot acknowledge
#define SD_CMD_BOOT_MODE_BIT        (3<<24)     //2'b00 normal command,2'b01 mandatory boot operation
                                                //2'b01 alternate boot operation
#define SD_CMD_PRG_CLK_BIT          (1<<21)     //0 normal command,1 change card clock
#define SD_CMD_SENT_INIT_SEQ        (1<<15)     //0 normal command,1 send initialization sequence
                                                //before send command
#define SD_CMD_STP_ABT_CMD           (1<<14)     //0 normal command,1 send stop or abort command to stop
                                                //current data transfer in progress
#define SD_CMD_WAIT_PRE_OVER_BIT    (1<<13) //0,send cmd at once,1 wait for data transfer completion 
                                            //before send current command  
#define SD_CMD_STOP_CMD_FLAG_BIT    (1<<12) //0 do not send stop command at the end of transfer,
                                            //1 automatically send stop command
#define SD_CMD_TRANS_MODE_BIT       (1<<11) //0 block data transfer command,1 stream data transfer command
#define SD_CMD_TRANS_DIR_BIT        (1<<10) //0 read operation,1 write operation
#define SD_CMD_CHK_RESP_CRC_BIT     (1<<8)  //0 don't check response crc,1 check crc
#define SD_CMD_DATA_TRANS_BIT       (1<<9)  //0 without data transfer,1 with data transfer
#define SD_CMD_LONG_RESP_BIT        (1<<7)  //0 short response (48bit),1 long response (136bit)
#define SD_CMD_RESP_RECV_BIT        (1<<6)  //0 command without response,1 command with response
#define SD_CMD_CMD_INDEX            (0x1f)  //sdio command index code



#define SD_INTMASK_CARD_REMOVE_BIT  (1<<31)
#define SD_INTMASK_CARD_INSERT_BIT  (1<<30)
#define SD_INTMASK_SDIO_INT_BIT     (1<<16)
#define SD_INTMASK_DATA_END_BIT_ERR (1<<15)
#define SD_INTMASK_AUTO_STOP_CMD_DONE    (1<<14)
#define SD_INTMASK_DATA_START_ERROR (1<<13)
#define SD_INTMASK_CMD_BUSY_ILL_WRITE   (1<<12)
#define SD_INTMASK_FIFO_UNDER_RUN_OV    (1<<11)
#define SD_INTMASK_DATAS_OT_1V8_SW_DONE (1<<10)
#define SD_INTMASK_DATA_OT_BOOT_DS  (1<<9)
#define SD_INTMASK_RESP_TO_BOOT_ACK (1<<8)
#define SD_INTMASK_DATA_CRC_ERR     (1<<7)
#define SD_INTMASK_RESP_CRC_ERR     (1<<6)
#define SD_INTMASK_
#endif