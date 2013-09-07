/* -------------------------------------------------------------------------------------- *\

                      menu_direction.h

	       Header file for <program>:: Action Bar menu
               to select animation direction

	Author:				Peter Deutsch (engineerbill@stemchest.com)
	Date Created:		8/16/13		- implemented for Tempus Fugit
	Modified:			9/7/13		- adapated for Nickleodeon app


\* -------------------------------------------------------------------------------------- */
#ifndef TF_MENU_DIRECTION_BUTTONS_H
#define TF_MENU_DIRECTION_BUTTONS_H

#define NUMBER_OF_CHOICES	2

#define BUTTON_DEFAULT		0
#define BUTTON_FORWARD		0
#define BUTTON_REVERSE		1


extern void menu_direction_show_window();
//extern void menu_direction_select();
extern void menu_direction_init();
extern void menu_direction_deinit();

#endif
