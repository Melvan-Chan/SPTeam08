// This is the main file for the game logic and function
//
//
#include "game.h"
#include "Framework\console.h"
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;


double  g_dElapsedTime;
double  g_dDeltaTime;
bool    g_abKeyPressed[K_COUNT];
int		menu_item = 3;

// Game specific variables here
SGameChar   g_sChar;
EGAMESTATES g_eGameState = S_SPLASHSCREEN;
double  g_dBounceTime; // this is to prevent key bouncing, so we won't trigger keypresses more than once

// Console object
Console g_Console(100, 25, "SP Team"); // The size of the console

//--------------------------------------------------------------
// Purpose  : Initialisation function
//            Initialize variables, allocate memory, load data from file, etc. 
//            This is called once before entering into your main loop
// Input    : void
// Output   : void
//--------------------------------------------------------------
void init( void )// Position of player
{
    // Set precision for floating point output
    g_dElapsedTime = 0.0;
    g_dBounceTime = 0.0;

    // sets the initial state for the game
    g_eGameState = S_SPLASHSCREEN;

    g_sChar.m_cLocation.X = g_Console.getConsoleSize().X / 2;
    g_sChar.m_cLocation.Y = g_Console.getConsoleSize().Y - 1;
    g_sChar.m_bActive = true;
    // sets the width, height and the font name to use in the console
    g_Console.setConsoleFont(0, 16, L"Consolas");
	//Init put the text file in an 2D array
}

//--------------------------------------------------------------
// Purpose  : Reset before exiting the program
//            Do your clean up of memory here
//            This is called once just before the game exits
// Input    : Void
// Output   : void
//--------------------------------------------------------------
void shutdown( void )
{
    // Reset to white text on black background
    colour(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);

    g_Console.clearBuffer();
}

//--------------------------------------------------------------
// Purpose  : Getting all the key press states
//            This function checks if any key had been pressed since the last time we checked
//            If a key is pressed, the value for that particular key will be true
//
//            Add more keys to the enum in game.h if you need to detect more keys
//            To get other VK key defines, right click on the VK define (e.g. VK_UP) and choose "Go To Definition" 
//            For Alphanumeric keys, the values are their ascii values (uppercase).
// Input    : Void
// Output   : void
//--------------------------------------------------------------
void getInput( void )
{    
    g_abKeyPressed[K_UP]     = isKeyPressed(VK_UP);//0x57 is W key
    g_abKeyPressed[K_DOWN]   = isKeyPressed(VK_DOWN);//0x53 is S key
    g_abKeyPressed[K_LEFT]   = isKeyPressed(VK_LEFT);//0x41 is A key
    g_abKeyPressed[K_RIGHT]  = isKeyPressed(VK_RIGHT);//0x44 is D key
    g_abKeyPressed[K_SPACE]  = isKeyPressed(VK_SPACE);
	g_abKeyPressed[K_NUMBER1] = isKeyPressed(0X31);
    g_abKeyPressed[K_ESCAPE] = isKeyPressed(VK_ESCAPE);
	g_abKeyPressed[K_ENTER] = isKeyPressed(VK_RETURN);
}

//--------------------------------------------------------------
// Purpose  : Update function
//            This is the update function
//            double dt - This is the amount of time in seconds since the previous call was made
//
//            Game logic should be done here.
//            Such as collision checks, determining the position of your game characters, status updates, etc
//            If there are any calls to write to the console here, then you are doing it wrong.
//
//            If your game has multiple states, you should determine the current state, and call the relevant function here.
//
// Input    : dt = deltatime
// Output   : void
//--------------------------------------------------------------
void update(double dt)
{
    // get the delta time
    g_dElapsedTime += dt;
    g_dDeltaTime = dt;

    switch (g_eGameState)
    {
        case S_SPLASHSCREEN : splashScreenWait(); // game logic for the splash screen
            break;
        case S_GAME: gameplay(); // gameplay logic when we are in the game
            break;
    }
}
//--------------------------------------------------------------
// Purpose  : Render function is to update the console screen
//            At this point, you should know exactly what to draw onto the screen.
//            Just draw it!
//            To get an idea of the values for colours, look at console.h and the URL listed there
// Input    : void
// Output   : void
//--------------------------------------------------------------
void render()
{
    clearScreen();      // clears the current screen and draw from scratch 
    switch (g_eGameState)
    {
        case S_SPLASHSCREEN: renderSplashScreen();
            break;
        case S_GAME: renderGame();
            break;
    }
    renderFramerate();  // renders debug information, frame rate, elapsed time, etc
    renderToScreen();   // dump the contents of the buffer to the screen, one frame worth of game
}

void splashScreenWait()    // waits for time to pass in splash screen
{
	COORD c = g_Console.getConsoleSize();
	renderSplashScreen();
	if (menu_item == 0)
		g_eGameState = S_GAME;
	if (menu_item == 1)
	{
		c.X = 10;
		c.Y = 8;
		g_Console.writeToBuffer(c, "Story", 0x09);
	}
}

void gameplay()         // gameplay logic
{
    processUserInput(); // checks if you should change states or do something else with the game, e.g. pause, exit
    moveCharacter();    // moves the character, collision detection, physics, etc
                        // sound can be played here too.
}

void moveCharacter()
{
    bool bSomethingHappened = false;
    if (g_dBounceTime > g_dElapsedTime)
        return;

    // Updating the location of the character based on the key press
    // providing a beep sound whenver we shift the character
    if (g_abKeyPressed[K_UP] && g_sChar.m_cLocation.Y > 0)
    {
        Beep(1440, 30);
        g_sChar.m_cLocation.Y--;
        bSomethingHappened = true;
    }
    if (g_abKeyPressed[K_LEFT] && g_sChar.m_cLocation.X > 0)
    {
        Beep(1440, 30);
        g_sChar.m_cLocation.X--;
        bSomethingHappened = true;
    }
    if (g_abKeyPressed[K_DOWN] && g_sChar.m_cLocation.Y < g_Console.getConsoleSize().Y - 1)
    {
        Beep(1440, 30);
        g_sChar.m_cLocation.Y++;
        bSomethingHappened = true;
    }
    if (g_abKeyPressed[K_RIGHT] && g_sChar.m_cLocation.X < g_Console.getConsoleSize().X - 1)
    {
        Beep(1440, 30);
        g_sChar.m_cLocation.X++;
        bSomethingHappened = true;
    }
    if (g_abKeyPressed[K_SPACE])
    {
        g_sChar.m_bActive = !g_sChar.m_bActive;
        bSomethingHappened = true;
    }

    if (bSomethingHappened)
    {
        // set the bounce time to some time in the future to prevent accidental triggers
        g_dBounceTime = g_dElapsedTime + 0.125; // 125ms should be enough
    }
}
void processUserInput()
{
    // quits the game if player hits the escape key
    if (g_abKeyPressed[K_ESCAPE])
        g_bQuitGame = true;    
}

void clearScreen()
{
    // Clears the buffer with this colour attribute
    g_Console.clearBuffer(0x000);// changes console background color
}

void renderSplashScreen()  // renders the splash screen
{
	double timer = (g_dElapsedTime * 66);
	int x1 = 10;
	static bool g_Pressed = false;
	string line1 = "########  ######## ########  ##     ##  ######    ######   ######## ########";
	string line2 = "##     ## ##       ##     ## ##     ## ##    ##  ##    ##  ##       ##     ##";
	string line3 = "##     ## ##       ##     ## ##     ## ##        ##        ##       ##     ##";
	string line4 = "##     ## ######   ########  ##     ## ##   #### ##   #### ######   ########";
	string line5 = "##     ## ##       ##     ## ##     ## ##    ##  ##    ##  ##       ##   ##";
	string line6 = "##     ## ##       ##     ## ##     ## ##    ##  ##    ##  ##       ##    ##";
	string line7 = "########  ######## ########   #######   ######    ######   ######## ##     ##";
	string line8 = "===============================================================By Proplaers============";
	COORD c = g_Console.getConsoleSize();
	c.X = x1;
	c.Y = 3;
	for (double i = 0.0; i < timer && i != 76.0; i++)
	{
		if (line1[i] == '#')
		{
			c.X++;
			g_Console.writeToBuffer(c, 219, 0x06);
		}
		else if (line1[i] == ' ')
		{
			c.X++;
			g_Console.writeToBuffer(c, ' ', 0x00);
		}
	}
	c.X = x1;
	c.Y = 4;
	for (double i = 0.0; i < timer && i != 77.0; i++)
	{
		if (line2[i] == '#')
		{
			c.X++;
			g_Console.writeToBuffer(c, 219, 0x06);
		}
		else if (line2[i] == ' ')
		{
			c.X++;
			g_Console.writeToBuffer(c, ' ', 0x00);
		}
	}
	c.X = x1;
	c.Y = 5;
	for (double i = 0.0; i < timer && i != 77.0; i++)
	{
		if (line3[i] == '#')
		{
			c.X++;
			g_Console.writeToBuffer(c, 219, 0x06);
		}
		else if (line3[i] == ' ')
		{
			c.X++;
			g_Console.writeToBuffer(c, ' ', 0x00);
		}
	}
	c.X = x1;
	c.Y = 6;
	for (double i = 0.0; i < timer && i != 76.0; i++)
	{
		if (line4[i] == '#')
		{
			c.X++;
			g_Console.writeToBuffer(c, 219, 0x06);
		}
		else if (line4[i] == ' ')
		{
			c.X++;
			g_Console.writeToBuffer(c, ' ', 0x00);
		}
	}
	c.X = x1;
	c.Y = 7;
	for (double i = 0.0; i < timer && i != 75.0; i++)
	{
		if (line5[i] == '#')
		{
			c.X++;
			g_Console.writeToBuffer(c, 219, 0x06);
		}
		else if (line5[i] == ' ')
		{
			c.X++;
			g_Console.writeToBuffer(c, ' ', 0x00);
		}
	}
	c.X = x1;
	c.Y = 8;
	for (double i = 0.0; i < timer && i != 76.0; i++)
	{
		if (line6[i] == '#')
		{
			c.X++;
			g_Console.writeToBuffer(c, 219, 0x06);
		}
		else if (line6[i] == ' ')
		{
			c.X++;
			g_Console.writeToBuffer(c, ' ', 0x00);
		}
	}
	c.X = x1;
	c.Y = 9;
	for (double i = 0.0; i < timer && i != 77.0; i++)
	{
		if (line7[i] == '#')
		{
			c.X++;
			g_Console.writeToBuffer(c, 219, 0x06);
		}
		else if (line7[i] == ' ')
		{
			c.X++;
			g_Console.writeToBuffer(c, ' ', 0x00);
		}
	}
	c.X = x1 - 5;
	c.Y = 10;
	for (double i = 0.0; i < (timer + 20) && i != 87.0; i++)
	{
		if (line8[i] == '=')
		{
			c.X++;
			g_Console.writeToBuffer(c, '=', 0x07);
		}
		else if (line8[i] == ' ')
		{
			c.X++;
			g_Console.writeToBuffer(c, ' ', 0x00);
		}
		else
		{
			c.X++;
			g_Console.writeToBuffer(c, line8[i], 0x03);
		}
	}
	c.Y = 14;
	c.X = 45;
	g_Console.writeToBuffer(c, "Start Game", 0x06);
	c.Y = 15;
	c.X = 45;
	g_Console.writeToBuffer(c, "Story", 0x06);

	if (g_Pressed == false)
	{
		c.X = 43;
		c.Y = 14;
		g_Console.writeToBuffer(c, "  ", 0x00);
		c.Y = 14;
		c.X = 43;
		g_Console.writeToBuffer(c, ">");
		if (g_abKeyPressed[K_ENTER])
		{
			menu_item = 0;
		}
	}
	else // if boolean is true
	{
		c.X = 43;
		c.Y = 15;
		g_Console.writeToBuffer(c, "  ", 0x00);
		c.Y = 15;
		c.X = 43;
		g_Console.writeToBuffer(c, ">");
		if (g_abKeyPressed[K_ENTER])
		{
			menu_item = 1;
		}
	}
	// Check what key is pressed and set boolean to true or false
	if (g_abKeyPressed[K_DOWN] && c.Y != 11)
	{
		g_Pressed = true; // if press down key, set bool to true
	}
	if (g_abKeyPressed[K_UP] && c.Y != 10)
	{
		g_Pressed = false; // if press up key, set bool to false
	}
}

void renderGame()
{
    renderMap();        // renders the map to the buffer first
    renderCharacter();  // renders the character into the buffer
}

void renderMap()
{
    // Set up sample colours, and output shadings
    const WORD colors[] = {
        0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F,
        0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6
    };

    COORD c;
    for (int i = 0; i < 12; ++i)
    {
        c.X = 5 * i;
        c.Y = i + 1;
        colour(colors[i]);
        g_Console.writeToBuffer(c, 'c', colors[i]);
		/*g_Console.writeToBuffer(c, " °±²Û", colors[i]);*/
    }
}

void renderCharacter()
{
    // Draw the location of the character
    WORD charColor = 0x0C;
    if (g_sChar.m_bActive)
    {
        charColor = 0x0A;
    }
    g_Console.writeToBuffer(g_sChar.m_cLocation, (char)2 , charColor);
}

void renderFramerate()
{
    COORD c;
    // displays the framerate
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(3);
    ss << 1.0 / g_dDeltaTime << "fps";
    c.X = g_Console.getConsoleSize().X - 9;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str());

    // displays the elapsed time
    ss.str("");
    ss << g_dElapsedTime << "secs";
    c.X = 0;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str(), 0x59);//5 and 9 changes color
}
void renderToScreen()
{
    // Writes the buffer to the console, hence you will see what you have written
    g_Console.flushBufferToConsole();
}
