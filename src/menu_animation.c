/* -------------------------------------------------------------------------------------- *\

                      menu_animation.c

	Implements SteamCab SlideShow animation module Help Menu

	Called:		Animation module select_click_handler()

   --------------------------------------------------------------------------------
	Author:			Peter Deutsch (engineerbill@stemchest.com)
	Date Created:	8/24/13
	Modified:		8/24/13 - implemented in Tempus Fugit
	Modified:		9/3/13  - implemented in SteamCab Nickelodeon
   --------------------------------------------------------------------------------


	Main Menu Commands:
		FAST		- Start Slideshow with FAST Framerate
		SLOW		- Start Slideshow with SLOW Framerate
		STEP		- stops timer, resets Framerate, enables stepping

		Images		- invokes Direction Action Bar Menu
		Direction	- invokes Direction Action Bar Menu

		Clock (Left)	- Lefthanded Analog Watchface
		Clock (Right)	- Righthanded Analog Watchface

		Help		- Context-sensitive Help page
		About		- Copyright splash page	

 -------------------------------------------------------------------------------- 
   TODO: Final test, document and release

   Implementation Plan
	1) (done) Create feature menu
	2) (done) Implement each menu command (see above)
	3) (done) Implement Help module
		- Requires modifying Help.c to take argument	
	4) Add Analog Watchfaces
	5) Populate Feature Help Text

\* -------------------------------------------------------------------------------------- */
#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#include "resource_ids.auto.h"		// system-generated hook to program resources
	
#include "SlideShow.h"			// Pull in global declares
#include "menu_animation.h"		// pull in local module declares

#include "menu_images.h"		// Add Images Action Bar Menu
#include "menu_direction.h"		// Add Direction Action Bar Menu

#include "feature_analog.h"		// Add analog watchface module
	
#include "animation.h"			// pull in needed declares 
#include "Help.h"				// & function prototypes
#include "page_about.h"			// & function prototypes

// --------------------------------------------------------
//  declare private function protoypes
// --------------------------------------------------------
//static void handle_appear();
//static void handle_disappear();
//static void handle_load();
//static void handle_unload();


// --------------------------------------------------------
//  local module defines and variable definitions
// --------------------------------------------------------
#define NUM_MENU_SECTIONS 		3
#define NUM_MENU_FIRST_ITEMS	3
#define NUM_MENU_SECOND_ITEMS	2
#define NUM_MENU_THIRD_ITEMS 	4

static struct AnimationMenuData {
	SimpleMenuLayer menu_layer;
	SimpleMenuSection menu_sections[NUM_MENU_SECTIONS];
	SimpleMenuItem first_menu_items[NUM_MENU_FIRST_ITEMS];
	SimpleMenuItem second_menu_items[NUM_MENU_SECOND_ITEMS];
	SimpleMenuItem third_menu_items[NUM_MENU_THIRD_ITEMS];
} menu_data;

static Window menu_window;	// program primary menu window


// --------------------------------------------------------
//  public module defines and variable definitions
// --------------------------------------------------------
// --------------------------------------------------------
//			menu_animation_show_window()
//
//    Display Animation module menu window
// --------------------------------------------------------
void menu_animation_show_window() {

	window_stack_push(&menu_window, true);   // create menu window

}  // menu_animation_show_window()


// --------------------------------------------------------
//			menu_window_cleanup()
//
//    remove menu window so Action bar 
//     returns to calling to function
// --------------------------------------------------------
void menu_animation_cleanup() {

	window_stack_remove(&menu_window, false);

}  // menu_animation_cleanup()

	
// ------------------------------------------------------------------
//        Menu Callbacks section
//
//  Contains menu selection callbacks
// ------------------------------------------------------------------
static void menu_fast_callback(){

	animation_timer_run(FRAMERATE_FAST); // reset timer
	window_stack_pop(true);		// pop widow & return

}

void menu_slow_callback(){

	animation_timer_run(FRAMERATE_SLOW);
	window_stack_pop(true);

}

void menu_step_callback() {

	animation_timer_run(FRAMERATE_STEP);
	window_stack_pop(true);
	
}

void menu_images_callback() {

	menu_images_show_window();	// Call Image Bank Action Bar
//	window_stack_pop(true);		// then return to caling function
	
}

void menu_direction_callback() {

	menu_direction_show_window();	// call Direction Action Bar
//	window_stack_pop(true);		// then return to calling function
	
}

void menu_clock_left_callback() {

	feature_analog_config(LEFTHANDED, USESECONDS);
	feature_analog_show_window();
	
}

void menu_clock_right_callback() {

	feature_analog_config(RIGHTHANDED, USESECONDS);
	feature_analog_show_window();
	
}

void menu_about_callback() {

	page_about_show_window();			// Display About page

}

void menu_help_callback() {

	help_show_window(ANIMATION_HELP);	// Call context-sensitive Help page

}


// --------------------------------------------------------
//			menu_animation_init()
//
//     called by TempusFugit.c to initialize menu data
// --------------------------------------------------------
void menu_animation_init() {

// -------------------------------
// init window handlers
// -------------------------------
	window_init(&menu_window, "Timer Menu");
	window_set_background_color(&menu_window, GColorWhite);
//	window_set_window_handlers(&menu_window, (WindowHandlers) {
//        .appear = (WindowHandler)handle_appear,
//        .disappear = (WindowHandler)handle_disappear,
// 		.load = handle_load,
//		.unload = handle_unload,
//    });

// -------------------------------
// Initialize first menu section
// -------------------------------
int menu_count = 0;
	menu_data.first_menu_items[menu_count++] = (SimpleMenuItem) {
		.title = "FAST",
		.callback = menu_fast_callback,
	};
	menu_data.first_menu_items[menu_count++] = (SimpleMenuItem) {
		.title = "SLOW",
		.callback = menu_slow_callback,
	};	
	menu_data.first_menu_items[menu_count++] = (SimpleMenuItem) {
		.title = "STEP",
		.callback = menu_step_callback,
	};

	// -------------------------------
	// Initialize second menu section
	// -------------------------------
	menu_count = 0;

	menu_data.second_menu_items[menu_count++] = (SimpleMenuItem) {
		.title = "Images ->",
		.callback = menu_images_callback,
	};

	menu_data.second_menu_items[menu_count++] = (SimpleMenuItem) {
		.title = "Direction ->",
		.callback = menu_direction_callback,
	};
	

	// -------------------------------
	// Initialize third menu section
	// -------------------------------
	menu_count = 0;
	menu_data.third_menu_items[menu_count++] = (SimpleMenuItem) {
		.title = "Clock (Left)",
		.callback = menu_clock_left_callback,
	};

	menu_data.third_menu_items[menu_count++] = (SimpleMenuItem) {
		.title = "Clock (Right)",
		.callback = menu_clock_right_callback,
	};
		menu_data.third_menu_items[menu_count++] = (SimpleMenuItem) {
		.title = "Help ->",
		.callback = menu_help_callback,
	};

	menu_data.third_menu_items[menu_count++] = (SimpleMenuItem) {
		.title = "About ->",
		.callback = menu_about_callback,
	};

	// -------------------------------
	//  Bind menu items to
	//  corresponding menu sections
	// -------------------------------
	menu_data.menu_sections[0] = (SimpleMenuSection) {
		.num_items = NUM_MENU_FIRST_ITEMS,
		.items = menu_data.first_menu_items,
	};
	menu_data.menu_sections[1] = (SimpleMenuSection) {
		.title = "(More...)",
		.num_items = NUM_MENU_SECOND_ITEMS,
		.items = menu_data.second_menu_items,
	};
	menu_data.menu_sections[2] = (SimpleMenuSection) {
		.title = "(More...)",
		.num_items = NUM_MENU_THIRD_ITEMS,
		.items = menu_data.third_menu_items,
	};

// -------------------------------
//  initialize menu window
// -------------------------------
	simple_menu_layer_init(&menu_data.menu_layer, menu_window.layer.frame, &menu_window, menu_data.menu_sections, NUM_MENU_SECTIONS, NULL);
	layer_add_child(&menu_window.layer, &menu_data.menu_layer.menu.scroll_layer.layer);

}  // menu__animation_init()

// --------------------------------------------------------
//			menu_animation_deinit()
// --------------------------------------------------------
void menu_animation_deinit() {

} // menu_animation_deinit()
