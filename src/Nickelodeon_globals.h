/* -------------------------------------------------------------------------------------- *\

                      Nickelodeon_globals.h
					  
		   Global header file for SteamCab SlideShow program
	
	
	Includes definitions and space allocation for global
	        variables needed across modules.
	
	Note: variable declares and function prototypes are handled
		  in each module through a corresponding "<modulename>.h"" file.

	Author:				Peter Deutsch (engineerbill@stemchest.com)
	Date Created:		9/1/13
	Modified:			9/3/13 - adapated for SteamCab Nickelodeon


\* -------------------------------------------------------------------------------------- */
#ifndef PROGRAM_GLOBALS_H
#define PROGRAM_GLOBALS_H


// --------------------------------------------------------
//
//    Define global program variables
//
// (these should  eventually move to a state manager
// and be accessible only through public functions)
// --------------------------------------------------------

// --------------------------------------------------------
//        Create global data structures
// --------------------------------------------------------

AppContextRef g_app_context;	// stores global App context

#endif
