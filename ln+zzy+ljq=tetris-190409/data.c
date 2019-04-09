#include <stdio.h>
#include <stdlib.h>

extern int NeighborSize[40];
extern int NeighborX[40][10];
extern int NeighborY[40][10];
extern char NeighborColor[30][40];

#include <string.h>

void InitNeighbor()
{
	// single
	strcpy(NeighborColor[0], "red");
	NeighborSize[0] = 1;
	NeighborX[0][0] = 0,
	NeighborY[0][0] = 0;

	//QuadLump
	strcpy(NeighborColor[1], "cyan");
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
	strcpy(NeighborColor[2], "Magenta");
	NeighborSize[2] = 2;
	NeighborX[2][0] = 0,
	NeighborX[2][1] = 0,
	NeighborY[2][0] = 0,
	NeighborY[2][1] = 1;

	//DoubleHori
	strcpy(NeighborColor[3], "Magenta");
	NeighborSize[3] = 2;
	NeighborX[3][0] = 0,
	NeighborX[3][1] = 1,
	NeighborY[3][0] = 0,
	NeighborY[3][1] = 0;

	//TriVert
	strcpy(NeighborColor[4], "Green");
	NeighborSize[4] = 3;
	NeighborX[4][0] = 0,
	NeighborX[4][1] = 0,
	NeighborX[4][2] = 0;
	NeighborY[4][0] = 0,
	NeighborY[4][1] = 1,
	NeighborY[4][2] = -1;

	//TriHori
	strcpy(NeighborColor[5], "Green");
	NeighborSize[5] = 3;
	NeighborX[5][0] = 0,
	NeighborX[5][1] = 1,
	NeighborX[5][2] = -1;
	NeighborY[5][0] = 0,
	NeighborY[5][1] = 0,
	NeighborY[5][2] = 0;

	//QuadVert
	strcpy(NeighborColor[6], "Violet");
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
	strcpy(NeighborColor[7], "Violet");
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
	strcpy(NeighborColor[8], "Yellow");
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
	strcpy(NeighborColor[9], "Yellow");
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
	strcpy(NeighborColor[10], "Yellow");
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
	strcpy(NeighborColor[11], "Yellow");
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