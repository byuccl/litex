








#include <generated/csr.h>
#include <stdio.h>
#include <stdint.h>
#include <uart.h>
#include <time.h>
#include <console.h>

#include <generated/mem.h> /* MAIN_RAM_BASE, MAIN_RAM_SIZE */
#include <generated/soc.h> /* CONFIG_BIST_DATA_WIDTH */
#include <libbase/memtest.h> /* memtest_inject_errors */
#include <liblitedram/bist_nodma.h>

#define PRINT_TITLE_NUM_CYCLES 10
#define ACKNOWLEDGE_FLAG 1
#define LOWER_FLAG 0
#define START_BIST 1
#define BYTE_INTERVALS 32
#define CSR_DEFAULT_VAL 0

#define CYCLES_UNTIL_DATA_DISPLAYED 100000
#define CYCLES_DELAY_READ 100000000



typedef uint32_t(*IntFuncVoid)(void);

// CSR registers to get data
IntFuncVoid data_output[] = {
    bist_nodma_output_data_pattern1_read,
    bist_nodma_output_data_pattern2_read,
    bist_nodma_output_data_pattern3_read,
    bist_nodma_output_data_pattern4_read,
    bist_nodma_output_data_pattern5_read,
    bist_nodma_output_data_pattern6_read,
    bist_nodma_output_data_pattern7_read,
    bist_nodma_output_data_pattern8_read,
    bist_nodma_output_data_pattern9_read,
    bist_nodma_output_data_pattern10_read,
    bist_nodma_output_data_pattern11_read,
    bist_nodma_output_data_pattern12_read,
    bist_nodma_output_data_pattern13_read,
    bist_nodma_output_data_pattern14_read,
    bist_nodma_output_data_pattern15_read,
    bist_nodma_output_data_pattern16_read,
    bist_nodma_output_data_pattern17_read,
    bist_nodma_output_data_pattern18_read
};


void sdram_bist_pattern(uint32_t value) {
    bist_nodma_input_data_pattern_write(value);
}

void sdram_bist_info(void) {
    printf("\nBist port address width: %lu\nBist port data width: %lu\n", 
    bist_nodma_bist_port_addr_width_read(), 
    bist_nodma_bist_port_data_width_read());
}



void sdram_bist_writer(uint32_t length, uint32_t beg_addr) {
    bist_nodma_write_only_mode_write(ACKNOWLEDGE_FLAG);
    bist_nodma_length_address_write(length);
    bist_nodma_base_address_write(beg_addr);
    printf("DRAM controller has address width %lu, data width %lu in bits\n", bist_nodma_bist_port_addr_width_read(), bist_nodma_bist_port_data_width_read());
    printf("Writing from address %lx to address %lx ...", bist_nodma_base_address_read(), bist_nodma_length_address_read());

    bist_nodma_start_write(START_BIST);

    while(bist_nodma_writer_finished_state_read() == 0) {
        ;
    }

    printf("Done\n");

    // Print stats
    printf(" WRITE TICKS   READ TICKS TOTAL WRITES  TOTAL READS  WR-SPEED(MiB/s)  RD-SPEED(MiB/s)      ADDRESSES TESTED     ERRORS\n");

    // Write speed = Number of writes * Number of bytes per write * Clock frequency in MHz / Number of clock cycles for the write
    uint64_t write_speed = 0;
    // Read speed = Number of reads * Number of bytes per read * Clock frequency in MHz / Number of clock cycles for the read
    uint64_t read_speed = 0;

    // Only calculate speeds if number of ticks is greater than zero.
    // Only calculate speeds if number of ticks is greater than zero.
    if (bist_nodma_write_ticks_read() > 0) {
        write_speed = (uint64_t)((uint64_t)(bist_nodma_total_writes_read() * (uint64_t)(bist_nodma_bist_port_data_width_read() / 8) * (uint64_t)(CONFIG_CLOCK_FREQUENCY/(1024 * 1024))) / (uint64_t)bist_nodma_write_ticks_read());
    }
    if (bist_nodma_read_ticks_read() > 0) {
        read_speed = (uint64_t)((uint64_t)(bist_nodma_total_reads_read() * (uint64_t)(bist_nodma_bist_port_data_width_read() / 8) * (uint64_t)(CONFIG_CLOCK_FREQUENCY/(1024 * 1024))) / (uint64_t)bist_nodma_read_ticks_read());
    }
    

    printf("%12lu %12lu %12lu %12lu %16llu %16llu   0x%07lx-0x%07lx %10lu\n", 
        bist_nodma_write_ticks_read(), 
        bist_nodma_read_ticks_read(), 
        bist_nodma_total_writes_read(), 
        bist_nodma_total_reads_read(),
        write_speed,
        read_speed,
        bist_nodma_beginning_address_read(),
        bist_nodma_ending_address_read(),
        bist_nodma_error_counter_read());
    

    printf("Finishing state machine ...");

    bist_nodma_start_write(LOWER_FLAG);
    bist_nodma_write_only_mode_write(LOWER_FLAG);
    bist_nodma_writer_finished_acknowledge_write(ACKNOWLEDGE_FLAG);

    while(bist_nodma_bist_idle_read() == 0) {
        ;
    }

    bist_nodma_writer_finished_acknowledge_write(LOWER_FLAG);

    printf("Done\n");
}



void sdram_bist_reader(uint32_t length, uint32_t beg_addr) {
    bist_nodma_reader_only_mode_write(ACKNOWLEDGE_FLAG);
    bist_nodma_length_address_write(length);
    bist_nodma_base_address_write(beg_addr);
    printf("DRAM controller has address width %lu, data width %lu in bits\n", bist_nodma_bist_port_addr_width_read(), bist_nodma_bist_port_data_width_read());
    printf("Reading from address %lx to address %lx\n", bist_nodma_base_address_read(), bist_nodma_base_address_read() + bist_nodma_length_address_read());

    bist_nodma_start_write(START_BIST);

    printf("Current address:\n");

    // uint64_t index = 0;
    while(bist_nodma_reader_finished_state_read() == 0) {

        if (bist_nodma_error_found_flag_read()) {
            
            printf("ERROR at address 0x%07lx: data read: ", bist_nodma_current_address_read());

            for (int i = 0; i < sizeof(data_output) / sizeof(*data_output); i++) 
            {
                if (bist_nodma_bist_port_data_width_read() >= ((i * BYTE_INTERVALS) + BYTE_INTERVALS)) {
                    printf("%8lx ", data_output[i]());
                }
            }

            printf("\n                             Expected: ");

            for (int i = 0; i < sizeof(data_output) / sizeof(*data_output); i++) 
            {
                if (bist_nodma_bist_port_data_width_read() >= ((i * BYTE_INTERVALS) + BYTE_INTERVALS)) {
                    printf("%8lx ", bist_nodma_input_data_pattern_read());
                }
            }

            printf("\n");

            bist_nodma_error_acknowledge_flag_write(ACKNOWLEDGE_FLAG);
            bist_nodma_error_acknowledge_flag_write(LOWER_FLAG);

        } 
        
        // else if (index % CYCLES_UNTIL_DATA_DISPLAYED == 0) {
        //     printf("%lx\n", bist_nodma_current_address_read());
        // }
        // ++index;
    }

    printf("Done reading\n");

    // Print stats
    printf(" WRITE TICKS   READ TICKS TOTAL WRITES  TOTAL READS  WR-SPEED(MiB/s)  RD-SPEED(MiB/s)      ADDRESSES TESTED     ERRORS\n");

    // Write speed = Number of writes * Number of bytes per write * Clock frequency in MHz / Number of clock cycles for the write
    uint64_t write_speed = 0;
    // Read speed = Number of reads * Number of bytes per read * Clock frequency in MHz / Number of clock cycles for the read
    uint64_t read_speed = 0;

    // Only calculate speeds if number of ticks is greater than zero.
    if (bist_nodma_write_ticks_read() > 0) {
        write_speed = (uint64_t)((uint64_t)(bist_nodma_total_writes_read() * (uint64_t)(bist_nodma_bist_port_data_width_read() / 8) * (uint64_t)(CONFIG_CLOCK_FREQUENCY/(1024 * 1024))) / (uint64_t)bist_nodma_write_ticks_read());
    }
    if (bist_nodma_read_ticks_read() > 0) {
        read_speed = (uint64_t)((uint64_t)(bist_nodma_total_reads_read() * (uint64_t)(bist_nodma_bist_port_data_width_read() / 8) * (uint64_t)(CONFIG_CLOCK_FREQUENCY/(1024 * 1024))) / (uint64_t)bist_nodma_read_ticks_read());
    }

    printf("%12lu %12lu %12lu %12lu %16llu %16llu   0x%07lx-0x%07lx %10lu\n", 
        bist_nodma_write_ticks_read(), 
        bist_nodma_read_ticks_read(), 
        bist_nodma_total_writes_read(), 
        bist_nodma_total_reads_read(),
        write_speed,
        read_speed,
        bist_nodma_beginning_address_read(),
        bist_nodma_ending_address_read(),
        bist_nodma_error_counter_read());

    printf("Finishing state machine ...");

    bist_nodma_start_write(LOWER_FLAG);
    bist_nodma_reader_only_mode_write(LOWER_FLAG);
    bist_nodma_reader_finished_acknowledge_write(ACKNOWLEDGE_FLAG);

    while(bist_nodma_bist_idle_read() == 0) {
        ;
    }

    bist_nodma_reader_finished_acknowledge_write(LOWER_FLAG);

    printf("Done\n");
    

}



void sdram_bist(uint32_t length, uint32_t delay, int amode, int wmode) {
    bist_nodma_length_address_write(length);
    bist_nodma_address_mode_write(amode);
    bist_nodma_wr_mode_write(wmode);

    uint32_t start_address = 0x0;

    bist_nodma_base_address_write(start_address);
    // bist_nodma_end_address_write(0x1f);
    // bist_nodma_length_address_write(0xf);
    bist_nodma_seconds_delay_write(delay);


    // Print title
    printf("DRAM controller has address width %lu, data width %lu in bits\n", bist_nodma_bist_port_addr_width_read(), bist_nodma_bist_port_data_width_read());
    printf("Starting Bist with length %lu, address mode %d, wmode %d\n", length, amode, wmode);
    
    bist_nodma_start_write(START_BIST);

    int i = 0;
    for (;;){

        // Exit on key pressed 
		if (readchar_nonblock()) {
            break;
        }

        // Every 8 times 
        if(i % (PRINT_TITLE_NUM_CYCLES + 1) == 0) {
            printf(" WRITE TICKS   READ TICKS TOTAL WRITES  TOTAL READS  WR-SPEED(MiB/s)  RD-SPEED(MiB/s)      ADDRESSES TESTED     ERRORS\n");
            i++;
        }

        if (bist_nodma_data_pause_display_flag_read()) {
            i++;

            // Write speed = Number of writes * Number of bytes per write * Clock frequency in MHz / Number of clock cycles for the write
            uint64_t write_speed = 0;
            // Read speed = Number of reads * Number of bytes per read * Clock frequency in MHz / Number of clock cycles for the read
            uint64_t read_speed = 0;

            // Only calculate speeds if number of ticks is greater than zero.
            // Only calculate speeds if number of ticks is greater than zero.
    if (bist_nodma_write_ticks_read() > 0) {
        write_speed = (uint64_t)((uint64_t)(bist_nodma_total_writes_read() * (uint64_t)(bist_nodma_bist_port_data_width_read() / 8) * (uint64_t)(CONFIG_CLOCK_FREQUENCY/(1024 * 1024))) / (uint64_t)bist_nodma_write_ticks_read());
    }
    if (bist_nodma_read_ticks_read() > 0) {
        read_speed = (uint64_t)((uint64_t)(bist_nodma_total_reads_read() * (uint64_t)(bist_nodma_bist_port_data_width_read() / 8) * (uint64_t)(CONFIG_CLOCK_FREQUENCY/(1024 * 1024))) / (uint64_t)bist_nodma_read_ticks_read());
    }

            printf("%12lu %12lu %12lu %12lu %16llu %16llu   0x%07lx-0x%07lx %10lu\n", 
                bist_nodma_write_ticks_read(), 
                bist_nodma_read_ticks_read(), 
                bist_nodma_total_writes_read(), 
                bist_nodma_total_reads_read(),
                write_speed,
                read_speed,
                bist_nodma_beginning_address_read(),
                bist_nodma_ending_address_read(),
                bist_nodma_error_counter_read());

            bist_nodma_data_acknowledge_flag_write(ACKNOWLEDGE_FLAG);
            bist_nodma_data_acknowledge_flag_write(LOWER_FLAG);

        } else if (bist_nodma_error_found_flag_read()) {
            
            printf("ERROR at address 0x%07lx: data read: ", bist_nodma_current_address_read());

            for (int i = 0; i < sizeof(data_output) / sizeof(*data_output); i++) 
            {
                if (bist_nodma_bist_port_data_width_read() >= ((i * BYTE_INTERVALS) + BYTE_INTERVALS)) {
                    printf("%lx ", data_output[i]());
                }
            }

            printf("\n                          Expected: ");

            for (int i = 0; i < sizeof(data_output) / sizeof(*data_output); i++) 
            {
                if (bist_nodma_bist_port_data_width_read() >= ((i * BYTE_INTERVALS) + BYTE_INTERVALS)) {
                    printf("%lx ", bist_nodma_input_data_pattern_read());
                }
            }

            printf("\n");

            bist_nodma_error_acknowledge_flag_write(ACKNOWLEDGE_FLAG);
            bist_nodma_error_acknowledge_flag_write(LOWER_FLAG);
            

        }

    }

    // Deactivate the state machine
    bist_nodma_start_write(0);

    // Wait for the state machine to finish
    while (bist_nodma_bist_idle_read() == 0) 
        ;
    
}