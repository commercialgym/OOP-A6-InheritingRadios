/*
* CLASS: AmFmRadio
* FILE: AmFmRadio.h
* PROJECT: A-06 : CREATING MORE AND MORE RADIOS
* PROGRAMMER: Alexia Tu
* DESCRIPTION: This class simulates an AM/FM radio, it's main interfacing attributes are band (either AM or FM), current station (frequency
	* of the current band that the user is on), volume, and presets (which is an array holding the frequencies of pairs of AM and FM bands.
	* Users can toggle between AM/FM bands, scan up or down between stations, set and select between 5 sets of presets, change the volume, turn the radio
	* on or off (while also ensuring that the values upon turning the radio back on are restored to the values they were at before the radio
	* was turned off.
	* It's accessors ensures that the values of the attributes are returned safely, and it's mutators perform extensive validation
	* in order to only set the attribues to values within range. To reduce code repetition, it makes use of overloaded functions, and also uses various
	* #defines which also ensures that the code is readable and so that there are no magic numbers.
* UPDATE DESCRIPTION: the constructor now takes another parameter, newCurrStation, if there is a specific value that the object should be defaulted to. 
	* This is mostly useful for the grandchild class "PioneerWorld" whose AM range is only between 531 and 1602, which means that the current station
	* cannot be defaulted to 530 like the other ones can. It's necessary to validate and set the current_station data member in the AmFmRadio constructor
	* due to the fact that PioneerWorld cannot access the current_station member directly since it's private in the AmFmClass.
*/

#ifndef _CARRADIO_H
#define _CARRADIO_H

#pragma once 

/* my defines */
#define SUCCESS 1
#define ERROR 0
#define ERROR_OFF 2
//for AM
#define MIN_AM_FREQ 530
#define MAX_AM_FREQ 1700
#define SHIFT_AM 10
//for FM
#define MIN_FM_FREQ 87.9
#define MAX_FM_FREQ 107.9
#define SHIFT_FM 0.2
//for both AM and FM
#define CHAR_AMFM 3
//for string comparison
#define IS_EQUAL 0 
//for presets
#define NUM_PRESETS 5
#define MIN_PRESET_CHOICE 0 
#define MAX_PRESET_CHOICE 4
//for volume
#define MIN_VOLUME 0
#define MAX_VOLUME 100
//for inputs
#define SIZE_INPUT 20

//STRUCT: Freqs
//DESCRIPTION: Holds the station values for both AM and FM frequencies. Used to toggle between AM and FM so that when switching back 
// to the other band, the current station contains the frequency of the band that they were on before switching. In this program, it's 
// used for when a user wants to switch their currentstation to an AM and FM of one of their presets, and storing the current AM and FM into a temp
// variable so that the band and frequency - that the radio was set to before the radio was turned off - is restored if there is no validation in
// driver and the values were changed.
//FIELDS: int AMFreq: value held in kilo-hertz, double FMFreq: value in mega-hertz
struct Freqs
{
	int AMFreq;
	double FMFreq;
};

class AmFmRadio
{
private:
	/* attributes */
	/* ------------------------*/
	Freqs	initPreset[NUM_PRESETS]; //can i change this variable name, maybe to frequency presets?.. doesn't say i can in requirements so maybe I'll leave
	Freqs	copyOfPresets[NUM_PRESETS]; //used in the accessor to send a copy of the presets safely
	double	current_station;
	char	band[CHAR_AMFM];
	char	copyBand[CHAR_AMFM]; //used in the accessor for current band
	char	prevBand[CHAR_AMFM]; //for when the car is turned back on and is to resume the previous band... IS THIS NECESSARY
	Freqs	onOffFreq; /*also used for when car is turned on / off. but different from other data member "Freqs previousFreq" because that data member
	can still be changed when the radio is off, however in requirements the radio must return to the settings it had before it was turned off.*/
	int		volume;
	int		prevVolume; //when turning on and off and need to restore the old volume, will hold the value of the current value before it gets set to zero
	bool	on;
	//from hint about switching from AM/FM with ToggleBand() method... need to create new data member
	Freqs	previousFreq; //these values are updated alongside the current station in the SetCurrentStation, ScanUp, and ScanDown methods
	bool	displayOutput; //keeps track of whether or not to display output MAKE SURE DEFAULT IS OFF/FALSE

public:

	/* constructors and destructor */
	AmFmRadio(bool isOn = false); //using default parameters
	AmFmRadio(bool isOn, struct Freqs initPresets[], double newCurrStation = MIN_AM_FREQ); //
	virtual ~AmFmRadio(void); //destructor

	/* accessors */
	/* ------------------------*/
	bool IsRadioOn(void); //returns a true if the radio is currently powered on, and false if the radio is in the off position
	char* GetBand(void); //returns a copied string representing the current band 
	struct Freqs* GetPresets(void); //returns a copy of an array of the current presets
	int GetVolume(void);
	double GetCurrentStation(void);
	bool GetDisplayOutput(void);

	/* mutators */
	/* ------------------------*/
	void PowerToggle(void); //turns radio on or off
	virtual void ToggleBand(void); //toggles frequency between AM and FM and sets current station to what it was before switching
	int SetPresetButton(int buttonNum); //sets button (radio preset) with current station by being passed a button number
	int SelectPresetButton(int buttonNum); //sets current station to radio preset by being passed a button number
	int SetVolume(void);
	bool SetDisplayOutput(bool display); //allows user to change whether or not they want outputs to be displayed
	bool SetCurrentStation(double newStation); //enter a new double
	int SetVolume(int newVolume); //compile-time polymorphism, this guy is the worker bee of the SetVolume method

	/* method prototypes */
	/* ------------------------*/
	//shows volume, button settings, current station, AM or FM
	virtual void ShowCurrentSettings(void);
	//changes frequency up in increments of .2 for FM, 10 for AM
	virtual void ScanUp(void);
	//behaves similarly to ScanUp() but it scans down
	virtual void ScanDown(void);

};
#endif