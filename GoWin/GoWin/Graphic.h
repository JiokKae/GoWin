#pragma once

#include <wtypes.h>

namespace GoWin
{
	class Graphic
	{
	public:
		class Bitmap
		{
		public:
			Bitmap();
			Bitmap(HINSTANCE hInst, int resource_id);
			Bitmap(const Bitmap& bitmap) = delete;
			Bitmap(Bitmap&& bitmap) noexcept;
			~Bitmap();

			HANDLE handle() const;
			LONG width() const;
			LONG height() const;

		private:
			void SetBitmapInfo();

			HANDLE m_handle = nullptr;
			LONG m_width = 0;
			LONG m_height = 0;
		};
	};
}
