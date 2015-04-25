
#include "framework.h"
#include "Clock.h"
#include <stdio.h>

#define RECORDING_COLOR  RGB(48, 196,  48)
#define PLAYING_COLOR    RGB(96,  96,  96)
#define INACTIVE_COLOR   RGB(60,  60,  60)
#define BACK_COLOR       RGB(0,    0,   0)

////////////////////////////////////////////////////////////////////////////////

Clock::Clock()
{
	font = NULL;
	brush = ::CreateSolidBrush(BACK_COLOR);
	state = CLOCK_INACTIVE;
}

////////////////////////////////////////////////////////////////////////////////

Clock::~Clock()
{
	::DeleteObject(font);
	::DeleteObject(brush);
}

////////////////////////////////////////////////////////////////////////////////

void Clock::SetTime(uint32 millis)
{
	int secs = (millis / 1000) % 60;
	int mins = (millis / 60000) % 60;

	char buf[32];
	sprintf(buf, "%02d:%02d", mins, secs);
	::SetWindowText(hwnd, buf);
}

////////////////////////////////////////////////////////////////////////////////

void Clock::Blink()
{
	blink = !blink;

	if (blink)
		::SetWindowText(hwnd, "---");
	else
		::SetWindowText(hwnd, "");
}

////////////////////////////////////////////////////////////////////////////////

void Clock::SetState(int state_)
{
	state = state_;

	switch (state)
	{
		case CLOCK_INACTIVE:
			::SetWindowText(hwnd, "---");
			break;

		case CLOCK_WAITING:
			::SetWindowText(hwnd, "");
			blink = false;
			break;

		case CLOCK_RECORDING:
		case CLOCK_PLAYING:
			SetTime(0);
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////

void Clock::OnSubclass()
{
	::SetWindowLong(hwnd, GWL_STYLE, ::GetWindowLong(hwnd, GWL_STYLE) | SS_NOTIFY);

	font = DeriveFont(NULL, true, false, 24);
	::SendMessage(hwnd, WM_SETFONT, (WPARAM) font, (LPARAM) 0);
}

////////////////////////////////////////////////////////////////////////////////

LRESULT Clock::OnReflect(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_CTLCOLORSTATIC:
		{
			HDC hdc = (HDC) wParam;

			if (state == CLOCK_INACTIVE)
				::SetTextColor(hdc, INACTIVE_COLOR);
			else if (state == CLOCK_PLAYING)
				::SetTextColor(hdc, PLAYING_COLOR);
			else
				::SetTextColor(hdc, RECORDING_COLOR);

			::SetBkMode(hdc, TRANSPARENT);

			return (LRESULT) brush;
		}
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
