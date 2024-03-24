#include "Font.h"

GoWin::Font::Font(int height, int width, int escapement, int orientation, int weight, bool italic, bool underline, bool strike_out
	, DWORD char_set, DWORD out_precision, DWORD clip_precision, DWORD quality, DWORD pitch_and_family, const std::wstring& face_name)
{
	m_handle = CreateFont(height, width, escapement, orientation, weight, italic, underline, strike_out
		, char_set, out_precision, clip_precision, quality, pitch_and_family, face_name.c_str());
}

GoWin::Font::~Font()
{
	if (m_handle)
	{
		DeleteObject(m_handle);
	}
}

HFONT GoWin::Font::handle() const
{
	return m_handle;
}
