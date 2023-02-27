#include "MyWinLibrary.h"

#define MSG_TO_STR(x) case x: return #x
std::string ReadMessage(UINT message)
{
	switch (message)
	{
	MSG_TO_STR(WM_CREATE);
	MSG_TO_STR(WM_MOUSEMOVE);
	MSG_TO_STR(WM_COMMAND);
	MSG_TO_STR(WM_LBUTTONDOWN);
	MSG_TO_STR(WM_PAINT);
	MSG_TO_STR(WM_SETCURSOR);
	MSG_TO_STR(WM_KEYDOWN);
	default:
		return std::to_string(message);
	}	
}
