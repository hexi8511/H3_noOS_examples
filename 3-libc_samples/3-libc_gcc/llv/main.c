
/**
 * @file main
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include <stdlib.h>
//#include <unistd.h>

#include "lvgl/lvgl.h"
#include "lv_drivers/display/noosfbdev.h"
#include "lv_drivers/indev/key_esd.h"
#include "lv_examples/lv_apps/demo/demo.h"
#include "lv_examples/lv_apps/benchmark/benchmark.h"
#include "lv_examples/lv_tests/lv_test_theme/lv_test_theme.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void hal_init(void);
static void lv_tick_thread(void );

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

int lv_main(int argc, char** argv)
{
    /*Initialize LittlevGL*/
    lv_init();

    /*Initialize the HAL for LittlevGL*/
    hal_init();

    /*Load a demo*/
    demo_create();

    /*Try the benchmark to see how fast is your GUI*/
    //benchmark_create();

    /*Check the themes too*/
    //lv_test_theme_1(lv_theme_night_init(210, NULL));

    while(1) {
        /* Periodically call the lv_task handler.
         * It could be done in a timer interrupt or an OS task too.*/
        lv_task_handler();
        usleep(1000);       /*Just to let the system breath*/
		lv_tick_thread();
    }

    return 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * Initialize the Hardware Abstraction Layer (HAL) for the Littlev graphics library
 */
static void hal_init(void)
{
    /* Add a display
     * Use the 'monitor' driver which creates window on PC's monitor to simulate a display*/
    fbdev_init();
    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);            /*Basic initialization*/
    disp_drv.disp_flush = fbdev_flush;
    disp_drv.disp_fill = fbdev_fill;
    disp_drv.disp_map = fbdev_map;
    lv_disp_drv_register(&disp_drv);

    /* Add the mouse (or touchpad) as input device
     * Use the 'mouse' driver which reads the PC's mouse*/
    keyboard_init();
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);          /*Basic initialization*/
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read = keyboard_read;         /*This function will be called periodically (by the library) to get the mouse position and state*/
    lv_indev_drv_register(&indev_drv);

    /* Tick init.
     * You have to call 'lv_tick_handler()' in every milliseconds
     * Create an SDL thread to do this*/
   // SDL_CreateThread(tick_thread, "tick", NULL);
}

/**
 * A task to measure the elapsed time for LittlevGL
 * @param data unused
 * @return never return
 */
static void lv_tick_thread(void )
{

        lv_tick_inc(1);

}
