#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stddef.h>
#include "graphics.h"
#include "genlib.h"
#include "conio.h"

#include <windows.h>
#include <olectl.h>
#include <stdio.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>

/* Constants */

#define BLINK 100

#define ROWS 12
#define COLS 8
#define BLOCK_SIZE 0.3

#define BORDER_X 3.0
#define BORDER_Y 3.0
#define BorderWidth 2.4
#define BorderHeight 3.6

#define TEXT_X 7.0
#define TEXT_Y 6.0
#define TEXT_OFFSET 0.2

#define GO_RIGHT 1
#define GO_LEFT 2
#define GO_DOWN 3

#define TYPE_SINGLE 0
#define TYPE_QUAD 1

const int NeighborSize[10] = {1, 4};
int NeighborX[10][10];
int NeighborY[10][10];

/* Global variables */

static int CenterX;
static int CenterY;
static int CurrentType;
static int BlockExistence[ROWS][COLS];
static int BlockCnt[ROWS];
static int CurrentScore;

/* Function prototypes */

void InitBlocks();
void InitNeighbor();

void DrawSingleBlock(double x, double y);
void ClearBlocks();
void MoveThis(int type);
void DrawAllBlocks();
void DrawBorder();
void DrawItems();
void ClearSingleRow(int row);
void EliminationDetermination();
void ChangePosition(int type);

void GenerateNewBlock();
void UpdateScore();
int MovementDetermination(int type);
int CollisionDetermination();
void CountCnt();

void KeyboardEventProcess(int key,int event);
void TimerEventProcess(int TimerID);

/* Main program */

void Main()
{

	InitBlocks();
	InitNeighbor();
	InitGraphics();

	registerTimerEvent(TimerEventProcess);
	registerKeyboardEvent(KeyboardEventProcess);
	startTimer(0, BLINK);

	GenerateNewBlock();
	Randomize();
	DrawItems();
	UpdateScore();
}

//Hello!
//I am a
//Dividing Line!
//( '▿ ' )

void InitBlocks()
{
	int i, j;
	for(i = 0;i < ROWS;i++)
	for(j = 0;j < COLS;j++)
		BlockExistence[i][j] = BlockCnt[i] = 0;
}

void InitNeighbor()
{
	NeighborX[0][0] = 0,
	NeighborY[0][0] = 0;

	NeighborX[1][0] = 0,
	NeighborX[1][1] = 0,
	NeighborX[1][2] = 1,
	NeighborX[1][3] = 1,
	NeighborY[1][0] = 0,
	NeighborY[1][1] = 1,
	NeighborY[1][2] = 0,
	NeighborY[1][3] = 1;
}

void ClearBlocks()
{
	int i, j;
	double x, y;
	SetEraseMode(TRUE);
	for(i = 0;i < ROWS;i++)
	for(j = 0;j < COLS;j++)
	{
		x = j*BLOCK_SIZE + BORDER_X;
		y = i*BLOCK_SIZE + BORDER_Y;
		DrawSingleBlock(x, y);
	}
	SetEraseMode(FALSE);
}

void MoveThis(int type)
{
	int i, dx, dy, newx, newy;
	for(i = 0;i < NeighborSize[CurrentType];i++)
	{
		dx = NeighborX[CurrentType][i];
		dy = NeighborY[CurrentType][i];
		newx = CenterX + dx;
		newy = CenterY + dy;
		BlockExistence[newy][newx] = type;
	}
}

//Hello!
//I am a
//Dividing Line!
//( '▿ ' )

void DrawSingleBlock(double x, double y)
{
	MovePen(x, y);
	DrawLine(BLOCK_SIZE, 0);
	DrawLine(0, BLOCK_SIZE);
	DrawLine(-BLOCK_SIZE, 0);
	DrawLine(0, -BLOCK_SIZE);
}

void DrawAllBlocks()
{
	int i, j;
	double x, y;
	for(i = 0;i < ROWS;i++)
	for(j = 0;j < COLS;j++)
	{
		if(!BlockExistence[i][j]) continue;
		x = j*BLOCK_SIZE + BORDER_X;
		y = i*BLOCK_SIZE + BORDER_Y;
		DrawSingleBlock(x, y);
	}
}

void DrawBorder()
{
	MovePen(BORDER_X, BORDER_Y);
	DrawLine(BorderWidth, 0);
	DrawLine(0, BorderHeight);
	DrawLine(-BorderWidth, 0);
	DrawLine(0, -BorderHeight);
}

void DrawItems()
{
	DrawAllBlocks();
	DrawBorder();
}

void ClearSingleRow(int row)
{
	int j;
	BlockCnt[row] = 0;
	for(j = 0;j < COLS;j++)
		BlockExistence[row][j] = 0;
}

void EliminationDetermination()
{
	int i, i2, j;
	DisplayClear();
	for(i = 0;i < ROWS;i++)
	{
		if(BlockCnt[i] == COLS)
		{
			CurrentScore += 10;
			ClearSingleRow(i);
			for(i2 = i;i2 < ROWS;i2++)
			{
				if(i2 == ROWS - 1)
					ClearSingleRow(i2);
				else
				{
					BlockCnt[i2] = BlockCnt[i2+1];
					for(j = 0;j < COLS;j++)
						BlockExistence[i2][j] = BlockExistence[i2+1][j];
				}
			}
			i--;
		}
	}
	DrawItems();
	UpdateScore();
}

void ChangePosition(int type)
{
	ClearBlocks();
	MoveThis(0);
	BlockExistence[CenterY][CenterX] = 0;
	switch(type)
	{
		case GO_RIGHT:CenterX++;break;
		case GO_LEFT:CenterX--;break;
		case GO_DOWN:CenterY--;break;
	}
	MoveThis(1);
	DrawItems();
}

//Hello!
//I am a
//Dividing Line!
//( '▿ ' )

void GenerateNewBlock()
{
	CenterX = RandomInteger(0, COLS - 1);
	CenterY = ROWS - 1;
	CurrentType = 0;//for test
}

void UpdateScore()
{
	MovePen(TEXT_X, TEXT_Y);
	DrawTextString("your score:");
	MovePen(TEXT_X, TEXT_Y - TEXT_OFFSET);
	DrawTextString(IntegerToString(CurrentScore));
}

int MovementDetermination(int type)
{
	int i, dx, dy, newx, newy;
	for(i = 0;i < NeighborSize[CurrentType];i++)
	{
		dx = NeighborX[CurrentType][i];
		dy = NeighborY[CurrentType][i];
		newx = CenterX + dx;
		newy = CenterY + dy;
		newx = CenterX;
		newy = CenterY;
		switch(type)
		{
			case GO_RIGHT:if(newx >= COLS - 1 || BlockExistence[newy][newx+1]) return 0;break;
			case GO_LEFT:if(newx <= 0 || BlockExistence[newy][newx-1]) return 0;break;
			case GO_DOWN:if(newy <= 0 || BlockExistence[newy-1][newx]) return 0;break;
		}
	}
	return 1;
}

int CollisionDetermination()
{
	if(MovementDetermination(GO_DOWN)) return 0;
	else return 1;
}

void CountCnt()
{
	int i;
	for(i = 0;i < NeighborSize[CurrentType];i++)
		BlockCnt[CenterY + NeighborY[CurrentType][i]]++;
}

//Hello!
//I am a
//Dividing Line!
//( '▿ ' )

void TimerEventProcess(int TimerID)
{
	if(CollisionDetermination())
	{
		CountCnt();
		EliminationDetermination();
		GenerateNewBlock();
	}
	else ChangePosition(GO_DOWN);
}

void KeyboardEventProcess(int key,int event)
{
	switch(event)
	{
		case KEY_DOWN:
			switch(key)
			{
				case VK_RIGHT:if(MovementDetermination(GO_RIGHT)) ChangePosition(GO_RIGHT);break;
				case VK_LEFT:if(MovementDetermination(GO_LEFT)) ChangePosition(GO_LEFT);break;
				case VK_DOWN:if(MovementDetermination(GO_DOWN)) ChangePosition(GO_DOWN);break;
			}
	}
}
