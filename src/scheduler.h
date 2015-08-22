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

#ifndef _SCHEDULER_INCLUDED_
#define _SCHEDULER_INCLUDED_

#ifdef  SCHEDULER_GLOBALS
#define SCHEDULER_EXT
#else
#define SCHEDULER_EXT extern
#endif

//-----------------------------------------------------------------------------------
//                        DEFINE CONSTANTS & MACROS
//-----------------------------------------------------------------------------------
#define SCHEDULER_MAX_TASKS ((uint8_t)0x05)     // MAX available task (use 7bytes SRAM/Task)
#define SCHEDULER_EMPTY     ((uint8_t)0x00)     // Schedule empty value

#define SCHEDULER_OK        ((uint8_t)0x00)     // Process end SCHEDULER_OK
#define SCHEDULER_NG        ((uint8_t)0xFF)     // Process end SCHEDULER_NG

#define SCHEDULER_READY     ((uint8_t)0x01)     // Task ready status
#define SCHEDULER_WAIT      ((uint8_t)0x00)     // Task waiting status
//-----------------------------------------------------------------------------------
//                        PUBLIC FUNCTION PROTOTYPE
//-----------------------------------------------------------------------------------
SCHEDULER_EXT void SCHEDULER_Init (void);
SCHEDULER_EXT void SCHEDULER_TaskDispatch (void);
SCHEDULER_EXT uint8_t SCHEDULER_AddTask (const void (*)(), uint16_t, uint16_t);
SCHEDULER_EXT uint8_t SCHEDULER_DelTask (uint8_t taskId);
SCHEDULER_EXT void SCHEDULER_Start (void);
//-----------------------------------------------------------------------------------
//                        PUBLIC DATA DECLARATION
//-----------------------------------------------------------------------------------

#endif
