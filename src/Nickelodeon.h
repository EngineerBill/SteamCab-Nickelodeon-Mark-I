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
//#define ANIMATION_HELP		2

// -----------------------------------
//  global variables
// -----------------------------------
extern AppContextRef g_app_context;		// Store global app context

//extern AppTimerHandle animation_timer;	

extern bool g_debugging;				// enables debugging messages in headers


// -----------------------------------
//      Public functions
// -----------------------------------



#endif