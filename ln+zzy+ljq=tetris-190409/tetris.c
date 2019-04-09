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

#include <imgui.h>
#include "data.c"

/* Constants */

#define BLINK 200

#define ROWS 18
#define COLS 10
#define BLOCK_SIZE 0.3

#define BORDER_X 1.0
#define BORDER_Y 1.0
#define BorderWidth 3.0
#define BorderHeight 5.4
#define FRAME_X 5.0
#define FRAME_Y 5.0
#define FrameWidth 1.2
#define FrameHeight 1.2

#define TEXT_X 7.0
#define TEXT_Y 6.0
#define TEXT_OFFSET 0.2

#define GO_RIGHT 1
#define GO_LEFT 2
#define GO_DOWN 3

#define Max_Type 11
#define TYPE_Single 0
#define TYPE_QuadLump 1
#define TYPE_DoubleVert 2
#define TYPE_DoubleHori 3
#define TYPE_TriVert 4
#define TYPE_TriHori 5
#define TYPE_QuadVert 6
#define TYPE_QuadHori 7
#define TYPE_TriAngleRight 8
#define TYPE_TriAngleDown 9
#define TYPE_TriAngleLeft 10
#define TYPE_TriAngleUp 11

/* Global variables */

static int CenterX;
static int CenterY;
static int CurrentType, NextType = 0;
static int NeighborExistence[ROWS][COLS];
static int BlockExistence[ROWS][COLS];
static int BlockCnt[ROWS];
static int CurrentScore;
static char BlockColor[ROWS][COLS][40];


/* Function prototypes */

void InitBlocks();
void InitNeighbor();
int TypeMap();

void DrawSingleBlock(double x, double y);
void DrawAllBlocks();
void DrawBorder();
void DrawItems();

void ClearSingleRow(int row);
void EliminationDetection();
void MoveThis(int isMove);
void ClearNeighbors();
void ChangePosition(int MoveType);

void GenerateNewBlock();
int MoveSingleDetection(int x, int y);
int MoveWholeDetection(int MoveType);
int CollisionDetection();
void CountCnt();

int RotateDetection();
void RotateBlock();

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

#include <string.h>
void InitBlocks()
{
	int i, j;
	for(i = 0;i < ROWS;i++)
	for(j = 0;j < COLS;j++)
	{
		BlockExistence[i][j] = NeighborExistence[i][j] = BlockCnt[i] = 0;
		strcpy(BlockColor[i][j], "");
	}
}

int TypeMap()
{
	switch(CurrentType)
	{
		case TYPE_DoubleHori:return TYPE_DoubleVert;
		case TYPE_DoubleVert:return TYPE_DoubleHori;

		case TYPE_TriHori:return TYPE_TriVert;
		case TYPE_TriVert:return TYPE_TriHori;

		case TYPE_QuadHori:return TYPE_QuadVert;
		case TYPE_QuadVert:return TYPE_QuadHori;

		case TYPE_TriAngleRight:return TYPE_TriAngleDown;
		case TYPE_TriAngleDown:return TYPE_TriAngleLeft;
		case TYPE_TriAngleLeft:return TYPE_TriAngleUp;
		case TYPE_TriAngleUp:return TYPE_TriAngleRight;
	}
	return -1;
}

//Hello!
//I am a
//Dividing Line!
//( '▿ ' )

void DrawSingleBlock(double x, double y)
{
	drawRectangle(x, y, BLOCK_SIZE, BLOCK_SIZE, 1);
	SetPenColor("blue");
	drawRectangle(x, y , BLOCK_SIZE, BLOCK_SIZE, 0);
	SetPenColor("black");
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
		SetPenColor(BlockColor[i][j]);
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

void DrawNextType()
{
	int i;
	double x, y;
	MovePen(FRAME_X, FRAME_Y);
	DrawLine(FrameWidth, 0);
	DrawLine(0, FrameHeight);
	DrawLine(-FrameWidth, 0);
	DrawLine(0, -FrameHeight);
	for(i = 0;i < NeighborSize[NextType];i++)
	{
		x = FRAME_X + (NeighborX[NextType][i] + 1) * BLOCK_SIZE;
		y = FRAME_Y + (NeighborY[NextType][i] + 1) * BLOCK_SIZE;
		SetPenColor(NeighborColor[NextType]);
		DrawSingleBlock(x, y);
	}
}

void DrawItems()
{
	DrawAllBlocks();
	DrawBorder();
	DrawNextType();
	UpdateScore();
}

void ClearSingleRow(int row)
{
	int j;
	BlockCnt[row] = 0;
	for(j = 0;j < COLS;j++)
	{
		BlockExistence[row][j] = 0;
		strcpy(BlockColor[row][j], "");
	}
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
					{
						BlockExistence[i2][j] = BlockExistence[i2+1][j];
						strcpy(BlockColor[i2][j], BlockColor[i2+1][j]);
					}
				}
			}
			i--;
		}
	}
	DrawItems();
}

void MoveThis(int isMove)
{
	int i, newx, newy;
	for(i = 0;i < NeighborSize[CurrentType];i++)
	{
		newx = CenterX + NeighborX[CurrentType][i];
		newy = CenterY + NeighborY[CurrentType][i];
		BlockExistence[newy][newx] = isMove;
		NeighborExistence[newy][newx] = isMove;
		if(isMove) strcpy(BlockColor[newy][newx], NeighborColor[CurrentType]);
		else strcpy(BlockColor[newy][newx], "");
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

void ChangePosition(int MoveType)
{
	DisplayClear();
	MoveThis(0);
	switch(MoveType)
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
	CurrentType = NextType;
	NextType = RandomInteger(0, Max_Type);//[left, right]
	while(TryTimes < 100)
	{
		CenterX = RandomInteger(0, COLS - 1);
		CenterY = RandomInteger(ROWS - 5, ROWS - 1);
		if(!CollisionDetection()) return;
		TryTimes++;
	}
	if(CollisionDetection()) exit(0);
}

int OutOfBorder(int x, int y)
{
	return (y <= 0 || x <= 0 || x >= COLS-1 || y >= ROWS-1);
}

int MoveSingleDetection(int x, int y)
{
	return (BlockExistence[y][x] && !NeighborExistence[y][x]);
}

int MoveWholeDetection(int MoveType)
{
	int i, newx, newy;
	for(i = 0;i < NeighborSize[CurrentType];i++)
	{
		newx = CenterX + NeighborX[CurrentType][i];
		newy = CenterY + NeighborY[CurrentType][i];
		switch(MoveType)
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
		if(BlockExistence[newy][newx] || OutOfBorder(newx, newy))
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

int RotateDetection()
{
	int i, newx, newy, MapedType;
	MapedType = TypeMap();
	if(MapedType == -1) return 0;
	for(i = 0;i < NeighborSize[MapedType];i++)
	{
		newx = CenterX + NeighborX[MapedType][i];
		newy = CenterY + NeighborY[MapedType][i];
		if(MoveSingleDetection(newx, newy) || OutOfBorder(newx, newy))
			return 0;
	}
	return 1;
}

void RotateBlock()
{
	int i, newx, newy;
	if(!RotateDetection()) return;
	ClearNeighbors();
	for(i = 0;i < NeighborSize[CurrentType];i++)
	{
		newx = CenterX + NeighborX[CurrentType][i];
		newy = CenterY + NeighborY[CurrentType][i];
		BlockExistence[newy][newx] = 0;
	}
	CurrentType = TypeMap();
	for(i = 0;i < NeighborSize[CurrentType];i++)
	{
		newx = CenterX + NeighborX[CurrentType][i];
		newy = CenterY + NeighborY[CurrentType][i];
		BlockExistence[newy][newx] = NeighborExistence[newy][newx] = 1;
	}
}

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
				case VK_F1:RotateBlock();break;
			}
	}
}
