/* -------------------------------------------------------------------------------------- *\

                      animation_config.h

		Header file for animation module config info.
	
Program:			SteamCab SlideShow 

NUMBER_OF_IMAGES:	Number of bitmaps to load

IMAGE_RESOURCE_IDS[] Array of resource IDs, one per Bitmap resource
					 (Note: system automatically prepends
					  "IMAGE_ID_" to each resources)


Use:	Include this header file in corresponding module's implementatio
		file (animation.c)
	
	Author:			Peter Deutsch (engineerbill@stemchest.com)
	Date Created:	8/18/13
	Modified:		8/27/13 - released with Tempus Fugit
	Modified:		9/3/13  - integrated into SteamCab SlideShow

\* -------------------------------------------------------------------------------------- */
#ifndef TF_PAGE_HELP_H
#define TF_PAGE_HELP_H
	

	
#define NUMBER_OF_IMAGES	22		// Count of bitmaps included

// -----------------------------------
//    Array of animation image IDs
// -----------------------------------
// Program Bitmap resource ID array. If you increase
// number of bitmaps, you'll need to increase the number of
// entries in this array. The names of the new entries
// are actually not important, but they do need to be there
// as placeholders.
// Note: if you increase this array to much, the program will
// no longer load as it will become too large to store into
// flash memory. If this happens you can try making each
// individual bitmap smaller.
int IMAGE_RESOURCE_IDS[NUMBER_OF_IMAGES] = {
	RESOURCE_ID_ZERO, 
	RESOURCE_ID_ONE, RESOURCE_ID_TWO, RESOURCE_ID_THREE,
	RESOURCE_ID_FOUR, RESOURCE_ID_FIVE, RESOURCE_ID_SIX,
	RESOURCE_ID_SEVEN, RESOURCE_ID_EIGHT, RESOURCE_ID_NINE,
	RESOURCE_ID_TEN, RESOURCE_ID_ELEVEN, RESOURCE_ID_TWELVE,
	RESOURCE_ID_THIRTEEN, RESOURCE_ID_FOURTEEN, RESOURCE_ID_FIFTEEN,
	RESOURCE_ID_SIXTEEN, RESOURCE_ID_SEVENTEEN, RESOURCE_ID_EIGHTEEN,
	RESOURCE_ID_NINETEEN, RESOURCE_ID_TWENTY, RESOURCE_ID_TWENTYONE,
};



#endif