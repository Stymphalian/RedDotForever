
#include <direct.h>
#include <ole2.h>
#include <tchar.h>

#include "framework.h"

// extern defined
HINSTANCE appInstance;

////////////////////////////////////////////////////////////////////////////////

WndBase::WndBase()
{
	hwnd = NULL;
	parent = NULL;
	id = 0;
}

////////////////////////////////////////////////////////////////////////////////

WndBase::~WndBase()
{
	// do nothing
}

////////////////////////////////////////////////////////////////////////////////

void WndBase::Invalidate(bool repaint)
{
	// NULL means redraw the entire client area
	::InvalidateRect(hwnd, NULL, repaint);
}

////////////////////////////////////////////////////////////////////////////////

Window::Window()
{
	wndclass.cbSize         = sizeof(WNDCLASSEX);
	wndclass.style          = 0;
	wndclass.lpfnWndProc    = WndProc;
	wndclass.cbClsExtra     = 0;
	wndclass.cbWndExtra     = 0;
	wndclass.hInstance      = appInstance;
	wndclass.hIcon          = NULL;
	wndclass.hCursor        = ::LoadCursor(NULL, IDC_ARROW); // use the default cursor
	wndclass.hbrBackground  = (HBRUSH) (COLOR_WINDOW + 1);
	wndclass.lpszMenuName   = NULL;
	wndclass.lpszClassName  = NULL;
	wndclass.hIconSm        = NULL;
}

////////////////////////////////////////////////////////////////////////////////

bool Window::RegisterClass()
{
	WNDCLASSEX tmpclass;
	if (!::GetClassInfoEx(appInstance, wndclass.lpszClassName, &tmpclass))
	{
		if (!::RegisterClassEx(&wndclass))
			return false;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK Window::WndProc(
	HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Window* ptr = NULL;

	if (msg == WM_NCCREATE)
	{
		ptr = (Window*) ((LPCREATESTRUCT) lParam)->lpCreateParams;
		ASSURE(ptr != NULL)

		::SetWindowLong(hwnd, GWL_USERDATA, (LONG) ptr);
		ptr->hwnd = hwnd;
	}
	else
	{
		ptr = (Window*) ::GetWindowLong(hwnd, GWL_USERDATA);
	}

	if (ptr != NULL)
		return ptr->OnMsg(msg, wParam, lParam);
	else
		return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////////

LRESULT Window::CallDefault(UINT msg, WPARAM wParam, LPARAM lParam)
{
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////////

LRESULT Window::Reflect(UINT msg, WPARAM wParam, LPARAM lParam)
{
	return OnReflect(msg, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////////

LRESULT Window::OnReflect(UINT msg, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

////////////////////////////////////////////////////////////////////////////////

BOOL CALLBACK Dialog::DlgProc(
	HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Dialog* ptr = NULL;

	if (msg == WM_INITDIALOG)
	{
		ptr = (Dialog*) lParam;
		ASSURE(ptr != NULL)

		::SetWindowLong(hwnd, GWL_USERDATA, lParam);
		ptr->hwnd = hwnd;
	}
	else
	{
		// retrieve the lParam value which was originally pased
		// in on the initial creation of the dialog...?
		ptr = (Dialog*) ::GetWindowLong(hwnd, GWL_USERDATA);
	}

	if (ptr != NULL)
		return ptr->OnMsg(msg, wParam, lParam);
	else
		return FALSE;
}

////////////////////////////////////////////////////////////////////////////////

BOOL Dialog::Reflect(UINT msg, WPARAM wParam, LPARAM lParam)
{
	return OnReflect(msg, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////////

BOOL Dialog::OnReflect(UINT msg, WPARAM wParam, LPARAM lParam)
{
	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////

void Dialog::Enable(UINT id, bool flag)
{
	::EnableWindow(::GetDlgItem(hwnd, id), flag);
}

////////////////////////////////////////////////////////////////////////////////

void Dialog::Check(UINT id, bool flag)
{
	::CheckDlgButton(hwnd, id, flag ? BST_CHECKED : BST_UNCHECKED);
}

////////////////////////////////////////////////////////////////////////////////

bool Dialog::IsChecked(UINT id)
{
	return ::IsDlgButtonChecked(hwnd, id) == TRUE;
}

////////////////////////////////////////////////////////////////////////////////

void Subclassed::Subclass()
{
	Subclass(WndProc);
}

////////////////////////////////////////////////////////////////////////////////

void Subclassed::Subclass(WNDPROC wndProc)
{
	::SetWindowLong(hwnd, GWL_USERDATA, (LONG) this);

	oldProc = (WNDPROC) ::SetWindowLong(hwnd, GWL_WNDPROC, (LONG) wndProc);

	OnSubclass();
}

////////////////////////////////////////////////////////////////////////////////

void Subclassed::Subclass(HWND parent_, UINT id_)
{
	ASSURE(parent_ != NULL)

	parent = parent_;
	id = id_;

	hwnd = ::GetDlgItem(parent, id);
	ASSURE(hwnd != NULL)

	Subclass();
}

////////////////////////////////////////////////////////////////////////////////

LRESULT Subclassed::CallOld(UINT msg, WPARAM wParam, LPARAM lParam)
{
	return ::CallWindowProc(oldProc, hwnd, msg, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK Subclassed::WndProc(
	HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Subclassed* ptr = (Subclassed*) ::GetWindowLong(hwnd, GWL_USERDATA);
	ASSURE(ptr != NULL)
	return ptr->OnMsg(msg, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////////

LRESULT Subclassed::Reflect(UINT msg, WPARAM wParam, LPARAM lParam)
{
	return OnReflect(msg, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////////

LRESULT Subclassed::OnReflect(UINT msg, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

////////////////////////////////////////////////////////////////////////////////

void Subclassed::OnSubclass()
{
	// do nothing
}

////////////////////////////////////////////////////////////////////////////////

LRESULT Subclassed::OnMsg(UINT msg, WPARAM wParam, LPARAM lParam)
{
	return CallOld(msg, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////////

void MemDC::Begin(HDC destDC_, bool enabled_)
{
	ASSURE(destDC_ != NULL)

	destDC = destDC_;
	enabled = enabled_;

	if (enabled)
	{
		::GetClipBox(destDC, &rect);

		hdc = ::CreateCompatibleDC(destDC);

		bitmap = ::CreateCompatibleBitmap(
			destDC, rect.right - rect.left, rect.bottom - rect.top);

		oldBitmap = ::SelectObject(hdc, bitmap);

		::SetWindowOrgEx(hdc, rect.left, rect.top, NULL);
	}
	else
	{
		hdc = destDC;
	}
}

////////////////////////////////////////////////////////////////////////////////

void MemDC::End()
{
	if (enabled)
	{
		::BitBlt(
			destDC, rect.left, rect.top, rect.right - rect.left,
			rect.bottom - rect.top, hdc, rect.left, rect.top, SRCCOPY);

		::SelectObject(hdc, oldBitmap);
		::DeleteDC(hdc);
		::DeleteObject(bitmap);
	}
}

////////////////////////////////////////////////////////////////////////////////

HFONT DeriveFont(HFONT origFont, bool bold, bool underline, int newSize)
{
	// object to hold the font.
	LOGFONT logfont;

	if (origFont == NULL){
		NONCLIENTMETRICS resultMetrics;
		resultMetrics.cbSize = sizeof(NONCLIENTMETRICS);
		BOOL rs = ::SystemParametersInfo(
			SPI_GETNONCLIENTMETRICS,	// property we want to retrive
			sizeof(NONCLIENTMETRICS),  // the size of the structure we are putting data into
			(PVOID)(&resultMetrics),   // a pointer to the structure to place the data
			0						   // flag to say we are not setting any data, only read.
		);

		ASSURE(rs == TRUE);
		logfont = resultMetrics.lfMessageFont;
		// origFont = (HFONT) ::GetStockObject(DEFAULT_GUI_FONT);
	}else{
		// User has supplied an HFONT object.. so use that to get the font
		::GetObject(origFont, sizeof(LOGFONT), &logfont);
	}

	if (newSize != -1)
	{
		// user has requested the size of the font to be bigger..
		HDC hdc = ::GetDC(NULL);
		logfont.lfHeight = -MulDiv(newSize, ::GetDeviceCaps(hdc, LOGPIXELSY), 72);
		::ReleaseDC(NULL, hdc);
	}

	if (bold){
		logfont.lfWeight = FW_BOLD;
	}

	if (underline){
		logfont.lfUnderline = (BYTE) TRUE;
	}

	return ::CreateFontIndirect(&logfont);
}

////////////////////////////////////////////////////////////////////////////////

HCURSOR LoadHandCursor()
{
	// Loads a "hand" cursor from the winhlp32.exe module.
	// Based on code from Paul DiLascia's Jan 1998 MSJ articles.

	HCURSOR cursor = NULL;

	char windir[MAX_PATH + 1];
	//wchar_t windir[MAX_PATH + 1];

	if (::GetWindowsDirectory(windir, MAX_PATH) != 0)
	{
		strcat(windir, TEXT("\\winhlp32.exe"));
		//wcscat(windir, TEXT("\\winhlp32.exe"));

		HMODULE module = ::LoadLibrary(windir);
		if (module)
		{
			HCURSOR hand = ::LoadCursor(module, MAKEINTRESOURCE(106));
			if (hand) { cursor = CopyCursor(hand); }
			::FreeLibrary(module);
		}
	}

	return cursor;
}

////////////////////////////////////////////////////////////////////////////////

char* GetWndText(HWND hwnd)
//wchar_t* GetWndText(HWND hwnd)
{
	int len = ::GetWindowTextLength(hwnd);
	//wchar_t* str = (wchar_t*) malloc(sizeof(wchar_t)*(len + 1));
	char* str = (char*) malloc(len + 1);
	::GetWindowText(hwnd, str, len + 1);
	return str;
}

////////////////////////////////////////////////////////////////////////////////

void ScreenToClient(HWND hwnd, LPRECT rect)
{
	ASSURE(::IsWindow(hwnd) != FALSE)

	::ScreenToClient(hwnd,  (LPPOINT) rect);
	::ScreenToClient(hwnd, ((LPPOINT) rect) + 1);
}

////////////////////////////////////////////////////////////////////////////////

void FillRect(HDC hdc, const RECT& rect, const COLORREF color)
{
	ASSURE(hdc != NULL)

	COLORREF old_color = ::SetBkColor(hdc, color);
	::ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
	::SetBkColor(hdc, old_color);
}

////////////////////////////////////////////////////////////////////////////////

HBITMAP LoadBitmap(UINT id)
{
	return (HBITMAP) ::LoadBitmap(appInstance, MAKEINTRESOURCE(id));
}

////////////////////////////////////////////////////////////////////////////////

void FreeBitmap(HBITMAP bitmap)
{
	if (bitmap != NULL)
		::DeleteObject(bitmap);
}

////////////////////////////////////////////////////////////////////////////////

void DrawBitmap(HDC dc, HBITMAP bmap, int x, int y)
{
	BITMAP bm;
	::GetObject(bmap, sizeof(bm), &bm);

	HDC bmapdc = ::CreateCompatibleDC(dc);
	HGDIOBJ oldbmap = ::SelectObject(bmapdc, bmap);

	::BitBlt(dc, x, y, bm.bmWidth, bm.bmHeight, bmapdc, 0, 0, SRCCOPY);

	::SelectObject(bmapdc, oldbmap);
	::DeleteDC(bmapdc);
}

////////////////////////////////////////////////////////////////////////////////

//void CenterText(HDC hdc, int x, int w, int y, int h, const wchar_t* str, RECT* out)
void CenterText(HDC hdc, int x, int w, int y, int h, const char* str, RECT* out)
{
	ASSURE(str != NULL)

	size_t len = strlen(str);
	//size_t len = wcslen(str);

	SIZE extent;
	::GetTextExtentPoint32(hdc, str, len, &extent);

	int dx, dy;

	if (w != -1)
		dx = x + (w - extent.cx)/2;
	else
		dx = x;

	if (h != -1)
		dy = y + (h - extent.cy)/2;
	else
		dy = y;

	::TextOut(hdc, dx, dy, str, len);

	if (out != NULL)
	{
		(*out).left   = dx;
		(*out).right  = dx + extent.cx;
		(*out).top    = dy;
		(*out).bottom = dy + extent.cy;
	}
}

////////////////////////////////////////////////////////////////////////////////

void EnableDlgItem(HWND hwnd, UINT id, BOOL enable)
{
	::EnableWindow(::GetDlgItem(hwnd, id), enable);
}

////////////////////////////////////////////////////////////////////////////////

bool StringEmpty(const char* string)
{
	ASSURE(string != NULL)
	return string[0] == '\0';
}

////////////////////////////////////////////////////////////////////////////////

char* FilePicker(
	HWND hwnd, bool isSave, DWORD flags, const char* initial,
	const char* title, const char* filter, const char* defaultExt)
{
	ASSURE(hwnd != NULL)

	char* out = (char*) malloc(MAX_PATH + 1);
	if (out != NULL)
	{
		if (initial != NULL)
			_tcscpy(out, initial);
		else
			out[0] = '\0';

		OPENFILENAME ofn;
		ofn.lStructSize       = sizeof(OPENFILENAME);
		ofn.hwndOwner         = hwnd;
		ofn.hInstance         = NULL;
		ofn.lpstrFilter       = filter;
		ofn.lpstrCustomFilter = NULL;
		ofn.nMaxCustFilter    = 0;
		ofn.nFilterIndex      = 0;
		ofn.lpstrFile         = out;
		ofn.nMaxFile          = MAX_PATH;
		ofn.lpstrFileTitle    = NULL;
		ofn.nMaxFileTitle     = MAX_PATH;
		ofn.lpstrInitialDir   = NULL;
		ofn.lpstrTitle        = title;
		ofn.Flags             = flags;
		ofn.nFileOffset       = 0;
		ofn.nFileExtension    = 0;
		ofn.lpstrDefExt       = defaultExt;
		ofn.lCustData         = 0L;
		ofn.lpfnHook          = NULL;
		ofn.lpTemplateName    = NULL;

		// If the initial file name is invalid, the dialog box returns right
		// away without giving the user a chance to input a correct name. If
		// that happens we simply try again, but now without an initial name.

		bool res;
		DWORD error;

		do
		{
			if (isSave)
				res = (::GetSaveFileName(&ofn) == TRUE);
			else
				res = (::GetOpenFileName(&ofn) == TRUE);

			error = ::CommDlgExtendedError();

			if (error == FNERR_INVALIDFILENAME)
				ofn.lpstrFile[0] = '\0';
		}
		while ((res == false) && (error != 0));

		if (!res)  // cancelled
		{
			free(out);
			out = NULL;
		}
	}

	return out;
}

////////////////////////////////////////////////////////////////////////////////

bool ChangeDir(const char* dir)
{
	ASSURE(dir != NULL)
	return _chdir(dir) == 0;
	//return chdir(dir) == 0;
}

////////////////////////////////////////////////////////////////////////////////

char* GetCurrentDir()
{
	char* dir = (char*) malloc(MAX_PATH + 1);
	if (dir != NULL)
		//if (getcwd(dir, MAX_PATH) == NULL)
		if(_getcwd(dir, MAX_PATH) == NULL)
			_tcscpy(dir, "");
	return dir;
}

////////////////////////////////////////////////////////////////////////////////

char* GetInstallDir(const char* registryKey)
{
	return NULL;
	// char* dir;

	// dir = ReadRegistryString(HKEY_LOCAL_MACHINE, registryKey, "InstallDir");
	// if ((dir != NULL) && !StringEmpty(dir)) { return dir; }
	// free(dir);

	// dir = ReadRegistryString(HKEY_CURRENT_USER, registryKey, "InstallDir");
	// if ((dir != NULL) && !StringEmpty(dir)) { return dir; }
	// free(dir);

	// return NULL;
}

////////////////////////////////////////////////////////////////////////////////

void OpenUrl(const char* url)
{
	::ShellExecute(NULL, "open", url, NULL, NULL, SW_SHOW);
}

////////////////////////////////////////////////////////////////////////////////

bool InitFramework()
{
	// why oh why window do you not explain anything...
	// common controls allows for communication between windows.
	::InitCommonControls();

	// object linking and embedding?
	::OleInitialize(NULL);

	return true;
}

////////////////////////////////////////////////////////////////////////////////

void DoneFramework()
{
	// close the OLEInitialize
	::OleUninitialize();
}

////////////////////////////////////////////////////////////////////////////////
