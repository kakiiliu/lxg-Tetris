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

#define BLINK 200

#define ROWS 18
#define COLS 10
#define BLOCK_SIZE 0.3

#define BORDER_X 1.0
#define BORDER_Y 1.0
#define BorderWidth 3.0
#define BorderHeight 5.4

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

int NeighborSize[40];
int NeighborX[40][10];
int NeighborY[40][10];

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
int TypeMap();

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

void InitBlocks()
{
	int i, j;
	for(i = 0;i < ROWS;i++)
	for(j = 0;j < COLS;j++)
		BlockExistence[i][j] = NeighborExistence[i][j] = BlockCnt[i] = 0;
}

void InitNeighbor()
{
	// single
	NeighborSize[0] = 1;
	NeighborX[0][0] = 0,
	NeighborY[0][0] = 0;

	//QuadLump
	NeighborSize[1] = 4;
	NeighborX[1][0] = 0,
	NeighborX[1][1] = 0,
	NeighborX[1][2] = 1,
	NeighborX[1][3] = 1,
	NeighborY[1][0] = 0,
	NeighborY[1][1] = 1,
	NeighborY[1][2] = 0,
	NeighborY[1][3] = 1;

	//DoubleVert
	NeighborSize[2] = 2;
	NeighborX[2][0] = 0,
	NeighborX[2][1] = 0,
	NeighborY[2][0] = 0,
	NeighborY[2][1] = 1;

	//DoubleHori
	NeighborSize[3] = 2;
	NeighborX[3][0] = 0,
	NeighborX[3][1] = 1,
	NeighborY[3][0] = 0,
	NeighborY[3][1] = 0;

	//TriVert
	NeighborSize[4] = 3;
	NeighborX[4][0] = 0,
	NeighborX[4][1] = 0,
	NeighborX[4][2] = 0;
	NeighborY[4][0] = 0,
	NeighborY[4][1] = 1,
	NeighborY[4][2] = -1;

	//TriHori
	NeighborSize[5] = 3;
	NeighborX[5][0] = 0,
	NeighborX[5][1] = 1,
	NeighborX[5][2] = -1;
	NeighborY[5][0] = 0,
	NeighborY[5][1] = 0,
	NeighborY[5][2] = 0;

	//QuadVert
	NeighborSize[6] = 4;
	NeighborX[6][0] = 0,
	NeighborX[6][1] = 1,
	NeighborX[6][2] = 2,
	NeighborX[6][3] = -1;
	NeighborY[6][0] = 0,
	NeighborY[6][1] = 0,
	NeighborY[6][2] = 0,
	NeighborY[6][3] = 0;

	//QuadHori
	NeighborSize[7] = 4;
	NeighborX[7][0] = 0,
	NeighborX[7][1] = 0,
	NeighborX[7][2] = 0,
	NeighborX[7][3] = 0;
	NeighborY[7][0] = 0,
	NeighborY[7][1] = 1,
	NeighborY[7][2] = 2,
	NeighborY[7][3] = -1;

	//TriAngelRight
	NeighborSize[8] = 4;
	NeighborX[8][0] = 0,
	NeighborX[8][1] = 0,
	NeighborX[8][2] = 0,
	NeighborX[8][3] = 1;
	NeighborY[8][0] = 0,
	NeighborY[8][1] = 1,
	NeighborY[8][2] = -1,
	NeighborY[8][3] = 0;

	//TriAngelDown
	NeighborSize[9] = 4;
	NeighborX[9][0] = 0,
	NeighborX[9][1] = 1,
	NeighborX[9][2] = -1,
	NeighborX[9][3] = 0;
	NeighborY[9][0] = 0,
	NeighborY[9][1] = 0,
	NeighborY[9][2] = 0,
	NeighborY[9][3] = -1;

	//TriAngelLeft
	NeighborSize[10] = 4;
	NeighborX[10][0] = 0,
	NeighborX[10][1] = 0,
	NeighborX[10][2] = 0,
	NeighborX[10][3] = -1;
	NeighborY[10][0] = 0,
	NeighborY[10][1] = 1,
	NeighborY[10][2] = -1,
	NeighborY[10][3] = 0;

	//TriAngelUp
	NeighborSize[11] = 4;
	NeighborX[11][0] = 0,
	NeighborX[11][1] = 1,
	NeighborX[11][2] = -1,
	NeighborX[11][3] = 0;
	NeighborY[11][0] = 0,
	NeighborY[11][1] = 0,
	NeighborY[11][2] = 0,
	NeighborY[11][3] = 1;
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
	DrawLine(BLOCK_SIZE, 0);
	DrawLine(0, BLOCK_SIZE);
	DrawLine(-BLOCK_SIZE, 0);
	DrawLine(0, -BLOCK_SIZE);

	SetPenColor("red");
	// StartFilledRegion(1);
	MovePen(x + BLOCK_SIZE / 2, y + BLOCK_SIZE / 2);
	DrawArc(0.05, 0.0, 360.0);
	// EndFilledRegion();
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
	CurrentType = RandomInteger(0, Max_Type);//[left, right]
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
	int i, newx, newy, NextType;
	NextType = TypeMap();
	if(NextType == -1) return 0;
	for(i = 0;i < NeighborSize[NextType];i++)
	{
		newx = CenterX + NeighborX[NextType][i];
		newy = CenterY + NeighborY[NextType][i];
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
