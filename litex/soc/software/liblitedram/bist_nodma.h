#ifndef __SDRAM_BIST_NODMA_H
#define __SDRAM_BIST_NODMA_H

#include <stdint.h>

void sdram_bist_pattern(uint32_t value);
void sdram_bist_info(void);
void sdram_bist(uint32_t base_addr, uint32_t length, uint32_t delay, int amode, int wmode, uint32_t err_max_cnt, int error_break);
void sdram_bist_writer(uint32_t length, uint32_t beg_addr);
void sdram_bist_reader(uint32_t length, uint32_t beg_addr, uint32_t err_max_cnt);

#endif /* __SDRAM_BIST_NODMA_H */