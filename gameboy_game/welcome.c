#include "myLib.h"
#include "text.h"
#include <stdio.h>
#include "backgroundpug.h"

void welcome()
{
	int keyPressed = 0;
	drawBackground(240, 160, backgroundpug);		
	while (!keyPressed)
	{
		drawString(40, 40, "Welcome to Let the Pug Eat!", TURQ);
		drawString(40, 41, "Welcome to Let the Pug Eat!", TURQ);
		drawString(95, 60, "Press Start to Begin!", TURQ);
		drawString(95, 61, "Press Start to Begin!", TURQ);


       	 	if(KEY_DOWN_NOW(BUTTON_START))
       	 	{
       			keyPressed = 1;
       			clearScreen();
        	}

	}
}
