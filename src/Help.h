/* -------------------------------------------------------------------------------------- *\

                      page_help.h

	Header file for Tempus Fugit context-sensitive Help Module

Use:	Include this header file in any module that needs to access
		The Help Window's public functions.
	
	Author:				Peter Deutsch (engineerbill@stemchest.com)
	Date Created:		8/18/13
	Last Update:		8/27/13


\* -------------------------------------------------------------------------------------- */
#ifndef TF_PAGE_HELP_H
#define TF_PAGE_HELP_H

extern void help_init();		// call this to init module
extern void help_deinit();		// call this to init module
extern void help_show_window();	// call this to display Help Window
								// See  Help.c opening comments
								// block for calling details
								// or SlideShow.c for defines for
								// suitable calling values.

#endif