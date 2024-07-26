/*
* FILE: PioneerCarRadio.cpp
* PROJECT:  A-06 : CREATING MORE AND MORE RADIOS
* PROGRAMMER: Alexia Tu
* UPDATE DESCRIPTION: Contains the source code for the class PioneerCarRadio. It contains 4 method definitions (including constructor and destructor)
	* and a static variable definition. The static variable is used to initialize presets which are used to send to the parent constructor. The 
	* constructor of PioneerCarRadio no longer looks base... this is because a new data member, string nameRadio, was added to hold the name of the 
	* type of radio obejct being instantiated. The KeystrokeHandling function is the largest as it handles all the keystroke cases and calls the appropriate 
	* methods. Since the ShowCurrentSettings() method is a virtual overriden method, the destructor is also virtual.
*/

#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <conio.h>
#pragma warning(disable: 4996)
#include "AmFmRadio.h" 
#include "PioneerCarRadio.h"

//NAME: initPresets[]
//DESCRIPTION: this is an array of (static) struct freqs. it's main purpose is to provide the values (from the requirements) to the parent
// constructor in order for the presets to be defaulted. The parent's constructor validates these values then initializes the actual presets. 
// The reason for this is that there is no way to directly access and assign the private members of the presets. It has to be a static variable
// because it will exist even if the object doesn't exist.
Freqs PioneerCarRadio::initPresets[NUM_PRESETS] = {
	{FIRST_AM, FIRST_FM},
	{SEC_AM, SEC_FM},
	{THIRD_AM, THIRD_FM},
	{FOURTH_AM, FOURTH_FM},
	{FIFTH_AM, FIFTH_FM}
};

//??????added new data member so include that
//METHOD: Constructor for PioneerCarRadio
//DESCRIPTION: This constructor makes use of the parent constructor through the use of an initialization list. the arguments it provides is false - meaning
// instantiate with radio off, an array of Freqs - potentially providing the constructor with values to validate and initialize, and a double for a potential 
// current_station, which is sent to parent with initialization list. since this constructor is inherited, this constructor now takes parameters in the case 
// that a child class would like to have specific default parameters.
//PARAMETERS: string newRadioName: the name of the type of radio being instantiated, uses a default value in the prototype which is the name of THIS class. -
// struct Freqs newPresets[]: potential list of presets that can be passed to the parent destructor, the default values in the prototype is the same as last assignment -
// double newCurrStation: potential current station (of band AM) that can be initialized in the parent class, default value in prototype is the same as AmFmRadio's 
// default. This is only really useful for the final class (PioneerWorld)
//RETURNS: nothing
PioneerCarRadio::PioneerCarRadio(string newRadioName, struct Freqs newPresets[], double newCurrStation) : AmFmRadio(false, newPresets, newCurrStation)
{
	if (newRadioName == kWrldName) //for inheritence, next classes uses this constructor in initialization list
	{
		nameRadio = newRadioName;
	}
	else if (newRadioName == kAmName)
	{
		nameRadio = newRadioName;
	}
	else //just in case someone entered a garbage name
	{
		nameRadio = kCarRadioName;
	}
}

//METHOD: virtual destructor for PioneerCarRadio
//DESCRIPTION: The destructor prints that the pioneerCarRadio object was destroyed if the displayOutput variable is true. Requirement: after it prints the 
// necessary statement, it sets the display to false so that the base class destructor does not print that it's object is being destroyed.
//PARAMETERS: void - none
//RETURNS: nothing
PioneerCarRadio::~PioneerCarRadio(void)
{
	if (GetDisplayOutput())
	{
		printf("Destroying PioneerCarRadio\n\n");
		SetDisplayOutput(false);
	}
}

//METHOD: KeyStrokeHandling()
//DESCRIPTION: handles getting user input by comparing the inputted value to valid key stroke entries, then calling upon the appropriate method for the use case. it prints
// first status before the user enters anything to meet the requirement that says that the radio should always be displaying output. It uses a while loop that is true until
// the user enters the value 'x' which means exit. It also allows the user to toggle on/off, increment and decrement the volume by 1, scanning the current station up or down, 
// switching bands from am to fm to am, and setting or selecting a preset of the current station. The ShowCurrentSettings() method is called after each valid keystroke whether
// it's on or off, as per requirements say
//PARAMETERS: void - none
//RETURNS: void - none
void PioneerCarRadio::KeystrokeHandling(void)
{
	//when the object is first instantiated it would be off, when calling this function for the first time, it will print the first condition
	if (!IsRadioOn())
	{
		printf("%s\n", GetRadioName().c_str());
		printf("Radio is off\n\n");
	}
	else //if for some reason this function is called again, this will run
	{
		printf("%s\n", GetRadioName().c_str());
		printf("Radio is on\n\n");
	}


	bool menuChoice = true; //used in while loop, turns false when the user chooses to exit

	while (menuChoice)
	{
		char userInput = getch(); //collect user input without showing what they've entered

		if (userInput == kExit)
		{
			menuChoice = false;
			//show current setting?
			ShowCurrentSettings(); //still show current setting because it's a valid key stroke
			continue;
		}
		else if (userInput == kOnOff)
		{
			//turn the radio on/off
			PowerToggle(); //COULD THE RETURN VALUE BE CHANGED TO BOOL SO THAT IT DETERMIENS SHOWCURRENTSETTINGS
			//show current setting after each valid key stroke, whether on or off
			ShowCurrentSettings();
		}
		else if (userInput == kIncVolume)
		{
			if (IsRadioOn())
			{
				//turn volume up by 1
				SetVolume(GetVolume() + NEW_VOLUME);
			}
			//show current setting after each valid key stroke, whether on or off
			ShowCurrentSettings();
		}
		else if (userInput == kDecVolume)
		{
			if (IsRadioOn())
			{
				//turn volume down by 1
				SetVolume(GetVolume() - NEW_VOLUME);
			}
			//show current setting after each valid key stroke, whether on or off
			ShowCurrentSettings();
		}
		else if (userInput == kScanUp)
		{
			if (IsRadioOn())
			{
				//scan up
				ScanUp();
			}
			//show current setting after each valid key stroke, whether on or off
			ShowCurrentSettings();
		}
		else if (userInput == kScanDown)
		{
			if (IsRadioOn())
			{
				//scan downn
				ScanDown();
			}
			//show current setting after each valid key stroke, whether on or off
			ShowCurrentSettings();
		}
		else if (userInput == kSwitchBand)
		{
			if (IsRadioOn())
			{
				//switch band
				ToggleBand();
			}
			//show current setting after each valid key stroke, whether on or off
			ShowCurrentSettings();
		}
		else if (userInput == kSelectFirst || userInput == kSelectSec || userInput == kSelectThird || userInput == kSelectFourth || userInput == kSelectFifth)
		{
			if (IsRadioOn())
			{
				//choose button 1 to 5
				int numInput = 0; //send to select preset function

				switch (userInput) {
				case kSelectFirst:
					numInput = FIRST_BUTTON;
					break;
				case kSelectSec:
					numInput = SEC_BUTTON;
					break;
				case kSelectThird:
					numInput = THIRD_BUTTON;
					break;
				case kSelectFourth:
					numInput = FOURTH_BUTTON;
					break;
				case kSelectFifth:
					numInput = FIFTH_BUTTON;
					break;
				}
				SelectPresetButton(numInput);
			}
			//show current setting after each valid key stroke, whether on or off
			ShowCurrentSettings();
		}
		else if (userInput == kSetFirst || userInput == kSetSecond || userInput == kSetThird || userInput == kSetFourth || userInput == kSetFifth)
		{
			if (IsRadioOn())
			{
				//set button one to 5
				int numInput = 0; //send to set preset function

				switch (userInput) {
				case kSetFirst:
					numInput = FIRST_BUTTON;
					break;
				case kSetSecond:
					numInput = SEC_BUTTON;
					break;
				case kSetThird:
					numInput = THIRD_BUTTON;
					break;
				case kSetFourth:
					numInput = FOURTH_BUTTON;
					break;
				case kSetFifth:
					numInput = FIFTH_BUTTON;
					break;
				}
				SetPresetButton(numInput);
			}
			//show current setting after each valid key stroke, whether on or off
			ShowCurrentSettings();
		}
	}
}

//METHOD: virtual ShowCurrentSettings()
//DESCRIPTION: is an overriden method of the ShowCurrentSettings() method from the AmFmRadio class. it does this by using the keyword virtual (in the base class). 
// the purpose is to match the output of the requirements without altering the original AmFmRadio method.
//PARAMETERS: void - none
//RETURNS: void - none
void PioneerCarRadio::ShowCurrentSettings(void)
{
	if (IsRadioOn())
	{
		printf("Pioneer XS440\n"); //this can be constant since this method is going to overriden in the next function
		printf("Radio is on\n");
	}
	else
	{
		printf("Pioneer XS440\n");
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
	printf("%d: %6d ", NUM_PRESETS, button[4].AMFreq); //so that there is no comma

	printf("\nFM Buttons: \n");
	for (int j = 0; j < NUM_PRESETS - 1; j++)
	{
		printf("%d: %6.1f, ", j + 1, button[j].FMFreq);
	}
	printf("%d: %6.1f \n\n", NUM_PRESETS, button[4].FMFreq); //so that there is no comma and it is properly spaced
}

//accessor for new data member
//METHOD: 
//PARAMETERS:
//DESCRIPTION: 
//RETURNS: 
string PioneerCarRadio::GetRadioName(void)
{
	return nameRadio;
}