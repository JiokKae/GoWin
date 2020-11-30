#pragma once
#include "framework.h"

class Image
{
public:
	enum IMAGE_LOAD_KIND
	{
		RESOURCE,
		FILE,
		EMPTY,
		END
	};

	typedef struct tagImageInfo
	{
		DWORD resID;				// ���ҽ��� ������ ���̵�
		HDC hMemDC;					// �׸��⸦ �ְ��ϴ� �ڵ� (�޸� ����)
		HBITMAP hBitmap;			// �̹��� ����
		HBITMAP hOldBit;			// ���� �̹��� ����
		int width;					// �̹����� ���� ũ��
		int height;					// �̹����� ���� ũ��
		BYTE loadType;				// �ε� Ÿ��

		// Blend
		HDC hBlendDC;
		HBITMAP hBlendBitmap;
		HBITMAP hOldBlendBit;

		// Animation�� �ʿ��� ����
		int maxFrameX;		// �ִ� ������ �ε���
		int maxFrameY;
		int currFrame;		// ���� ������ �ε���
		int frameWidth;		// ������ �� ũ��
		int frameHeight;

		// Animation ���� �� ��ġ
		float x, y;

		tagImageInfo()
		{
			resID = 0;
			hMemDC = NULL;
			hBitmap = NULL;
			hOldBit = NULL;
			width = 0;
			height = 0;
			loadType = IMAGE_LOAD_KIND::EMPTY;

			hBlendDC = NULL;
			hBlendBitmap = NULL;
			hOldBlendBit = NULL;

			maxFrameX = 0;
			maxFrameY = 0;
			currFrame = 0;
			frameWidth = 0;
			frameHeight = 0;

			x = y = 0.0f;
		};
	} IMAGE_INFO, * LPIMAGE_INFO;

private:
	IMAGE_INFO* imageInfo;			// �̹��� ���� ����ü ������
	//LPIMAGE_INFO imageInfo;

	char* fileName;					// ���� ��� + �̸�
	bool isTrans;					// ����ȭ ����
	COLORREF transColor;			// ����ȭ�� ��

	BLENDFUNCTION blendFunc;

public:
	// �� ��Ʈ�� �̹����� �����.
	HRESULT Init(int width, int height);

	// ���ҽ��κ��� �����͸� �о ��Ʈ���� �����.
	HRESULT Init(const DWORD resID, int width, int height, bool isTrans = false, COLORREF transColor = false);

	// ���Ϸκ��� �����͸� �о ��Ʈ���� �����.
	HRESULT Init(LPCTSTR fileName, int width, int height, bool isTrans = false, COLORREF transColor = false);

	// ���Ϸκ��� �����͸� �о ��Ʈ���� �����. (�ִϸ��̼�)
	HRESULT Init(LPCTSTR fileName, int width, int height, int maxFrameX, int maxFrameY, bool isTrans = false, COLORREF transColor = false);

	void Release();			// �޸� ����
	void Render(HDC hdc);
	void Render(HDC hdc, int destx, int destY, int currFrame = 0);
//	void AlphaRender(HDC hdc, BYTE alpha);
//	void AlphaRender(HDC hdc, int destX, int destY, BYTE alpha, int currFrame = 0);
	HDC GetMemDC() { return imageInfo->hMemDC; }
	int GetMaxFrame() { return imageInfo->maxFrameX * imageInfo->maxFrameY; }
	int GetHeight() { return imageInfo->height; }
	Image();
	~Image();
};

