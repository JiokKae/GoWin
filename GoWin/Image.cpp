#include "Image.h"

HRESULT Image::Init(int width, int height)
{
	// �׸��� ��� ���� ������
	HDC hdc = GetDC(g_hWnd);

	imageInfo = new IMAGE_INFO;
	imageInfo->resID = 0;

	// ���� �̹����� ������
	imageInfo->hMemDC = CreateCompatibleDC(hdc);

	// ���� �̹����� ������ ���� ���� ����
	imageInfo->hBitmap = CreateCompatibleBitmap(hdc, width, height);

	// ���� �̹����� �����ڿ���, ���� �̹����� ������ ���� ������ �����ϵ��� ���� -> �����ڴ� �� ���� �����ϴ� ������ ��ȯ
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

	blendFunc.AlphaFormat = 0; // AC_SRC_ALPHA (������ RGBA�� ��)
	blendFunc.BlendFlags = 0;
	blendFunc.BlendOp = AC_SRC_OVER;
	blendFunc.SourceConstantAlpha = 0;

	ReleaseDC(g_hWnd, hdc);
	return S_OK;
}

HRESULT Image::Init(const DWORD resID, int width, int height, bool isTrans, COLORREF transColor)
{
	// �׸��� ��� ���� ������
	HDC hdc = GetDC(g_hWnd);

	imageInfo = new IMAGE_INFO;
	imageInfo->resID = 0;

	// ���� �̹����� ������
	imageInfo->hMemDC = CreateCompatibleDC(hdc);

	// ���� �̸��� ���� �̹��� �ҷ�����
	imageInfo->hBitmap = (HBITMAP)LoadBitmap(g_hInstance, MAKEINTRESOURCE(resID));

	// ���� �̹����� �����ڿ���, ���� �̹����� ������ ���� ������ �����ϵ��� ���� -> �����ڴ� �� ���� �����ϴ� ������ ��ȯ
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

	blendFunc.AlphaFormat = 0; // AC_SRC_ALPHA (������ RGBA�� ��)
	blendFunc.BlendFlags = 0;
	blendFunc.BlendOp = AC_SRC_OVER;
	blendFunc.SourceConstantAlpha = 0;

	ReleaseDC(g_hWnd, hdc);
	return S_OK;
}

HRESULT Image::Init(LPCTSTR fileName, int width, int height, bool isTrans, COLORREF transColor)
{
	// �׸��� ��� ���� ������
	HDC hdc = GetDC(g_hWnd);

	imageInfo = new IMAGE_INFO;
	imageInfo->resID = 0;

	// ���� �̹����� ������
	imageInfo->hMemDC = CreateCompatibleDC(hdc);

	// ���� �̸��� ���� �̹��� �ҷ�����
	imageInfo->hBitmap = (HBITMAP)LoadImage(g_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);

	// ���� �̹����� �����ڿ���, ���� �̹����� ������ ���� ������ �����ϵ��� ���� -> �����ڴ� �� ���� �����ϴ� ������ ��ȯ
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

	blendFunc.AlphaFormat = 0; // AC_SRC_ALPHA (������ RGBA�� ��)
	blendFunc.BlendFlags = 0;
	blendFunc.BlendOp = AC_SRC_OVER;
	blendFunc.SourceConstantAlpha = 0;

	ReleaseDC(g_hWnd, hdc);
	return S_OK;
}

HRESULT Image::Init(LPCTSTR fileName, int width, int height, int maxFrameX, int maxFrameY, bool isTrans, COLORREF transColor)
{
	// �׸��� ��� ���� ������
	HDC hdc = GetDC(g_hWnd);

	imageInfo = new IMAGE_INFO;
	imageInfo->resID = 0;

	// ���� �̹����� ������
	imageInfo->hMemDC = CreateCompatibleDC(hdc);

	// ���� �̸��� ���� �̹��� �ҷ�����
	imageInfo->hBitmap = (HBITMAP)LoadImage(g_hInstance, fileName, IMAGE_BITMAP, width, height, LR_LOADFROMFILE);

	// ���� �̹����� �����ڿ���, ���� �̹����� ������ ���� ������ �����ϵ��� ���� -> �����ڴ� �� ���� �����ϴ� ������ ��ȯ
	imageInfo->hOldBit = (HBITMAP)SelectObject(imageInfo->hMemDC, imageInfo->hBitmap);

	imageInfo->width = width;
	imageInfo->height = height;
	imageInfo->loadType = IMAGE_LOAD_KIND::FILE;

	// �ִϸ��̼� ����
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

	blendFunc.AlphaFormat = 0; // AC_SRC_ALPHA (������ RGBA�� ��)
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
		SelectObject(imageInfo->hBlendDC, imageInfo->hOldBlendBit);	// ��ȯ���� hBitmap�� �ȴ�.
		DeleteObject(imageInfo->hBlendBitmap);
		//DeleteObject(imageInfo->hOldBit);
		DeleteDC(imageInfo->hBlendDC);

		SelectObject(imageInfo->hMemDC, imageInfo->hOldBit);	// ��ȯ���� hBitmap�� �ȴ�.
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

	if (isTrans)	// ����O
	{
		GdiTransparentBlt(hdc,									// ���� ������ DC
			(int)imageInfo->x, (int)imageInfo->y,							// ���� ���� ��ġ
			imageInfo->frameWidth,								// �������� ����� ������� ���� ũ��
			imageInfo->frameHeight,								// �������� ����� ������� ���� ũ��
			imageInfo->hMemDC,									// ���� DC
			(imageInfo->currFrame % imageInfo->maxFrameX) * imageInfo->frameWidth,		// �������� ���� ���� ��ġ
			(imageInfo->currFrame / imageInfo->maxFrameX) * imageInfo->frameHeight,
			imageInfo->frameWidth, imageInfo->frameHeight,		// ������ ���� ���������κ����� ũ��
			transColor											// ���� �ɼ�
		);
	}
	else			// ����X
	{
		BitBlt(hdc,																		// ���� ������ DC
			imageInfo->x, imageInfo->y,													// ���� ���� ��ġ
			imageInfo->frameWidth,														// �������� ����� ������� ���� ũ��
			imageInfo->frameHeight,														// �������� ����� ������� ���� ũ��
			imageInfo->hMemDC,															// ���� DC
			(imageInfo->currFrame % imageInfo->maxFrameX) * imageInfo->frameWidth,		// �������� ���� ���� ��ġ
			(imageInfo->currFrame / imageInfo->maxFrameX) * imageInfo->frameHeight,
			SRCCOPY																		// ���� �ɼ�
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
		// ���� �۾� : blendDC, blendBitmap�� �غ�

		// 1. ������ DC�� �׷��� �ִ� ������ blendDC�� ����
		BitBlt(imageInfo->hBlendDC, 0, 0,
			imageInfo->frameWidth, imageInfo->frameHeight,
			hdc, (int)imageInfo->x, (int)imageInfo->y,
			SRCCOPY);
		// 2. ����� �̹���DC�� blendDC�� ������ ������ �����ϸ鼭 ����
		GdiTransparentBlt(imageInfo->hBlendDC, 0, 0,
			imageInfo->frameWidth, imageInfo->frameHeight,
			imageInfo->hMemDC,
			(imageInfo->currFrame % imageInfo->maxFrameX) * imageInfo->frameWidth,		// �������� ���� ���� ��ġ
			(imageInfo->currFrame / imageInfo->maxFrameX) * imageInfo->frameHeight,
			imageInfo->frameWidth, imageInfo->frameHeight,
			transColor);
		// 3. blendDC�� ������ ������ DC�� ���� ���� �Լ��� ����
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
