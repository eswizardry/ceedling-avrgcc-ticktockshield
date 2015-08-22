//===================================================================================
//  File       : pushbutton.c
//-----------------------------------------------------------------------------------
//  Object     : Implement a simple pushButton interface with debouce
//
//  contents   : pushbutton monitor with debouce
//
//  Note	   : To use should define PUSHBUTTON_DDR, PUSHBUTTON_PORT and PUSHBUTTON_PIN to-
//					the direction, port and pin register respectively then change
//					PUSHBUTTON_MARK as HW connection. (!!!pushButton port should be contiguous)
//===================================================================================
//-----------------------------------------------------------------------------------
// Date     | Author | Revision       | Description
//----------+--------+----------------+----------------------------------------------
// 23.11.09 | EDRIT  |                | Initial revision
//----------+--------+----------------+----------------------------------------------
//===================================================================================
#define PUSHBUTTON_GLOBALS
//-----------------------------------------------------------------------------------
//                             INCLUDE FILE
//-----------------------------------------------------------------------------------
#include <avr/io.h>
#include "pushbutton.h"
//-----------------------------------------------------------------------------------
//                             TYPE DEFINITION
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//                             FUNCTION PROTOTYPE
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
//                             ENUMERATION
//-----------------------------------------------------------------------------------
#define PUSHBUTTON_MARK       ((uint8_t)0x0E)	// Use Pin Px3, Px2 and Px1
//-----------------------------------------------------------------------------------
//                             LOCAL DATA DECLARATIONS
//-----------------------------------------------------------------------------------
static  uint8_t debouceCount;
static  uint8_t oldPushButtonStatus;
static  uint8_t pushButtonStatus;

//===================================================================================
//  Function   : pushButtonInitial
//-----------------------------------------------------------------------------------
//  Object     : Initial port pin as input and internal pullup
//
//  Return     : -
//
//-----------------------------------------------------------------------------------
//  Argument Name     |IN |OUT| Utilisation
//--------------------+---+---+------------------------------------------------------
//                    |   |   |
//--------------------+---+---+------------------------------------------------------
//===================================================================================
void PUSHBUTTON_Init (void)
{
    PUSHBUTTON_DDR &= ~(PUSHBUTTON_MARK);
    PUSHBUTTON_PORT |= (PUSHBUTTON_MARK);

    debouceCount 	= 0;
    pushButtonStatus	= 0;
    oldPushButtonStatus	= 0;
}
//===================================================================================
//  Function   : pushButtonUpdate
//-----------------------------------------------------------------------------------
//  Object     : Scan pushButton
//
//  Return     : -
//
//-----------------------------------------------------------------------------------
//  Argument Name     |IN |OUT| Utilisation
//--------------------+---+---+------------------------------------------------------
//                    |   |   |
//--------------------+---+---+------------------------------------------------------
//===================================================================================
void PUSHBUTTON_Update (void)
{
    if((PUSHBUTTON_PIN & PUSHBUTTON_MARK) != PUSHBUTTON_MARK)
    {
         pushButtonStatus = PUSHBUTTON_PIN & PUSHBUTTON_MARK;

         if(pushButtonStatus == oldPushButtonStatus)
         {
            debouceCount++;

			// normal press
			if(debouceCount == 3)
            {
               pushButtonStatus = ((~pushButtonStatus) & PUSHBUTTON_MARK);
               return;
            }

			// enter auto-continue if press more than 2 second
			//  then auto-continue every 1/5 second
			if(debouceCount == 100)
            {
            	pushButtonStatus = ((~pushButtonStatus) & PUSHBUTTON_MARK);

			   	debouceCount = 90;
              	return;
            }

            pushButtonStatus = 0;
            return;
         }
		 oldPushButtonStatus = pushButtonStatus;
    }
    pushButtonStatus = 0;
    debouceCount = 0;
}
//===================================================================================
//  Function   : pushButtonReadStatus
//-----------------------------------------------------------------------------------
//  Object     : read pushButton status
//
//  Return     : pushButton status
//
//-----------------------------------------------------------------------------------
//  Argument Name     |IN |OUT| Utilisation
//--------------------+---+---+------------------------------------------------------
//                    |   |   |
//--------------------+---+---+------------------------------------------------------
//===================================================================================
uint8_t PUSHBUTTON_ReadStatus(void)
{
	uint8_t i;
	uint8_t pushButtonResult;
	uint8_t mark = 0x01;

	// finding the LSB bit for push button
    for (i = 0; i < 8; i++)
	{
		if (PUSHBUTTON_MARK & mark)
		{
			break;
		}
		mark <<= 1;
	}

	pushButtonResult = pushButtonStatus / mark;
	pushButtonStatus = 0;
	return (pushButtonResult);
}
