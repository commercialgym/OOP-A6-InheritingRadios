/*
* FILE: AmFmRadio.cpp
* PROJECT: A-06 : CREATING MORE AND MORE RADIOS
* PROGRAMMER: Alexia Tu
* DESCRIPTION: contains the source code for the AmFmRadio class. 20 methods total which includes constructors,
	* a destructor, accessors, and mutators. It follows best practices of encapsulation by ensuring that the methods
	* follow requirements regardless of the test harness. and I tried my best to not have code repetition... UPDATE: 
	* The constructor has been slightly modified to allow efficient validation and assignment of child and grandchild 
	* classes.
*/


#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#pragma warning(disable: 4996)
#include "AmFmRadio.h"

//METHOD: Constructor - AmFmRadio()
//DESCRIPTION: Constructor that intializes all attributes of class AmFmRadio so that they don't recieve garbage values. Had to 
// add another parameter due to the requirements for a grandchild class, who cannot validate and assign a private variable directly.
//PARAMETERS: bool isOn: indicates whether radio should be off or on upon instantiation, struct Freqs initPreset[]: 
// a list of potential presets that will be validated before assigning to the actual list of presets of the class, 
// double newCurrStation: potential value that the current station could be defaulted to (before and/or after the radio is on)
//RETURNS:  NOTHING
AmFmRadio::AmFmRadio(bool isOn, struct Freqs initPresets[], double newCurrStation)
{
	for (int i = 0; i < NUM_PRESETS; i++) //changed to post-increment
	{
		if (initPresets[i].AMFreq <= MIN_AM_FREQ)
		{
			initPreset[i].AMFreq = MIN_AM_FREQ;
			copyOfPresets[i].AMFreq = MIN_AM_FREQ;
			continue;
		}
		if (initPresets[i].AMFreq >= MAX_AM_FREQ)
		{
			initPreset[i].AMFreq = MAX_AM_FREQ;
			copyOfPresets[i].AMFreq = MAX_AM_FREQ;
			continue;
		}
		initPreset[i].AMFreq = initPresets[i].AMFreq; //means that the inputted value of the frequency list is valid
		copyOfPresets[i].AMFreq = initPresets[i].AMFreq;

	}
	for (int j = 0; j < NUM_PRESETS; j++)
	{
		if (initPresets[j].FMFreq <= MIN_FM_FREQ)
		{
			initPreset[j].FMFreq = MIN_FM_FREQ;
			copyOfPresets[j].FMFreq = MIN_FM_FREQ;
			continue;
		}
		if (initPresets[j].FMFreq >= MAX_FM_FREQ)
		{
			initPreset[j].FMFreq = MAX_FM_FREQ;
			copyOfPresets[j].FMFreq = MAX_FM_FREQ;
			continue;
		}
		initPreset[j].FMFreq = initPresets[j].FMFreq;
		copyOfPresets[j].FMFreq = initPresets[j].FMFreq;
	}

	//is the radio on?
	if (isOn)
	{
		on = true;
	}
	else
	{
		on = false;
	}
	/*since all radios are always instantiated on the AM band, the constructor only needs to worry about setting the current station in the AM range.
	The main purpose of this is so that other classes can still use the parent class initialization list to define the current station, specifically for
	PioneerWorld, the class' AM range is different than the others, so it's current station cannot be set to the same as all the others as it does not meet requirements*/
	if (newCurrStation >= MIN_AM_FREQ || newCurrStation <= MAX_AM_FREQ)
	{
		current_station = newCurrStation;
		/*used for ToggleBand method, should be set to the same value of current_station(in AM) and min value of FM in the case that a user switches between 
		stations before changing the frequencies (technically never used in this project but doing this is good for consistency in the case that FM was added to 
		PioneerAM or PioneerWorld)*/
		previousFreq.AMFreq = newCurrStation;
		/*similar to case above, but used for when the car radio is turned on and off. whenever a radio is turned on, the current_station is always set to
		the value of "onOffFreq". if turning the radio on for the first time, the onOffFreq should be the same value of current_station that the object was 
		instantiated with*/
		onOffFreq.AMFreq = newCurrStation;
	}
	else
	{
		current_station = MIN_AM_FREQ;
		previousFreq.AMFreq = MIN_AM_FREQ;
		onOffFreq.AMFreq = MIN_AM_FREQ;
	}
	previousFreq.FMFreq = MIN_FM_FREQ;
	onOffFreq.FMFreq = MIN_FM_FREQ;


	strcpy(band, "AM");
	strcpy(copyBand, "");
	strcpy(prevBand, "AM"); /*should still be initialized to AM in the case that the user is turning the radio on for the first time. In the toggle
	method, the band always get's set to the prevBand*/
	volume = 0;
	prevVolume = 0;
	displayOutput = false; //helpful for determining which destructor to print
}

//METHOD: Constructor - AmFmRadio()
//DESCRIPTION: Constructor that intializes all attributes of class AmFmRadio so that they don't recieve garbage values. 
//PARAMETERS: bool isOn: indicates whether radio should be off or on upon instantiation. if an argument is not included during instantiation, 
// it recieves a default value of false which is indicated in this prototype.
//RETURNS:  NOTHING
AmFmRadio::AmFmRadio(bool isOn)
{
	//obviously need to change this
	for (int i = 0; i < NUM_PRESETS; i++)
	{
		initPreset[i].AMFreq = MIN_AM_FREQ;
		copyOfPresets[i].AMFreq = MIN_AM_FREQ;
		initPreset[i].FMFreq = MIN_FM_FREQ;
		copyOfPresets[i].FMFreq = MIN_FM_FREQ;
	}
	//still need an else statement if an object is instantiated with bool value indicating radio off.. 
	if (isOn)
	{
		on = true;
	}
	else
	{
		on = false;
	}
	current_station = MIN_AM_FREQ;
	strcpy(band, "AM");
	strcpy(copyBand, "");
	strcpy(prevBand, "AM");
	volume = 0;
	prevVolume = 0;
	displayOutput = false; //will never be true in this project
	//used for ToggleBand method, should be set to min values in the case that a user switches between stations before changing the frequencies
	previousFreq.AMFreq = MIN_AM_FREQ;
	previousFreq.FMFreq = MIN_FM_FREQ;
	//similar to case above, but used for when the car radio is turned on and off.
	onOffFreq.AMFreq = MIN_AM_FREQ;
	onOffFreq.FMFreq = MIN_FM_FREQ;
}

//METHOD: virtual Destructor - ~AmFmRadio()
//DESCRIPTION: destroys an object of class AmFmRadio once it loses scope
//PARAMETERS: NOTHING
//RETURNS:  NOTHING
AmFmRadio::~AmFmRadio(void)
{
	if (displayOutput)
	{
		printf("Destroying AmFmRadio\n");
	}
}

//METHOD: PowerToggle()
//DESCRIPTION: Turns the radio on or off. When turning off, temporary variables are assigned the values of the current variables (band, frequency)
// values and then the current variables are reassigned their previous values when turned back on, to ensure that the requirement of "When turning 
// the radio on after being off, go immediately to the previous band (AM or FM) and last frequency tuned on that band which the radio was set to 
// before being powered off" is followed regardless of what's happening on the outside (eg the test harness/driver)
//PARAMETERS: void
//RETURNS:  void - simply toggles class attribute "on"
void AmFmRadio::PowerToggle(void)
{
	if (!on) //if it's already off turn it on
	{
		on = true;
		volume = prevVolume; //restore volume to what it was before. if it's the first time it will be zero
		strcpy(band, prevBand); //restore band, if it's the first time being turned on it will be AM
		//restore to previous frequency
		if (strcmp(band, "AM") == IS_EQUAL)
		{
			current_station = onOffFreq.AMFreq;
			previousFreq.AMFreq = (int)current_station; /*ensure to keep updated with current station for accurate band and frequency toggling,
			just in case it was changed while the radio was off*/
		}
		else
		{
			current_station = onOffFreq.FMFreq;
			previousFreq.FMFreq = current_station;
		}
	}
	else
	{
		on = false;
		//turn volume to 0 when turning off, restore to prev when turned back on
		prevVolume = volume;
		volume = MIN_VOLUME;
		//prevBand will be used to set the band back to what it was when the radio gets turned on
		strcpy(prevBand, band);
		/* using both fields of previousFreq to set both fields of onOffFreq because both frequencies should be restored to what it
		was before the radio was turned off. As mentioned before, when the radio is turned off, previousFreq field values can still change if not validated by driver.
		If the band was on AM 530 before being turned off and it's expected that it should return to AM 530 when turned back on, the FM and
		it's frequency should also be restored to what it was before being turned off... I don't think it should be allowed to change while off.*/
		onOffFreq.AMFreq = previousFreq.AMFreq;
		onOffFreq.FMFreq = previousFreq.FMFreq;
	}
}

//METHOD: IsRadioOn()
//DESCRIPTION: accessor for the "on" attribute
//PARAMETERS: void
//RETURNS:  bool representing the current value of the attribute bool "on". true if radio is on, false if radio is off
bool AmFmRadio::IsRadioOn(void)
{
	return on;
}

//METHOD: SetVolume()
//DESCRIPTION: Prompts a user for input to change the volume. This input is then sent to the other function (the worker bee) in order
// to validate and assign to the actual volume attribute
//PARAMETERS: void - takes nothing since within the method itself it prompts the user for a new value
//RETURNS:  returns int Value of the worker bee, which is either 0 if the inputted value was <= the min allowed volume (0)
// or 100 if the inputted value was >= the max allowed volume. These are then used in the driver to let the user
//know what the volume was set to in the case that they entered a value out of range. I used the return values of MIN_VOLUME
//and MAX_VOLUME because it's readable 
int AmFmRadio::SetVolume(void)
{
	if (!on) //if the radio is off the option to change volume should be unavailable
	{
		if (displayOutput)
		{
			printf("Cannot change volume while radio is off\n");
		}
		return ERROR_OFF; //look into how to use either success or error defines. see if error value can be changed from 0
	}

	char buf[SIZE_INPUT] = "";

	printf("\nEnter the volume level (%d - %d). ", MIN_VOLUME, MAX_VOLUME); //make sure this is okay... does this count as changing the user interface?
	fgets(buf, sizeof buf, stdin);
	int tempVolume = atoi(buf);
	//maybe put if conditional statement to check whether or not setting the volume was successful
	int returnVal = SetVolume(tempVolume); // send to "worker bee" for validation 
	return returnVal;
}

//METHOD: SetVolume()
//DESCRIPTION: second version of SetVolume, the "worker bee". is called by the other SetVolume function to do the validating and assigning of
// the volume attribute of class AmFmRadio. 
//PARAMETERS: int newVolume - the potential new volume, in this program it's always a value inputted by the user
//RETURNS:  int value indicating the condition in which the volume was changed. if the volume was below range it gets set to 0, if above range it 
//gets set to 100. these values are then used in the driver to tell the user (if they entered a value out of range, tells them what it was set to)
int AmFmRadio::SetVolume(int newVolume)
{
	if (!on)
	{
		return ERROR_OFF; //look into how to use either success or error defines. see if error value can be changed from 0
	}

	if (newVolume <= MIN_VOLUME) //if user enters volume less than 0, volume = 0
	{
		volume = MIN_VOLUME;
		return MIN_VOLUME;
	}

	if (newVolume >= MAX_VOLUME) //if user enters volume greater than 100, volume = 100
	{
		volume = MAX_VOLUME;
		return MAX_VOLUME;
	}

	//is there any other validation i have to do here?
	volume = newVolume;

	return SUCCESS;
}

//METHOD: GetVolume()
//DESCRIPTION: accessor method for the volume attribute of class AmFmRadio()
//PARAMETERS: void
//RETURNS:  int volume - the actual value of that attribute
int AmFmRadio::GetVolume(void)
{
	return volume;
}

//METHOD: ToggleBand()
//DESCRIPTION: allows user to toggle between AM and FM. When toggling back and fourth, it resumes the frequency of the station they were on for the band they were
// previously on prior to switching bands
//PARAMETERS: void
//RETURNS:  void - simply toggling
void AmFmRadio::ToggleBand(void)
{
	if (strcmp(band, "AM") == 0)
	{
		strcpy(band, "FM");
		current_station = previousFreq.FMFreq; /* the current station is set to the "previous frequency", however if it's the first time the radio is turned on,
		the previous frequency values will be the default lowest values of 530 and 87.5 for AM and FM respectively. */
	}
	else
	{
		strcpy(band, "AM");
		current_station = previousFreq.AMFreq;
	}
}

//METHOD: GetBand()
//DESCRIPTION: accessor method for the current band that the user is on. 
//PARAMETERS: void
//RETURNS: char* to a copied string that holds the same value of current band
char* AmFmRadio::GetBand(void)
{
	//ensure that I have implemented safest way to return string
	memcpy(copyBand, band, sizeof(band));
	return copyBand;
}

//METHOD: SetPresetButton()
//DESCRIPTION: uses the inputted index (int buttonNum) to change that index's preset value to the current station the
// user is on - depending on whether they are on AM or FM
//PARAMETERS: int buttonNum - takes a number representing the index of the array that holds the preset values
//RETURNS:  int value of either SUCCESS or ERROR
int AmFmRadio::SetPresetButton(int buttonNum)
{
	if ((buttonNum >= MIN_PRESET_CHOICE) && (buttonNum <= MAX_PRESET_CHOICE)) //4 works since the value is decremented in the driver
	{
		if (strcmp("AM", band) == IS_EQUAL)
		{
			initPreset[buttonNum].AMFreq = (int)current_station;
		}
		else
		{
			initPreset[buttonNum].FMFreq = current_station;
		}
		return SUCCESS;
	}
	return ERROR;
}

//METHOD: SelectPresetButton()
//DESCRIPTION: a user can choose to set their current station to one of their preset values depending on which band they're currently in
//PARAMETERS: int buttonNum - value indicating the index of the array of the preset they want to select from
//RETURNS:  int value indicating whether it selecting the preset was a SUCCESS or ERROR
int AmFmRadio::SelectPresetButton(int buttonNum)
{
	if ((buttonNum >= MIN_PRESET_CHOICE) && (buttonNum <= MAX_PRESET_CHOICE))
	{
		if (strcmp("AM", band) == IS_EQUAL)
		{
			current_station = initPreset[buttonNum].AMFreq;
			previousFreq.AMFreq = (int)current_station; //update so that a user can toggle between their current preset
		}
		else
		{
			current_station = initPreset[buttonNum].FMFreq;
			previousFreq.FMFreq = current_station;
		}

		return SUCCESS; //magic
	}
	return ERROR;


}

//METHOD: GetPresets()
//DESCRIPTION: accessor that allows someone on the outside to view the current button presets
//PARAMETERS: void
//RETURNS: struct Freqs* copyOfPresets - returns a pointer to a copy of the current button/frequency presets
struct Freqs* AmFmRadio::GetPresets(void)
{
	for (int i = 0; i < NUM_PRESETS; i++)
	{
		copyOfPresets[i] = initPreset[i];
	}

	return copyOfPresets;
}

//METHOD: ShowCurrentSettings()
//DESCRIPTION: displays the current (real/relevant) attributes of the class AmFmRadio. 
//PARAMETERS: void
//RETURNS:  void
void AmFmRadio::ShowCurrentSettings(void)
{

	if (on)
	{
		printf("\n\nRadio is on. \n");
	}
	else
	{
		printf("\n\nRadio is off. \n");
	}

	printf("\nRadio Band: %s\n", band);
	printf("Volume: %d\n", volume);
	printf("Current Station: %.1f %s\n", current_station, band);
	printf("AM Button Settings: ");
	for (int i = 0; i < NUM_PRESETS; i++)
	{
		printf("%d) %6d ", i + 1, initPreset[i].AMFreq);
	}

	printf("\nFM Button Settings: ");
	for (int j = 0; j < NUM_PRESETS; j++)
	{
		printf("%d) %6.1f ", j + 1, initPreset[j].FMFreq);
	}
}

//METHOD: ScanUp()
//DESCRIPTION: allows the user to switch the current station by scanning up, increasing the frequency value. if they reach
// the highest range, it loops back to the beginning (the lowest value). updates the previous frequency as well to ensure that
// when they want to switch from AM/FM, the value they return to is up to date to their last current_station of that band
//PARAMETERS: void
//RETURNS:  void
void AmFmRadio::ScanUp(void)
{
	if (strcmp("AM", band) == IS_EQUAL)
	{
		//if current_station is already (or greater than... which is should never be) 1700, the current_station becomes 530
		if (current_station >= MAX_AM_FREQ)
		{
			current_station = MIN_AM_FREQ;
			previousFreq.AMFreq = (int)current_station;
		}
		else
		{
			current_station += SHIFT_AM;
			previousFreq.AMFreq = (int)current_station;
		}
	}
	else
	{
		//if the current_station is 107.9, the current_station becomes 87.9
		//Note: car radios jump .2 for the FM. That's how it's modeled here.
		if (current_station >= MAX_FM_FREQ)
		{
			current_station = MIN_FM_FREQ;
			previousFreq.FMFreq = current_station;
		}
		else
		{
			current_station += SHIFT_FM;
			previousFreq.FMFreq = current_station;
		}
	}
	if (displayOutput)
	{
		printf("\nCurrent station: %f %s\n", current_station, band);
	}
}

//METHOD: ScanDown()
//DESCRIPTION: Behaves similarly to ScanDown except it allows the user to switch the current station by scanning down, decreasing the frequency value. 
// if they reach the lowest range, it loops back to the beginning (the highest value). updates the previous frequency as well to ensure that
// when they want to switch from AM/FM, the value they return to is up to date to their last current_station of that band
//PARAMETERS: void
//RETURNS:  void
void AmFmRadio::ScanDown(void)
{
	if (strcmp("AM", band) == IS_EQUAL)
	{
		//if current_station is min frequency, rolls over to max
		if (current_station <= MIN_AM_FREQ)
		{
			current_station = MAX_AM_FREQ;
			previousFreq.AMFreq = (int)current_station; //ensure to always keep prevFreq updated
		}
		else
		{
			current_station -= SHIFT_AM;
			previousFreq.AMFreq = (int)current_station;
		}
	}
	else
	{
		//if the current_station is 87.9, the current_station becomes 107.9
		//Note: car radios jump .2 for the FM. That's how it's modeled here.
		if (current_station <= MIN_FM_FREQ)
		{
			current_station = MAX_FM_FREQ;
			previousFreq.FMFreq = current_station;
		}
		else
		{
			current_station -= SHIFT_FM;
			previousFreq.FMFreq = current_station;
		}
	}
	if (displayOutput)
	{
		printf("\nCurrent station: %f %s\n", current_station, band);
	}
}

//METHOD: SetCurrentStation()
//DESCRIPTION: validates the incoming value so that only a valid number - within range - is assigned to the actual current station and
// the previous AM/FM frequencies. I couldn't find a use for it within this project....
//PARAMETERS: double newStation - the new potential value of the current station
//RETURNS: bool value indicating if changing the current station was a success or failure
bool AmFmRadio::SetCurrentStation(double newStation)
{
	if (strcmp(band, "AM") == IS_EQUAL)
	{
		if (newStation <= MIN_AM_FREQ)
		{
			current_station = MIN_AM_FREQ;
			previousFreq.AMFreq = (int)current_station;
			return true;
		}
		if (newStation >= MAX_AM_FREQ)
		{
			current_station = MAX_AM_FREQ;
			previousFreq.AMFreq = (int)current_station;
			return true;
		}
		current_station = (int)newStation;
		previousFreq.AMFreq = (int)current_station;
		return true;
	}
	if (strcmp(band, "FM") == IS_EQUAL)
	{
		if (newStation <= MIN_FM_FREQ)
		{
			current_station = MIN_FM_FREQ;
			previousFreq.FMFreq = current_station;
			return true;
		}
		if (newStation >= MAX_FM_FREQ)
		{
			current_station = MAX_AM_FREQ;
			previousFreq.FMFreq = current_station;
			return true;
		}
		current_station = newStation;
		previousFreq.FMFreq = current_station;
		return true;
	}
	return false; //this would most likely never be true unless the band got messed up
}

//METHOD: GetCurrentStation()
//DESCRIPTION: accessor for the current station that the user is on
//PARAMETERS: void
//RETURNS:  double, the data type of the current_station attribute
double AmFmRadio::GetCurrentStation(void)
{
	return current_station;
}

//METHOD: SetDisplayOutput()
//DESCRIPTION: mutator for if someone on the outside wants to change it so that methods of the class AmFmRadio
// prints output. It's value is used in an if condition, if true, the block of code runs and methods print.
// For this project it is always false, meaning no printing from those methods.
//PARAMETERS: bool display - value indicating whether or not to print
//RETURNS:  bool value indicating whether changing the displayOutput attribute was a success or failure
bool AmFmRadio::SetDisplayOutput(bool display)
{
	if (display)
	{
		displayOutput = true;
		return true;
	}
	else
	{
		displayOutput = false;
		return true;
	}
	return false;
}

//METHOD: GetDisplayOutput()
//DESCRIPTION: accessor that tells user whether or not the current display status will cause methods to print
//PARAMETERS: void
//RETURNS:  bool value that is equal to the current value of the displayOutput attribute
bool AmFmRadio::GetDisplayOutput(void)
{
	return displayOutput;
}