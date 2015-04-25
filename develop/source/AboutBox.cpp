
#include "framework.h"
#include "AboutBox.h"
#include "defs.h"

////////////////////////////////////////////////////////////////////////////////

void AboutBox::Go(HWND parent_)
{
	ASSURE(parent_ != NULL)
	parent = parent_;

	::DialogBoxParam(
		appInstance, MAKEINTRESOURCE(IDD_ABOUT), parent, DlgProc,
		(LPARAM) this);
}

////////////////////////////////////////////////////////////////////////////////

BOOL AboutBox::OnMsg(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_COMMAND:
		case WM_CTLCOLORSTATIC:
		{
			if ((HWND) lParam == webLink.hwnd)
				return webLink.Reflect(msg, wParam, lParam);
			if ((HWND) lParam == donateLink.hwnd)
				return donateLink.Reflect(msg, wParam, lParam);
			else if ((HWND) lParam == titleLabel.hwnd)
				return titleLabel.Reflect(msg, wParam, lParam);

			break;
		}
	}

	switch (msg)
	{
		case WM_INITDIALOG:
			OnInitDialog();
			return TRUE;

		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case IDOK:
				case IDCANCEL:
					::EndDialog(hwnd, 0);
					return TRUE;
			}
			break;
	}

	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////

void AboutBox::OnInitDialog()
{
	titleLabel.SetBold(true);
	titleLabel.Subclass(hwnd, IDC_ABOUT_TITLE);

	webLink.Subclass(hwnd, IDC_WEBSITE);
	webLink.SetURL(WEBSITE_URL);

	donateLink.Subclass(hwnd, IDC_DONATE);
	donateLink.SetURL(DONATE_URL);
}

////////////////////////////////////////////////////////////////////////////////
