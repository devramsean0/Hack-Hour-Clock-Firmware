/******************************************************************************
 * @file segment_service.c
 *
 * @brief Segment service implementation and FreeRTOS task creation.
 *        This can be used as a reference for how to create a simple "service"
 *        (FreeRTOS task), and how to pass output strings to the CLI service
 *        (in lieu of stdout/printf) via the 'cli_print' functions which utilize
 *        inter-task queues.
 *
 * @author Cavin McKinley (MCKNLY LLC)
 *
 * @date 02-14-2024
 * 
 * @copyright Copyright (c) 2024 Cavin McKinley (MCKNLY LLC)
 *            Released under the MIT License
 * 
 * SPDX-License-Identifier: MIT
 ******************************************************************************/

#include "hardware_config.h"
#include "rtos_utils.h"
#include "services.h"
#include "service_queues.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "device_drivers.h"
#include "pico/cyw43_arch.h"

#define KILOBIT   0XFE //KiloBit
#define HUNDREDS  0XFD //hundreds
#define TENS      0XFB //tens
#define UNITS     0XF7 //units
#define Dot       0x80 //decimal separator

UBYTE SEG8Code[] ={
        0x3F, // 0
        0x06, // 1
        0x5B, // 2
        0x4F, // 3
        0x66, // 4
        0x6D, // 5
        0x7D, // 6
        0x07, // 7
        0x7F, // 8
        0x6F, // 9
        0x77, // A
        0x7C, // b
        0x39, // C
        0x5E, // d
        0x79, // E
        0x71, // F
    };

// when using this example to implement a new service, replace all references
// to "hearbeat" with the new service name.

static void prvSegmentTask(void *pvParameters); // entry function for this service, implementation below
TaskHandle_t xSegmentTask; // FreeRTOS task handle for this service

// service creation function, creates FreeRTOS task xSegmentTask from entry function prvSegmentTask
// this is the function pointer added to the service_functions[] array in services.h
BaseType_t segment_service(void)
{
    BaseType_t xReturn;

    xReturn = xTaskCreate(
        prvSegmentTask,             // main function of this service, defined below
        xstr(SERVICE_NAME_SEGMENT), // name defined in services.h
        STACK_SEGMENT,              // stack size defined in services.h
        NULL,                         // parameters passed to created task (not used)
        PRIORITY_SEGMENT,           // priority of this service, defined in services.h
        &xSegmentTask               // FreeRTOS task handle
    );

    if (xReturn == pdPASS) {
        cli_print_raw("segment service started");
    }
    else {
        cli_print_raw("Error starting the segment service");
    }

    return xReturn;
}

// FreeRTOS task created by segment_service
static void prvSegmentTask(void *pvParameters)
{
    //
    // Service startup (run once) code can be placed here
    // (similar to Arduino setup(), if that's your thing)
    //
    if (pico_8seg_led_init()) {
        cli_print_timestamped("Initialized 8-segment LED display");
    };
    while(true) {
        //
        // Main service (run continuous) code can be placed here
        // (similar to Arduino loop(), if that's your thing)
        //
        pico_8seg_led_send_command(UNITS, SEG8Code[1]);        // always include the below, with REPEAT & DELAY settings in services.h,
        // otherwise the service will starve out other RTOS tasks

        // update this task's schedule
        task_sched_update(REPEAT_SEGMENT, DELAY_SEGMENT);
    }

    // the task runs forever unless the RTOS kernel suspends or kills it.
    // for a one-shot service, just eliminate the infinite loop.
}