/*--------------------------------------------------------------------*/
/* miniassembler.c                                                    */
/* Author: Bob Dondero, Donna Gabai , Anish K                                 */
/*--------------------------------------------------------------------*/

#include "miniassembler.h"
#include <assert.h>
#include <stddef.h>

/*--------------------------------------------------------------------*/
/* Modify *puiDest in place,
   setting uiNumBits starting at uiDestStartBit (where 0 indicates
   the least significant bit) with bits taken from uiSrc,
   starting at uiSrcStartBit.
   uiSrcStartBit indicates the rightmost bit in the field.
   setField sets the appropriate bits in *puiDest to 1.
   setField never unsets any bits in *puiDest.                        */
static void setField(unsigned int uiSrc, unsigned int uiSrcStartBit,
                     unsigned int *puiDest, unsigned int uiDestStartBit,
                     unsigned int uiNumBits)
{
   for (unsigned int bit = 0; bit < uiNumBits; bit++) {
      unsigned int sourceBit = uiSrcStartBit + bit;
      unsigned int destBit = uiDestStartBit + bit;
      
      /* Check if the current bit in uiSrc is set */
      if ((uiSrc >> sourceBit) & 1U) {
         /* Set the corresponding bit in *puiDest */
         *puiDest |= (1U << destBit);
      }
   }
}


/*--------------------------------------------------------------------*/

unsigned int MiniAssembler_mov(unsigned int uiReg, int iImmed)
{
   unsigned int uiInstr = 0x52800000; /* Base opcode for MOV */

   /* Insert the destination register into bits [0:4] */
   setField(uiReg, 0, &uiInstr, 0, 5);

   /* Insert the immediate value into bits [5:20] */
   setField((unsigned int)iImmed, 0, &uiInstr, 5, 16);

   return uiInstr;
}


/*--------------------------------------------------------------------*/

unsigned int MiniAssembler_adr(unsigned int uiReg, unsigned long ulAddr,
   unsigned long ulAddrOfThisInstr)
{
   unsigned int uiInstr;
   unsigned int uiDisp;

   /* Base Instruction Code */
   uiInstr = 0x10000000;

   /* register to be inserted in instruction */
   setField(uiReg, 0, &uiInstr, 0, 5);

   /* displacement to be split into immlo and immhi and inserted */
   uiDisp = (unsigned int)(ulAddr - ulAddrOfThisInstr);

   setField(uiDisp, 0, &uiInstr, 29, 2);
   setField(uiDisp, 2, &uiInstr, 5, 19);

   return uiInstr;
}

/*--------------------------------------------------------------------*/

unsigned int MiniAssembler_strb(unsigned int uiFromReg, unsigned int uiToReg)
{
   unsigned int uiInstr = 0x39000000;

   setField(uiFromReg, 0, &uiInstr, 0, 5);
   setField(uiToReg, 0, &uiInstr, 5, 5);

   return uiInstr;
}

/*--------------------------------------------------------------------*/

unsigned int MiniAssembler_b(unsigned long ulAddr,
   unsigned long ulAddrOfThisInstr)
{
   unsigned int uiInstr = 0x14000000;
   unsigned int offset = (unsigned int)((ulAddr - ulAddrOfThisInstr) >> 2);

   setField(offset, 0, &uiInstr, 0, 26);

   return uiInstr;
}
