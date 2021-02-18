# Arduino_LCD_MENU

This library is to combine the functionality of the LCD(LiquidCrystal) and the use of a menu tree.
Very common functionality that. This is combined to speed up the startup time of creating menus on LCDs.

### Getting Started

Include the neccessary libraries (See Installing for more details).

    #include <MenuEntry.h>
    #include <MenuIntHelper.h>
    #include <MenuLCD.h>
    #include <MenuManager.h>

Identify which pins you will be using for your LCD screen( see https://learn.adafruit.com/character-lcds/wiring-a-character-lcd for more details)

    #define LCD_D7 = 4;
    #define LCD_D6 = 6;
    #define LCD_D5 = 7;
    #define LCD_D4 = 8;
    #define LCD_E  = 9;
    #define LCD_RS = 10;

Declare your global variables MenuLCD and MenuManager

    MenuLCD(RS, E, D4, D5, D6, D7, NumOfColumns, NumOfRows)
	MenuManager(MenuLCD* pMenuLCD); 
    
Here is an example with the values defined.  The use of the '&' symbol gives the menuController access to the lcdController.
This getting started and example expects the use of a 16x2 LCD display.
	
    MenuLCD lcdController(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7, 16, 2);
    MenuManager menuController( &lcdController); 	
	
Now it is time to setup your menu. This is where it is helpful to write out your tree before you begin.
For this example we will be using the following Tree. It is a simple tree that will print "M#-S#" if you select the Node where M# refers to the Main Node, and S# is for the subnode.

    M1Node
	 |
	 |----M1-S1Node (prints "M1-S1 Callback", delay 5s)
	 |
	 |----M1-S2Node (prints "M1-S2 Callback", delay 5s)
	 |
	M2Node (prints "M2 Callback", delay 5s)
	  
In your 'void setup()' Code section add the following to initialize the LCD.

    lcdController.MenuLCDSetup();	  
	  
In order for the Nodes to perform some task. It will need instructions on what to do.
We do this by setting up a callback function. It is the function that we want to call when the node is selected.

    void M1S1Callback( char* menuText, void *userData)
    {
		char *menuLines[2] = {"M1-S1 Callback", "" };
		lcdController.PrintMenu(menuLines, 2, 3);// PrintMenu( char ** MenuString, int number of Lines, SelectedLine)
		delay(5000);
    }

    void M1S2Callback( char* menuText, void *userData)
	{
	  char *menuLines[2] = {"M1-S2 Callback", "" };
	  lcdController.PrintMenu(menuLines, 2, 3);// "Hello" is the string to print, 0 is the Row
	  delay(5000);
	}

	void M2Callback( char* menuText, void *userData)
	{
	  char *menuLines[2] = {"M2 Callback", "" };
	  lcdController.PrintMenu(menuLines, 2, 3);
	  delay(5000);
	}

Now that we have our callbacks, lets start creating the Menu options starting with the root node. Order matters with the library it uses a depth-first traversal order.

    // Define the objects 
	MenuEntry * rootMenuEntry = new MenuEntry("M1", NULL, NULL);
	MenuEntry * M1S1MenuEntry = new MenuEntry("M1-S1", NULL, M1S1Callback);
	MenuEntry * M1S2MenuEntry = new MenuEntry("M1-S2", NULL, M1S2Callback);
	MenuEntry * M2MenuEntry = new MenuEntry("M2", NULL, M2Callback);


	//Add the root node
	menuController.addMenuRoot(rootMenuEntry);
    menuController.addChild(M1S1MenuEntry);    
    menuController.addChild(M1S2MenuEntry);  
    menuController.addSibling(M2MenuEntry);

	 
It is important to make sure that the menu is in the root starting location when you finish setting up.

     menuController.SelectRoot();

To draw the Menu

    menuController.DrawMenu();	 
	
After the 'setup' and once you are in the 'loop' folder. You will need to manage whether to go 'UP', 'DOWN', 'SELECT', or 'BACK'.
Here is an example of all 4 options. Note: in the example2.ino the actions are triggered by button presses.

     menuController.DoMenuAction( MENU_ACTION_UP);
     menuController.DoMenuAction( MENU_ACTION_DOWN);
     menuController.DoMenuAction( MENU_ACTION_SELECT);
     menuController.DoMenuAction( MENU_ACTION_BACK);

Whether these actions are tied to a button, or serial input, or something else, It is up to you.
In order to run the 'M1-S1Callback' run the following.

    menuController.DoMenuAction( MENU_ACTION_SELECT);
    menuController.DoMenuAction( MENU_ACTION_SELECT);

For full implementation of Hello Menu World see Example2.ino(todo link to example).

In order to create Mutliple root level options here is how.

//TODO Display the Multiple Root level tree
//TODO code example of multiple Root level tree
	
### Installing

Install the library like any other arduino zip library.

     Download the arduino library as a zip file (Arduino_LCD_Menu.zip)
	 
In the Arduino IDE goto

    Sketch -> Include Library -> Add .ZIP Library

Find the Arduino_LCD_Menu.zip file and upload that.

Then add the library by selecting in the list of libraries

    Sketch -> Include Library -> Arduino_LCD_Menu
	
That's it the library has be added. Goto the Getting started section for more details on how to use the library.

### Contributing

Feel free to post contributions to this library. 

### Authors

Original library: By David Andrews - May 6, 2012

Modified by @Tweep, February 2017
Modified by @Ashitakalax, March 2017

*License*: Modified BSD (See complete license at end)

*Purpose*: To shorten the time to get an Adriuno project running using a menuing system.


Changes : 
 - Added additional example
 - Added Syntax Highlighting
 - new constructor added which allows to enable / disable the execution of callback functions when the root 
   menu has been selected.  

   
   
    const bool doRootAction = false;
    MenuLCD g_menuLCD( LCDRS, LCDE, LCDD4, LCDD5, LCDD6, LCDD7, 16, 2);
    MenuManager g_menuManager( &g_menuLCD, doRootAction); 


Getting Started: Open the Arduino_Menu_LCD.ino sketch in the Arduino IDE.  Upload the sketch.  In the serial window, send the following keys to control the menu:

u - move UP
d - move DOWN
s - Select a menu option
b - move back in the menu tree

The idea is that you can invoke the menu system when appropriate, and have the Menu code drive all the navigation by piping your hardware inputs into the appropriate call in MenuManager.  When a menu item is selected that has an action associated with it, the menu system calls your callback so you can accomplish whatever the menu item is supposed to do.

This library builds on top of the Liquid Crystal library for doing LCD output.  On a new system, you should verify that you can create simple outputs using the Liquid Crystal library first, then move to this library to do your menuing.

The built-in actions right now are:

Back - Add a menu item labled "Back" and pass it the MenuEntry_BackCallbackFunc pointer, and you now have a Back menu entry (in case you don't have a hardware back button)

BoolTrue/False - These set a boolean to true or false so you can have off/on functionality easily.  This callback needs some more thinking and work right now to provide user feedback that the option was selected.


In addition, there is a MenuIntHelper class that assists in taking an integer input and setting a value. (See MenuIntHelper.h)


/*
 Arduino_LCD_Menu Library
Copyright Dustin Andrews, David Andrews 2012 
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
