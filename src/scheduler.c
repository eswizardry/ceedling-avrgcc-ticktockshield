//===================================================================================
//  File       : scheduler.c
//-----------------------------------------------------------------------------------
//  Object     : Implement schedule that work as backbone of program
//
//  contents   : Schedule
//
//===================================================================================
//-----------------------------------------------------------------------------------
// Date     | Author | Revision       | Description
//----------+--------+----------------+----------------------------------------------
// 11.11.09 | EDRIT  |                | Initial revision
//----------+--------+----------------+----------------------------------------------
//===================================================================================
#define SCHEDULER_GLOBALS
//-----------------------------------------------------------------------------------
//                             INCLUDE FILE
//-----------------------------------------------------------------------------------
#include <avr/interrupt.h>
#include "scheduler.h"
//-----------------------------------------------------------------------------------
//                             TYPE DEFINITION
//-----------------------------------------------------------------------------------
typedef struct{							// SCHEDULE Struct
    const void (*pTask)(void);			//		Task pointer
    uint16_t delayTick;					//		Task delay
    uint16_t periodTick;				//		Task period
    uint8_t status;						//		Task status
}SCHEDULER_S;
//-----------------------------------------------------------------------------------
//                             FUNCTION PROTOTYPE
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
//                             ENUMERATION
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
//                             LOCAL DATA DECLARATIONS
//-----------------------------------------------------------------------------------
SCHEDULER_S scheduleTask[SCHEDULER_MAX_TASKS];

//===================================================================================
//  Function   : scheduleInitial
//-----------------------------------------------------------------------------------
//  Object     : scheduleInitial
//
//  Return     : -
//
//-----------------------------------------------------------------------------------
//  Argument Name     |IN |OUT| Utilisation
//--------------------+---+---+------------------------------------------------------
//                    |   |   |
//--------------------+---+---+------------------------------------------------------
//===================================================================================
void SCHEDULER_Init(void)
{
    uint8_t idx;

    // Tasks initialise
    // Delete tasks
    for (idx = 0; idx < SCHEDULER_MAX_TASKS; idx++)
    {
        SCHEDULER_DelTask (idx);
    }
}
//===================================================================================
//  Function   : scheduleTaskDispatch
//-----------------------------------------------------------------------------------
//  Object     : Finding and Dispatching ready task
//
//  Return     : -
//
//-----------------------------------------------------------------------------------
//  Argument Name     |IN |OUT| Utilisation
//--------------------+---+---+------------------------------------------------------
//                    |   |   |
//--------------------+---+---+------------------------------------------------------
//===================================================================================
void SCHEDULER_TaskDispatch(void)
{
    uint8_t idx;

    // Run the next task (if ready)
    for (idx = 0; idx < SCHEDULER_MAX_TASKS; idx++)
    {
        if (scheduleTask[idx].status > 0)
        {
            // Run the task
            (*scheduleTask[idx].pTask)();

            // Reduce status flag
            scheduleTask[idx].status--;

            // Periodic tasks will automatically run again
            // - if this is a 'one shot' task, remove it from schedule
            if (scheduleTask[idx].periodTick == 0)
            {
                SCHEDULER_DelTask(idx);
            }
        }
    }
    //        // Report system status
    //        scheduleReportStatus();
    //        // The scheduler enters idle mode at this point
    //        scheduleGo2Sleep();
}
//===================================================================================
//  Function   : scheduleAddTask
//-----------------------------------------------------------------------------------
//  Object     : Assign task to schedule
//
//  Return     : Task ID
//
//-----------------------------------------------------------------------------------
//  Argument Name     |IN |OUT| Utilisation
//--------------------+---+---+------------------------------------------------------
//  (*fnPtr)()        | x |   | Address of function
//  delay             | x |   | Delay before first execute task
//  period            | x |   | Interval time to execute task
//--------------------+---+---+------------------------------------------------------
//===================================================================================
uint8_t SCHEDULER_AddTask (const void (*fnPtr)(), uint16_t delay, uint16_t period)
{
    uint8_t taskId = 0;

    // Find a space in task array
    while ((scheduleTask[taskId].pTask != SCHEDULER_EMPTY) && (taskId < SCHEDULER_MAX_TASKS))
    {
        taskId++;
    }

    if (taskId == SCHEDULER_MAX_TASKS)
    {
        // Task list full
        return (SCHEDULER_MAX_TASKS);
    }

    // Have a space in task array
    scheduleTask[taskId].pTask		= fnPtr;
    scheduleTask[taskId].delayTick  = delay;
    scheduleTask[taskId].periodTick = period;
    scheduleTask[taskId].status     = SCHEDULER_WAIT;

    // Return position of task
    return (taskId);
}

//===================================================================================
//  Function   : scheduleDelTask
//-----------------------------------------------------------------------------------
//  Object     : Assign task to schedule
//
//  Return     : SCHEDULER_OK / SCHEDULER_NG
//
//-----------------------------------------------------------------------------------
//  Argument Name     |IN |OUT| Utilisation
//--------------------+---+---+------------------------------------------------------
//  task_id           | x |   | Task ID that need to delete from schedule
//--------------------+---+---+------------------------------------------------------
//===================================================================================
uint8_t SCHEDULER_DelTask (uint8_t taskId)
{
    if (scheduleTask[taskId].pTask == SCHEDULER_EMPTY)
    {
        // No task at this location
        return (SCHEDULER_NG);
    }
    // Delete task from scheduler
    scheduleTask[taskId].pTask	    = SCHEDULER_EMPTY;
    scheduleTask[taskId].delayTick  = SCHEDULER_EMPTY;
    scheduleTask[taskId].periodTick = SCHEDULER_EMPTY;
    scheduleTask[taskId].status		= SCHEDULER_EMPTY;
    // Delete task is complete
    return (SCHEDULER_OK);
}
//===================================================================================
//  Function   : ISR(TIMER1_COMPA_vect)
//-----------------------------------------------------------------------------------
//  Object     : Update tasks in 'Compare match A' interrupt
//               every 1 milli second
//
//  Return     : -
//
//-----------------------------------------------------------------------------------
//  Argument Name     |IN |OUT| Utilisation
//--------------------+---+---+------------------------------------------------------
//                    |   |   |
//--------------------+---+---+------------------------------------------------------
//===================================================================================
void SCHEDULER_Start(void)
{
    // OCRn =  [ (clock_speed / Prescaler_value) * Desired_time_in_Seconds ] - 1
    // OCRn =  [ (16MHz / 1) * 0.001 ] - 1
    OCR1A = 0x3E7F;

    // Mode 4, CTC top=OCR1A
    TCCR1B |= (1 << WGM12);

    // No Prescaling and start the timer
    TCCR1B |= (1 << CS10);

    // Enable interrupt on compare match
    TIMSK1 |= (1 << OCIE1A);
}
//===================================================================================
//  Function   : ISR(TIMER1_COMPA_vect)
//-----------------------------------------------------------------------------------
//  Object     : Update tasks in 'Compare match A' interrupt
//               every 1 milli second
//
//  Return     : -
//
//-----------------------------------------------------------------------------------
//  Argument Name     |IN |OUT| Utilisation
//--------------------+---+---+------------------------------------------------------
//                    |   |   |
//--------------------+---+---+------------------------------------------------------
//===================================================================================
ISR(TIMER1_COMPA_vect)
{
    uint8_t idx;

    // Calculations 'Ticks count'
    for (idx = 0; idx < SCHEDULER_MAX_TASKS; idx++)
    {
        // Check if there is a task at this location
        if (scheduleTask[idx].pTask)
        {
            if (scheduleTask[idx].delayTick == 0)
            {
                // The task is ready to run
                // Inc. the 'status' flag
                scheduleTask[idx].status++;

                if (scheduleTask[idx].periodTick)
                {
                    // Run again for schedule periodic tasks
                    scheduleTask[idx].delayTick = scheduleTask[idx].periodTick;
                }
            }
            else
            {
                // Not ready yet: decrement delay
                scheduleTask[idx].delayTick--;
            }
        }
    }
}
