#include "types.h"
#include "defs.h"
#include "param.h"
#include "arm.h"
#include "memlayout.h"
#include "mmu.h"


static volatile uint* gic_base;
// static volatile uint* gic_cpu_base;

#define SGI_TYPE		1
#define PPI_TYPE		2
#define SPI_TYPE		3

#define GICD_CTLR		0x000
#define GICD_TYPER		0x004
#define GICD_IIDR		0x008

#define GICD_IGROUP		0x080
#define GICD_ISENABLE		0x100
#define GICD_ICENABLE		0x180
#define GICD_ISPEND		0x200
#define GICD_ICPEND		0x280
#define GICD_ISACTIVE		0x300
#define GICD_ICACTIVE		0x380
#define GICD_IPRIORITY		0x400
#define GICD_ITARGET		0x800
#define GICD_ICFG		0xC00

#define GICC_CTLR		0x000
#define GICC_PMR		0x004
#define GICC_BPR		0x008
#define GICC_IAR		0x00C
#define GICC_EOIR		0x010
#define GICC_RRR		0x014
#define GICC_HPPIR		0x018

#define GICC_ABPR		0x01C
#define GICC_AIAR		0x020
#define GICC_AEOIR		0x024
#define GICC_AHPPIR		0x028

#define GICC_APR		0x0D0
#define GICC_NSAPR		0x0E0
#define GICC_IIDR		0x0FC
#define GICC_DIR		0x1000 /* v2 only */



#define GICD_REG(o)		(*(uint *)(((uint) gic_base) + 0x1000 + o))
#define GICC_REG(o)		(*(uint *)(((uint) gic_base) + 0x2000 + o))
/* #define GICD_REGOFF(o1, o2)	(*(uint *)(((uint) gic_base) + 0x1000 + o1 + o2)) */

/* to supress warning message **/
#define static 
#if 0

static void gd_spi_setmode(int spi, int is_edge) 
{
	int id = spi2id(spi);
	int offset = (id*2)/32;
	int bitpos = ((id*2)%32)+ 1;
	uint val = GICD_REGOFF(GICD_ICFGR, offset);
	if(is_edge)
		val |= is_edge << bitpos;
	else
		val &= ~(1<< bitpos);
	GICD_REG(GICD_ICFGR, offset) = val;
}
/*
 * set target to cpu0
 */
static void gd_spi_target0(int spi)
{
	#warning empty
	int id = spi2id(spi);
}

/*
 * set target to group0
 */
static void gd_spi_target0(int spi)
{
	#warning empty
	int id = spi2id(spi);
}



/*
 * get signaled interrupt and also ack it thru cpu interface
 * no handle sgi in this function
 */
int get_ack() {
	int id = GIC_REG(GICC_IAR) & 0x3FF; /* higher is cpu id of sgi */
	if (id == 1023)
		return 0;
	return id;
}
/*
 * set eoi thru cpu interface, if sgi, the id should include cpuid
 */
int set_eoi(int id)
{
	GIC_REG(GICC_EOIR) = id;
}


int int_enable(int id)
{

}
#else
/*  id is m
 *  offset n= m DIV 32
 *  bit    pos = m MOD 32;
 */
static void gicd_set_bit(int base, int id, int bval) {
	int offset = id/32;
	int bitpos = id%32;
	uint rval = GICD_REG(base+4*offset);
	if(bval)
		rval |= 1 << bitpos;
	else
		rval &= ~(1<< bitpos);
	GICD_REG(base+ 4*offset) = rval;
}


void gicc_set_bit(int base, int id, int bval) {
	int offset = id/32;
	int bitpos = id%32;
	uint rval = GICC_REG(base+4*offset);
	if(bval)
		rval |= 1 << bitpos;
	else
		rval &= ~(1<< bitpos);
	GICC_REG(base+ 4*offset) = rval;
}

static int spi2id(int spi)
{
	return spi+32;
}

static void gd_spi_enable(int spi)
{
	int id = spi2id(spi);
	gicd_set_bit(GICD_ISENABLE, id, 1);
}

/* By default, SPI is group 0
 * GIC spec ch4.3.4
 *
 */
static void gd_spi_group0(int spi)
{
	return;

}

/* set target processor
 */
static void gd_spi_target0(int spi)
{
	int id=spi2id(spi);
	int offset = id/4;
	int bitpos = (id%4)*8;
	uint rval = GICD_REG(GICD_ITARGET+4*offset);
	unsigned char tcpu=0x01;
	rval |= tcpu << bitpos;
	GICD_REG(GICD_ITARGET+ 4*offset) = rval;	
}
/* set cfg
 */
static void gd_spi_setcfg(int spi, int is_edge)
{
	int id=spi2id(spi);
	int offset = id/16;
	int bitpos = (id%16)*2;
	uint rval = GICD_REG(GICD_ICFG+4*offset);
	uint vmask=0x03;
	rval &= ~(vmask << bitpos);
	if (is_edge)
		rval |= 0x02 << bitpos;
	GICD_REG(GICD_ICFG+ 4*offset) = rval;	
}
/*
 * TODO: process itype here
 */
static void gic_dist_configure(int itype, int num)
{
	int spi= num;
	gd_spi_setcfg(spi, 1);
	gd_spi_enable(spi);
	gd_spi_group0(spi);
	gd_spi_target0(spi);
}

/*
 * initial every spi pin 
 */
static void gic_dist_init() 
{
	cprintf("gic type: 0x%x\n", GICD_REG(GICD_TYPER));
}

static void gic_cpu_init() 
{
	cprintf("gic cpuif type:0x%x\n", GICC_REG(GICC_IIDR));
	GICC_REG(GICC_PMR) = 0x0f; /* priority value 0 to 0xe is supported */

}


#endif

/* enable group 0 only
 */
static void gic_enable()
{
	GICD_REG(GICD_CTLR) |= 1;
	GICC_REG(GICC_CTLR) |= 1;
}

/* disable group 0 only
 */
static void gic_disable()
{
	GICD_REG(GICD_CTLR) &= ~(uint)1;
	GICD_REG(GICC_CTLR) &= ~(uint)1;
}
/* configure and enable interrupt
 */
static void gic_configure(int itype, int num)
{
	gic_dist_configure(itype, num);
}
/*
 * This section init gic according to CORTEX A15 reference manual
 * 8.3.1 distributor register and 8.3.2 
 */
void gic_init(void * base)
{
	gic_base = base;
	gic_dist_init();
	gic_cpu_init();

	gic_configure(SPI_TYPE, PIC_TIMER01);
	gic_configure(SPI_TYPE, PIC_TIMER23);
	gic_configure(SPI_TYPE, PIC_UART0);
	gic_enable();
}


void pic_enable (int n, ISR isr)
{

}

// dispatch the interrupt
void pic_dispatch (struct trapframe *tp)
{
	cprintf("pic dispatch\n");
/*    uint intstatus;
    int		i;

    intstatus = vic_base[VIC_IRQSTATUS];

    for (i = 0; i < NUM_INTSRC; i++) {
        if (intstatus & (1<<i)) {
            isrs[i](tp, i);
        }
    }

    intstatus = vic_base[VIC_IRQSTATUS];  */
}

