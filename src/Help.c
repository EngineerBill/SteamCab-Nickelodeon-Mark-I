/* -------------------------------------------------------------------------------------- *\

                      help.c

	    Implements context-sensitive Help Window

	Called from :
					- Animation module

	To invoke, call the public function help_show_window(CONTEXT)
	 the values for CONTEXT are defined in Help.h and may
	 include:
				- ANIMATION_HELP
				- ANIMATION_ABOUT

TODO:

The text to be displayed is currently declared in an array in this module.
Ideally this should be moved to a config header file for easier access.
	
History:
		Derived from - PebbleSDK-1-12:

	~/Pebble/demos/feature_scroll_layer/src/feature_scroll_layer.c

   ----------------------------------------------------------------
	Author:			Peter Deutsch (engineerbill@stemchest.com)
	Date Created:	8/18/13
	Modified:		8/27/13 - Final version for Tempus Fugit
	MOdified:		9/3/13  - Adapted for SteamCab Slideshow
							  (move test strings to config file)
   ----------------------------------------------------------------


\* -------------------------------------------------------------------------------------- */
#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
	
#include "SlideShow.h"		// global declares
#include "Help.h"			// local declares
#include "Help_config.h"

// Needed Window Layer and other defines
//static TextLayer help_layer;	// info about each feature
//} help_data;

static ScrollLayer scroll_layer;	// here's where the magic happens
static TextLayer text_layer;		// only one text layer for now

static const int vert_scroll_text_padding = 4;


static Window help_window;

static void handle_appear();
static void handle_disappear();
static void handle_load();
static void handle_unload();

// --------------------------------------------------------
//			help_show_window()
//
//   public function to show context sensitive Help Page
//   (Called with flag indicting which Help page to show.
//     these flags are defined in "TempusFugit.h Add flag
//         there, plus another text array above and 
//          another case statement here to extend)
//
// --------------------------------------------------------
void help_show_window(int page){


	scroll_layer_set_content_offset(&scroll_layer, GPointZero, false);

	switch (page) {

	case ANIMATION_ABOUT:
				text_layer_set_text(&text_layer, animation_about_scroll_text);
				break;

	case ANIMATION_HELP:
				text_layer_set_text(&text_layer, animation_help_scroll_text);
				break;
	}
	
	window_stack_push(&help_window, true);

}  // help_show_window()


// --------------------------------------------------------------------
//            Window Callbacks
// --------------------------------------------------------------------
// --------------------------------------------------------
//		handle_appear()
//
//   called for housekeeping when window appears.
// --------------------------------------------------------
static void handle_appear() {

	
}  // handle_appear()

// --------------------------------------------------------
//		handle_disappear()
//
//   perform cleanup when window is hidden.
// --------------------------------------------------------
static void handle_disappear() {

	
}  // handle_disappear()

static void handle_load() {

}  // handle_load()

static void handle_unload() {

}  // handle_unload()
// --------------------------------------------------------------------------
//          click provider section
// --------------------------------------------------------------------------

// --------------------------------------------------------
//			clicked_up()
// --------------------------------------------------------
//static void clicked_up(ClickRecognizerRef recognizer, void *context) {
//	window_stack_pop(true);			// & return
//}  // clicked_up()

// --------------------------------------------------------
//			clicked_select()
// --------------------------------------------------------
static void clicked_select(ClickRecognizerRef recognizer, void *context) {

	window_stack_pop(true);			// & return

}  // clicked_select()

// --------------------------------------------------------
//			clicked_down()
// --------------------------------------------------------
//static void clicked_down(ClickRecognizerRef recognizer, void *context) {
//	window_stack_pop(true);			// & return
//}  // clicked_down()

// --------------------------------------------------------
//			clicked_config_provider()
// --------------------------------------------------------
static void click_config_provider(ClickConfig **config, void* context) {
//    config[BUTTON_ID_UP]->click.handler = clicked_up;
    config[BUTTON_ID_SELECT]->click.handler = clicked_select;
//    config[BUTTON_ID_DOWN]->click.handler = clicked_down;
//    config[BUTTON_ID_SELECT]->long_click.handler = long_clicked_select;
}

// --------------------------------------------------------
//		help_init()
//
//  Called by TempusFugit.c when program is first init'ed
// --------------------------------------------------------
void help_init() {

	window_init(&help_window, "Tempus Fugit Help");
	window_set_window_handlers(&help_window, (WindowHandlers) {
		.appear = handle_appear,
		.disappear = handle_disappear,
		.load = handle_load,
		.unload = handle_unload,
  });

	window_set_background_color(&help_window, GColorBlack);

// set up text bounds beyond window bounds for ScrollLater capability
const GRect max_text_bounds = GRect(0, 0, 144, 2000);

// Initialize the scroll layer
	scroll_layer_init(&scroll_layer, help_window.layer.bounds);

//  --------------------------------------
//     Set up window click handlers
//  --------------------------------------
	
ScrollLayerCallbacks overrideClbs = {
	.click_config_provider = &click_config_provider,
	.content_offset_changed_handler = NULL //Not interested.
};
//Call this to effectively set the overrideConfigProvider.
//This overrideConfigProvider will be called in scroll_layer_set_click_config_onto_window().
	scroll_layer_set_callbacks((ScrollLayer *)&scroll_layer, overrideClbs);
	
// Now bind the scroll layer to the window so that up and down buttons
// map to scrolling. You can choose to set up and use callbacks using
// scroll_layer_set_callbacks to add or override interactivity
	scroll_layer_set_click_config_onto_window(&scroll_layer, &help_window);

// Set the scroll layer's max size
	scroll_layer_set_content_size(&scroll_layer, max_text_bounds.size);


// Now initialize the text layer
	text_layer_init(&text_layer, max_text_bounds);
	text_layer_set_text(&text_layer, animation_help_scroll_text);	// NOTE: we start with largest text to setmax_size below...
	text_layer_set_background_color(&text_layer, GColorBlack);	// In operation, each call to this module will
	text_layer_set_text_color(&text_layer, GColorWhite);		// specify text to show

//	text_layer_set_font(&text_layer, norm14);
// Change the font to something large enough to be legible...
// This uses a default system font; you can check out all the system fonts
// in the system include file "pebble_fonts.h"" or you can take a look at
// the sample program "feature_custom_font" " to see how to add your own...
	text_layer_set_font(&text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));

// Trim text layer and scroll content to fit text box
	GSize max_size = text_layer_get_max_used_size(app_get_current_graphics_context(), &text_layer);
	text_layer_set_size(&text_layer, max_size);
	scroll_layer_set_content_size(&scroll_layer, GSize(144, max_size.h + vert_scroll_text_padding));
	scroll_layer_set_content_offset(&scroll_layer, GPointZero, false);

// Add the layers for display
	scroll_layer_add_child(&scroll_layer, &text_layer.layer);

// The inverter layer will highlight some text
//	inverter_layer_init(&inverter_layer, GRect(0, 28, window.layer.frame.size.w, 28));
//	scroll_layer_add_child(&scroll_layer, &inverter_layer.layer);

	layer_add_child(&help_window.layer, &scroll_layer.layer);

}  // help_init()

void help_deinit() {

}  // help_deinit()
