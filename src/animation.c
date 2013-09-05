/* -------------------------------------------------------------------------------------- *\

                      animation.c

	  Implements SteamCab SlideShow animation feature

	Called:		from SlideShow::

Theory of Operation:

	The SlideShow:: program entry point pbl_main() sets up
	a handle_timer() callback to process timer ticks and the
	handle_init() function schedules an inital timer tick to
	begin animation. The handle_init() routine in SlideShow.c
	program initiation module calls this module's
	page_animation_init()  function to create the appropriate
	Window structure and set up the needed layers. 	

	As timer ticks are received in <Program>:handle_timer(),
	they are dispatched to this module's page_animation_tick()
	function. This monitors the frame_rate flag to determine
	how frequently the screen image should be updated and processes
	as needed.
	
	Pressing the appropriate menu button can set the animation
	rate and animation direction. This is used to change timer
	duration, as needed.
	
	
	Note that the needed animation images are loaded in the
	page_animation:: handle_init() function (and *must* therefore
	be properly deinit'ed when the program exits or you will
	introduce a memory leak that will lead to unstable operation!)
	The BmpContainer array that holds these is declared global so
	this module's page_animate_tick() routine can access when operating.
	
	When in single step, hitting the UP and DOWN buttons will single
	step through the images. Pressing SELECT will initially cycle
	through	the framerates. Eventually it is planned to bring up a
	menu which will allow the user to START/STOP operation, change
	mode and see the About and Help pages.


Things you can do:
				- Try adding your own set of images
				- Try adding multiple sets and a menu item 
				  to change between them
				- Try adding a FORWARD/REVERS button

				- Most importantly - Have Fun!


   ----------------------------------------------------------------
	Author:				Peter Deutsch (engineerbill@stemchest.com)
	Date Created:		9/1/13
	Last Update:		9/3/13 - Converted for SteamCab SlideShow
   ----------------------------------------------------------------


\* -------------------------------------------------------------------------------------- */
#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
	

#include "SlideShow.h"			// provides global program defines
#include "animation.h"			// provides local module defines	
#include "animation_config.h"	// Holds compiled defaults. See file for details
#include "menu_animation.h"	// program defines for other modules

#include "resource_ids.auto.h"	// system-generated hook to program resources

// --------------------------------------------------------
//			Declare Local Module variables
// --------------------------------------------------------


// Provides image animation data for module Window
typedef struct {
	Layer animation_layer;							// imageslideshow layer
	BmpContainer image_container[NUMBER_OF_IMAGES];	// image containers
	int current_image;								// tracks active image
} ImageData;

static ImageData image_data;	// tracks animation data

//static int tick_count;				// used to cycle between images

static int frame_rate;			// allowed values defined in animation.h file

static Window animation_window;		// This module's window

static int timer_count = 0;			// introduces a start delay
//static int pause = 0;				// pause delay				
static char debug_buffer[30];


static TextLayer debug_layer;
/*
static TextLayer debug_layer;
//static TextLayer tempus_layer;
static TextLayer fugit_layer;
static TextLayer version_layer;
*/
// ----------------------------------------
//    Locally defined Window callbacks
// ----------------------------------------
static void handle_appear();
static void handle_disappear();
static void handle_load();
static void handle_unload();

static void click_config_provider();
static void clicked_down();
static void clicked_up();
static void clicked_select();
//static bool animation_running = 0;		// tracks animation state

// ----------------------------------------------------------------------------
//               Public Window functions
// ----------------------------------------------------------------------------
// --------------------------------------------------------
//			animation_timer_*()
//
//       controls speed of animation timer
// --------------------------------------------------------
void animation_timer_run(int new_rate) {
	
// first cancel old timer
	app_timer_cancel_event(g_app_context, animation_timer);

	if(new_rate == FRAMERATE_STEP) {
		frame_rate = FRAMERATE_STEP;  // don't reset timer if stopping
		return;
	}

// else confirm framerate within specified range
	if(new_rate < FRAMERATE_FAST) {
		frame_rate = FRAMERATE_FAST;
	}
	else if (new_rate > FRAMERATE_SLOW) {
		frame_rate = FRAMERATE_SLOW;	
	}
	else {
		frame_rate = new_rate;
	}

// ... and reset timer for next event
	animation_timer = app_timer_send_event(g_app_context, frame_rate, 42);

}  // animation_timer_run()

/*	
void animation_timer_fast() {
	
	frame_rate = FRAMERATE_FAST;
	animation_timer_run(frame_rate);
	
}  // animation_timer_stop()

	
void animation_timer_slow() {
	
	frame_rate = FRAMERATE_SLOW;

}  // animation_timer_stop()


void animation_timer_step() {
	
	frame_rate = FRAMERATE_STEP;

}  // animation_timer_stop()
*/
void animation_timer_start() {

	frame_rate = FRAMERATE_DEFAULT;
	animation_timer = app_timer_send_event(g_app_context, frame_rate, 42);
	
}  // animationtimer_start()

// --------------------------------------------------------
//			start_show_window()
//       (called from main program)
// --------------------------------------------------------
void animation_show_page() {

// activate app window
	window_stack_push(&animation_window, true);

}  //page_start_show_page()

//void page_animation_start_animation() {
//
//	animation_timer = app_timer_send_event(g_app_context, FRAMERATE_DEFAULT, 42);
//
//}  // animation_show_page()

// ----------------------------------------------------------------------------
//    helper functions
//
void debug(char *msg, int arg1, int arg2) {

GFont norm24 = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);	

	snprintf(debug_buffer, 20, "%s: arg1: %d arg2: %d",msg, arg1, arg2);
	text_layer_set_text(&debug_layer, debug_buffer);

}  // debug()

// --------------------------------------------------------
//			page_animation_timer()
//		(called from TempusFugit tick handler)
// --------------------------------------------------------
void handle_animation_timer() {

	if(timer_count == -1) {  // process end of loop
//  remove element 9
		layer_remove_from_parent(&image_data.image_container[image_data.current_image].layer.layer);
		bmp_deinit_container(&image_data.image_container[image_data.current_image]);
// now add in element 0
		image_data.current_image = 0;		
		bmp_init_container(IMAGE_RESOURCE_IDS[image_data.current_image], &image_data.image_container[image_data.current_image]);
		layer_set_frame(&image_data.image_container[image_data.current_image].layer.layer, GRect(0,0,144,130));
		layer_add_child(&animation_window.layer, &image_data.image_container[image_data.current_image].layer.layer);
		timer_count = 0;
		animation_timer_run(frame_rate);
	debug("flop:", image_data.current_image, 0);
		return;
	}

	if(timer_count < (NUMBER_OF_IMAGES-1)) {  // start another time interval
		timer_count++;
//  we be animating...
		layer_remove_from_parent(&image_data.image_container[image_data.current_image].layer.layer);
		bmp_deinit_container(&image_data.image_container[image_data.current_image]);

		image_data.current_image++; // get next image
		
		bmp_init_container(IMAGE_RESOURCE_IDS[image_data.current_image], &image_data.image_container[image_data.current_image]);
		layer_set_frame(&image_data.image_container[image_data.current_image].layer.layer, GRect(0,0,144,130));
		layer_add_child(&animation_window.layer, &image_data.image_container[image_data.current_image].layer.layer);

//		snprintf(debug_buffer, 20, "tm: %d img: %d", timer_count, image_data.current_image);
//		text_layer_set_text(&debug_layer, debug_buffer);

		if(image_data.current_image == (NUMBER_OF_IMAGES-1)) {
			timer_count = -1;
//			pause = 0;
		}
	}	

	animation_timer_run(frame_rate);
	debug("flip:", image_data.current_image, 0);

	//	text_layer_init(&debug_layer, GRect (17, 108, 110, 28));

	
}  // handle_animation_timer()


// ----------------------------------------------------------------------------
//    private module functions
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
//               Window Handlers
// ----------------------------------------------------------------------------

// --------------------------------------------------------
//      handle_load()
// --------------------------------------------------------

static void handle_load(Window *window) {

}  // handle_load

// --------------------------------------------------------
//      handle_unload()
// --------------------------------------------------------
static void handle_unload(Window *window) {

}  // handle_unload()

// --------------------------------------------------------
//      handle_appear()
// --------------------------------------------------------
static void handle_appear(Window *window) {

}  // handle_appear()

// --------------------------------------------------------
//      handle_disappear()
// --------------------------------------------------------
static void handle_disappear(Window *window) {

}  // handle_disappear()

// --------------------------------------------------------------------------
//          click provider section
// --------------------------------------------------------------------------

// --------------------------------------------------------
//			clicked_up()
// --------------------------------------------------------
static void clicked_up(ClickRecognizerRef recognizer, void *context) {



	if(frame_rate == FRAMERATE_STEP) {  // then step image
		frame_rate = FRAMERATE_SLOW;
		handle_animation_timer();
		return;
	}
	
	frame_rate = frame_rate - 50;
	
	if(frame_rate < FRAMERATE_FAST) {
		frame_rate = FRAMERATE_FAST;
	}

	debug("UP", frame_rate, 0);

}  // clicked_up()


// --------------------------------------------------------
//			clicked_select()
// --------------------------------------------------------
static void clicked_select(ClickRecognizerRef recognizer, void *context) {

	debug("SELECT", frame_rate, 0);
	
		menu_animation_show_page();

}  // clicked_select()


// --------------------------------------------------------
//			clicked_down()
// --------------------------------------------------------
static void clicked_down(ClickRecognizerRef recognizer, void *context) {



	if(frame_rate == FRAMERATE_STEP) {  // the step image
		handle_animation_timer();		// need to pass "reverse" flag
		return;
	}
	
	frame_rate = frame_rate + 50;
	
	if(frame_rate > FRAMERATE_SLOW){
		frame_rate = FRAMERATE_STEP;
		handle_animation_timer();		// need to pass "reverse" flag
	}
	debug("DOWN", frame_rate, 0);
}  // clicked_down()

// --------------------------------------------------------
//			clicked_config_provider()
// --------------------------------------------------------
static void click_config_provider(ClickConfig **config, void* context) {
    config[BUTTON_ID_DOWN]->click.handler = clicked_down;
    config[BUTTON_ID_UP]->click.handler = clicked_up;
    config[BUTTON_ID_SELECT]->click.handler = clicked_select;
//    config[BUTTON_ID_SELECT]->long_click.handler = long_clicked_select;
}

// --------------------------------------------------------
//			page_start_init()
//
//         Build window layers here
// --------------------------------------------------------
void handle_animation_init(){

//	GFont norm28 = fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD);
	GFont norm24 = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
//	GFont norm18 = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
// ---------------------------------------
// Initialize module variables
// ---------------------------------------
	frame_rate = FRAMERATE_DEFAULT;		// start with static image

	resource_init_current_app(&APP_RESOURCES);

// Initialize Image Container
//	for(int i=0; i<NUMBER_OF_IMAGES; i++) {
//        bmp_init_container(IMAGE_RESOURCE_IDS[i], &image_data.image_container[i]);
//	}
// initialize first bitmap container
	bmp_init_container(IMAGE_RESOURCE_IDS[0], &image_data.image_container[0]);

// ---------------------------------------
// Set up local Window handlers
// ---------------------------------------
	window_init(&animation_window, "Muybridge Animate");
    window_set_window_handlers(&animation_window, (WindowHandlers) {
        .appear = (WindowHandler)handle_appear,
        .disappear = (WindowHandler)handle_disappear,
		.load = handle_load,
		.unload = handle_unload,
    });

	window_set_fullscreen(&animation_window, true);	// remove top bar and replace with debug layer 
	window_set_background_color(&animation_window, GColorBlack);

	window_set_click_config_provider(&animation_window, (ClickConfigProvider) click_config_provider);
	

// Init child_a layer at (0,0) and size (40, 46):
//	layer_init(&image_data.image_container[0].layer.layer, GRect(0, 0, 144, 91));
//	layer_init(&image_data.image_container[0].layer.layer, GRect(5, 20, 130, 0));

	image_data.current_image = 0;		// initialize logo tracking GRect(0,0,144,130)
	layer_set_frame(&image_data.image_container[0].layer.layer, GRect(0,0,144,130));
	layer_add_child(&animation_window.layer, &image_data.image_container[0].layer.layer);

// set up Text layers
//	text_layer_init(&debug_layer, GRect (17, 108, 110, 28));
	text_layer_init(&debug_layer, GRect (0, 125, 144, 28));
//	text_layer_set_text(&debug_layer, "- Muybridge -");
	snprintf(debug_buffer, 20, "tm: %d img: %d", timer_count, image_data.current_image);
	text_layer_set_text(&debug_layer, debug_buffer);
	text_layer_set_background_color(&debug_layer, GColorBlack);
	text_layer_set_text_color(&debug_layer, GColorWhite);
	text_layer_set_text_alignment(&debug_layer, GTextAlignmentCenter);
	text_layer_set_font(&debug_layer, norm24);
	layer_add_child(&animation_window.layer, &debug_layer.layer);	

/*

	text_layer_init(&fugit_layer, GRect (86, 108, 75, 28));
	text_layer_set_text(&fugit_layer, "Fugit");
	text_layer_set_background_color(&fugit_layer, GColorBlack);
	text_layer_set_text_color(&fugit_layer, GColorWhite);
	text_layer_set_font(&fugit_layer, norm24);

	text_layer_init(&version_layer, GRect (28, 137, 90, 20));
	text_layer_set_text_alignment(&version_layer, GTextAlignmentCenter);
	text_layer_set_text(&version_layer, "- beta 0.1");
	text_layer_set_background_color(&version_layer, GColorBlack);
	text_layer_set_text_color(&version_layer, GColorWhite);
	text_layer_set_font(&version_layer, norm18);
*/

	
}  // handle_animation_init()


// --------------------------------------------------------
//			page_start_deinit()
// --------------------------------------------------------
void handle_animation_deinit() {

// clean up image containers
//	for(int i = 0; i< NUMBER_OF_IMAGES; i++){
//		bmp_deinit_container(&image_data.image_container[i]);
//	}
	bmp_deinit_container(&image_data.image_container[image_data.current_image]);
	
}  // handle_animation_deinit()
