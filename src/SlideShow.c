/* -------------------------------------------------------------------------------------- *\

                  	SteamCab SlideShow - Mark I
					      (version 1.0)

				    Simple Bitmap Viewer...

 Allows user to load up to n bitmaps. These can be view individually, 
 or as an animated slideshow. User can control speed of slideshow
 with the Pebble UP/DOWN buttons. SELECT calls up Help Menu.
 
 (Note: Current max 10 full screen bitmaps. This can be increased if
 you install smaller bitmaps. Total size is currently limited to x bytes)

Modes:
	- Single Step:	Single step thru imageset (UP and DOWN change current image)

	- Slow Motion:	Cycles thru imageset at SLOW rate (user selectable)
		
	- Full Speed:	Cycles thru imageset at Historic rate (user selectable))
		
	- Forward/Reverse:	Toggles Direction of image animation (user selectable)
		
	- Clock (Left)	Left handed analog watchface

	- Clock (Right)	Right handed analog watchface

         -------------------------------------------
			  
	Author:			Peter Deutsch (engineerbill@stemchest.com)
	Date Created:	9/1/13
	Update:			9/1/13 - shipped with Tempus Fugit
	Last Modified	9/3/13 - Modified for SteamCab SlideShow
						    (added *_config.h files for each module)
	
	Version:		Mark I (beta v0.1)
	
				        (c) 2013,
				    Steamchest Chronicles
				Licensed under the MIT License
				(see License file for details)

         -------------------------------------------

	Wishlist:
			- Means of saving state between instantiations
			   (Planned Android State Manager to come or
			    Pebble Central provides persistent storage)

         -------------------------------------------

Implementation Plan:

	1) Implement Basic Animation Page (SlideShow.c, animation.c)

	2) Implement program Help menu (Help.c)
		- includes Help_config.h config file

	3) Implement program menu (menu_main.c)
		- FAST
		- SLOW
		- STEP
		- Settings	FrameRate: (SINGLE|SLOW(5f/sec)|NORMAL(20fps)
					Direction (FORWARD|REVERSE)
					Invert (TRUE|FALSE)
		- Help
		- About
	
	4) Implement Animation (full speed)
		- display single image
		- single step image
		- slow animation (5fps)
		- fast animation (20fps)

	
	5) Implement About/Help menu Contents
	
	6) Implement Lefthanded SteamCab watchface
	
	7) Implement Righthanded SteamCab watchface
	
	8) document and release...

Implementatiion Notes:

	1) Each module has a corresponding <module>.h file, which contains global
      function prototypes and declares. This can be safely included in any other
	  module that needs them (for example, to be able to call a public function).

	2) As far as possible, we avoid global variables, but  those that were used
	  were all defined in "<Program>_globals.h". This file is included once in
	  "<Program>.c", allocating the required space. To pick up any such globals,
	  a module should include the file "<Program>.h" at the start of its implementation.

	3) Additional documentation and description of the implementation of this program
	will be posted to the blog "steamca.wordpress.com" as time and resources permit.

\* -------------------------------------------------------------------------------------- */

#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
	

// ----------------------------------------------
//			program header
// ----------------------------------------------
// Unique program ID (required by PebbleOS for synch/updates)
//#define MY_UUID {0xED, 0xA7, 14, 0xD0, 13, 0xE7, 11, 0xE3, 0x8F, 0xFD, 0x08, 00, 20, 0x0C, 0x9A, 66}
//#define MY_UUID {0xED, 0xA7, 14, 0xD0, 13, 0xE7, 11, 0xE3, 0x8F, 0xFD, 0x08, 00, 20, 0x0C, 0x9A, 61}
#define MY_UUID {0xED, 0xA7, 14, 0xD0, 13, 0xE7, 11, 0xE3, 0x8F, 0xFD, 0x08, 00, 20, 0x0C, 0x9A, 62}

PBL_APP_INFO(MY_UUID,
//             "SlideShow - Mk I",		// Program Name
             "Nickleodeon",		// Program Name
             "SteamChest Chronicles",	// Author
             1, 0,						// Version
             RESOURCE_ID_MENU_ICON,		// this icon used in Pebble menu
             APP_INFO_STANDARD_APP		// tells OS this is not a watchface
			 							// so e.g. we can get button events
);


// --------------------------------------------------------
// Bring in global defines
// --------------------------------------------------------
#include "SlideShow.h"				// public program declares & functions
#include "SlideShow_globals.h"		// define global variables & functions
//#include "animation_config.h"		// customize program settings here
//#include "SlideShow_configs.h"	// customize program settings here

//#include "feature_calc.h"			// feature module declares & functiontypes
//#include "feature_stop.h"			//
//#include "feature_timer.h"		//
//#include "feature_an:alog.h"		//

//#include "menu_program.h"			// menu module declares & function prototypes
#include "menu_animation.h"			//
#include "menu_images.h"			//
#include "menu_direction.h"			//
//#include "menu_calc.h"			//
//#include "menu_stop.h"			//
//#include "menu_timer.h"			//
//#include "menu_settings.h"		// RFU - we eventually want to provide
//#include "menu_defaults.h"		// settings & defaults when we have persistent storage

#include "animation.h"				// support Pages declares & function prototypes 
//#include "page_start.h"			//
#include "Help.h"					// 
//#include "page_about.h"			// 

AppTimerHandle animation_timer;		// holds return value from system timer

//static int starting = 1;


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ------------------------------------------
//		global timer routines ()
//
//  Sets and cancels timers
// Note: routine handle_timer() dispatches
// timer events to individual modules.
// ------------------------------------------
/*	
void g_cancel_timer() {
	
	app_timer_cancel_event(g_app_context, handle_animation_timer);

}  // g_cancel_timer()

void g_start_timer(int frame_rate) {
	
	animation_timer = app_timer_send_event(g_app_context, frame_rate, 42);

}  // g_start_timer()
*/

// -------------------------------------------------------
//				handle_timer()
//
//  called by system every frameRate interval, in turn
//  it calls each function module that requires service.
//  In this program the timer is actually managed from 
//  the animation module.
// -------------------------------------------------------
static void handle_timer(AppContextRef ctx, AppTimerHandle handle, uint32_t cookie) {

//	if(starting == 1) {
		handle_animation_timer();
//		starting = 2;
//	}
	
//	if(starting == 2) {
//		handle_animation_timer();
//	}
	
}  // handle_timer()
// ----------------------------------------------------------------------------
//				handle_tick()
//
//  called by system every TICK interval, in turn
//  it calls each function module that requires service.
// ----------------------------------------------------------------------------
//void handle_tick(AppContextRef ctx, PebbleTickEvent *t) {
//	
//	feature_calc_tick();					// despatch tick events
//	feature_stop_tick();					// to individual functions
//	feature_timer_tick();					//
//	feature_analog_tick();					//
//
//}

// --------------------------------------------------------
//			handle_init()
//
//   called on program start. Set up individual modules.
// --------------------------------------------------------

static void handle_init(AppContextRef ctx) {
(void)ctx;

// -------------------------------------------------
// set up global app context
// -------------------------------------------------
	g_app_context = ctx;

// TODO: when we have persistent storage we'll reload it here

// ---------------------------------------------------
//     Initialize program modules
// Note: we create first window in menu_program_init()
// ---------------------------------------------------

// init animation page (need to call animation module first
// to set up variables, menu inits then call to get these values)
	feature_animation_init();		
	menu_animation_init();			// init main menu
	menu_images_init();				// init bank select Action Bar
	menu_direction_init();			// init bank select Action Bar
	help_init();					// init Help page

//   init program feature modules
//	feature_calc_init();			// init Calculator feature
//	feature_stop_init();			// init Stopwatch feature
//	feature_timer_init();			// init countdown Timer feature
//	feature_analog_init();			// init Analog Clock feature

//   init various menu modules
//	menu_calc_init();				// init program Main config menu
//	menu_stop_init();				// (reserved for future use)
//	menu_timer_init();				// (reserved for future use)
//	menu_settings_init();			// (reserved for future use)
//	menu_defaults_init();			// r(eserved for future use)


//   init program display pages modules
//	page_start_init();				// init start page
//	page_about_init();				// init about page
//	page_timer_init();				//

	
// and now launch the main program Window
// and then start the animation timer
	animation_show_window();
	animation_timer_start();

}  // handle_init()


// --------------------------------------------------------
//			handle_deinit()
// --------------------------------------------------------

static void handle_deinit() {

// clean up any allocated resources on exit
	feature_animation_deinit();
	menu_animation_deinit();		// init animation module menu menu
	menu_images_deinit();			// init program Home Page menu
	menu_direction_deinit();		// init program Home Page menu
	help_deinit();					// init page

///	menu_program_deinit();
//	feature_calc_deinit();
//	page_about_deinit();
//	page_start_deinit();

	
}  // handle_program_deinit()



// ----------------------------------------------------------------------------
//			pbl_main()
// ----------------------------------------------------------------------------

void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
    .deinit_handler = &handle_deinit,
	.timer_handler = &handle_timer,
//	.tick_info = {
//	.tick_handler = &handle_tick,
//	.tick_units = MINUTE_UNIT		// uncomment to use minute ticks 
//	.tick_units = SECOND_UNIT		// uncomment to use second ticks
//	}
  };
  app_event_loop(params, &handlers);

}
