#ifndef __LOOKUP_H__
#define __LOOKUP_H__

// Put the function prototype for your lookup() procedure here.
#define LOOKUP() lookup((uint8_t *)ADDR, SIZE, row, column) //table, size, row #, col# (placeholders)
uint8_t lookup(uint8_t* ADDER,  uint8_t SIZE, uint8_t row, uint8_t column );

// In your definition of LOOKUP, use (not define) the macros ADDR and SIZE

// for the names of the look-up table and size variables used in your C code.
// The actual definitions of ADDR and SIZE go in your C code, not here.
//test lookup and test init to test and use definitions to test algorithim
//take col and row and pass them on

#endif // __LOOKUP_H__
