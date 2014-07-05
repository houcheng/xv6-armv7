//
// Board specific information for the VersatilePB board
//
#ifndef VERSATILEPB
#define VERSATILEPB


// the VerstatilePB board can support up to 256MB memory.
// but we assume it has 128MB instead. During boot, the lower
// 64MB memory is mapped to the flash, needs to be remapped
// the the SDRAM. We skip this for QEMU
#define PHYSTOP         (0x08000000 + PHY_START)
#define BSP_MEMREMAP    0x04000000

#define DEVBASE1        0x1c000000
#define DEVBASE2        0x2c000000
#define DEV_MEM_SZ      0x01000000
#define VEC_TBL         0xFFFF0000


#define STACK_FILL      0xdeadbeef

#define UART0           0x1c090000
#define UART_CLK        24000000    // Clock rate for UART

#define TIMER0          0x1c110000
#define TIMER1          0x1c120000
#define CLK_HZ          1000000     // the clock is 1MHZ

#define VIC_BASE        0x2c002000
#define PIC_TIMER01     4
#define PIC_TIMER23     5
#define PIC_UART0       12
#define PIC_GRAPHIC     19

#endif
