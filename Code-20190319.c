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

#define ROWS 16
#define COLS 12
#define BLOCK_SIZE 0.3

#define BORDER_X 1.0
#define BORDER_Y 1.0
#define BorderWidth 3.6
#define BorderHeight 4.8

#define TEXT_X 7.0
#define TEXT_Y 6.0
#define TEXT_OFFSET 0.2

#define GO_RIGHT 1
#define GO_LEFT 2
#define GO_DOWN 3

#define TYPE_SINGLE 0
#define TYPE_QUAD 1
#define TYPE_DOUBLEDOWN 2
#define TYPE_DOUBLEHEN 3

const int NeighborSize[10] = {1, 4, 2, 2};
int NeighborX[10][10];
int NeighborY[10][10];

/* Global variables */

static int CenterX;
static int CenterY;
static int CurrentType;
static int NeighborExistence[ROWS][COLS];
static int BlockExistence[ROWS][COLS];
static int BlockCnt[ROWS];
static int CurrentScore;

/* Function prototypes */

void InitBlocks();
void InitNeighbor();

void ClearBlocks();
void DrawSingleBlock(double x, double y);
void DrawAllBlocks();
void DrawBorder();
void DrawItems();

void ClearSingleRow(int row);
void EliminationDetection();
void MoveThis(int type);
void ClearNeighbors();
void ChangePosition(int type);

void GenerateNewBlock();
int MoveSingleDetection(int x, int y);
int MoveWholeDetection(int type);
int CollisionDetection();
void CountCnt();

void UpdateScore();

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

	Randomize();
	GenerateNewBlock();
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
		BlockExistence[i][j] = NeighborExistence[i][j] = BlockCnt[i] = 0;
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

	NeighborX[2][0] = 0,
	NeighborX[2][1] = 0,
	NeighborY[2][0] = 0,
	NeighborY[2][1] = 1;

	NeighborX[3][0] = 0,
	NeighborX[3][1] = 1,
	NeighborY[3][0] = 0,
	NeighborY[3][1] = 0;
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

//Hello!
//I am a
//Dividing Line!
//( '▿ ' )

void DrawSingleBlock(double x, double y)
{
	MovePen(x, y);
	// StartFilledRegion(1);
	DrawLine(BLOCK_SIZE, 0);
	DrawLine(0, BLOCK_SIZE);
	DrawLine(-BLOCK_SIZE, 0);
	DrawLine(0, -BLOCK_SIZE);
	// StartFilledRegion(0);
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

void EliminationDetection()
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

void MoveThis(int type)
{
	int i, newx, newy;
	for(i = 0;i < NeighborSize[CurrentType];i++)
	{
		newx = CenterX + NeighborX[CurrentType][i];
		newy = CenterY + NeighborY[CurrentType][i];
		BlockExistence[newy][newx] = type;
		NeighborExistence[newy][newx] = type;
	}
}

void ClearNeighbors()
{
	int i, newx, newy;
	for(i = 0;i < NeighborSize[CurrentType];i++)
	{
		newx = CenterX + NeighborX[CurrentType][i];
		newy = CenterY + NeighborY[CurrentType][i];
		NeighborExistence[newy][newx] = 0;
	}
}

void ChangePosition(int type)
{
	ClearBlocks();
	MoveThis(0);
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
	int TryTimes = 0;
	CurrentType = RandomInteger(0, 3);//[left, right]
	while(TryTimes < 100)
	{
		CenterX = RandomInteger(0, COLS - 1);
		CenterY = RandomInteger(ROWS - 3, ROWS - 1);
		if(!CollisionDetection()) return;
		TryTimes++;
	}
	if(CollisionDetection()) exit(0);
}

int MoveSingleDetection(int x, int y)
{
	return (BlockExistence[y][x] && !NeighborExistence[y][x]);
}

int MoveWholeDetection(int type)
{
	int i, newx, newy;
	for(i = 0;i < NeighborSize[CurrentType];i++)
	{
		newx = CenterX + NeighborX[CurrentType][i];
		newy = CenterY + NeighborY[CurrentType][i];
		switch(type)
		{
			case GO_RIGHT:if(newx >= COLS - 1 || MoveSingleDetection(newx+1, newy)) return 0;break;
			case GO_LEFT:if(newx <= 0 || MoveSingleDetection(newx-1, newy)) return 0;break;
			case GO_DOWN:if(newy <= 0 || MoveSingleDetection(newx, newy-1)) return 0;break;
		}
	}
	return 1;
}

int CollisionDetection()
{
	int i, newx, newy;
	for(i = 0;i < NeighborSize[CurrentType];i++)
	{
		newx = CenterX + NeighborX[CurrentType][i];
		newy = CenterY + NeighborY[CurrentType][i];
		if(BlockExistence[newy][newx]
		|| newy <= 0 || newx <= 0 || newx >= COLS-1 || newy >= ROWS-1)
			return 1;
	}
	return 0;
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

void UpdateScore()
{
	MovePen(TEXT_X, TEXT_Y);
	DrawTextString("your score:");
	MovePen(TEXT_X, TEXT_Y - TEXT_OFFSET);
	DrawTextString(IntegerToString(CurrentScore));
}

void TimerEventProcess(int TimerID)
{
	if(!MoveWholeDetection(GO_DOWN))
	{
		CountCnt();
		ClearNeighbors();
		EliminationDetection();
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
				case VK_RIGHT:if(MoveWholeDetection(GO_RIGHT)) ChangePosition(GO_RIGHT);break;
				case VK_LEFT:if(MoveWholeDetection(GO_LEFT)) ChangePosition(GO_LEFT);break;
				case VK_DOWN:if(MoveWholeDetection(GO_DOWN)) ChangePosition(GO_DOWN);break;
				case VK_UP:while(MoveWholeDetection(GO_DOWN)) ChangePosition(GO_DOWN);break;
			}
	}
}
