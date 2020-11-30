#include "ImageManager.h"

HRESULT ImageManager::Init()
{
	return S_OK;
}

void ImageManager::Release()
{
	for (itImageDatas = mapImageDatas.begin(); itImageDatas != mapImageDatas.end(); itImageDatas++)
	{
		if (itImageDatas->second != nullptr)
		{
			(itImageDatas->second)->Release();
			delete (itImageDatas->second);
		}

	}
	mapImageDatas.clear();
	ReleaseSingleton();
}

void ImageManager::AddImage(string strKey, LPCWSTR fileName, int width, int height, bool isTrans, COLORREF transColor)
{
	// 맵에 키에 해당하는 데이터가 있는지 확인해서
	// 있으면 추가 없이 리턴
	if (FindImage(strKey))
	{
		return;
	}

	// 없으면 추가
	Image* image = new Image();
	if (FAILED(image->Init(fileName, width, height, isTrans, transColor)))
	{
		MessageBox(g_hWnd, _T("이미지 로드 실패"), _T("실패"), MB_OK);

		image->Release();
		delete image;

		return;
	}

	mapImageDatas.insert(make_pair(strKey, image));
}

void ImageManager::AddImage(string strKey, const DWORD resID, int width, int height, bool isTrans, COLORREF transColor)
{
	// 맵에 키에 해당하는 데이터가 있는지 확인해서
// 있으면 추가 없이 리턴
	if (FindImage(strKey))
	{
		return;
	}

	// 없으면 추가
	Image* image = new Image();
	if (FAILED(image->Init(resID, width, height, isTrans, transColor)))
	{
		MessageBox(g_hWnd, _T("이미지 로드 실패"), _T("실패"), MB_OK);

		image->Release();
		delete image;

		return;
	}

	mapImageDatas.insert(make_pair(strKey, image));
}

void ImageManager::AddFrameImage(string strKey, LPCWSTR fileName, int width, int height, int maxFrameX, int maxFrameY, bool isTrans, COLORREF transColor)
{
	// 맵에 키에 해당하는 데이터가 있는지 확인해서
	// 있으면 추가 없이 리턴
	if (FindImage(strKey))
	{
		return;
	}

	// 없으면 추가
	Image* image = new Image();
	if (FAILED(image->Init(fileName, width, height, maxFrameX, maxFrameY, isTrans, transColor)))
	{
		MessageBox(g_hWnd, _T("이미지 로드 실패"), _T("실패"), MB_OK);

		image->Release();
		delete image;

		return;
	}

	mapImageDatas.insert(make_pair(strKey, image));
}

void ImageManager::DeleteImage(string strKey)
{
	itImageDatas = mapImageDatas.find(strKey);
	if (itImageDatas != mapImageDatas.end())
	{
		(itImageDatas->second)->Release();
		delete itImageDatas->second;

		mapImageDatas.erase(itImageDatas);
	}
}

Image* ImageManager::FindImage(string strKey)
{
	itImageDatas = mapImageDatas.find(strKey);
	if (itImageDatas != mapImageDatas.end())
	{
		return itImageDatas->second;
	}

	return nullptr;
}
