/* -------------------------------------------------------------------------------------- *\

                      animation.h

		Header file for SlideShow animation module


	Author:				Peter Deutsch (engineerbill@stemchest.com)
	Date Created:		9/2/13
	Modified:			-/-/-


\* -------------------------------------------------------------------------------------- */
#ifndef TF_PAGE_ANIMATE_H
#define TF_PAGE_ANIMATE_H

// --------------------------------------------------------
//	module variable defines
// --------------------------------------------------------
// Animation parameter defines
#define FRAMERATE_DEFAULT	500		// (expressed in milliseconds per frame)
#define FRAMERATE_MAX		 10		// 2fps
#define FRAMERATE_FAST		 10		// 20fps
#define FRAMERATE_SLOW		500		// 2fps
#define FRAMERATE_START		 -1		// Handle first tick
#define FRAMERATE_STEP		  0		// =>  0 frames/sec
									// => 20 frames/sec
#define PAUSE_DELAY			  1		// =>  0 frames/sec

extern int IMAGE_RESOURCE_IDS[];
	
// --------------------------------------------------------
//	module public functions
// --------------------------------------------------------
extern void handle_animation_init();   // call to set up module
extern void handle_animation_deinit(); // call to close module

extern void animation_show_page();		// call to display this Window
extern void handle_animation_timer(); 	// call to init module
extern void animation_timer_run(); 	 	// call to init module
extern void animation_timer_start(); 	// call to init module

extern void animation_fast();			// call to service timer tick
extern void animation_slow();			// call to service timer tick
extern void animation_step();			// call to service timer tick


#endif