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
   for (unsigned int bitIndex = 0; bitIndex < uiNumBits; bitIndex++) {
      unsigned int srcBitPos = uiSrcStartBit + bitIndex;
      unsigned int destBitPos = uiDestStartBit + bitIndex;
      /* check if that particular bit in uiSrc is set */
      if ((uiSrc >> srcBitPos) & 1U) {
         /* set the corresponding bit in *puiDest */
         *puiDest |= (1U << destBitPos);
      }
   }
}


/*--------------------------------------------------------------------*/

unsigned int MiniAssembler_mov(unsigned int uiReg, int iImmed)
{
   unsigned int uiInstr = 0x52800000; 

   /* place the register in [0:4] */
   setField(uiReg, 0, &uiInstr, 0, 5);

   /* place the immediate in [5:20], 16 bits */
   /* no masking done here; if needed, caller ensures correctness */
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

   /* place wFromReg in [0:4] */
   setField(uiFromReg, 0, &uiInstr, 0, 5);

   /* place xToReg in [5:9] */
   setField(uiToReg, 0, &uiInstr, 5, 5);

   return uiInstr;
}

/*--------------------------------------------------------------------*/

unsigned int MiniAssembler_b(unsigned long ulAddr,
   unsigned long ulAddrOfThisInstr)
{
   unsigned int uiInstr = 0x14000000;
   unsigned long distance = ulAddr - ulAddrOfThisInstr;
   unsigned int offset = (unsigned int)(distance >> 2);

   /* insert offset in [0:25] */
   setField(offset, 0, &uiInstr, 0, 26);

   return uiInstr;
}
