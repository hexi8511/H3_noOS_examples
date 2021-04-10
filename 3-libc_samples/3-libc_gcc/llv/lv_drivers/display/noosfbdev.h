/**
 * @file fbdev.h
 *
 */

#ifndef NOOSFBDEV_H
#define NOOSFBDEV_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "../../lv_drv_conf.h"
#if USE_FBDEV

#include <stdint.h>
#include "lvgl/lv_misc/lv_color.h"

#define __u32 unsigned int
#define __u16 unsigned short
struct fb_bitfield {
	__u32 offset;			/* beginning of bitfield	*/
	__u32 length;			/* length of bitfield		*/
	__u32 msb_right;

};

struct fb_var_screeninfo {
	__u32 xres;			/* visible resolution		*/
	__u32 yres;
	__u32 xres_virtual;		/* virtual resolution		*/
	__u32 yres_virtual;
	__u32 xoffset;			/* offset from virtual to visible */
	__u32 yoffset;			/* resolution			*/

	__u32 bits_per_pixel;		/* guess what			*/
	__u32 grayscale;		/* != 0 Graylevels instead of colors */

	struct fb_bitfield red;		/* bitfield in fb mem if true color, */
	struct fb_bitfield green;	/* else only length is significant */
	struct fb_bitfield blue;
	struct fb_bitfield transp;	/* transparency			*/

	__u32 nonstd;			/* != 0 Non standard pixel format */

	__u32 activate;			/* see FB_ACTIVATE_*		*/

	__u32 height;			/* height of picture in mm    */
	__u32 width;			/* width of picture in mm     */

	__u32 accel_flags;		/* (OBSOLETE) see fb_info.flags */

	/* Timing: All values in pixclocks, except pixclock (of course) */
	__u32 pixclock;			/* pixel clock in ps (pico seconds) */
	__u32 left_margin;		/* time from sync to picture	*/
	__u32 right_margin;		/* time from picture to sync	*/
	__u32 upper_margin;		/* time from sync to picture	*/
	__u32 lower_margin;
	__u32 hsync_len;		/* length of horizontal sync	*/
	__u32 vsync_len;		/* length of vertical sync	*/
	__u32 sync;			/* see FB_SYNC_*		*/
	__u32 vmode;			/* see FB_VMODE_*		*/
	__u32 rotate;			/* angle we rotate counter clockwise */
	__u32 reserved[5];		/* Reserved for future compatibility */
};



struct fb_fix_screeninfo {
	char id[16];			/* identification string eg "TT Builtin" */
	unsigned long smem_start;	/* Start of frame buffer mem */
					/* (physical address) */
	__u32 smem_len;			/* Length of frame buffer mem */
	__u32 type;			/* see FB_TYPE_*		*/
	__u32 type_aux;			/* Interleave for interleaved Planes */
	__u32 visual;			/* see FB_VISUAL_*		*/
	__u16 xpanstep;			/* zero if no hardware panning	*/
	__u16 ypanstep;			/* zero if no hardware panning	*/
	__u16 ywrapstep;		/* zero if no hardware ywrap	*/
	__u32 line_length;		/* length of a line in bytes	*/
	unsigned long mmio_start;	/* Start of Memory Mapped I/O	*/
					/* (physical address) */
	__u32 mmio_len;			/* Length of Memory Mapped I/O	*/
	__u32 accel;			/* Indicate to driver which	*/
					/*  specific chip/card we have	*/
	__u16 reserved[3];		/* Reserved for future compatibility */
};

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void fbdev_init(void);
void fbdev_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p);
void fbdev_fill(int32_t x1, int32_t y1, int32_t x2, int32_t y2, lv_color_t color);
void fbdev_map(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_p);

/**********************
 *      MACROS
 **********************/

#endif  /*USE_FBDEV*/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*FBDEV_H*/
