/**
 * @file sdl_kb.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "key_esd.h"
#if USE_KEYBOARD

#include "lvgl/lv_core/lv_group.h"
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static uint32_t keycode_to_ascii(uint32_t sdl_key);

/**********************
 *  STATIC VARIABLES
 **********************/
static uint32_t last_key;
static lv_indev_state_t state;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Initialize the keyboard
 */
void keyboard_init(void)
{
    /*Nothing to init*/
}

/**
 * Get the last pressed or released character from the PC's keyboard
 * @param data store the read data here
 * @return false: because the points are not buffered, so no more data to be read
 */
bool keyboard_read(lv_indev_data_t * data)
{
    data->state = state;
    data->key = keycode_to_ascii(last_key);

    return false;
}


void keyboard_handler(void)
{

            last_key = 0;
            state = LV_INDEV_STATE_PR;

          //  state = LV_INDEV_STATE_REL;


}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static uint32_t keycode_to_ascii(uint32_t sdl_key)
{
 return sdl_key;

}
#endif
