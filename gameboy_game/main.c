#include <stdio.h>
#include <stdlib.h>
#include "myLib.h"
#include "text.h"
#include "welcome.h"
#include "ryan.h"
#include "pug1.h"
#include "sadpug.h"


#define NUMOBJS 7
#define MAXBULL 5
#define NUMENEMIES 4
#define PUG_SPEED_DEFAULT 2
#define LIVES_DEFAULT 3

enum {WELCOME, GAME, WIN, LOSE};
void splash(int* pLives);
int game(int* pLives);
void lose(int* pLives);
void win();
int main();

int main() {
        int state = WELCOME;
        int lives = LIVES_DEFAULT;
        while(1)
        {
			switch(state)
			{
				case WELCOME:
					splash(&lives);
					state = GAME;
					break;

				case GAME:
					state = game(&lives);
					break;

				case LOSE:
					lose(&lives);
					state = WELCOME;
					break;

				case WIN:
					win();
					state = WELCOME;
					break;

				default:
					break;
			}
        }
}
void splash(int* plives)
{
        REG_DISPCNT = MODE3 | BG2_ENABLE;
        welcome();
}

int game(int* plives)
{
        REG_DISPCNT = MODE3 | BG2_ENABLE;

        PUG pug = {160 - 30, 240 - 30, LEFT};
        PUG oldPug = pug;
        int pugSpeed = PUG_SPEED_DEFAULT;
        COOKIES sCookies = {rand()%50, rand()%50};


        ENEMY enemies [NUMENEMIES];
        ENEMY oldEnemies [NUMENEMIES];
        ENEMY *cur;
        int d[] = {-1, 0, 1};
        int numd = sizeof(d) / sizeof(d[0]);

        for (int i = 0; i < NUMENEMIES; i++)
        {
			enemies[i].row = rand()%60 + rand()%60;
			enemies[i].col = rand()%100 + 60;
			enemies[i].drow = d[rand()%numd];
			enemies[i].dcol = d[rand()%numd];
			while (!enemies[i].drow && !enemies[i].dcol)
			{
				enemies[i].drow = d[rand()%numd];
				enemies[i].dcol = d[rand()%numd];
			}
			oldEnemies[i] = enemies[i];
        }
        drawPug(pug);
        for (int i = 0; i < NUMENEMIES; i++)
        {
			drawEnemy(enemies[i]);
        }
        drawCookies(sCookies);
        drawChar(76, 117, '3', YELLOW);
        char buffer[41];
		sprintf(buffer, "Lives Remaining: %d", *plives);
		drawString(0, 130, buffer, YELLOW);
        for (int i = 0; i < 300; i++)
        {
			waitForVblank();
			if(KEY_DOWN_NOW(BUTTON_SELECT))
			{
				return WELCOME;
			}
        }
		drawRect(76, 117, 7, 7, BLACK);
        drawRect(76, 117, 7, 7, BLACK);
        drawChar(76, 117, '2', YELLOW);
        for (int i = 0; i < 300; i++)
        {
			waitForVblank();
			if(KEY_DOWN_NOW(BUTTON_SELECT))
			{
				return WELCOME;
			}
        }
        drawRect(76, 117, 7, 7, BLACK);
        drawRect(76, 117, 7, 7, BLACK);
        drawChar(76, 117, '1', YELLOW);
        for (int i = 0; i < 300; i++)
        {
			waitForVblank();
			if(KEY_DOWN_NOW(BUTTON_SELECT))
			{
				return WELCOME;
			}
        }
        drawRect(0, 130, 240 - 130, 15, BLACK);
        drawRect(76, 117, 7, 7, BLACK);
        while(1)
        {
        if(KEY_DOWN_NOW(BUTTON_UP))
        {
			pug.row -= pugSpeed;
			if (pug.row < 0)
			{
				pug.row = 160 - 24;
			}
        }
        if(KEY_DOWN_NOW(BUTTON_DOWN))
        {
			pug.row += pugSpeed;
			if (pug.row > 160 - PUG_SIZE)
			{
				pug.row = 0;
			}
        }
        if(KEY_DOWN_NOW(BUTTON_LEFT))
        {
			pug.col -= pugSpeed;
			pug.facing = LEFT;
			if (pug.col < 0)
			{
				pug.col = 240 - 20;
			}
        }
        if(KEY_DOWN_NOW(BUTTON_RIGHT))
        {
			pug.col += pugSpeed;
			pug.facing = RIGHT;
			if (pug.col > 240 - 18)
			{
				pug.col = 0;
			}
        }
        if(KEY_DOWN_NOW(BUTTON_SELECT))
        {
			*plives = LIVES_DEFAULT;
			return WELCOME;
        }
        for (int i = 0; i < NUMENEMIES; i++)
        {
				cur = enemies + i;
				cur->row = cur->row + cur->drow;
				cur->col = cur->col + cur->dcol;
			if(cur->row < 0)
			{
				cur->row = 0;
				cur->drow=-cur->drow;
			}
			if(cur->row > 160 - 18)
			{
				cur->row = 160 - 18;
				cur->drow=-cur->drow;
			}

			if(cur->col < 0)
			{
				cur->col = 0;
				cur->dcol = -cur->dcol;
			}
			if(cur->col > 240 - 24)
			{
				cur->col = 240 - 24;
				cur->dcol =-cur->dcol;
			}
        }
        for (int i = 0; i < NUMENEMIES; i++)
        {
			if(enemyCollision(pug, enemies[i]))
			{
				if (*plives)
				{
					*plives -= 1;
					clearScreen();
					return GAME;
				}
				else
				{
					return LOSE;
				}
			}
        }

        if(cookiesCollision(pug, sCookies))
        {
			return WIN;
        }


        waitForVblank();


        drawRect(oldPug.row, oldPug.col, 18, 24, BLACK);
        drawPug(pug);
        oldPug = pug;
        for (int i = 0; i < NUMENEMIES; i++)
        {
			drawRect(oldEnemies[i].row, oldEnemies[i].col, 24, 18, BLACK);
        }
        for (int i = 0; i < NUMENEMIES; i++)
        {
			drawEnemy(enemies[i]);
			oldEnemies[i] = enemies[i];
        }
        drawCookies(sCookies);
        }

}
void lose(int* pLives)
{
        REG_DISPCNT = MODE3 | BG2_ENABLE;
        clearScreen();
        drawBackground(240, 160, sadpug);
		drawString(50, 90, "Gameover", YELLOW);
		drawString(100, 50, "Press Start to Play Again", YELLOW);
        while(!KEY_DOWN_NOW(BUTTON_START));
        while(KEY_DOWN_NOW(BUTTON_START));
        if(KEY_DOWN_NOW(BUTTON_SELECT))
        {
			welcome();
        }
}

void win()
{
			REG_DISPCNT = MODE3 | BG2_ENABLE;
			drawBackground(240, 160, sadpug);
			drawString(50, 85, "YOU WIN! :)", YELLOW);
			drawString(100, 45, "Press Start to Play Again", YELLOW);
			while(!KEY_DOWN_NOW(BUTTON_START));
			while(KEY_DOWN_NOW(BUTTON_START));
			if(KEY_DOWN_NOW(BUTTON_SELECT))
			{
				welcome();
			}
}
