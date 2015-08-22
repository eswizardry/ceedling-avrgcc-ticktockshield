//===================================================================================
//  File       : main.c
//-----------------------------------------------------------------------------------
//  Object     : Implement main of digital clock control
//
//  contents   : Digital clock
//
//===================================================================================
//-----------------------------------------------------------------------------------
// Date     | Author | Revision       | Description
//----------+--------+----------------+----------------------------------------------
// 23.11.09 | EDRIT  |                | Initial revision
//----------+--------+----------------+----------------------------------------------
//===================================================================================

//-----------------------------------------------------------------------------------
//                             INCLUDE FILE
//-----------------------------------------------------------------------------------
#include <avr/io.h>
#include <avr/interrupt.h>
#include "global.h"
#include "scheduler.h"
#include "pushbutton.h"
#include "led.h"

//-----------------------------------------------------------------------------------
//                             TYPE DEFINITION
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//                             FUNCTION PROTOTYPE
//-----------------------------------------------------------------------------------
static  void    initialise          (void);
//-----------------------------------------------------------------------------------
//                             ENUMERATION
//-----------------------------------------------------------------------------------
enum{                                   // Mode index
    MODE_NORM = 0,                      //      Normal mode (scrolling mode)
    MODE_SETT,                          //      Set time mode
    MODE_N,
};

enum{                                   // Set digit index
    HOUR_DIG = 0,                       //      Hour digit blink to set
    MIN_DIG,                            //      Minute digit blink to set
};
//-----------------------------------------------------------------------------------
//                             LOCAL DATA DECLARATIONS
//-----------------------------------------------------------------------------------
static  uint8_t     mode = MODE_NORM;
static  uint8_t     setDig = 0;
static  SET_TIME_S  setTime;

//===================================================================================
//  Function   : main
//-----------------------------------------------------------------------------------
//  Object     : C main function
//
//  Return     : Compiler mandatory return
//
//-----------------------------------------------------------------------------------
//  Argument Name     |IN |OUT| Utilisation
//--------------------+---+---+------------------------------------------------------
//                    |   |   |
//--------------------+---+---+------------------------------------------------------
//===================================================================================
void pressModeButton(void)
{
    uint8_t buttonStatus;

    buttonStatus = PUSHBUTTON_ReadStatus();

    if (buttonStatus == PUSHBUTTON_DOWN)
    {
        PORTD   |= (1 << PORTD3);
    }
    else if (buttonStatus == PUSHBUTTON_UP)
    {
        PORTD   |= (1 << PORTD4);
    }
    else if (buttonStatus == PUSHBUTTON_MODE)
    {
        PORTD   |= (1 << PORTD5);
    }    
    else if (buttonStatus == PUSHBUTTON_ALL)
    {
        PORTD   &= ~(1 << PORTD3);
        PORTD   &= ~(1 << PORTD4);        
        PORTD   &= ~(1 << PORTD5); 
    }
    else
    {
        //
    }
}

int16_t main (void)
{
    
    // initialise
    initialise();

    // periodic task update switch every 20 msec
    SCHEDULER_AddTask ((const void *)&PUSHBUTTON_Update, 1007, 20);

    SCHEDULER_AddTask ((const void *)&LED_BlinkTask, 0, 500);
    
    SCHEDULER_AddTask ((const void *)&pressModeButton, 3, 5);

    // start scheduler
    SCHEDULER_Start();

    // Loop
    while(1)
    {
        // Dispatch tasks
        SCHEDULER_TaskDispatch();
    }

    return 0;
}
//===================================================================================
//  Function   : initialise
//-----------------------------------------------------------------------------------
//  Object     : initialise
//
//  Return     : -
//
//-----------------------------------------------------------------------------------
//  Argument Name     |IN |OUT| Utilisation
//--------------------+---+---+------------------------------------------------------
//                    |   |   |
//--------------------+---+---+------------------------------------------------------
//===================================================================================
void initialise(void)
{
    // Disable global interrupt
    cli();

    // Scheduler initialise
    SCHEDULER_Init();    

    // Switch initialise
    PUSHBUTTON_Init();

    LED_Init();

    // Enable global interrupt
    sei();
}
