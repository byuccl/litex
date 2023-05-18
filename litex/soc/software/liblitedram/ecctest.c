#include <generated/csr.h>
#include <stdio.h>
#include "ecctest.h"

#if defined(CSR_ECC_READER_BASE)

void sdram_ecc_w(uint32_t address, uint32_t data, uint32_t count) {
    ecc_writer_address_sig_write(address);
    ecc_writer_data_sig_write(data);
    ecc_writer_counter_write(count);
    ecc_writer_start_write(1);
    printf("Started write ... ");
    while (ecc_writer_done_read() == 0) {
        ;
    }
    printf("Done\n");
    ecc_writer_start_write(0);
}

void sdram_nonecc_w(uint32_t address, uint32_t data, uint32_t count) {
    non_ecc_writer_address_sig_write(address);
    non_ecc_writer_data_sig_write(data);
    non_ecc_writer_counter_write(count);
    non_ecc_writer_start_write(1);
    printf("Started write ... ");
    while (non_ecc_writer_done_read() == 0) {
        ;
    }
    printf("Done\n");
    non_ecc_writer_start_write(0);
}

void sdram_ecc_r(uint32_t address, uint32_t count) {
    ecc_reader_address_sig_write(address);
    ecc_reader_counter_write(count);
    ecc_reader_data_handshake1_write(1);
    ecc_reader_start_write(1);
    printf("Started read ... ");
    while (ecc_reader_data_ready_read() == 0) {
        ;
    }
    printf("Done\n");
    while (1)
    {
        printf("Data: %08lx %08lx %08lx %08lx %08lx %08lx %08lx %08lx %08lx %08lx %08lx %08lx %08lx %08lx %08lx %08lx %08lx %08lx\n", 
            ecc_reader_data_status_17_read(),
            ecc_reader_data_status_16_read(),
            ecc_reader_data_status_15_read(),
            ecc_reader_data_status_14_read(),
            ecc_reader_data_status_13_read(),
            ecc_reader_data_status_12_read(),
            ecc_reader_data_status_11_read(),
            ecc_reader_data_status_10_read(),
            ecc_reader_data_status_9_read(),
            ecc_reader_data_status_8_read(),
            ecc_reader_data_status_7_read(),
            ecc_reader_data_status_6_read(),
            ecc_reader_data_status_5_read(),
            ecc_reader_data_status_4_read(),
            ecc_reader_data_status_3_read(),
            ecc_reader_data_status_2_read(),
            ecc_reader_data_status_1_read(),
            ecc_reader_data_status_0_read()
        );
        printf("Error counts: Sec: %08lx Ded: %08lx\n", sdram_eccr_sec_errors_read(), sdram_eccr_ded_errors_read());
        ecc_reader_data_handshake1_write(0);
        ecc_reader_data_handshake2_write(1);
        while((ecc_reader_done_read() == 0) && (ecc_reader_data_ready_read() == 1)) {
            ;
        }
        if (ecc_reader_done_read() == 1) {
            break;
        }
        ecc_reader_data_handshake2_write(0);
        ecc_reader_data_handshake1_write(1);
        while(ecc_reader_data_ready_read() == 0) {
            ;
        }
    }
    ecc_reader_data_handshake2_write(0);
    ecc_reader_data_handshake1_write(1);
    ecc_reader_start_write(0);
}

void addrstateami() {
    if (non_ecc_reader_read_addr_idle_fsm_read()) {
        printf("Addr idle state\n");
    } else if (non_ecc_reader_read_addr_run_fsm_read()) {
        printf("Addr run state\n");
    } else if (non_ecc_reader_read_addr_done_fsm_read()) {
        printf("Addr run state\n");
    }
}

void datastateami() {
    if (non_ecc_reader_read_data_idle_fsm_read()) {
        printf("Data idle state\n");
    } else if (non_ecc_reader_read_data_run_fsm_read()) {
        printf("Data run state\n");
    } else if (non_ecc_reader_read_data_wait_fsm_read()) {
        printf("Data wait state\n");
    } else if (non_ecc_reader_read_data_done_fsm_read()) {
        printf("Data done state\n");
    }
}

void sdram_nonecc_r(uint32_t address, uint32_t count) {
    non_ecc_reader_address_sig_write(address);
    non_ecc_reader_counter_write(count);
    non_ecc_reader_data_handshake1_write(1);
    non_ecc_reader_start_write(1);
    printf("Started read ... ");
    while (non_ecc_reader_data_ready_read() == 0) {
        addrstateami();
        datastateami();
    }
    printf("Done\n");
    while (1)
    {
        printf("Data: %08lx %08lx %08lx %08lx %08lx %08lx %08lx %08lx %08lx %08lx %08lx %08lx %08lx %08lx %08lx %08lx %08lx %08lx\n", 
            non_ecc_reader_data_status_17_read(),
            non_ecc_reader_data_status_16_read(),
            non_ecc_reader_data_status_15_read(),
            non_ecc_reader_data_status_14_read(),
            non_ecc_reader_data_status_13_read(),
            non_ecc_reader_data_status_12_read(),
            non_ecc_reader_data_status_11_read(),
            non_ecc_reader_data_status_10_read(),
            non_ecc_reader_data_status_9_read(),
            non_ecc_reader_data_status_8_read(),
            non_ecc_reader_data_status_7_read(),
            non_ecc_reader_data_status_6_read(),
            non_ecc_reader_data_status_5_read(),
            non_ecc_reader_data_status_4_read(),
            non_ecc_reader_data_status_3_read(),
            non_ecc_reader_data_status_2_read(),
            non_ecc_reader_data_status_1_read(),
            non_ecc_reader_data_status_0_read()
        );
        non_ecc_reader_data_handshake1_write(0);
        non_ecc_reader_data_handshake2_write(1);
        while((non_ecc_reader_done_read() == 0) && (non_ecc_reader_data_ready_read() == 1)) {
            ;
        }
        if (non_ecc_reader_done_read() == 1) {
            printf("\nDone reading\n");
            break;
        }
        non_ecc_reader_data_handshake2_write(0);
        non_ecc_reader_data_handshake1_write(1);
        while(non_ecc_reader_data_ready_read() == 0) {
            ;
        }
    }
    non_ecc_reader_data_handshake2_write(0);
    non_ecc_reader_data_handshake1_write(1);
    printf("Exited loop\n");
    non_ecc_reader_start_write(0);
    addrstateami();
    datastateami();
}

void sdram_ecc_clear_error_counts(void) {
    sdram_eccr_clear_write(1);
    sdram_eccr_clear_write(0);
}

#endif

// /* port_ecc */
// #define CSR_PORT_ECC_BASE (CSR_BASE + 0x3800L)
// #define CSR_PORT_ECC_ENABLE_ADDR (CSR_BASE + 0x3800L)
// #define CSR_PORT_ECC_ENABLE_SIZE 1
// static inline uint32_t port_ecc_enable_read(void) {
// 	return csr_read_simple((CSR_BASE + 0x3800L));
// }
// static inline void port_ecc_enable_write(uint32_t v) {
// 	csr_write_simple(v, (CSR_BASE + 0x3800L));
// }
// #define CSR_PORT_ECC_CLEAR_ADDR (CSR_BASE + 0x3804L)
// #define CSR_PORT_ECC_CLEAR_SIZE 1
// static inline uint32_t port_ecc_clear_read(void) {
// 	return csr_read_simple((CSR_BASE + 0x3804L));
// }
// static inline void port_ecc_clear_write(uint32_t v) {
// 	csr_write_simple(v, (CSR_BASE + 0x3804L));
// }
// #define CSR_PORT_ECC_SEC_ERRORS_ADDR (CSR_BASE + 0x3808L)
// #define CSR_PORT_ECC_SEC_ERRORS_SIZE 1
// static inline uint32_t port_ecc_sec_errors_read(void) {
// 	return csr_read_simple((CSR_BASE + 0x3808L));
// }
// #define CSR_PORT_ECC_DED_ERRORS_ADDR (CSR_BASE + 0x380cL)
// #define CSR_PORT_ECC_DED_ERRORS_SIZE 1
// static inline uint32_t port_ecc_ded_errors_read(void) {
// 	return csr_read_simple((CSR_BASE + 0x380cL));
// }



// #define CSR_PORT_ECC_W_NONECC_ADDRESS_ADDR (CSR_BASE + 0x3810L)
// #define CSR_PORT_ECC_W_NONECC_ADDRESS_SIZE 1
// static inline uint32_t port_ecc_w_nonecc_address_read(void) {
// 	return csr_read_simple((CSR_BASE + 0x3810L));
// }
// #define CSR_PORT_ECC_W_NONECC_DATA1_ADDR (CSR_BASE + 0x3814L)
// #define CSR_PORT_ECC_W_NONECC_DATA1_SIZE 1
// static inline uint32_t port_ecc_w_nonecc_data1_read(void) {
// 	return csr_read_simple((CSR_BASE + 0x3814L));
// }
// #define CSR_PORT_ECC_W_NONECC_DATA2_ADDR (CSR_BASE + 0x3818L)
// #define CSR_PORT_ECC_W_NONECC_DATA2_SIZE 1
// static inline uint32_t port_ecc_w_nonecc_data2_read(void) {
// 	return csr_read_simple((CSR_BASE + 0x3818L));
// }
// #define CSR_PORT_ECC_W_ECC_ADDRESS_ADDR (CSR_BASE + 0x381cL)
// #define CSR_PORT_ECC_W_ECC_ADDRESS_SIZE 1
// static inline uint32_t port_ecc_w_ecc_address_read(void) {
// 	return csr_read_simple((CSR_BASE + 0x381cL));
// }
// #define CSR_PORT_ECC_W_ECC_DATA1_ADDR (CSR_BASE + 0x3820L)
// #define CSR_PORT_ECC_W_ECC_DATA1_SIZE 1
// static inline uint32_t port_ecc_w_ecc_data1_read(void) {
// 	return csr_read_simple((CSR_BASE + 0x3820L));
// }
// #define CSR_PORT_ECC_W_ECC_DATA2_ADDR (CSR_BASE + 0x3824L)
// #define CSR_PORT_ECC_W_ECC_DATA2_SIZE 1
// static inline uint32_t port_ecc_w_ecc_data2_read(void) {
// 	return csr_read_simple((CSR_BASE + 0x3824L));
// }
// #define CSR_PORT_ECC_R_NONECC_ADDRESS_ADDR (CSR_BASE + 0x3828L)
// #define CSR_PORT_ECC_R_NONECC_ADDRESS_SIZE 1
// static inline uint32_t port_ecc_r_nonecc_address_read(void) {
// 	return csr_read_simple((CSR_BASE + 0x3828L));
// }
// #define CSR_PORT_ECC_R_NONECC_DATA1_ADDR (CSR_BASE + 0x382cL)
// #define CSR_PORT_ECC_R_NONECC_DATA1_SIZE 1
// static inline uint32_t port_ecc_r_nonecc_data1_read(void) {
// 	return csr_read_simple((CSR_BASE + 0x382cL));
// }
// #define CSR_PORT_ECC_R_NONECC_DATA2_ADDR (CSR_BASE + 0x3830L)
// #define CSR_PORT_ECC_R_NONECC_DATA2_SIZE 1
// static inline uint32_t port_ecc_r_nonecc_data2_read(void) {
// 	return csr_read_simple((CSR_BASE + 0x3830L));
// }
// #define CSR_PORT_ECC_R_ECC_ADDRESS_ADDR (CSR_BASE + 0x3834L)
// #define CSR_PORT_ECC_R_ECC_ADDRESS_SIZE 1
// static inline uint32_t port_ecc_r_ecc_address_read(void) {
// 	return csr_read_simple((CSR_BASE + 0x3834L));
// }
// #define CSR_PORT_ECC_R_ECC_DATA1_ADDR (CSR_BASE + 0x3838L)
// #define CSR_PORT_ECC_R_ECC_DATA1_SIZE 1
// static inline uint32_t port_ecc_r_ecc_data1_read(void) {
// 	return csr_read_simple((CSR_BASE + 0x3838L));
// }
// #define CSR_PORT_ECC_R_ECC_DATA2_ADDR (CSR_BASE + 0x383cL)
// #define CSR_PORT_ECC_R_ECC_DATA2_SIZE 1
// static inline uint32_t port_ecc_r_ecc_data2_read(void) {
// 	return csr_read_simple((CSR_BASE + 0x383cL));
// }
// #define CSR_PORT_ECC_R_TRANSACTION_OCCURED_ADDR (CSR_BASE + 0x3840L)
// #define CSR_PORT_ECC_R_TRANSACTION_OCCURED_SIZE 1
// static inline uint32_t port_ecc_r_transaction_occured_read(void) {
// 	return csr_read_simple((CSR_BASE + 0x3840L));
// }
// #define CSR_PORT_ECC_W_TRANSACTION_OCCURED_ADDR (CSR_BASE + 0x3844L)
// #define CSR_PORT_ECC_W_TRANSACTION_OCCURED_SIZE 1
// static inline uint32_t port_ecc_w_transaction_occured_read(void) {
// 	return csr_read_simple((CSR_BASE + 0x3844L));
// }

void sdram_ecc_info(void) {

    printf("Write nonecc transaction occured (1 for yes, 0 for no): %ld\n", port_ecc_w_transaction_occured_read());
    printf("Write nonecc side address: %lx\n", port_ecc_w_nonecc_address_read());
    printf("Write nonecc side data (first 72 of 576 bits): %lx %lx %lx\n", port_ecc_w_nonecc_data3_read(), port_ecc_w_nonecc_data2_read(), port_ecc_w_nonecc_data1_read());
    printf("Write ecc side address: %lx\n", port_ecc_w_ecc_address_read());
    printf("Write ecc side data (first 64 bits of 512): %lx %lx\n\n", port_ecc_w_ecc_data2_read(), port_ecc_w_ecc_data1_read());

    printf("Read nonecc transaction occured (1 for yes, 0 for no): %ld\n", port_ecc_r_transaction_occured_read());
    printf("Read nonecc side address: %lx\n", port_ecc_r_nonecc_address_read());
    printf("Read nonecc side data (first 72 of 576 bits): %lx %lx %lx\n", port_ecc_r_nonecc_data3_read(), port_ecc_r_nonecc_data2_read(), port_ecc_r_nonecc_data1_read());
    printf("Read ecc side address: %lx\n", port_ecc_r_ecc_address_read());
    printf("Read ecc side data (first 64 bits of 512): %lx %lx\n", port_ecc_r_ecc_data2_read(), port_ecc_r_ecc_data1_read());

}