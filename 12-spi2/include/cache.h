#ifndef __CACHE_H__
#define __CACHE_H__


void enable_icache(void);
void enable_dcache(void);
void disable_icache(void);
void disable_dcache(void);
void flush_dcache(void);
void flush_icache(void);
void enable_prediction(void);
#endif
