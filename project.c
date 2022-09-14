#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <windows.h>

int nScreenWidth = 120;
int nScreenHeight = 40;
float fPlayerX = 7.0;
float fPlayerY = 7.0;
float fPlayerA = 0.0;
int nMapHeight = 16;
int nMapWidth = 16;
float fFOV = -(3.14159 / 1.35);
float fDepth = 30.0;
int x;
int y;
char map[16][17];
char screen[40][120];
int nCeiling;
int nFloor;
float fDistanceToWall;
int bHitWall;
float fRayAngle;
float fEyeX;
float fEyeY;
int nTestX;
int nTestY;
int v, v1;
int z, z1;

void MiniMap(float Y, float X, float A)
{
	sprintf(map[0], "################");
	sprintf(map[1], "#..............#");
	sprintf(map[2], "#..............#");
	sprintf(map[3], "#..............#");
	sprintf(map[4], "#..............#");
	sprintf(map[5], "#..............#");
	sprintf(map[6], "#..............#");
	sprintf(map[7], "#..............#");
	sprintf(map[8], "#..............#");
	sprintf(map[9], "#..............#");
	sprintf(map[10], "#..............#");
	sprintf(map[11], "#..............#");
	sprintf(map[12], "#..............#");
	sprintf(map[13], "#..............#");
	sprintf(map[14], "#..............#");
	sprintf(map[15], "################");
    y = (int)Y;
	x = (int)X;
	map[y][x] = '@';/*
	z = 2*(sin((fPlayerA - fFOV) + (0 / (float)nScreenWidth) * fFOV/2));
	v = 2*(cos((fPlayerA - fFOV) + (0 / (float)nScreenWidth) * fFOV/2));
	z1 = 2*(sin((fPlayerA - fFOV) + fFOV/2));
	v1 = 2*(cos((fPlayerA - fFOV) + fFOV/2));
	map[(int)(fPlayerX - v)][(int)(fPlayerX - z)] = '*';
	map[(int)(fPlayerX - v1)][(int)(fPlayerX - z1)] = '*';*/
	printf("X = %f, y = %f, a = %f, sinX = %f, cosY = %f\n", X+1, Y+1, A, sin(A), cos(A));
    for (int i=0; i<17; i++)
        printf("%s\n", map[i]);
}

void Contr()
{
	if (map[(int)(fPlayerY + sin(fPlayerA) * 1.0)][(int)(fPlayerX + cos(fPlayerA) * 1.0)] != '#')
    	if (GetKeyState('W') < 0)
    	{
			fPlayerY += sin(fPlayerA) * 1.0;
			fPlayerX += cos(fPlayerA) * 1.0;
		}
	if (map[(int)(fPlayerY - sin(fPlayerA) * 1.0)][(int)(fPlayerX - cos(fPlayerA) * 1.0)] != '#')
		if (GetKeyState('S') < 0)
		{
			fPlayerY -= sin(fPlayerA) * 1.0;
			fPlayerX -= cos(fPlayerA) * 1.0;
		}
	if (GetKeyState('A') < 0)
		fPlayerA -= (0.78539816);
	if (GetKeyState('D') < 0)
		fPlayerA += (0.78539816);
	if (abs((int)fPlayerA) == 6.0)
		fPlayerA = 0.0;
}

void Clr()
{
	COORD coord;
	coord.Y = 0;
	coord.X = 0;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void RayCst()
{
	for (int x = 1; x <= nScreenWidth; x++) // Проходим по всем X
	{
		printf("x = %d\n", x);
		fRayAngle = (fPlayerA - fFOV) + ((float)x / (float)nScreenWidth) * fFOV/1.4;  // Направление луча
		printf("fray=%f\n", fRayAngle);
    	// Находим расстояние до стенки в направлении fRayAngle 
		fDistanceToWall = 0.1; // Расстояние до препятствия в направлении fRayAngle
		bHitWall = 0; // Достигнул ли луч стенку
    	fEyeX = sin(fRayAngle); // Координаты единичного вектора fRayAngle
    	fEyeY = cos(fRayAngle);
    	printf("eye x=%f y=%f\n", fEyeX, fEyeY);
    	while (!bHitWall && fDistanceToWall < fDepth) // Пока не столкнулись со стеной
		{											  // Или не вышли за радиус видимости
			fDistanceToWall += 0.1;
    		nTestX = (int)(fPlayerX + (fEyeX*fDistanceToWall)); // Точка на игровом поле
			nTestY = (int)(fPlayerY + (fEyeY*fDistanceToWall)); // в которую попал луч
			//printf("d=%f x=%d y=%d\n", fDistanceToWall, nTestX, nTestY);
			if (nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight)
    	    { // Если мы вышли за зону
				bHitWall = 1;
    	    	fDistanceToWall = fDepth;
    	    }
			else if (map[nTestY][nTestX] == '#')
				bHitWall = 1;
 		}
 		
 		printf("x=%d y=%d\n", nTestX, nTestY);
		printf("d=%f\n", fDistanceToWall);
		nCeiling = (int)(nScreenHeight/2.0 * (1 - 1 / fDistanceToWall));
		nFloor = nScreenHeight - nCeiling;
		printf("%d %d\n", nCeiling, nFloor);
		int ti;
		for (int i = 0; i < nCeiling; i++)
			ti = i;
			sprintf(screen[ti], " ");
			
			//printf("%d\n", ti);
		for (int i = 40; i > nFloor; i--)
			ti = i;
			sprintf(screen[ti], " ");
			
			//printf("%d\n", ti);
    	for (int y = nCeiling; y <= nFloor; y++) // При заданном X проходим по всем Y
    	{ // В этом цикле рисуется вертикальная полоска
			screen[y][x] = '#';
			//printf("%d\n", y);
    	}
		//screen[nFloor][x] = '\0';
	}
}

void Play()
{
	for (int y = 0; y < nScreenHeight; y++)
		printf("%s\n", screen[y]);
}
int main()
{
	do
	{
		MiniMap(fPlayerY, fPlayerX, fPlayerA);
		RayCst();
		Play();
		Contr();
		Clr();
		Sleep(1000);
	}
	while(GetKeyState('E') == 0);
	system("cls");
	printf("Hello World");
	return 2730;
}