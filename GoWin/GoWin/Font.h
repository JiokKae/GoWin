#pragma once

#include <string>
#include <windows.h>

namespace GoWin {
	class Font
	{
	public:
		Font(int height, int width, int escapement, int orientation, int weight, bool italic, bool underline, bool strike_out
			, DWORD char_set, DWORD out_precision, DWORD clip_precision, DWORD quality, DWORD pitch_and_family, const std::wstring& face_name);
		virtual ~Font();

		HFONT handle() const;
	private:
		HFONT m_handle = nullptr;
	};
}
