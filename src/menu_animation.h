/* -------------------------------------------------------------------------------------- *\

                   menu_animation.h

	Header file for SteamCab SlideShow Menu Module

	Author:			Peter Deutsch (engineerbill@stemchest.com)
	Date Created:	8/24/13
	Modified:		8/24/13 - implemented Tempus Fugit
	Modified:		9/3/13  - implemented SteamCab SlideShow


\* -------------------------------------------------------------------------------------- */
#ifndef TF_MENU_TIMER_H
#define TF_MENU_TIMER_H


// ----------------------------------------------
// 		Public functions for this module
// ----------------------------------------------
extern void menu_animation_init();		// call to init module	
extern void menu_animation_deinit();	// call to clean up module

extern void menu_animation_show_page();	// call this to display Timer Menu
extern void menu_animation_cleanup();	// Call this to remove parent from
										// Window stack and return to function
										// Module
#endif
