/* ------------------------------------------------------------------ *\

                      Nickelodeon.h

	Program declares for SteamCab Nickelodeon Pebble app.
 
	Author:			Peter Deutsch (engineerbill@stemchest.com)
	Date Created:	9/1/13
	Modified:		9/3/13 - adapated for SteamCab Nickelodeon	
							 (added *_config.h file)
\* ------------------------------------------------------------------ */	
#ifndef NICKELODEON_H
#define NICKELODEON_H

#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


// ----------------------------------------------
//		Public program declares
// ----------------------------------------------

// Feature defines used e.g. when calling 
// context sensitive help function
#define PROGRAM				0
#define ANIMATION_ABOUT		1
#define ANIMATION_HELP		2

// -----------------------------------
//  global variables
// -----------------------------------
extern AppContextRef g_app_context;		// Store global app context

extern AppTimerHandle animation_timer;	

extern bool g_debugging;				// enables debugging messages in headers


// -----------------------------------
//      Public functions
// -----------------------------------

//extern void g_skip_start_page();		// hitting "Select" on splash page skips
//extern int g_active_window;				// Make public active window flag
//extern bool g_animating;				// controls logo animation throughout program

//extern void g_cancel_timer();		// manages global timer
//extern void g_start_timer();		// 


#endif