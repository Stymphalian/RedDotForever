
#include "framework.h"
#include "Label.h"

////////////////////////////////////////////////////////////////////////////////

Label::Label()
{
	font = NULL;
	bold = false;
}

////////////////////////////////////////////////////////////////////////////////

Label::~Label()
{
	if (font != NULL)
		::DeleteObject(font);
}

////////////////////////////////////////////////////////////////////////////////

void Label::SetBold(bool bold_)
{
	bold = bold_;
}

////////////////////////////////////////////////////////////////////////////////

void Label::OnSubclass()
{
	::SetWindowLong(
		hwnd, GWL_STYLE, ::GetWindowLong(hwnd, GWL_STYLE) | SS_NOTIFY);

	if (bold)
	{
		font = DeriveFont(NULL, true, false, 10);
		::SendMessage(hwnd, WM_SETFONT, (WPARAM) font, (LPARAM) 0);
	}
}

////////////////////////////////////////////////////////////////////////////////

LRESULT Label::OnReflect(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_CTLCOLORSTATIC:
		{
			HDC hdc = (HDC) wParam;

			//::SetTextColor(hdc, RGB(255, 0, 0));
			::SetBkMode(hdc, TRANSPARENT);

			return (LRESULT) ::GetStockObject(NULL_BRUSH);
		}
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
