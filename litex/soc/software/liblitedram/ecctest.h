// This file is Copyright (c) 2018-2020 Florent Kermarrec <florent@enjoy-digital.fr>
// License: BSD

#ifndef __ECC_TEST_H
#define __ECC_TEST_H

#include <stdint.h>

#if defined(CSR_ECC_READER_BASE)
void sdram_ecc_w(uint32_t address, uint32_t data, uint32_t count);
void sdram_nonecc_w(uint32_t address, uint32_t data, uint32_t count);
void sdram_ecc_r(uint32_t address, uint32_t count);
void sdram_nonecc_r(uint32_t address, uint32_t count);
void sdram_ecc_clear_error_counts(void);
#endif
void sdram_ecc_info(void);


#endif /* __SDRAM_BIST_H */
