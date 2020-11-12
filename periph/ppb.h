#ifndef _PPB_H
#define _PPB_H
//#include "osc.h"

// Frequency of switching between left and right LED display
/*#define   SWITCHFREQ    200 //taken from number 5 on canvas lab page

// Add the calculation for your reload value here.
#define RELOAD_VALUE    (MAINOSCFREQ/SWITCHFREQ) //120M/200HZ */

#define PPB                     ((volatile uint32_t *) 0xe000e000)

enum {
  PPB_STCTRL = (0x10 >> 2),
#define   PPB_STCTRL_COUNT   (1<<16)
#define   PPB_STCTRL_CLK_SRC (1<<2)
#define   PPB_STCTRL_INTEN   (1<<1)
#define   PPB_STCTRL_ENABLE  (1<<0)
  PPB_STRELOAD,
  PPB_STCURRENT,
  PPB_EN0 = (0x100 >> 2),
#define PPB_EN0_TIMER2A (1<<23)
      PPB_EN2 = (0x108 >> 2),
#define PPB_EN2_TIMER5A (1<<(65-2*32))
      PPB_PRI16 =(0x440 >> 2),
#define PPB_PRI_INTB_M (0b111<<13) //mask because labelled _M
#define PPB_PRI_INTB_S 13 //shift value??? ask prof
};

#endif //ends PPB_H
