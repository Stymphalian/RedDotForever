
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void trace(char* file, int line, const char* format, ...)
{
	char buf[1024];

	char* ptr = buf;
	sprintf(ptr, "%s(%d) : ", file, line);
	ptr = buf + strlen(buf);

	va_list ap;
	va_start(ap, format);
	vsprintf(ptr, format, ap);
	va_end(ap);

	ptr = buf + strlen(buf);

	sprintf(ptr, "\n");

#ifdef _WIN32
	OutputDebugString(buf);
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
		OutputDebugString(buf);
		MessageBox(NULL, buf, "Error!", MB_OK | MB_ICONSTOP);
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
