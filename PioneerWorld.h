/*
* CLASS: PioneerWorld
* FILE: PioneerWorld.h
* PROJECT: A-06 : CREATING MORE AND MORE RADIOS
* PROGRAMMER: Alexia Tu
* DESCRIPTION: This class publically inherits from the PioneerAM class, behaving the same except that the AM range is between 531khz and 1602khz, 
	* scanning up and down now increments/decrements by 9khz instead of 10. It has 4 methods including the constructor and destructor. The ScanUp and ScanDown
	* methods have been overriden to meet the range and shift of khz requirements. 
*/

#pragma once


#ifndef _PIONEERWORLD_H
#define _PIONEERWORLD_H

#define WRLD_MAX_AM 1602
#define WRLD_MIN_AM 531
#define WRLD_SHIFT_AM 9

#include "PioneerAM.h"


class PioneerWorld : public PioneerAM
{
private:
	static struct Freqs initializePresets[NUM_PRESETS]; //initialize all presets to 531

public:
	/* constructor and deconstructor */

	//Req: set all presets to 531.. "can do it in pioneer world if you think about it"..
	//METHOD: PioneerWorld - constructor
	//PARAMETERS: void - nothing since there are no more child classes
	//DESCRIPTION: takes advantage of parent constructor and sends it's name (PioneerWorld), it's required preset values (of 531) and the current station 
	// it should be set to, which is also 531. This is because this class' AM range is 531 to 1602 so it cannot be instantiated to 530 like all the other classes.
	//RETURNS: nothing
	PioneerWorld(void) : PioneerAM(kWrldName, initializePresets, WRLD_MIN_AM)
	{
		// blah
	}

	//METHOD: ~PioneerWorld() - virtual destructor
	//PARAMETERS: void - none
	//DESCRIPTION: virtual destructor since methods are virtual. in main the displayOutput variable is set to true, if it's this radio object being deleted, then the
	// print statement will print and the displayOut is set to false so that the other base class destructors don't print anything
	//RETURNS: nothing
	virtual ~PioneerWorld(void)
	{
		//printf("Destroying %s Radio\n", GetRadioName().c_str());
		if (GetDisplayOutput())
		{
			printf("Destroying Pioneer XS440-WRLD Radio\n\n");
			SetDisplayOutput(false);
		}
	}

	/* additional methods */

	//METHOD: virtual ScanUp()
	//PARAMETERS: void - none
	//DESCRIPTION:  range changed to 531 to 1602 khz and scans up by 9khz instead of 10. there is also no longer a need to include code for when
	// the band is AM. I still kept checking to see if the band is AM if for some reason the band magically becomes FM.. 
	//RETURNS: void - none
	virtual void ScanUp(void)
	{
		//technically it will always be AM but... just in case i guess
		if (strcmp("AM", GetBand()) == IS_EQUAL) 
		{
			//if current_station is already (or greater than... which is should never be) 1602, the current_station becomes 531
			if (GetCurrentStation() >= WRLD_MAX_AM)
			{
				SetCurrentStation(WRLD_MIN_AM); //since not switching between bands, don't need to worry about previous station anymore
			}
			else
			{
				SetCurrentStation(GetCurrentStation() + WRLD_SHIFT_AM); //set current station to current station + 9khz shift
			}
		}
		
		if (GetDisplayOutput()) //keeping since it was in original requirements from A-03
		{
			printf("\nCurrent station: %f %s\n", GetCurrentStation(), GetBand());
		}
	}

	//METHOD: virtual ScanDown()
	//PARAMETERS: void - none
	//DESCRIPTION: range changed to 531 to 1602 khz and scans down by 9khz instead of 10. there is also no longer a need to include code for when
	// the band is AM. I still kept checking to see if the band is AM if for some reason the band magically becomes FM.
	//RETURNS: void - none
	virtual void ScanDown(void)
	{
		if (strcmp("AM", GetBand()) == IS_EQUAL)
		{
			//if current_station is min frequency, rolls over to max
			if (GetCurrentStation() <= WRLD_MIN_AM)
			{
				SetCurrentStation(WRLD_MAX_AM);
			}
			else
			{
				SetCurrentStation(GetCurrentStation() - WRLD_SHIFT_AM); //set current station to current station - 9khz shift
			}
		}
		//is this needed?
		if (GetDisplayOutput())
		{
			printf("\nCurrent station: %f %s\n", GetCurrentStation(), GetBand());
		}
	}


};


//initialize static members outside of class, FM freq is given value simply because it does not matter, it will never be accessed
//Req: initialize AM presets to 531
Freqs PioneerWorld::initializePresets[NUM_PRESETS]{
	{WRLD_MIN_AM, MIN_FM_FREQ},
	{WRLD_MIN_AM, MIN_FM_FREQ},
	{WRLD_MIN_AM, MIN_FM_FREQ},
	{WRLD_MIN_AM, MIN_FM_FREQ},
	{WRLD_MIN_AM, MIN_FM_FREQ},
};


#endif // !_PIONEERWORLD_H
