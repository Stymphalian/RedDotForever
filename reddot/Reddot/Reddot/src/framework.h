
#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#include <windows.h>
#include <commctrl.h>
#include <string>
#include "resource.h"
#include "bugsbgone.h"
#include "unicode_utils.h"


////////////////////////////////////////////////////////////////////////////////
extern HINSTANCE appInstance;

////////////////////////////////////////////////////////////////////////////////

bool InitFramework();
void DoneFramework();

////////////////////////////////////////////////////////////////////////////////

typedef __int8                int8;
typedef __int16               int16;
typedef int                   int32;
typedef __int64               int64;

typedef unsigned __int8       uint8;
typedef unsigned __int16      uint16;
typedef unsigned int          uint32;
typedef unsigned __int64      uint64;

////////////////////////////////////////////////////////////////////////////////

/*
	WndBase implements behaviour shared by the other classes.
	You are not supposed to use this directly.
	like what behavior you fucker. There is literally nothing in here.
	All it does is hold the handles and an id.
	If we could move the Reflect code in here, or some other things
	then this might actually be sensible.
 */
class WndBase
{
protected:
	WndBase();
	virtual ~WndBase();

public:
	void Invalidate(bool repaint);

	HWND hwnd;
	HWND parent;
	UINT id;
};

////////////////////////////////////////////////////////////////////////////////

/*
	Window is used for your own top-level windows and child controls.
	Your subclasses will typically have a Create() method that first
	sets up and registers the WNDCLASS, and then calls ::CreateWindow()
	with the proper flags. Pass "(LPVOID) this" as the cookie. At the
	end of OnMsg() you should always do "return CallDefault(...)"
 */

class Window : public WndBase
{
public:
	LRESULT Reflect(UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	// variables
	WNDCLASSEX wndclass;

	// constructor
	Window();

	// static
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	// virtuals
	virtual LRESULT OnMsg(UINT, WPARAM, LPARAM) = 0;
	virtual LRESULT OnReflect(UINT msg, WPARAM wParam, LPARAM lParam);

	// methods
	LRESULT CallDefault(UINT msg, WPARAM wParam, LPARAM lParam);
	bool RegisterClass();
};

////////////////////////////////////////////////////////////////////////////////

/*
	Dialog is used for modal and modeless dialog boxes. Your subclasses
	will typically have a Go() method that calls ::DialogBoxParam() or
	::CreateDialogParam(). Pass "(LPARAM) this" as the cookie. At the end
	of OnMsg(), you should do "return FALSE"
 */

class Dialog : public WndBase
{
public:
	BOOL Reflect(UINT msg, WPARAM wParam, LPARAM lParam);

protected:

	static BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

	virtual BOOL OnMsg(UINT, WPARAM, LPARAM) = 0;
	virtual BOOL OnReflect(UINT msg, WPARAM wParam, LPARAM lParam);

	void Enable(UINT id, bool flag);
	void Check(UINT id, bool flag);
	bool IsChecked(UINT id);
};

////////////////////////////////////////////////////////////////////////////////

/*
	Subclassed is used to replace the message handling of someone else's
	control. Typically, you will make a Create() method that first creates
	the control, and then call Subclass() to replace its WndProc. At the
	end of OnMsg(), you should do "return CallOld(msg, wParam, lParam);"
	You can also Subclass existing controls (e.g., on a dialog template).
 */

class Subclassed : public WndBase
{
public:
	void Subclass();
	void Subclass(WNDPROC wndProc);
	void Subclass(HWND hdlg, UINT id);

	LRESULT Reflect(UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	virtual LRESULT OnMsg(UINT, WPARAM, LPARAM);
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	virtual void OnSubclass();
	virtual LRESULT OnReflect(UINT msg, WPARAM wParam, LPARAM lParam);

	LRESULT CallOld(UINT, WPARAM, LPARAM);
	WNDPROC oldProc;
};

////////////////////////////////////////////////////////////////////////////////

class MemDC
{
public:
	void Begin(HDC destDC, bool enabled = true);
	void End();

public:
	HDC hdc;

private:
	HBITMAP bitmap;
	HGDIOBJ oldBitmap;
	HDC destDC;
	RECT rect;
	bool enabled;
};

////////////////////////////////////////////////////////////////////////////////

HFONT DeriveFont(HFONT origFont, bool bold, bool underline, int newSize);

HCURSOR LoadHandCursor();

//wchar_t* GetWndText(HWND hwnd);
std::string GetWndText(HWND hand);
// char* GetWndText(HWND hwnd);

void ScreenToClient(HWND hwnd, LPRECT rect);

void FillRect(HDC hdc, const RECT& rect, const COLORREF color);

HBITMAP LoadBitmap(UINT id);

void FreeBitmap(HBITMAP bitmap);

void DrawBitmap(HDC hdc, HBITMAP bmap, int x, int y);

void CenterText(HDC hdc, int x, int w, int y, int h, const char* str, RECT* out = NULL);
// void CenterText(HDC hdc, int x, int w, int y, int h, const wchar_t* str, RECT* out = NULL);

void EnableDlgItem(HWND hwnd, UINT id, BOOL enable);

////////////////////////////////////////////////////////////////////////////////

bool StringEmpty(const char* string);

////////////////////////////////////////////////////////////////////////////////

std::string FilePicker(
	HWND hwnd, bool isSave, DWORD flags, const char* initial,
	const char* title, const char* filter, const char* defaultExt);

bool ChangeDir(const char* dir);
std::string GetCurrentDir();

void OpenUrl(const char* url);

////////////////////////////////////////////////////////////////////////////////

#endif // FRAMEWORK_H
