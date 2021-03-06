//===================================================================================
//  File       : led.h
//-----------------------------------------------------------------------------------
//  Object     : 
//
//  contents   :
//
//===================================================================================
//-----------------------------------------------------------------------------------
// Date     | Author      | Revision       | Description
//----------+-------------+----------------+-----------------------------------------
// 15.08.17 | esWizardry  |                |
//----------+-------------+----------------+-----------------------------------------
//===================================================================================

#ifndef _LED_INCLUDED_
#define _LED_INCLUDED_

#ifdef  LED_GLOBALS
#define LED_EXT
#else
#define LED_EXT extern
#endif

//-----------------------------------------------------------------------------------
//                        DEFINE CONSTANTS & MACROS
//-----------------------------------------------------------------------------------
#define LED_PORT PORTD

enum {
    LED_1 = PORTD2,
    LED_2,
    LED_3,
    LED_4,
    LED_N
};
//-----------------------------------------------------------------------------------
//                        PUBLIC FUNCTION PROTOTYPE
//-----------------------------------------------------------------------------------
LED_EXT void LED_Init(void);
LED_EXT void LED_BlinkTask(void);
//-----------------------------------------------------------------------------------
//                        PUBLIC DATA DECLARATION
//-----------------------------------------------------------------------------------

#endif
