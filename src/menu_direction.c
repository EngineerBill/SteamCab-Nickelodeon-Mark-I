/* -------------------------------------------------------------------------------------- *\

                      menu_direction.c

	Implements <program>:: Direction Action Bar menu

	Called:		animation_menu callback

   ----------------------------------------------------------------
	Author:				Peter Deutsch (engineerbill@stemchest.com)
	Date Created:		8/19/13
	Modified:			9/7/13	- adapted for Nickleodeon app
   ----------------------------------------------------------------

\* -------------------------------------------------------------------------------------- */
#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#include "menu_direction.h"

#include "SlideShow.h"			// global declares
#include "animation.h"			// global declares
#include "menu_animation.h"			// global declares

static int pending_buttons;		// used to track user choice

static Window window;			// Local Action Bar Window

static ActionBarLayer action_bar;	// Needed Window layers
static TextLayer button_text_layer;	// 

static char *bank0_ptr = "Forward";	// Display strings
static char *bank1_ptr = "Reverse";	//

// If the following string appears it means
// that the pending_buttons variable has been
// gorf'ed and we've lost the bubble...
// Theoretically this can't happen, but
// things like writing past the end of a buffer
// can do some strange thigngs...
static char * error_ptr = "ERROR: pending_buttons!";


// array of action bar icons (Up arrow, Down arrow and Tick)

#define NUMBER_OF_ICONS	3		// Count of bitmaps PER BANK!

static int IMAGE_RESOURCE_ICONS[NUMBER_OF_ICONS] = {
	RESOURCE_ID_UP_ARROW, RESOURCE_ID_TICK, RESOURCE_ID_DOWN_ARROW
};

static BmpContainer icon_container[NUMBER_OF_ICONS]; // used by Action Bar menus...

// ----------------------------------------
//    Locally defined Window callbacks
// ----------------------------------------
//static void handle_appear();
//static void handle_disappear();
//static void handle_load();
//static void handle_unload();

// --------------------------------------------------------
//			menu_calc_button_select()
// --------------------------------------------------------
void menu_direction_show_window() {

	animation_direction_set(pending_buttons);
	menu_animation_cleanup();	// Remove menu window so we 
							//pop back to Stopwatch function, not menu

    window_stack_push(&window, true);

}  // menu_directon_show_window()

// --------------------------------------------------------
//			window_load()
// --------------------------------------------------------
static void window_load(){

}  // window_load()

// --------------------------------------------------------
//			window_unload()
// --------------------------------------------------------
static void window_unload(){

}  // window_unload()

// --------------------------------------------------------
//			clicked_up()
// --------------------------------------------------------
static void clicked_up(ClickRecognizerRef recognizer, void *context) {

char *ptr;
	
	ptr = bank0_ptr;

	if(++pending_buttons > 1) {  // should not hard-code number of choices... :-(
		pending_buttons = 0;
	}

	switch(pending_buttons) {
	case 0:	ptr = bank0_ptr;
			break;
	case 1:	ptr = bank1_ptr;
			break;
	}

	text_layer_set_text(&button_text_layer, (char *) ptr);

}  // clicked_up()


// --------------------------------------------------------
//			clicked_select()
// --------------------------------------------------------
static void clicked_select(ClickRecognizerRef recognizer, void *context) {
	
	animation_direction_set(pending_buttons);
	menu_animation_cleanup();	// Remove menu window so we 
							//pop back to Stopwatch function, not menu

	window_stack_pop(true);
	
	
}  // clicked_select()


// --------------------------------------------------------
//			clicked_down()
// --------------------------------------------------------
static void clicked_down(ClickRecognizerRef recognizer, void *context) {
char *ptr;
	
	ptr = bank0_ptr;

	if(--pending_buttons == -1) {
		pending_buttons = 1;
	}

	switch(pending_buttons) {
	case 0:	ptr = bank0_ptr;
			break;
	case 1:	ptr = bank1_ptr;
			break;
	}

	text_layer_set_text(&button_text_layer, (char *) ptr);

}  // clicked_down()


static void click_config_provider(ClickConfig **config, void* context) {
    config[BUTTON_ID_DOWN]->click.handler = clicked_down;
    config[BUTTON_ID_UP]->click.handler = clicked_up;
    config[BUTTON_ID_SELECT]->click.handler = clicked_select;
//    config[BUTTON_ID_SELECT]->long_click.handler = long_clicked_select;
}


// --------------------------------------------------------
//			menu_direction_select()
// --------------------------------------------------------
void menu_direction_init(){
	
//GFont norm28 = fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD);
GFont norm24 = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
//GFont norm14 = fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD);
	
    window_init(&window, "Image Bank - Select");
    window_set_window_handlers(&window, (WindowHandlers){
        .unload = window_unload,
        .load = window_load,
    });
// -------------------------------------------------
//     set up state and global graphics
// -------------------------------------------------
	
	for(int i=0; i<NUMBER_OF_ICONS; i++) {
        bmp_init_container(IMAGE_RESOURCE_ICONS[i], &icon_container[i]);
	}

// -------------------------------------------------
//     set up action bar
// -------------------------------------------------
	action_bar_layer_init(&action_bar);
    action_bar_layer_add_to_window(&action_bar, &window);
    action_bar_layer_set_click_config_provider(&action_bar, click_config_provider);

    action_bar_layer_set_icon(&action_bar, BUTTON_ID_UP, &icon_container[0].bmp);
    action_bar_layer_set_icon(&action_bar, BUTTON_ID_SELECT, &icon_container[1].bmp);
    action_bar_layer_set_icon(&action_bar, BUTTON_ID_DOWN, &icon_container[2].bmp);
	
	// set up text layer
	text_layer_init(&button_text_layer, GRect (35, 64, 80, 28));

	pending_buttons = animation_direction_get();
	
char *ptr = bank0_ptr;

	switch(pending_buttons) {
	case 0:	ptr = bank0_ptr;
			break;
	case 1:	ptr = bank1_ptr;
			break;
	default: ptr = error_ptr;
	}

	text_layer_set_text(&button_text_layer, (char *) ptr);
	text_layer_set_background_color(&button_text_layer, GColorWhite);
	text_layer_set_text_color(&button_text_layer, GColorBlack);
	text_layer_set_font(&button_text_layer, norm24);
	layer_add_child(&window.layer, &button_text_layer.layer);

//	window_stack_push(&window, true);
	

}  // menu_direction_init()

// -------------------------------------------------
//     clean up state and global graphics
// -------------------------------------------------
void menu_direction_deinit() {
	
	for(int i=0; i < NUMBER_OF_ICONS; i++) {
        bmp_deinit_container(&icon_container[i]);
	}


}  // menu_direction_init()

