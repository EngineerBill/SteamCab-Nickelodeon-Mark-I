/* -------------------------------------------------------------------------------------- *\

                      animation.c

	  Implements SteamCab SlideShow animation feature

	Called:		from SlideShow::

Theory of Operation:

	The Nickleodeon:: program entry point "pbl_main()" sets up
	a handle_timer() callback to process timer ticks. PebbleOS then
	calls the handle_init() function to allow your program to set
	itself up. This function in turns calls "<module>_init()" to
	set up each individual module. Here local BmpContainers are
	initialized, Window Layers are defined and program variables
	are loaded with their default values (specified in the 
	"<program>_config.h" file.
	
	The handle_init() function then calls "animation_show_window()"
	to push the opening Window onto the stack and then calls
	"animation_timer_start()" to schedule the first timer tick.
	
	As timer ticks are received in <Program>:handle_timer(),
	they are dispatched to this module's feature_animation_timer()
	function. This updates the currently displayed image and then
	calls "animation_timer_run()" to reset the timer (Note: if the
	current frame_rate is STEP, then no timer is launched. You can
	still change images with the UP/DOWN buttons or by pushing SELECT
	and resetting frame_rate to FAST or SLOW>

	Pressing the appropriate menu button can set the animation
	rate and animation direction. This is used to change timer
	duration, as needed.
	
	Note that the needed animation images are loaded in the
	animation::feature_animation_init() function (and *must* therefore
	be properly deinit'ed when the program exits or you will
	introduce a memory leak that will lead to unstable operation!)

	NOTE: A reasonable desire for naming consistency would suggest
		  calling the init routines for this module "animation_init()"
		  and "animation_deinit()" but as it turns out the Pebble API
		  already has a routine called "animation_init()" so we had
		  to improvise. Sorry about that...
	
	When single stepping, hitting the UP and DOWN buttons will single
	step through the images. Pressing SELECT will call up the program
	menu when you have three options to allow you to cycle through
	the framerates. Note that when animating, you can tune the framerate
	with the UP/DOWN buttons. When single stepping, UP/DOWN change images
	in either direction.


Things you can do:
				- Try adding your own set of images to or both banks
				- Try adding a FORWARD/REVERSE button

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

//static int tick_count;		// used to cycle between images

static int frame_rate;			// determines number of images per sec
								// allowed values defined in animation.h file)
static int direction;			// Direction to step through images (FORWARD or REVERSE)

static int bank;				// Selects set of images to display (in prototype can be HORSE or GIRL)


static int image_count;			// introduces a start delay


static Window animation_window;	// This module's window

static TextLayer debug_layer;	// used to show debug messages (will later becom marquee text)
static char debug_buffer[30];


/*
//static TextLayer tempus_layer;
static TextLayer fugit_layer;
static TextLayer version_layer;
*/


// ----------------------------------------
//    Locally defined Window callbacks
// ----------------------------------------
static void click_config_provider();
static void clicked_down();
static void clicked_up();
static void clicked_select();

static void debug();

// ----------------------------------------------------------------------------
//               Public Window functions
// ----------------------------------------------------------------------------
// --------------------------------------------------------
//			start_show_window()
//       (called from main program)
// --------------------------------------------------------
void animation_show_window() {

// activate app window
	window_stack_push(&animation_window, true);

}  //page_start_show_window()

//void page_animation_start_animation() {
//
//	animation_timer = app_timer_send_event(g_app_context, FRAMERATE_DEFAULT, 42);
//
//}  // animation_show_window()

// --------------------------------------------------------
//			page_animation_timer()
//		(called from TempusFugit tick handler)
// --------------------------------------------------------
void feature_animation_timer() {

	update_image();				// first update Window
	animation_timer_run(frame_rate);		// the set new timer (function will determine if needed)


}  // feature_animation_timer()

// --------------------------------------------------------
//			animation_timer_run()
//
//       controls speed of animation timer
//
//  Cancel any previous timer, set framerate and if
//  not STEP, then start a new timer.
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


// --------------------------------------------------------
//			animation_direction_set()
//
//       sets animation direction variable
//
//  Called from Direction Action Bar Menu.
// --------------------------------------------------------
void animation_direction_set(int buttons) {

	direction = buttons;

}  // animation_direction_set()


int animation_direction_get() {

	return direction;

}  // animation_direction_get()

// --------------------------------------------------------
//		 	animation_bank_set()
//
//         sets animation bank variable
//   (this determines which set of images are shown)
//
//      Called from Direction Action Bar Menu.
// --------------------------------------------------------
void animation_bank_set(int button) {

	bank = button;

}  // animation_direction_set()


int animation_bank_get() {

	return bank;

}  // animation_bank_get()


// ----------------------------------------------------------------------------
//    helper functions
//
// ----------------------------------------------------------------------------
static void debug(char *msg, int arg1, int arg2) {

//GFont norm24 = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);	
	if(DEBUGGING) {
		snprintf(debug_buffer, 20, "%s: arg1: %d arg2: %d",msg, arg1, arg2);
		text_layer_set_text(&debug_layer, debug_buffer);
	}

}  // debug()

// --------------------------------------------------------
//			animation_timer_start()
//
//  called once from <main program>::handle_init()
//  to set up default framerate and start initial timer
//  (if needed).
//
//  Note: you *can't* cancel a timer that hasn't been
//  defined yet, so need to call this once to get things
//  going. Then you can call animation_timer_run() after
//  each image change - it will then handle stepping, if
//  needed...
// --------------------------------------------------------
void animation_timer_start() {

	frame_rate = FRAMERATE_DEFAULT;
	if(frame_rate != FRAMERATE_STEP) {
		animation_timer = app_timer_send_event(g_app_context, frame_rate, 42);
	}

}  // animationtimer_start()


// ----------------------------------------------------------------------------
//			   update_image()
//
//   Main routine to change currently displayed image.
// ----------------------------------------------------------------------------

void update_image() {

//  we be animating...

// case: loop around the top
	if((direction == UP) && (image_data.current_image == (NUMBER_OF_IMAGES-1))) {
//  remove last element
		layer_remove_from_parent(&image_data.image_container[image_data.current_image].layer.layer);
		bmp_deinit_container(&image_data.image_container[image_data.current_image]);
// now add in bottom element
		bmp_init_container(IMAGE_RESOURCE_IDS[bank][0], &image_data.image_container[0]);
		layer_set_frame(&image_data.image_container[0].layer.layer, GRect(0,0,144,130));
		layer_add_child(&animation_window.layer, &image_data.image_container[0].layer.layer);
		image_data.current_image = 0;			// reset current_image
		animation_timer_run(frame_rate);		// and reset timer (if needed)
	debug("loop up:", image_data.current_image, 0);
	}


// case: loop around the bottom
	else if((direction == DOWN) && (image_data.current_image == 0)) {
//  remove first element
		layer_remove_from_parent(&image_data.image_container[image_data.current_image].layer.layer);
		bmp_deinit_container(&image_data.image_container[image_data.current_image]);
// now add in top element
	int temp = (NUMBER_OF_IMAGES - 1);
		bmp_init_container(IMAGE_RESOURCE_IDS[bank][temp], &image_data.image_container[temp]);
		layer_set_frame(&image_data.image_container[temp].layer.layer, GRect(0,0,144,130));
		layer_add_child(&animation_window.layer, &image_data.image_container[temp].layer.layer);
		image_data.current_image = temp;		// reset current_image
		animation_timer_run(frame_rate);		// and reset timer (if needed)
	debug("loop down:", image_data.current_image, 0);
	}

// case: increment the image
	else if(direction == UP) {  
		layer_remove_from_parent(&image_data.image_container[image_data.current_image].layer.layer);
		bmp_deinit_container(&image_data.image_container[image_data.current_image]);

		image_data.current_image++; // get next image
		bmp_init_container(IMAGE_RESOURCE_IDS[bank][image_data.current_image], &image_data.image_container[image_data.current_image]);
		layer_set_frame(&image_data.image_container[image_data.current_image].layer.layer, GRect(0,0,144,130));
		layer_add_child(&animation_window.layer, &image_data.image_container[image_data.current_image].layer.layer);
	debug("incrementing:", image_data.current_image, 0);

	}

// case: increment the image
	else  { // => direction == DOWN
		layer_remove_from_parent(&image_data.image_container[image_data.current_image].layer.layer);
		bmp_deinit_container(&image_data.image_container[image_data.current_image]);

		image_data.current_image--; // get next image
		bmp_init_container(IMAGE_RESOURCE_IDS[bank][image_data.current_image], &image_data.image_container[image_data.current_image]);
		layer_set_frame(&image_data.image_container[image_data.current_image].layer.layer, GRect(0,0,144,130));
		layer_add_child(&animation_window.layer, &image_data.image_container[image_data.current_image].layer.layer);
	debug("decrementing:", image_data.current_image, 0);

	}

// Now reset timer and go...
	animation_timer_run(frame_rate);
//	debug("flip:", image_data.current_image, 0);

	
}  // update_image()


// ----------------------------------------------------------------
//               Button Handlers
// ----------------------------------------------------------------
// --------------------------------------------------------
//			clicked_up()
// --------------------------------------------------------
static void clicked_up(ClickRecognizerRef recognizer, void *context) {
int temp;

	if(frame_rate == FRAMERATE_STEP) {  // step image and done
		temp = animation_direction_get();
		animation_direction_set(REVERSE);
		update_image();
		animation_direction_set(temp);
		return;
	}
// else adjust framerate and reset timer
	
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
	menu_animation_show_window();

}  // clicked_select()


// --------------------------------------------------------
//			clicked_down()
// --------------------------------------------------------
static void clicked_down(ClickRecognizerRef recognizer, void *context) {
int temp;

	if(frame_rate == FRAMERATE_STEP) {  // step image and done
		temp = animation_direction_get();
		animation_direction_set(FORWARD);
		update_image();
		animation_direction_set(temp);
		return;
	}
	
// else adjust framerate and reset timer
	frame_rate = frame_rate + 50;
	
	if(frame_rate > FRAMERATE_SLOW){
		frame_rate = FRAMERATE_SLOW;
		feature_animation_timer();		// need to pass "reverse" flag
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
void feature_animation_init(){

//	GFont norm28 = fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD);
	GFont norm24 = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
//	GFont norm18 = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);

// ---------------------------------------
// Initialize module defaults
// ---------------------------------------
	frame_rate = FRAMERATE_DEFAULT;				// set in animation_config.h
	direction  = DIRECTION_DEFAULT;				// set in animation_config.h
	bank       = BANK_DEFAULT;					// set in animation_config.h
	image_data.current_image = IMAGE_DEFAULT;	// set in animation_config.h

	resource_init_current_app(&APP_RESOURCES); // set up access to resources

// Initialize Image Container
//	for(int i=0; i<NUMBER_OF_IMAGES; i++) {
//        bmp_init_container(IMAGE_RESOURCE_IDS[bank][i], &image_data.image_container[i]);
//	}
// initialize first bitmap container
	bmp_init_container(IMAGE_RESOURCE_IDS[bank][0], &image_data.image_container[0]);

// ---------------------------------------
// Set up local Window handlers
// ---------------------------------------
	window_init(&animation_window, "Nickelodeon");

	window_set_fullscreen(&animation_window, true);	// remove top bar and replace with debug layer 
	window_set_background_color(&animation_window, GColorBlack);

	window_set_click_config_provider(&animation_window, (ClickConfigProvider) click_config_provider);
	

//  Initialize Image layer
//	layer_init(&image_data.image_container[0].layer.layer, GRect(0, 0, 144, 91));
//	layer_init(&image_data.image_container[0].layer.layer, GRect(5, 20, 130, 0));
	layer_set_frame(&image_data.image_container[0].layer.layer, GRect(0,0,144,130));
	layer_add_child(&animation_window.layer, &image_data.image_container[0].layer.layer);

// set up Text layers
//	text_layer_init(&debug_layer, GRect (17, 108, 110, 28));
	text_layer_init(&debug_layer, GRect (0, 125, 144, 28));
//	text_layer_set_text(&debug_layer, "- Muybridge -");
	snprintf(debug_buffer, 20, "tm: %d img: %d", image_count, image_data.current_image);
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

	
}  // animation_init()


// --------------------------------------------------------
//			feature_start_deinit()
// --------------------------------------------------------
void feature_animation_deinit() {

// clean up image containers
//	for(int i = 0; i< NUMBER_OF_IMAGES; i++){
//		bmp_deinit_container(&image_data.image_container[i]);
//	}
	bmp_deinit_container(&image_data.image_container[image_data.current_image]);
	
}  // feature_animation_deinit()
