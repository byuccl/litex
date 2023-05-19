#ifndef __SDRAM_BIST_NODMA_H
#define __SDRAM_BIST_NODMA_H

#include <stdint.h>

void sdram_bist_pattern(uint32_t value);
void sdram_bist_info(void);
void sdram_bist(uint32_t length, uint32_t delay, int amode, int wmode);
void sdram_bist_writer(uint32_t length, uint32_t beg_addr);
void sdram_bist_reader(uint32_t length, uint32_t beg_addr);

#endif /* __SDRAM_BIST_NODMA_H */