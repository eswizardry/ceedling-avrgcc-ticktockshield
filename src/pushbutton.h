//===================================================================================
//  File       : pushbutton.h
//-----------------------------------------------------------------------------------
//  Object     : Push button header file
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

#ifndef _PUSHBUTTON_INCLUDED_
#define _PUSHBUTTON_INCLUDED_

#ifdef  PUSHBUTTON_GLOBALS
#define PUSHBUTTON_EXT
#else
#define PUSHBUTTON_EXT extern
#endif
//-----------------------------------------------------------------------------------
//                        DEFINE CONSTANTS & MACROS
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
//                             ENUMERATION
//-----------------------------------------------------------------------------------

enum{                                   // Switch status
    PUSHBUTTON_NO = 0,                  //      Not press
    PUSHBUTTON_DOWN,                    //      pressing K1
    PUSHBUTTON_UP,                      //      pressing K2
    PUSHBUTTON_UP_DOWN,                 //      pressing K1+K2
    PUSHBUTTON_MODE,                    //      pressing K3
    PUSHBUTTON_MODE_DOWN,               //      pressing K1+K3
    PUSHBUTTON_MODE_UP,                 //      pressing K2+K3
    PUSHBUTTON_ALL,                     //      pressing K1+K2+K3
};
//-----------------------------------------------------------------------------------
//                        PUBLIC FUNCTION PROTOTYPE
//-----------------------------------------------------------------------------------
PUSHBUTTON_EXT void PUSHBUTTON_Init (void);
PUSHBUTTON_EXT void PUSHBUTTON_Update (void);
PUSHBUTTON_EXT uint8_t PUSHBUTTON_ReadStatus (void);
//-----------------------------------------------------------------------------------
//                        HARDWARE WIRING DEFINITION
//-----------------------------------------------------------------------------------
#define PUSHBUTTON_PIN      PINB   //SW PINx1..3
#define PUSHBUTTON_DDR      DDRB
#define PUSHBUTTON_PORT     PORTB
//-----------------------------------------------------------------------------------
//                        PUBLIC DATA DECLARATION
//-----------------------------------------------------------------------------------

#endif
