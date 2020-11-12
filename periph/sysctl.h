/*
 * sysctl.h
 *
 *  Created on: July 29, 2018
 *      Author: khughes
 *
 * System Control register definitions.  See Chapter 5 of the TM4C1294 
 * datasheet for complete information.
 */

#ifndef _SYSCTL_H
#define _SYSCTL_H

#include <stdint.h>

// Peripheral base addresses.
#define SYSCTL          (((volatile uint32_t *)0x400fe000))

// Peripheral register offsets and special fields.
enum {
    //declared for lab 5, pg 250 on datasheet
    SYSCTL_RCGCSSI = (0x61C >> 2),
#define SYSCTL_RCGCSSI_SSI3            (1<<3)

    SYSCTL_RCGCGPIO = (0x608 >> 2),
#define SYSCTL_RCGCGPIO_PORTA          (1<<0) //port A
#define SYSCTL_RCGCGPIO_PORTB          (1<<1) //Port B
#define SYSCTL_RCGCGPIO_PORTC          (1<<2) //Port C
#define SYSCTL_RCGCGPIO_PORTD          (1<<3) //Port D
#define SYSCTL_RCGCGPIO_PORTE          (1<<4) //Port E
#define SYSCTL_RCGCGPIO_PORTF          (1<<5) //Port E
#define SYSCTL_RCGCGPIO_PORTG          (1<<6) //Port E
#define SYSCTL_RCGCGPIO_PORTH          (1<<7) //Port E
#define SYSCTL_RCGCGPIO_PORTJ          (1<<8) //Port J
#define SYSCTL_RCGCGPIO_PORTK          (1<<9)
#define SYSCTL_RCGCGPIO_PORTL          (1<<10)
#define SYSCTL_RCGCGPIO_PORTM          (1<<11) //Port M
#define SYSCTL_RCGCGPIO_PORTN          (1<<12) //Port N
#define SYSCTL_RCGCGPIO_PORTP          (1<<13) //Port N
#define SYSCTL_RCGCGPIO_PORTQ          (1<<14) //Port N

    //this was a new declaration for lab 02
    SYSCTL_RCGCTIMER = (0x604 >> 2),

#define SYSCTL_RCGCTIMER_3              (1<<3)
    //defined bit field 3
    //Port D: 3
    //Port E: 4
#define SYSCTL_RCGCTIMER_2A              (1<<2)

  SYSCTL_RIS =            (0x050 >> 2),
#define   SYSCTL_RIS_MOSCPUPRIS   (1<<8)  // MOSC Power Up Raw Interrupt Status
  SYSCTL_MISC =           (0x058 >> 2),
#define   SYSCTL_MISC_MOSCPUMIS   (1<<8)  // MOSC Power Up Masked Interrupt Status
  SYSCTL_MOSCCTL =        (0x07C >> 2),
#define   SYSCTL_MOSCCTL_OSCRNG   (1<<4) // Oscillator Range
#define   SYSCTL_MOSCCTL_PWRDN    (1<<3) // Power Down
#define   SYSCTL_MOSCCTL_NOXTAL   (1<<2) // No Crystal Connected
#define   SYSCTL_MOSCCTL_MOSCIM   (1<<1) // MOSC Failure Action
#define   SYSCTL_MOSCCTL_CVAL     (1<<0) // Clock Validation for MOSC
  SYSCTL_RSCLKCFG =       (0x0B0 >> 2),
#define   SYSCTL_RSCLKCFG_MEMTIMU (1<<31)  // Memory Timing Register Update
#define   SYSCTL_RSCLKCFG_NEWFREQ (1<<30)  // New PLLFREQ Accept
#define   SYSCTL_RSCLKCFG_USEPLL  (1<<28)  // Use PLL
#define   SYSCTL_RSCLKCFG_PLLSRC_M     (0b1111 << 24)  // MOSC is the PLL input clock
#define   SYSCTL_RSCLKCFG_PLLSRC_MOSC  (3 << 24)  // MOSC is the PLL input clock
#define   SYSCTL_RSCLKCFG_OSCSRC_M     (0b1111 << 20)  // Oscillator Source
#define   SYSCTL_RSCLKCFG_OSCSRC_MOSC  (3 << 20)  // MOSC is oscillator source
#define   SYSCTL_RSCLKCFG_OSYSDIV_M    (0x3ff << 10)  // Oscillator System Clock Divisor
#define   SYSCTL_RSCLKCFG_PSYSDIV_M    (0x3ff << 0)  // PLL System Clock Divisor
#define   SYSCTL_RSCLKCFG_PSYSDIV_3    3
   SYSCTL_MEMTIM0 =       (0x0C0 >> 2),
#define   SYSCTL_MEMTIM0_EBCHT_M   (0b1111 << 22)  // EEPROM Clock High Time
#define   SYSCTL_MEMTIM0_EBCHT_3_5 (6 << 22)  // 3.5 system clock periods
#define   SYSCTL_MEMTIM0_EBCE      (1 <<21)  // EEPROM Bank Clock Edge
#define   SYSCTL_MEMTIM0_EWS_M     (0b1111 << 16)  // EEPROM Wait States
#define   SYSCTL_MEMTIM0_EWS_6     (6 << 16)  // 6 wait states
#define   SYSCTL_MEMTIM0_FBCHT_M   (0b1111 << 6)  // EEPROM Clock High Time
#define   SYSCTL_MEMTIM0_FBCHT_3_5 (6 << 6)  // 3.5 system clock periods
#define   SYSCTL_MEMTIM0_FBCE      (1 << 5)  // EEPROM Bank Clock Edge
#define   SYSCTL_MEMTIM0_FWS_M     (0b1111 << 0)  // EEPROM Wait States
#define   SYSCTL_MEMTIM0_FWS_6     (6 << 0)  // 6 wait states
   SYSCTL_PLLFREQ0 =      (0x160 >> 2),
#define   SYSCTL_PLLFREQ0_PLLPWR   (1 << 23)  // PLL Power
#define   SYSCTL_PLLFREQ0_MFRAC_M  (0x3ff << 10)  // PLL M Fractional Value
#define   SYSCTL_PLLFREQ0_MFRAC_S  10
#define   SYSCTL_PLLFREQ0_MINT_M   (0x3ff << 0) // PLL M Integer Value
#define   SYSCTL_PLLFREQ0_MINT_S   0
   SYSCTL_PLLFREQ1 =      (0x164 >> 2),
#define   SYSCTL_PLLFREQ1_Q_M      (0x1f << 8)  // PLL Q Value
#define   SYSCTL_PLLFREQ1_Q_S      8
#define   SYSCTL_PLLFREQ1_N_M      (0x1f << 0)  // PLL N Value
#define   SYSCTL_PLLFREQ1_N_S      0
  SYSCTL_PLLSTAT =        (0x168 >> 2),
//including RCGUART 8 from Exercise 8 for future lab use beyond lab4
  SYSCTL_RCGCUART =       (0x618 >> 2),
#define   SYSCTL_RCGCUART_UART0 (1<<0) // UART 0
  //INclusion of AFSEL
  SYSCTL_GPIO_AFSEL =     (0x420 >> 2),
};

#endif // _SYSCTL_H
