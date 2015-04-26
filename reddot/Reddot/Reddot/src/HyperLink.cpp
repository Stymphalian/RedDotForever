
#include "framework.h"
#include "HyperLink.h"

#define LINK_COLOR  RGB(0x0D, 0x45, 0xAC)

////////////////////////////////////////////////////////////////////////////////

HyperLink::HyperLink()
{
	url = NULL;
	cursor = NULL;
	underlineFont = NULL;
}

////////////////////////////////////////////////////////////////////////////////

HyperLink::~HyperLink()
{
	::DeleteObject(underlineFont);

	if (cursor != NULL)
		::DestroyCursor(cursor);

	free(url);
}

////////////////////////////////////////////////////////////////////////////////

void HyperLink::SetURL(const char* url_)
{
	ASSURE(url_ != NULL)
	url = _strdup(url_);
	//url = strdup(url_);
}

////////////////////////////////////////////////////////////////////////////////

void HyperLink::OnSubclass()
{
	::SetWindowLong(
		hwnd, GWL_STYLE, ::GetWindowLong(hwnd, GWL_STYLE) | SS_NOTIFY);

	underlineFont = DeriveFont(NULL, false, true, -1);
	::SendMessage(hwnd, WM_SETFONT, (WPARAM) underlineFont, (LPARAM) 0);

	FitWindow();

	cursor = LoadHandCursor();
}

////////////////////////////////////////////////////////////////////////////////

LRESULT HyperLink::OnMsg(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_SETCURSOR:
			if (cursor != NULL)
			{
				::SetCursor(cursor);
				return TRUE;
			}
			break;
	}

	return CallOld(msg, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////////

LRESULT HyperLink::OnReflect(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_COMMAND:
			return OnReflectCommand(wParam, lParam);

		case WM_CTLCOLORSTATIC:
			return OnReflectCtlColor(wParam, lParam);
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////

LRESULT HyperLink::OnReflectCommand(WPARAM wParam, LPARAM lParam)
{
	if (HIWORD(wParam) == STN_CLICKED)
	{
		if (url != NULL) { OpenUrl(url); }
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////

LRESULT HyperLink::OnReflectCtlColor(WPARAM wParam, LPARAM lParam)
{
	HDC hdc = (HDC) wParam;

	::SetTextColor(hdc, LINK_COLOR);
	::SetBkMode(hdc, TRANSPARENT);

	return (LRESULT) ::GetStockObject(NULL_BRUSH);
}

////////////////////////////////////////////////////////////////////////////////

void HyperLink::FitWindow()
{
	
	//wchar_t* text = GetWndText(hwnd);
	char* text = GetWndText(hwnd);

	RECT rect;
	::GetWindowRect(hwnd, &rect);
	ScreenToClient(parent, &rect);

	HDC hdc = ::GetDC(hwnd);
	HGDIOBJ oldFont = ::SelectObject(hdc, underlineFont);

	SIZE extent;		
	//::GetTextExtentPoint32(hdc, text, wcslen(text), &extent);
	::GetTextExtentPoint32(hdc, text, strlen(text), &extent);

	::SelectObject(hdc, oldFont);
	::ReleaseDC(hwnd, hdc);

	free(text);

	int x = rect.left;
	int y = rect.top;
	int w = extent.cx;
	int h = extent.cy;

	DWORD style = ::GetWindowLong(hwnd, GWL_STYLE);

	if (style & SS_CENTER)  // remain centered
		x -= (w - (rect.right - rect.left)) / 2;
	else if (style & SS_RIGHT)
		x -= w - (rect.right - rect.left);

	::SetWindowPos(hwnd, NULL, x, y, w, h, SWP_NOZORDER | SWP_NOACTIVATE);
}

////////////////////////////////////////////////////////////////////////////////
