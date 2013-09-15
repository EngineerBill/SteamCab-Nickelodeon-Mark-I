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
#define FRAMERATE_MAX		 10		// 2fps
#define FRAMERATE_FAST		 10		// 20fps
#define FRAMERATE_SLOW		500		// 2fps
#define FRAMERATE_STEP		  0		// =>  0 frames/sec

#define UP			  		  0		// 
#define DOWN			  	  1		// 

#define FORWARD		  		  0		// 
#define REVERSE			  	  1		// 

extern int IMAGE_RESOURCE_IDS[2][11];	// note: these dimensions defined in animation_conf.h file
	
// --------------------------------------------------------
//	module public functions
// --------------------------------------------------------
extern void feature_animation_init();  	// call to set up module
extern void feature_animation_deinit();	// call to close module

extern void animation_show_window();	// call to display this Window
extern void feature_animation_timer();	// call to init module
extern void animation_timer_run(); 	 	// call to init module
extern void animation_timer_start(); 	// call to init module
extern int  animation_direction_get();
extern void animation_direction_set();
extern int  animation_bank_get();
extern void animation_bank_set();
extern void update_image();

//extern void animation_fast();			// call to service timer tick
//extern void animation_slow();			// call to service timer tick
//extern void animation_step();			// call to service timer tick


#endif