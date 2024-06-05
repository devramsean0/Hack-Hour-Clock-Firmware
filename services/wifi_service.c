/******************************************************************************
 * @file wifi_service.c
 *
 * @brief Wifi service implementation and FreeRTOS task creation.
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
#include "pico/cyw43_arch.h"

// when using this example to implement a new service, replace all references
// to "hearbeat" with the new service name.

static void prvWifiTask(void *pvParameters); // entry function for this service, implementation below
TaskHandle_t xWifiTask; // FreeRTOS task handle for this service

// service creation function, creates FreeRTOS task xWifiTask from entry function prvWifiTask
// this is the function pointer added to the service_functions[] array in services.h
BaseType_t wifi_service(void)
{
    BaseType_t xReturn;

    xReturn = xTaskCreate(
        prvWifiTask,             // main function of this service, defined below
        xstr(SERVICE_NAME_HEARTBEAT), // name defined in services.h
        STACK_HEARTBEAT,              // stack size defined in services.h
        NULL,                         // parameters passed to created task (not used)
        PRIORITY_HEARTBEAT,           // priority of this service, defined in services.h
        &xWifiTask               // FreeRTOS task handle
    );

    if (xReturn == pdPASS) {
        cli_print_raw("wifi service started");
    }
    else {
        cli_print_raw("Error starting the wifi service");
    }

    return xReturn;
}

// FreeRTOS task created by wifi_service
static void prvWifiTask(void *pvParameters)
{
    //
    // Service startup (run once) code can be placed here
    // (similar to Arduino setup(), if that's your thing)
    //
    static char *wifi_string = "ba-bump";
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed");
        return -1;
    }
}