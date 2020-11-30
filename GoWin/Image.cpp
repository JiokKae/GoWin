#include "Image.h"

HRESULT Image::Init(int width, int height)
{
	// 그리기 담당 상위 관리자
	HDC hdc = GetDC(g_hWnd);

	imageInfo = new IMAGE_INFO;
	imageInfo->resID = 0;

	// 현재 이미지의 관리자
	imageInfo->hMemDC = CreateCompatibleDC(hdc);

	// 현재 이미지의 정보를 담을 공간 생성
	imageInfo->hBitmap = CreateCompatibleBitmap(hdc, width, height);

	// 현재 이미지의 관리자에게, 현재 이미지의 정보를 담을 공간을 관리하도록 설정 -> 관리자는 그 전에 관리하던 공간을 반환
	imageInfo->hOldBit = (HBITMAP)SelectObject(imageInfo->hMemDC, imageInfo->hBitmap);

	imageInfo->frameWidth = imageInfo->width = width;
	imageInfo->frameHeight = imageInfo->height = height;

	imageInfo->loadType = IMAGE_LOAD_KIND::EMPTY;

	if (imageInfo->hBitmap == NULL)
	{
		Release();
		return E_FAIL;
	}
	imageInfo->hBlendDC = CreateCompatibleDC(hdc);
	imageInfo->hBlendBitmap = CreateCompatibleBitmap(hdc, width, height);
	imageInfo->hOldBlendBit = (HBITMAP)SelectObject(imageInfo->hBlendDC, imageInfo->hBlendBitmap);

	blendFunc.AlphaFormat = 0; // AC_SRC_ALPHA (원본이 RGBA일 때)
	blendFunc.BlendFlags = 0;
	blendFunc.BlendOp = AC_SRC_OVER;
	blendFunc.SourceConstantAlpha = 0;

	ReleaseDC(g_hWnd, hdc);
	return S_OK;
}

HRESULT Image::Init(const DWORD resID, int width, int height, bool isTrans, COLORREF transColor)
{
	// 그리기 담당 상위 관리자
	HDC hdc = GetDC(g_hWnd);

	imageInfo = new IMAGE_INFO;
	imageInfo->resID = 0;

	// 현재 이미지의 관리자
	imageInfo->hMemDC = CreateCompatibleDC(hdc);

	// 파일 이름을 통해 이미지 불러오기
	imageInfo->hBitmap = (HBITMAP)LoadBitmap(g_hInstance, MAKEINTRESOURCE(resID));

	// 현재 이미지의 관리자에게, 현재 이미지의 정보를 담을 공간을 관리하도록 설정 -> 관리자는 그 전에 관리하던 공간을 반환
	imageInfo->hOldBit = (HBITMAP)SelectObject(imageInfo->hMemDC, imageInfo->hBitmap);

	imageInfo->maxFrameX = 1;
	imageInfo->maxFrameY = 1;
	imageInfo->frameWidth = imageInfo->width = width;
	imageInfo->frameHeight = imageInfo->height = height;
	imageInfo->loadType = IMAGE_LOAD_KIND::FILE;

	this->isTrans = isTrans;
	this->transColor = transColor;

	if (imageInfo->hBitmap == NULL)
	{
		Release();
		return E_FAIL;
	}
	imageInfo->hBlendDC = CreateCompatibleDC(hdc);
	imageInfo->hBlendBitmap = CreateCompatibleBitmap(hdc, width, height);
	imageInfo->hOldBlendBit = (HBITMAP)SelectObject(imageInfo->hBlendDC, imageInfo->hBlendBitmap);

	blendFunc.AlphaFormat = 0; // AC_SRC_ALPHA (원본이 RGBA일 때)
	blendFunc.BlendFlags = 0;
	blendFunc.BlendOp = AC_SRC_OVER;
	blendFunc.SourceConstantAlpha = 0;

	ReleaseDC(g_hWnd, hdc);
	return S_OK;
}

HRESULT Image::Init(LPCTSTR fileName, int width, int height, bool isTrans, COLORREF transColor)
{
	// 그리기 담당 상위 관리자
	HDC hdc = GetDC(g_hWnd);

	imageInfo = new IMAGE_INFO;
	imageInfo->resID = 0;

	// 현재 이미지의 관리자
	imageInfo->hMemDC = CreateCompatibleDC(hdc);

	// 파일 이름을 통해 이미지 불러오기
	imageInfo->hBitmap = (HBITMAP)LoadImage(g_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);

	// 현재 이미지의 관리자에게, 현재 이미지의 정보를 담을 공간을 관리하도록 설정 -> 관리자는 그 전에 관리하던 공간을 반환
	imageInfo->hOldBit = (HBITMAP)SelectObject(imageInfo->hMemDC, imageInfo->hBitmap);

	imageInfo->maxFrameX = 1;
	imageInfo->maxFrameY = 1;
	imageInfo->frameWidth = imageInfo->width = width;
	imageInfo->frameHeight = imageInfo->height = height;
	imageInfo->loadType = IMAGE_LOAD_KIND::FILE;

	this->isTrans = isTrans;
	this->transColor = transColor;

	if (imageInfo->hBitmap == NULL)
	{
		Release();
		return E_FAIL;
	}
	imageInfo->hBlendDC = CreateCompatibleDC(hdc);
	imageInfo->hBlendBitmap = CreateCompatibleBitmap(hdc, width, height);
	imageInfo->hOldBlendBit = (HBITMAP)SelectObject(imageInfo->hBlendDC, imageInfo->hBlendBitmap);

	blendFunc.AlphaFormat = 0; // AC_SRC_ALPHA (원본이 RGBA일 때)
	blendFunc.BlendFlags = 0;
	blendFunc.BlendOp = AC_SRC_OVER;
	blendFunc.SourceConstantAlpha = 0;

	ReleaseDC(g_hWnd, hdc);
	return S_OK;
}

HRESULT Image::Init(LPCTSTR fileName, int width, int height, int maxFrameX, int maxFrameY, bool isTrans, COLORREF transColor)
{
	// 그리기 담당 상위 관리자
	HDC hdc = GetDC(g_hWnd);

	imageInfo = new IMAGE_INFO;
	imageInfo->resID = 0;

	// 현재 이미지의 관리자
	imageInfo->hMemDC = CreateCompatibleDC(hdc);

	// 파일 이름을 통해 이미지 불러오기
	imageInfo->hBitmap = (HBITMAP)LoadImage(g_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);

	// 현재 이미지의 관리자에게, 현재 이미지의 정보를 담을 공간을 관리하도록 설정 -> 관리자는 그 전에 관리하던 공간을 반환
	imageInfo->hOldBit = (HBITMAP)SelectObject(imageInfo->hMemDC, imageInfo->hBitmap);

	imageInfo->width = width;
	imageInfo->height = height;
	imageInfo->loadType = IMAGE_LOAD_KIND::FILE;

	// 애니메이션 관련
	imageInfo->currFrame = 0;
	imageInfo->maxFrameX = maxFrameX;
	imageInfo->maxFrameY = maxFrameY;
	imageInfo->frameWidth = imageInfo->width / maxFrameX;
	imageInfo->frameHeight = imageInfo->height / maxFrameY;

	this->isTrans = isTrans;
	this->transColor = transColor;

	if (imageInfo->hBitmap == NULL)
	{
		Release();
		return E_FAIL;
	}
	imageInfo->hBlendDC = CreateCompatibleDC(hdc);
	imageInfo->hBlendBitmap = CreateCompatibleBitmap(hdc, width, height);
	imageInfo->hOldBlendBit = (HBITMAP)SelectObject(imageInfo->hBlendDC, imageInfo->hBlendBitmap);

	blendFunc.AlphaFormat = 0; // AC_SRC_ALPHA (원본이 RGBA일 때)
	blendFunc.BlendFlags = 0;
	blendFunc.BlendOp = AC_SRC_OVER;
	blendFunc.SourceConstantAlpha = 0;

	ReleaseDC(g_hWnd, hdc);
	return S_OK;
}

void Image::Release()
{
	if (imageInfo)
	{
		SelectObject(imageInfo->hBlendDC, imageInfo->hOldBlendBit);	// 반환값이 hBitmap이 된다.
		DeleteObject(imageInfo->hBlendBitmap);
		//DeleteObject(imageInfo->hOldBit);
		DeleteDC(imageInfo->hBlendDC);

		SelectObject(imageInfo->hMemDC, imageInfo->hOldBit);	// 반환값이 hBitmap이 된다.
		DeleteObject(imageInfo->hBitmap);
		//DeleteObject(imageInfo->hOldBit);
		DeleteDC(imageInfo->hMemDC);

		delete imageInfo;
		imageInfo = nullptr;
	}
}

void Image::Render(HDC hdc)
{
	BitBlt(hdc, 0, 0,
		imageInfo->frameWidth,
		imageInfo->frameHeight,
		imageInfo->hMemDC, 0, 0,
		SRCCOPY
	);
}


void Image::Render(HDC hdc, int destX, int destY, int currFrame)
{
	imageInfo->currFrame = currFrame;

	imageInfo->x = destX;
	imageInfo->y = destY;

	if (isTrans)	// 투명O
	{
		GdiTransparentBlt(hdc,									// 복사 목적지 DC
			(int)imageInfo->x, (int)imageInfo->y,							// 복사 시작 위치
			imageInfo->frameWidth,								// 원본에서 복사된 결과물의 가로 크기
			imageInfo->frameHeight,								// 원본에서 복사된 결과물의 세로 크기
			imageInfo->hMemDC,									// 원본 DC
			(imageInfo->currFrame % imageInfo->maxFrameX) * imageInfo->frameWidth,		// 원본에서 복사 시작 위치
			(imageInfo->currFrame / imageInfo->maxFrameX) * imageInfo->frameHeight,
			imageInfo->frameWidth, imageInfo->frameHeight,		// 원본의 복사 시작점으로부터의 크기
			transColor											// 복사 옵션
		);
	}
	else			// 투명X
	{
		BitBlt(hdc,																		// 복사 목적지 DC
			imageInfo->x, imageInfo->y,													// 복사 시작 위치
			imageInfo->frameWidth,														// 원본에서 복사된 결과물의 가로 크기
			imageInfo->frameHeight,														// 원본에서 복사된 결과물의 세로 크기
			imageInfo->hMemDC,															// 원본 DC
			(imageInfo->currFrame % imageInfo->maxFrameX) * imageInfo->frameWidth,		// 원본에서 복사 시작 위치
			(imageInfo->currFrame / imageInfo->maxFrameX) * imageInfo->frameHeight,
			SRCCOPY																		// 복사 옵션
		);
	}
}
/*
void Image::AlphaRender(HDC hdc, BYTE alpha)
{
	blendFunc.SourceConstantAlpha = alpha;
	AlphaBlend(hdc, imageInfo->x, imageInfo->y,
		imageInfo->frameWidth, imageInfo->frameHeight,
		imageInfo->hMemDC, 0, 0,
		imageInfo->frameWidth, imageInfo->frameHeight,
		blendFunc);
}

void Image::AlphaRender(HDC hdc, int destX, int destY, BYTE alpha, int currFrame)
{
	imageInfo->currFrame = currFrame;

	imageInfo->x = destX - (imageInfo->frameWidth / 2);
	imageInfo->y = destY - (imageInfo->frameHeight / 2);

	blendFunc.SourceConstantAlpha = alpha;

	if (isTrans)
	{
		// 선행 작업 : blendDC, blendBitmap을 준비

		// 1. 목적지 DC에 그려져 있는 내용을 blendDC에 복사
		BitBlt(imageInfo->hBlendDC, 0, 0,
			imageInfo->frameWidth, imageInfo->frameHeight,
			hdc, (int)imageInfo->x, (int)imageInfo->y,
			SRCCOPY);
		// 2. 출력할 이미지DC를 blendDC에 지정한 색상을 제외하면서 복사
		GdiTransparentBlt(imageInfo->hBlendDC, 0, 0,
			imageInfo->frameWidth, imageInfo->frameHeight,
			imageInfo->hMemDC,
			(imageInfo->currFrame % imageInfo->maxFrameX) * imageInfo->frameWidth,		// 원본에서 복사 시작 위치
			(imageInfo->currFrame / imageInfo->maxFrameX) * imageInfo->frameHeight,
			imageInfo->frameWidth, imageInfo->frameHeight,
			transColor);
		// 3. blendDC의 내용을 목적지 DC에 알파 블렌드 함수로 복사
		AlphaBlend(hdc, imageInfo->x, imageInfo->y,
			imageInfo->frameWidth, imageInfo->frameHeight,
			imageInfo->hBlendDC, 0, 0,
			imageInfo->frameWidth, imageInfo->frameHeight,
			blendFunc);
	}
	else
	{
		AlphaBlend(hdc, imageInfo->x, imageInfo->y,
			imageInfo->frameWidth, imageInfo->frameHeight,
			imageInfo->hMemDC, 0, 0,
			imageInfo->frameWidth, imageInfo->frameHeight,
			blendFunc);
	}
}
*/
Image::Image()
{
}

Image::~Image()
{
}
