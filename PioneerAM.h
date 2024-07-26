/*
* CLASS: PioneerAM
* FILE: PioneerAM.h
* PROJECT: A-06 : CREATING MORE AND MORE RADIOS
* PROGRAMMER: Alexia Tu
* DESCRIPTION: This class publically inherits from PioneerCarRadio, it behaves similarly to PioneerCarRadio except for the fact that it operates in the AM band only.
	* In order to do this, some functions are overriden to "get rid of" the FM band. This includes ToggleBand() which now does not allow the band to switch, and
	* ShowCurrentSettings() which does not print any FM info (like presets). The constructor takes 3 params, the name of the type of radio, the list of presets, and 
	* a potential current station. If the object being instantiated is THIS class, it uses the default values for all three of these parameters, and sends it to it's
	* parent constructor with the initialization list. If the object being created is this class' child, these default values are ignored and the values of the child
	* class are still sent to this constructor's parent constructor. When a radio of this class is being destroyed, only this destructor's print statement runs.
*/

#pragma once

#ifndef _PIONEERAM_H
#define _PIONEERAM_H

#include <string> //or <string>? check module
#include <cstdio>
#include <iostream>
using namespace std;
//do i need to include amfmradio.h?
#include "PioneerCarRadio.h"

class PioneerAM : public PioneerCarRadio
{
private:
	static struct Freqs initPresets[NUM_PRESETS];

public:
	/* constructor and deconstructor */
	//METHOD: PioneerAM() - constructor
	//PARAMETERS: string newRadioName : indicates the name of the type of radio being instantiated - struct Freqs newPresets : potential list of presets -
	// double newCurrStation : potential current station. 
	//DESCRIPTION: This constructor is helpful for the child, pioneerWorld, which requires it's presets AND the current station to be 531. All the parameters
	// of this constructor are passed to it's parent constructor. Even though this class only operates in the AM band, valid FM frequencies are sill passed as 
	// part of the struct Freq array.
	//RETURNS: Nothing
	PioneerAM(string newRadioName = kAmName, struct Freqs newPresets[] = initPresets, double newCurrStation = MIN_AM_FREQ) : PioneerCarRadio(newRadioName, newPresets, newCurrStation)
	{
		// :)
	}

	//METHOD: ~PioneerAM - virtual destructor
	//PARAMETERS: void - none
	//DESCRIPTION: virtual destructor since methods are virtual. in main the displayOutput variable is set to true, if it's this radio object being deleted, then the
	// print statement will print and the displayOut is set to false so that the other base class destructors don't print anything
	//RETURNS: nothing
	virtual ~PioneerAM(void)
	{
		//print statement?? only print this object's...
		//printf("Destroying %s Radio\n", GetRadioName().c_str()); //or could use const
		if (GetDisplayOutput())
		{
			printf("Destroying Pioneer XS440-AM Radio\n\n");
			SetDisplayOutput(false);
		}
	}

	/* additional methods */

	//METHOD: virtual ToggleBand()
	//PARAMETERS: void - none
	//DESCRIPTION: FM band should not be shown, or be toggled to. this is a virtual overriden function of it's parent which prevents the band from 
	// being able to switch. the object's band is already set to AM in the base class
	//RETURNS: void - none
	virtual void ToggleBand(void)
	{
		return; //should not be able to toggle to fm at all, therefore override granparent
	}

	//METHOD: virtual ShowCurrentSettings() 
	//PARAMETERS: void - none
	//DESCRIPTION: FM band info should not be shown, only the AM presets are shown. For printing, the value provided for the format
	// specifier comes from an accessor methods, this is so that this same function can be used for the next inherited class too.
	//RETURNS: void - none
	virtual void ShowCurrentSettings(void)
	{
		if (IsRadioOn())
		{
			printf("%s\n", GetRadioName().c_str()); //convert to c style string to print
			printf("Radio is on\n");
		}
		else
		{
			printf("%s\n", GetRadioName().c_str());
			printf("Radio is off\n");
		}
		printf("Volume: %d\n", GetVolume());
		printf("Current Station: %.1f %s\n", GetCurrentStation(), GetBand());
		printf("AM Buttons: \n");
		struct Freqs* button = GetPresets(); //get the values of presets iterate through and print
		for (int i = 0; i < NUM_PRESETS - 1; i++)
		{
			printf("%d: %6d, ", i + 1, button[i].AMFreq);
		}
		printf("%d: %6d \n\n", NUM_PRESETS, button[4].AMFreq); //so that there is no comma
	}
};

//initialize static members outside of class, FM freq is given value simply because it does not matter, it will never be accessed
Freqs PioneerAM::initPresets[NUM_PRESETS]
{
	{MIN_AM_FREQ, MIN_FM_FREQ},
	{MIN_AM_FREQ, MIN_FM_FREQ},
	{MIN_AM_FREQ, MIN_FM_FREQ},
	{MIN_AM_FREQ, MIN_FM_FREQ},
	{MIN_AM_FREQ, MIN_FM_FREQ},
};


#endif // !_PIONEERAM_H



#pragma once
