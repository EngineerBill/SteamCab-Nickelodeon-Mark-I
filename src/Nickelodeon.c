/* -------------------------------------------------------------------------------------- *\

                  	SteamCab Nickelodeon - Mark I
					      (version 1.0)

				    Animated Bitmap Viewer...

 Allows user to load up to two sets of bitmaps and either single step
 through them, or run them as an animation series at any rate between
 two and 20 frames per second. The user can toggle between single step
 and animation mode, as well as between image sets. To demonstrate the
 Pebble's ability to multitask (and to reduce the number of program
 slots required) the author has also included a copy of both the
 Lefthanded and Righthanded SteamCab Analog Watchfaces.

Maximum size of images that can be included is currently limited to
64k (this is the upper limit of a program resource segment in the
current version of the Pebble SDK). For demonstration purposes this version
of the program has been preloaded with two series of 11 images taken from
Eadweard Muybridge's groudbreaking work on film animation, including
his original 1878 series "Sallie Gardner at a Gallop" (also known as
"The Horse in Motion") and one of his later series of studies of
the human form produced while working at the University of Pittsburgh.

An extensive collection of Muybridge's film sequences can be found here:

    http://processocriativoolho.blogspot.com/2010/05/muybridge.html


Potential Hacks and/or Modifications:

Adding Your Own Bitmaps:

You can easily replace either or both sets of images. The quickest and easiest
way to do this would be to load the project from its GitHub repository into
Cloudpebble.net. You can then select each image from the Resources list on the 
bottom left had side menu (they are named Muybridge-*.png and woman-sits-*.png).
Hit the "Browse..." button and select your new image (Note: your replacement images
should be the same size or smaller than the originals to ensure they'll all load
properly) Select the "Compilation" link on the top left, select "Run Build" and
the program should rebuild properly. Once this completes, you can either type the
resulting "Download Compiled PBW" link in your Smart Phone's browser, or scan the 
resulting QR code and your phone should automatically load the new program onto
your watch (this requires that you have installed a QR code scanning program
such as LiveScan). That's it!

Adding Additional Images:

Note that the Horse series images are 144 x 93 pixels each and the Woman
series 104 x 166 pixels each. Together the entire collection, plus other 
needed logos, total some 54k. You can increase the number of individual
images by reducing  their individual sizes. You might also consider changing
the internal two dimensional array to a single array and load it with additional
images for a longer animation sequence. To do this, you will need to modify
the array IMAGE_RESOURCE_IDS[] in the file animation_config.h, as well as the
corresponding program code that accesses it in the module animation.c. This will
also affect the Action Bar menu used to switch between the two banks of images
in menu_images.c


Loading Images Dynamically

It is theoretically possible to load individual bitmap images from your
smart phone into a program and the author is currently investigating
techniques to do this. For now it should be assumed that this is beyond
the scope of most Pebble programmers but this situation may change in
future releases of the SDK. This is definitely an extension to the
program that the author hopr to implement at some point.

For now, the best we can offer is - "Watch This Space..."


Operation:

When launched the program currently starts in Single Step mode.
In this mode DOWN single steps forward and UP single steps reverse.
The user can start a slideshow by hitting SELECT, then either FAST
(20 frames/sec) or SLOW (2 frames/sec). Once started, you can adjust
animation speed with the UP and DOWN buttons. SELECT followed by STEP
returns to Single Step mode.

To toggle between the two sets of images, hit SELECT followed by
IMAGES and use the UP and DOWN buttons to select the set you want.
To toggle direction of animation, hit SELECT followed by DIRECTION
 and use the UP and DOWN buttons to select the direction you want.

To select one of the two built-in Watchfaces, hit SELECT and then
either CLOCK(LEFT) or CLOCK(RIGHT).

SELECT followed by ABOUT brings up the copyright page, which also
has a pointer to the SteamCab website. This is still a work-in-progress
but watch that link for updates and news on this and other SteamCab
projects.


Modes:
	- FAST:	Cycles thru imageset at 20 frames per second

	- SLOW:	Cycles thru imageset at SLOW (2 frames per second) rate
		
	- STEP:	Single step thru imageset (UP and DOWN change current image)
		
	- DIRECTION:		Toggles Direction of image animation
		
	- CLOCK (LEFT):		Left handed analog watchface

	- CLOCK (RIGHT):	Right handed analog watchface

	- ABOUT				(c) and pointer to Project Home Page
	
         -------------------------------------------
			  
	Author:			Peter Deutsch (engineerbill@steamchest.com)
	Date Created:	9/1/13 - created as animation splash page
	First Release:	9/1/13 - shipped as update to Tempus Fugit
	Modified:		9/14/13 - Modified for SteamCab Nickelodeon
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

	1) (done) Implement program dispatcher and main Page (Nickelodeon.c)

	2) (done) Implement Animation module (naimation.c, animation_config.h)
		- display single image
		- single step image
		- slow animation (2fps)
		- fast animation (20fps)

	3) (done) Implement program menu (menu_animation.c)
		- FAST
		- SLOW
		- STEP

	4) (done) Implement program Action Bar menus (menu_direction.c/menu_images.c)
		- Direction (FORWARD|REVERSE)
		- Images (HORSE|WOMAN)

	5) (done) Add Watchfaces (feature_analog.c)
			- CLOCK(LEFT)
			- CLOCK(RIGHT)
	
	6) (done) Implement ABOUT page (page_about.c)
	
	8) (in progress) Document and Release...


         -------------------------------------------

Implementation Notes:

	0) SteamCab programs are written as learning tools for those wishing
	   to learn Pebble programming and are being released under the MIT
	   license, which promotes reuse, provided credit for the original
	   author is preserved (see License file for details). Generally
	   each function or feature as a stand-alone module - it is hoped that
	   this will help with understanding individual functionality, as well
	   as promote reuse of code where appropriate.

	1) Each SteamCab program begins life with a main module that contains 
	   the required pbl_main() routine and appropriate Init()/Deinit() code
	   (for properly loading/unloading bitmaps, etc), as well as required
	   Clock Tick and/or Timer handling routines. When called, these in
	   turn call appropriate init()/deinit() and tick/timer handling routines
	   in each feature or function module (see code below for details).
	
	2) Each module has a corresponding <module>.h file, which contains global
      function prototypes and declares and needed program initation value
	  (such the FAST and SLOW animation rates). These header files can be safely
	  included in any other module that needs them (for example, to be able
	  to call a public function). Where needed, a corresponding <module>_config.h
	  file is provided. For example, to set starting parameters (such as default
	  direction and mode for this program) you would look in the file animation_configs.h.

	3) To reuse a program module, you would need to implement the following steps:
		- include the module and its corresponding .h file in your
		  program main function (this will allow calling the required functions)
		- put a call to <module>_init() and <module>_deinit() in your program
		  init() and deinit() callbacks
		- put a call to <module>_show_window() where needed (either at
		  the end of your init() code or in a program menu)
		- if you are including a module in a menu, you will need to
		  implement an appropriate menu callback function to set up
		  and call your module. See the implementation of menu_animation.c
		  for an example of how to do this in your own code.
	
	4) As far as possible, we avoid global variables, but  those that were used
	  were all defined in "<Program>_globals.h". This file is included once in
	  "<Program>.c", allocating the required space. To pick up any such globals,
	  a module should include the file "<Program>.h" at the start of its implementation.

	5) Additional documentation and description of the implementation of this program
	   will be posted to the blog "steamca.wordpress.com" as time and resources permit.


\* -------------------------------------------------------------------------------------- */
#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

// ----------------------------------------------
//			program header
// ----------------------------------------------
// Unique program ID (required by PebbleOS for synch/updates)
#define MY_UUID {0xED, 0xA7, 14, 0xD0, 13, 0xE7, 11, 0xE3, 0x8F, 0xFD, 0x08, 00, 20, 0x0C, 0x9A, 62}

PBL_APP_INFO(MY_UUID,
             "Nickleodeon - Mark I",	// Program Name
             "SteamChest Chronicles",	// Author
             1, 0,						// Version
             RESOURCE_ID_MENU_ICON,		// this icon used in Pebble menu
             APP_INFO_STANDARD_APP		// tells OS this is not a watchface
			 							// so e.g. we can get button events
);

// --------------------------------------------------------
// Bring in global defines
// --------------------------------------------------------
#include "Nickelodeon.h"			// public program declares & functions
#include "Nickelodeon_globals.h"	// define global variables & functions

#include "animation.h"				// feature declares & function prototypes 
#include "feature_analog.h"			//

#include "menu_animation.h"			// support menu modules
#include "menu_images.h"			//
#include "menu_direction.h"			//

#include "page_about.h"				// copyright/support page

// ----------------------------------------------------------------------------
//        Main Program Callbacks (Timers, init/deinit)
// ----------------------------------------------------------------------------
// -------------------------------------------------------
//				handle_timer()
//
//  called by system every frameRate interval, in turn
//  it calls each function module that requires service.
//  In this program this callback is used to drive the 
//  animation module and the timer is managed from within
//  that module.
// -------------------------------------------------------
static void handle_timer(AppContextRef ctx, AppTimerHandle handle, uint32_t cookie) {

		feature_animation_timer();

}  // handle_timer()

// ----------------------------------------------------------------------------
//				handle_tick()
//
//  called by system every TICK interval, in turn
//  it calls each function module that requires service.
//  In this program, this callback is used to drive
//  the in-built Watchface feature.
// ----------------------------------------------------------------------------
void handle_tick(AppContextRef ctx, PebbleTickEvent *t) {

	feature_analog_tick();

}  // handle_tick()

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

// init animation window (need to call animation module first
// to set up variables, then individual menu inits that call
// back to that module to get these values as needed)

//   init program feature modules
	feature_analog_init();			// init Analog Clock feature
	feature_animation_init();		

//   init various menu modules
	menu_animation_init();			// init main menu
	menu_images_init();				// init bank select Action Bar
	menu_direction_init();			// init bank select Action Bar

//   init program copyright/support page
	page_about_init();				// init about page
	
// and now launch the main program Window
// and possibly start the animation timer
	animation_show_window();
	animation_timer_start();

}  // handle_init()


// --------------------------------------------------------
//			handle_deinit()
// --------------------------------------------------------

static void handle_deinit() {

// clean up any allocated resources on exit
	feature_animation_deinit();
	feature_analog_deinit();			// init Analog Clock feature

	menu_animation_deinit();		// init animation module menu menu
	menu_images_deinit();			// init program Home Page menu
	menu_direction_deinit();		// init program Home Page menu

	page_about_deinit();


}  // handle_program_deinit()


// ----------------------------------------------------------------------------
//			pbl_main()
// ----------------------------------------------------------------------------

void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
    .deinit_handler = &handle_deinit,
	.timer_handler = &handle_timer,
	.tick_info = {
	.tick_handler = &handle_tick,
//	.tick_units = MINUTE_UNIT		// uncomment to use minute ticks 
	.tick_units = SECOND_UNIT		// uncomment to use second ticks
	}
  };
  app_event_loop(params, &handlers);

}
