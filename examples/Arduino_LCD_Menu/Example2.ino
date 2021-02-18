/*
 Arduino_LCD_Menu Example2
Copyright Levi Balling(aka Ashitakalax)
Licensed under the follwing license:
Redistribution and use in source and binary forms, with or without modification, are permitted provided that the 
following conditions are met:
Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer. 
Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer 
in the documentation and/or other materials provided with the distribution. 
The name of the author may not be used to endorse or promote products derived from this software without specific prior written permission. 
THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <MenuEntry.h>
#include <MenuIntHelper.h>
#include <MenuLCD.h>
#include <MenuManager.h>
// Using Bounce2 library for button control
#include <Bounce2.h>

// LCD PINS and Objects
#define LC_RS_PIN  23
#define LC_EN_PIN  21
#define LC_D7_PIN  41
#define LC_D6_PIN  40
#define LC_D5_PIN  39
#define LC_D4_PIN  38

MenuLCD lcdController(LC_RS_PIN, LC_EN_PIN, LC_D4_PIN, LC_D5_PIN, LC_D6_PIN, LC_D7_PIN, 16, 2);
MenuManager menuController( &lcdController);  

// Example uses 4 buttons
// User Input Switches
#define BUTTON_UP_PIN  45
#define BUTTON_DOWN_PIN  44
#define BUTTON_BACK_PIN  43
#define BUTTON_SELECT_PIN  42
Bounce debounceUp = Bounce();
Bounce debounceDown = Bounce();
Bounce debounceBack = Bounce();
Bounce debounceSelect = Bounce();

/**
 * Sets up the LCD Controller, the Menu, and calls the Setup Buttons.
 */
void setup() {
  // put your setup code here, to run once:

  lcdController.MenuLCDSetup();
  
  // Define the Menu objects 
  MenuEntry * rootMenuEntry = new MenuEntry("M1", NULL, NULL);
  MenuEntry * M1S1MenuEntry = new MenuEntry("M1-S1", NULL, M1S1Callback);
  MenuEntry * M1S2MenuEntry = new MenuEntry("M1-S2", NULL, M1S2Callback);
  MenuEntry * M2MenuEntry = new MenuEntry("M2", NULL, M2Callback);
  
  //Add the root node, then it's children
  menuController.addMenuRoot(rootMenuEntry);
  menuController.addChild(M1S1MenuEntry);    
  menuController.addChild(M1S2MenuEntry);  
  menuController.addSibling(M2MenuEntry);
  menuController.SelectRoot();
  menuController.DrawMenu();   

  //Setup buttons
  setupButtons();
}

/**
 * Sets up 4 debounced buttons to handle the Menu Options
 */
void setupButtons()
{
  // Setup the Inputs and debouncers
  pinMode(BUTTON_UP_PIN, INPUT_PULLUP);
  debounceUp.attach(BUTTON_UP_PIN);
  debounceUp.interval(5);
  
  // Setup Down Button
  pinMode(BUTTON_DOWN_PIN, INPUT_PULLUP);
  debounceDown.attach(BUTTON_DOWN_PIN);
  debounceDown.interval(5);

  // Setup Back Button
  pinMode(BUTTON_BACK_PIN, INPUT_PULLUP);
  debounceBack.attach(BUTTON_BACK_PIN);
  debounceBack.interval(5);

  // Setup Select Button
  pinMode(BUTTON_SELECT_PIN, INPUT_PULLUP);
  debounceSelect.attach(BUTTON_SELECT_PIN);
  debounceSelect.interval(5);
}

/**
 * Main Loop function that just checks the Buttons repeatedly
 */
void loop() 
{
  updateButtons();
}

/**
 * The Callback for the Sub Node M1-S1
 */
void M1S1Callback( char* menuText, void *userData)
{
  char *menuLines[2] = {"M1-S1 Callback", "" };
  lcdController.PrintMenu(menuLines, 2, 3);// PrintMenu( char ** MenuString, int number of Lines, SelectedLine)
  delay(5000);
}

/**
 * The Callback for the Sub Node M1-S2
 */
void M1S2Callback( char* menuText, void *userData)
{
  char *menuLines[2] = {"M1-S2 Callback", "" };
  lcdController.PrintMenu(menuLines, 2, 3);// "Hello" is the string to print, 0 is the Row
  delay(5000);
}

/**
 * The callback for the sibiling node M2
 */
void M2Callback( char* menuText, void *userData)
{
  char *menuLines[2] = {"M2 Callback", "" };
  lcdController.PrintMenu(menuLines, 2, 3);
  delay(5000);
}

/**
 * Simple function to listen to the button presses to trigger the menu actions
 */
void updateButtons()
{
  debounceUp.update();
  debounceDown.update();
  debounceBack.update();
  debounceSelect.update();
  
  if(debounceUp.fell())
  {  
    menuController.DoMenuAction(MENU_ACTION_UP);
  }
  else if(debounceDown.fell())
  {  
    menuController.DoMenuAction(MENU_ACTION_DOWN);
  }
  else if(debounceBack.fell())
  {  
    menuController.DoMenuAction(MENU_ACTION_BACK);
  }
  else if(debounceSelect.fell())
  {  
    menuController.DoMenuAction(MENU_ACTION_SELECT);
  }
}
