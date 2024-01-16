#ifndef CORTEX_M_REGS_H
#define CORTEX_M_REGS_H

#define EXPINFO_CAPTURE_REGISTERS( xCoreReg ) \
    {\
        register int regR0 asm("r0");\
        register int regR1 asm("r1");\
        register int regR2 asm("r2");\
        register int regR3 asm("r3");\
        register int regR4 asm("r4");\
        register int regR5 asm("r5");\
        register int regR6 asm("r6");\
        register int regR7 asm("r7");\
        register int regR8 asm("r8");\
        register int regR9 asm("r9");\
        register int regR10 asm("r10");\
        register int regR11 asm("r11");\
        register int regR12 asm("r12");\
        register int regSp asm("sp");\
        register int regLr asm("lr");\
        register int regD0 asm("d0");\
        register int regD1 asm("d1");\
        register int regD2 asm("d2");\
        register int regD3 asm("d3");\
        register int regD4 asm("d4");\
        register int regD5 asm("d5");\
        register int regD6 asm("d6");\
        register int regD7 asm("d7");\
        register int regD8 asm("d8");\
        register int regD9 asm("d9");\
        register int regD10 asm("d10");\
        register int regD11 asm("d11");\
        register int regD12 asm("d12");\
        register int regD13 asm("d13");\
        register int regD14 asm("d14");\
        register int regD15 asm("d15");\
        register uint32_t result; \
        asm volatile ("mov %0, pc\n" : "=r" (result) ); \
        xCoreReg.ulRegR0 = regR0;\
        xCoreReg.ulRegR1 = regR1;\
        xCoreReg.ulRegR2 = regR2;\
        xCoreReg.ulRegR3 = regR3;\
        xCoreReg.ulRegR4 = regR4;\
        xCoreReg.ulRegR5 = regR5;\
        xCoreReg.ulRegR6 = regR6;\
        xCoreReg.ulRegR7 = regR7;\
        xCoreReg.ulRegR8 = regR8;\
        xCoreReg.ulRegR9 = regR9;\
        xCoreReg.ulRegR10 = regR10;\
        xCoreReg.ulRegR11 = regR11;\
        xCoreReg.ulRegR12 = regR12;\
        xCoreReg.ulRegSp = regSp;\
        xCoreReg.ulRegLr = regLr;\
        xCoreReg.ulRegPc = result;\
        xCoreReg.ulRegD0 = regD0;\
        xCoreReg.ulRegD1 = regD1;\
        xCoreReg.ulRegD2 = regD2;\
        xCoreReg.ulRegD3 = regD3;\
        xCoreReg.ulRegD4 = regD4;\
        xCoreReg.ulRegD5 = regD5;\
        xCoreReg.ulRegD6 = regD6;\
        xCoreReg.ulRegD7 = regD7;\
        xCoreReg.ulRegD8 = regD8;\
        xCoreReg.ulRegD9 = regD9;\
        xCoreReg.ulRegD10 = regD10;\
        xCoreReg.ulRegD11 = regD11;\
        xCoreReg.ulRegD12 = regD12;\
        xCoreReg.ulRegD13 = regD13;\
        xCoreReg.ulRegD14 = regD14;\
        xCoreReg.ulRegD15 = regD15;\
    }

/* TODO: Below registers are not able to access like this method.
	register int regXpsr asm("xPSR");\
	register int regFpscr asm("fpscr");\
	register int regPrimask asm("primask");\
	register int regBasepri asm("basepri");\
	register int regFaultmask asm("faultmask");\
	register int regControl asm("control");\
	register uint32_t msp __asm("msp");\
	register uint32_t regPsp __asm("psp");\
	ulRegXpsr = regXpsr;\
	ulRegFpscr = regFpscr;\
	ulRegPrimask = regPrimask;\
	ulRegBasepri = regBasepri;\
	ulRegFaultmask = regFaultmask;\
	ulRegControl = regControl;\
	ulRegMsp = msp;\
	ulRegPsp = regPsp;\
 */

/*-----------------------------------------------------------*/

/* File structure. */
typedef struct CoreReg
{
    uint32_t ulRegR0;
    uint32_t ulRegR1;
    uint32_t ulRegR2;
    uint32_t ulRegR3;
    uint32_t ulRegR4;
    uint32_t ulRegR5;
    uint32_t ulRegR6;
    uint32_t ulRegR7;
    uint32_t ulRegR8;
    uint32_t ulRegR9;
    uint32_t ulRegR10;
    uint32_t ulRegR11;
    uint32_t ulRegR12;
    uint32_t ulRegSp;
    uint32_t ulRegLr;
    uint32_t ulRegPc;
    uint32_t ulRegXpsr;
    uint32_t ulRegD0;
    uint32_t ulRegD1;
    uint32_t ulRegD2;
    uint32_t ulRegD3;
    uint32_t ulRegD4;
    uint32_t ulRegD5;
    uint32_t ulRegD6;
    uint32_t ulRegD7;
    uint32_t ulRegD8;
    uint32_t ulRegD9;
    uint32_t ulRegD10;
    uint32_t ulRegD11;
    uint32_t ulRegD12;
    uint32_t ulRegD13;
    uint32_t ulRegD14;
    uint32_t ulRegD15;
    uint32_t ulRegFpscr;
    uint32_t ulRegPrimask;
    uint32_t ulRegBasepri;
    uint32_t ulRegFaultmask;
    uint32_t ulRegControl;
    uint32_t ulRegMsp;
    uint32_t ulRegPsp;
} CoreReg_t;

#endif /* CORTEX_M_REGS_H */
