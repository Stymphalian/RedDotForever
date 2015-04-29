
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "unicode_utils.h"

////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
//void trace(char* file, int line, const char* format, ...)
void trace(wchar_t* file, int line, const wchar_t* format, ...)
{
	/*char buf[1024];

	char* ptr = buf;
	sprintf(ptr, "%s(%d) : ", file, line);
	ptr = buf + strlen(buf);

	va_list ap;
	va_start(ap, format);
	vsprintf(ptr, format, ap);
	va_end(ap);

	ptr = buf + strlen(buf);

	sprintf(ptr, "\n");*/

	wchar_t buf[1024];

	wchar_t* ptr = buf;
	wsprintf(ptr, L"%s(%d) : ", file, line);
	ptr = buf + wcslen(buf);

	va_list ap;
	va_start(ap, format);
	wvsprintf(ptr, format, ap);
	va_end(ap);

	ptr = buf + wcslen(buf);

	wsprintf(ptr, L"\n");

#ifdef _WIN32
	//OutputDebugString(buf);
	OutputDebugString(buf);
	//OutputDebugString(unicode_utils::utf8_to_utf16(buf,NULL).c_str());
#else
	printf(buf);
#endif
}
#endif

////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void assure(char* file, int line, bool condition, char* cond_text)
{
	if (!(condition))
	{
		char buf[1024];

		sprintf(
			buf, "%s(%d) : Assertion \"%s\" failed!\n", file, line,
			cond_text);

#ifdef _WIN32
		OutputDebugString(unicode_utils::utf8_to_utf16(buf,NULL).c_str());
		MessageBox(NULL, unicode_utils::utf8_to_utf16(buf, NULL).c_str(), L"Error!", MB_OK | MB_ICONSTOP);
#else
		printf(buf);
#endif

#ifdef _MSC_VER
		{ _asm { int 3 } }
#else
		exit(EXIT_FAILURE);
#endif
	}
}
#endif

////////////////////////////////////////////////////////////////////////////////
