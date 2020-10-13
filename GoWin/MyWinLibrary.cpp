#include "MyWinLibrary.h"
#pragma warning (disable : 4996)

char * Read(UINT message, char * buffer)
{
	switch (message)
	{
	case WM_CREATE:
		strcpy(buffer, "WM_CREATE"); break;
	case WM_MOUSEMOVE:
		strcpy(buffer, "WM_MOUSEMOVE"); break;
	case WM_COMMAND:
		strcpy(buffer, "WM_COMMAND"); break;
	case WM_LBUTTONDOWN:
		strcpy(buffer, "WM_LBUTTONDOWN"); break;
	case WM_PAINT:
		strcpy(buffer, "WM_PAINT"); break;
	case WM_SETCURSOR:
		strcpy(buffer, "WM_SETCURSOR"); break;
	case WM_KEYDOWN:
		strcpy(buffer, "WM_KEYDOWN"); break;
	default:
		itoa(message, buffer, 10);
	}
	return buffer;
}

void GetMouseCoord(Coord2d & coord, LPARAM lParam)
{
	coord.x = LOWORD(lParam);
	coord.y = HIWORD(lParam);
}
