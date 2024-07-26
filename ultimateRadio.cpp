/*
* FILE: ultimateRadio.cpp
* PROJECT: A-06 : CREATING MORE AND MORE RADIOS
* PROGRAMMER: Alexia Tu
* DESCRIPTION: the test harness for the PioneerCarRadio, PioneerAM, and PioneerWorld class. The program first takes a command line argument which represents the
	* type of radio obejct to instantiate. It uses try and catch exception handling, if the command line argument passed is invalid, the error is caught and the program ends.
	* If a valid argument is passed, the object is created and the keystroke function of that class is called which acts as the radio UI. After a user presses 'x' to exit,
	* the radio is destroyed (ONLY printing the destructor print statement of the object being deleted), and the user will find themselves in another menu loop which will
	* allow them to either choose a new radio to start (either PioneerWorl, pioneerAM, or PioneerCarRadio) or exit. All invalid keystrokes are ignored - the only time that an
	* invalid input that triggers the catch block in main is when the argument from the command line is invalid. The function that creates these objects use the new new to 
	* instantiate, it uses the appropriate error checking - throwing errors if there is a memory allocation error, invalid input, or other/unknown - and returns a pointer to 
	* the object create upon success. Once the user chooses to exit the main menu loop, all dynamically allocated space is freed and the program ends.
	* memory is freed
*/
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string> 
#include <new>
#include <conio.h>
#pragma warning (disable: 4996)
#include "AmFmRadio.h"
#include "PioneerCarRadio.h"
#include "PioneerAM.h"
#include "PioneerWorld.h"
using namespace std;

//my defines
#define NUM_ARGS 2 //number of expected command line arguments
#define MAX_ARG_CHAR 4 //helps allocate enough space by making up the difference between the the min expected argv (-am) and max (-world) which is 3 plus 1 for null terminator
//increase flexibility in case expected argument names change
const string kCarRadio = "-car";
const string kAmRadio = "-am";
const string kWorldRadio = "-world";
const string kInvalidKey = "";
//for keystroke handling
const char kNewCarRadio = 'c';
const char kNewAmRadio = 'a';
const char kNewWorldRadio = 'w';
const char kQuit = 'x';


PioneerCarRadio* createRadio(char* nameRadio);

int main(int argc, char* argv[])
{
	if (argc != NUM_ARGS) 
	{
		printf("Invalid number of command line arguments\n");
		return ERROR;
	}
	
	//userInput will continue to be used in the while loop
	char* userInput = (char*)malloc(strlen(argv[1]) + MAX_ARG_CHAR);
	if (userInput == NULL) 
	{
		printf("Error allocating memory for command line argument\n");
		return ERROR;
	}

	strcpy(userInput, argv[1]);
	userInput[strcspn(userInput, "\n")] = '\0'; //command line args usually don't have newlines but this is just in case

	PioneerCarRadio* pRadio = NULL; //points to inherited classes as well

	bool menuChoice = true;
	//using do-while loop so that the first initial command line arg is sent to try block before any key stroke handling
	do
	{
		/*kInvalidKey is an empty string, this if statement will always be true the for the first command line arg (even if invalid) since command line arguments can't be an empty string. 
		however after this first iteration, the only time this will run is if the user inputs 'c', 'a', or 'w' since userInput is set to kInvalidInput (empty string) if any other keys are pressed*/
		if (strcmp(userInput, kInvalidKey.c_str()) != IS_EQUAL) 
		{ 
			try
			{
				pRadio = createRadio(userInput);
				//call the keystroke
				pRadio->KeystrokeHandling();
				//should i set display output to true and then after each destructor set it to false
				pRadio->SetDisplayOutput(true); //turn the displayOutput to true for the current object so that it's destructor prints
				delete pRadio;
				pRadio = NULL; //so that it is not a dangling pointer
			}
			catch (string& e) //catch by reference
			{
				printf("Exception Caught: %s\n\n", e.c_str());
				free(userInput);
				if (pRadio != NULL) //just in case this is pointing to memory..
				{
					delete pRadio;
				}
				return ERROR; //Requirement: exit program if there was an invalid arg
			}
		}

		char keystrokeInput = getch();

		switch (keystrokeInput) {
		case kQuit:
			menuChoice = false; //while condition no longer met
			break;
		case kNewCarRadio: //if they chose PioneerCarRadio
			strcpy(userInput, kCarRadio.c_str()); //will be sent to createRadio() in try block in next while iteration
			break;
		case kNewAmRadio: //if they chose PioneerAM
			strcpy(userInput, kAmRadio.c_str());
			break;
		case kNewWorldRadio: //if they chose PioneerWorld
			strcpy(userInput, kWorldRadio.c_str());
			break;
		default:
			strcpy(userInput, kInvalidKey.c_str()); //reset the value if it does not match any radio keystrokes, so that it does not meet
			//the if() condition enveloping the try block. 
			break;
		}

	} while (menuChoice);


	free(userInput);
	
	delete pRadio;

	printf("\nGoodbye!\n");
	return SUCCESS;
}

//NAME: createRadio()
//PARAMETERS: char* nameRadio - input from either the command line argument or called because the user pressed a valid keystroke, determines what type of radio to be instantiated
//DESCRIPTION: Compares which radio to instantiate with if statements, the final else statement throws an error indicating that the input was invalid. Within the if statements there are try
// blocks to dynamically allocate space to instantiate the radio object, it throws an error for bad memory allocation or for unknown.
//RETURNS: PioneerCarRadio* - pointer to the object that was successfully instantiated
PioneerCarRadio* createRadio(char* nameOfRadio)
{
	string unknownErr = "Unknown Error";
	string invalidInput = "Input from command line does not match radio options";
	string errorRadio = "Error with ";
		

	if (strcmp(nameOfRadio, kCarRadio.c_str()) == IS_EQUAL)
	{
		try
		{
			PioneerCarRadio* pCarRadio = new PioneerCarRadio();
			//best practices here
			return pCarRadio;
		}
		catch (bad_alloc& ba)
		{
			throw errorRadio + kCarRadio + " : " + ba.what(); //throw occurance of error from the specific radio and translate ba to human readable string, add " : " to look nice
		}
		catch (...) //or else i have no idea what error
		{
			throw unknownErr;
		}
	}
	else if (strcmp(nameOfRadio, kAmRadio.c_str()) == IS_EQUAL)
	{
		try 
		{
			PioneerAM* pAmRadio = new PioneerAM();
			//best practices
			return pAmRadio;
		}
		catch (bad_alloc& ba)
		{
			throw errorRadio + kAmRadio + " : " + ba.what();
		}
		catch (...)
		{
			throw unknownErr;
		}
	}
	else if (strcmp(nameOfRadio, kWorldRadio.c_str()) == IS_EQUAL)
	{
		try
		{
			PioneerWorld* pWorldRadio = new PioneerWorld();
			//best practices
			return pWorldRadio;
		}
		catch (bad_alloc& ba)
		{
			throw errorRadio + kWorldRadio + " : " + ba.what();
		}
		catch (...)
		{
			throw unknownErr;
		}
	}
	else
	{
		/*Requirement: " If an invalid choice is passed into createRadio() – it needs to throw an exception indicating an invalid choice. "
		My thought: technically this is a bad exception, slide 17 of module 11 says that a "bad exception" is one that is an
		"Invalid menu input", so technically this could be return NULL and then in main you can check for null indicating that
		the command line argument did not match any of the radio options. */
		throw invalidInput; 
	}
}