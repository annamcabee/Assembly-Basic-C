#include "myLib.h"
#include "text.h"
#include "pug1.h"
#include "cookies.h"
#include "ryan.h"
#include "sadpug.h"


u16* videoBuffer = (u16*) 0x6000000;

// A function to set pixel (r, c) to the color passed in.
void setPixel(int r, int c, u16 color)
{
        videoBuffer[OFFSET(r, c, 240)] = color;
}
void drawImage(const unsigned short arr[])
{
	DMA[3].src = arr;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = (240*160) | DMA_ON;
}

void drawImage3(int r, int c, int width, int height, const u16* image)
{
	for (int i = 0; i < height; i++)
    {
        DMA[3].src = &image[OFFSET(i, 0, width)];
        DMA[3].dst = videoBuffer + OFFSET(r+i, c, 240);
        DMA[3].cnt = width | DMA_ON;
    }
}

void drawPug(PUG pug)
{
	drawImage3(pug.row, pug.col, 18, 24, pug1);
}

void drawEnemy(ENEMY sEnemy)
{
	drawImage3(sEnemy.row, sEnemy.col, 24, 18, ryan);
}

void drawCookies(COOKIES sCookies)
{
	drawImage3(sCookies.row, sCookies.col, 18, 24, cookies);
}


void setColor(volatile u16 color)
{
	DMA[3].src = &color;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = (240*160) | DMA_SOURCE_FIXED | DMA_ON;
}

void clearScreen()
{
	setColor(BLACK);
}

// A function to draw a FILLED rectangle starting at (r, c).
void drawRect(int r, int c, int width, int height, u16 color)
{
        for (int i = 0; i < height; i++)
        {
			DMA[3].src = &color;
			DMA[3].dst = videoBuffer + OFFSET(r+i, c, 240);
			DMA[3].cnt = width | DMA_SOURCE_FIXED | DMA_ON;
        }
}

void drawHollowRect(int r, int c, int width, int height, u16 color)
{
        int row, col;
        row = 0;
        for (col = 0; col < width; col++) {
			setPixel(r+row, c+col, color);
        }

        col = 0;
        for (row = 1; row < height - 1; row++) {
			setPixel(r+row, c+col, color);
        }

        col = width - 1;
        for (row = 1; row < height - 1; row++) {
			setPixel(r+row, c+col, color);
        }


        row = height - 1;
        for (col = 0; col < width; col++) {
			setPixel(r+row, c+col, color);
        }
}

void waitForVblank()
{
        while(SCANLINECOUNTER > 160);
        while(SCANLINECOUNTER < 160);
}

int collision(int aRow, int aCol, int aWidth, int aHeight, int bRow, int bCol, int bWidth, int bHeight)
{
        if (aRow < (bRow + bHeight) && (aRow + aHeight) > bRow)
        {
			if (aCol < (bCol + bWidth) && (aCol + aWidth) > bCol)
			{
				return 1;
			}
        }
        return 0;
}

int enemyCollision(PUG pug, ENEMY theEnemy)
{
        return collision(pug.row, pug.col, PUG_SIZE, PUG_SIZE, theEnemy.row, theEnemy.col, ENEMY_WIDTH, ENEMY_HEIGHT);
}

int cookiesCollision(PUG pug, COOKIES cookies)
{
        return collision(pug.row, pug.col, PUG_SIZE, PUG_SIZE, cookies.row, cookies.col, COOKIES_WIDTH, COOKIES_HEIGHT);
}

void drawBackground(int width, int height, const u16* image)
{
        int pixelCount = 0;
        for (int h = 0; h < height; h++)
        {
			for (int w = 0; w < width; w++)
			{
				setPixel(h, w, image[pixelCount]);
				pixelCount++;
			}
        }

}
void drawString(int row, int col, char *str, u16 color)
{
        int chars_drawn = 0;
        while(*str){
			drawChar(row, col + 6*chars_drawn++, *str++, color);
        }
}
