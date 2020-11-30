#pragma once
#include "framework.h"
#include "Singleton.h"
#include <map>

class Image;
class ImageManager : public Singleton<ImageManager>
{
private:
	map<string, Image*> mapImageDatas;
	map<string, Image*>::iterator itImageDatas;

public:
	HRESULT Init();
	void Release();

	void AddImage(string strKey, LPCWSTR fileName, int width, int height, bool isTrans = false, COLORREF transColor = false);
	void AddImage(string strKey, const DWORD resID, int width, int height, bool isTrans = false, COLORREF transColor = false);
	void AddFrameImage(string strKey, LPCWSTR fileName, int width, int height, int maxFrameX, int maxFrameY, bool isTrans = false, COLORREF transColor = false);
	void DeleteImage(string strKey);
	Image* FindImage(string strKey);
};

