/*
* CLASS: PioneerCarRadio
* FILE: PioneerCarRadio.h
* PROJECT: A-06 : CREATING MORE AND MORE RADIOS
* PROGRAMMER: Alexia Tu
* DESCRIPTION: Publically inherits from the class AmFmRadio. It has 3 new/unique methods (besides the constructor and deconstructor).
* UPDATE DESCRIPTION: added a new data member - string "nameRadio" - which holds the name of the type of radio that was instantiated. The
	* First is GetRadioName() which is simply an accessor for the new data member, this is useful for the child classes for printing purposes.
	* The second is the KeystrokeHandling() method which essentially handles all UI, it: collects user input, processing the key, calling the
	* appropriate method, and displaying the current settings. At no point does it display what keys to press or the menu options. The third
	* method is the ShowCurrentSettings() method which overrides the parent method, this is done in order to meet the output requirement.
	* There are many constants and defines, the constants are for interpreting the key strokes, and the some of the defines are to translate
	* those key strokes into valid arguments for the method to be called.
*/

#pragma once

#ifndef _PIONEERRADIO_H
#define _PIONEERRADIO_H

#include "AmFmRadio.h"
#include <string>
using namespace std;

/* defines and constants */
//for initializing the presets from requirements
#define FIRST_AM 1000
#define FIRST_FM 102.1
#define SEC_AM 1210
#define SEC_FM 104.3
#define THIRD_AM 1210
#define THIRD_FM 100.1
#define FOURTH_AM 700
#define FOURTH_FM 99.3
#define FIFTH_AM 1300
#define FIFTH_FM 89.7
//for incrementing and decrementing volume
#define NEW_VOLUME 1
//for preset buttons, index value
#define FIRST_BUTTON 0
#define SEC_BUTTON 1
#define THIRD_BUTTON 2
#define FOURTH_BUTTON 3
#define FIFTH_BUTTON 4
//to make the print outputs that show the radio class' name more flexible
const string kAmName = "Pioneer XS440-AM";
const string kWrldName = "Pioneer XS440-WRLD";
const string kCarRadioName = "Pioneer XS440";

//for comparing the user input to valid keystrokes
const char kExit = 'x';
const char kOnOff = 'o';
const char kIncVolume = '+';
const char kDecVolume = '_';
const char kScanUp = '=';
const char kScanDown = '-';
const char kSwitchBand = 'b';
//still for valid keystrok but for selecting a preset
const char kSelectFirst = '1';
const char kSelectSec = '2';
const char kSelectThird = '3';
const char kSelectFourth = '4';
const char kSelectFifth = '5';
//still valid keystroke but for setting a preset
const char kSetFirst = '!';
const char kSetSecond = '@';
const char kSetThird = '#';
const char kSetFourth = '$';
const char kSetFifth = '%';


class PioneerCarRadio : public AmFmRadio
{
private:
	/* data members */
	static Freqs initPresets[NUM_PRESETS]; //used for initializing the presets
	string nameRadio; //new data member to make printing functions accomodate the specific radio class/datatype

public:

	/* constructors and destructor  */
	PioneerCarRadio(string newRadioName = kCarRadioName, struct Freqs newPresets[] = initPresets, double newCurrStation = MIN_AM_FREQ); /*using an initialization list,
	if no presets are given the default provided is used, name is defaulted to this class */
	virtual ~PioneerCarRadio(void);

	/* methods */
	void KeystrokeHandling(void);
	//shows volume, button settings, current station, AM or FM
	virtual void ShowCurrentSettings(void); //overrides parent, including keyword for best practice

	/* accessor */
	string GetRadioName(void); 

};


#endif