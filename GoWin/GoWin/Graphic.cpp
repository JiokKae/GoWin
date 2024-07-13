#include "Graphic.h"

#include <utility>

GoWin::Graphic::Bitmap::Bitmap(HINSTANCE hInst, int resource_id)
	: m_handle{ ::LoadImage(hInst, MAKEINTRESOURCE(resource_id), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR) }
{
	SetBitmapInfo();
}

GoWin::Graphic::Bitmap::Bitmap()
{
}

GoWin::Graphic::Bitmap::Bitmap(Bitmap&& bitmap) noexcept
	: m_handle{ std::exchange(bitmap.m_handle, nullptr) }
	, m_width{ bitmap.m_width }
	, m_height{ bitmap.m_height }
{
}

GoWin::Graphic::Bitmap::~Bitmap()
{
	if (m_handle)
	{
		::DeleteObject(m_handle);
	}
}

HANDLE GoWin::Graphic::Bitmap::handle() const
{
	return m_handle;
}

LONG GoWin::Graphic::Bitmap::width() const
{
	return m_width;
}

LONG GoWin::Graphic::Bitmap::height() const
{
	return m_height;
}

void GoWin::Graphic::Bitmap::SetBitmapInfo()
{
	BITMAP bitmap_info{};
	if (::GetObject(m_handle, sizeof(BITMAP), (LPVOID)&bitmap_info))
	{
		m_width = bitmap_info.bmWidth;
		m_height = bitmap_info.bmHeight;
	}
}
