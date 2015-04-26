#ifndef UNICODE_UTILS_H
#define UNICODE_UTILS_H

#include <string>

/**
 * @purpose Convert a UTF-8 encoded string into a UTF-16 encoded string using
 *  the windows 'WideCharToMultiByte' API function.
 *  We do two calls to WideCharToMultiByte.
 *      1st Call - Used to obtain the output length of the converted string.
 *          This is done so that we can allocate an appropriate sized buffer.
 *      2nd Call - The 'real' call that actually performs the conversion and
 *          outputs into the resulting buffer.
 * @param utf16_str pointer to the UTF-16 encoded string. NULL terminated.
 * @param success [optional] If not set to NULL, it will hold the success state
 *      of the conversion. ( 1 for sucess, 0 for fail)
 * @return return a std::string with the converted text.
 *      Return an "" if the conversion fails.
 */
std::string utf16_to_utf8(const std::wstring utf16_str,int* success);
std::string utf16_to_utf8(const wchar_t* utf16_str,int* success);


/**
 * @purpose Convert a UTF-16 encoded string into a UTF-8 encoded string using
 *  the windows 'MultiByteToWideChar' API function.
 *  We do two calls to MultiByteToWideChar.
 *      1st Call - Used to obtain the output length of the converted string.
 *          This is done so that we can allocate an appropriate sized buffer.
 *      2nd Call - The 'real' call that actually performs the conversion and
 *          outputs into the resulting buffer.
 * @param utf8_str pointer to the UTF-8 encoded string. Must be NULL terminated.
 * @param success [optional] If not set to NULL, it will hold the success state
 *      of the conversion. ( 1 for sucess, 0 for fail)
 * @return return a std::wstring with the converted text.
 *      Return an L"" if the conversion fails.
 */
std::wstring utf8_to_utf16(const std::string utf8_str,int* success);
std::wstring utf8_to_utf16(const char* utf8_str,int* success);

#endif