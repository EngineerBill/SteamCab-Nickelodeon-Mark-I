/* -------------------------------------------------------------------------------------- *\

                      menu_images.h

	Header file for <program>:: Action Bar menu to select image bank

	Author:				Peter Deutsch (engineerbill@stemchest.com)
	Date Created:		8/16/13
	Modified:			9/6/13		- adapated for Slideshow app


\* -------------------------------------------------------------------------------------- */
#ifndef TF_MENU_CALC_BUTTONS_H
#define TF_MENU_CALC_BUTTONS_H

#define NUMBER_OF_CHOICES	2

#define BUTTON_DEFAULT		0
#define BUTTON_HORSE		0
#define BUTTON_GIRL			1


extern void menu_images_show_window();
//extern void menu_images_select();
extern void menu_images_init();
extern void menu_images_deinit();

#endif
