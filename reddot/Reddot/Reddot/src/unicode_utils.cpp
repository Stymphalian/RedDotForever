#include <windows.h>         // WideCharToMultiByte(),MultiByteToWideChar()
#include "unicode_utils.h"


std::string utf16_to_utf8(const std::wstring& utf16_str,int* success)
{
    return utf16_to_utf8(utf16_str.c_str(),success);
}

std::string utf16_to_utf8(const wchar_t* utf16_str, int* success)
{
    if(success != NULL){*success = 0;}
    if(utf16_str == NULL){ return "";}

    int s_len = WideCharToMultiByte(
        CP_UTF8,                // convert into UTF-8
        WC_ERR_INVALID_CHARS,   // safe-fail if an invalid UTF-16 char is found

        utf16_str,              // the utf16 string to convert into utf8
        -1,                     // obtain the lenght of the string

        NULL,                   // NULL because we only want the len of the str
        0,                      // set to zero in order to request only the size

        NULL,                   // set to NULL for CP_UTF8 mode
        NULL                    // set to NULL for CP_UTF8 mode
    );

    // check for errors.
    // There is an ambiguous case.
    //  1) the api call returns 0 if there is an error; therefore s_len = 0
    //  2) The string is an empty string, therefore s_len = 0
    // Therefore we must check to see if the call was successful or not.
    if(s_len == 0){
        DWORD err = GetLastError();
        if(err != ERROR_SUCCESS){
            return "";
        }
    }

    // Resize the string to be large enough to hold the converted string.
    std::string utf8_str;
    utf8_str.resize(s_len);

    // Perform the real conversion.
    s_len = WideCharToMultiByte(
        CP_UTF8,               // convert into UTF-8
        WC_ERR_INVALID_CHARS,  // safe-fail if an invalid UTF-16 char is found

        utf16_str,             // the UTF-16 string to convert into UTF-8
        s_len,                 // obtain the length of the string

        const_cast<char*>(utf8_str.data()), // where we put the resulting string
        utf8_str.size(),                    // max number of bytes to read in

        NULL,                  // set to NULL for CP_UTF8 mode
        NULL                   // set to NULL for CP_UTF8 mode
        );

    if(s_len == 0){
        // Check for same ambiguous error case as sepcified above.
        DWORD err = GetLastError();
        if(err != ERROR_SUCCESS){
            return "";
        }
    }

    // Set the success flag as appropriate and return the converted string.
    if(success != NULL){*success = 1;}
    return utf8_str;
}


////////////////////////////////////////////////////////////////////////////////

std::wstring utf8_to_utf16(const std::string& utf8_str,int* success)
{
    return utf8_to_utf16(utf8_str.c_str(),success);
}

std::wstring utf8_to_utf16(const char* utf8_str,int* success)
{
    if(success != NULL){ *success = 0; }
    if(utf8_str == NULL){return L"";}

    int s_len = MultiByteToWideChar(
        CP_UTF8,                // convert from UTF-8
        WC_ERR_INVALID_CHARS,   // safe-fail if an invalid UTF-8 char is found

        utf8_str,               // the UTF-16 string to convert into UTF-8
        -1,                     // obtain the length of the string

        NULL,                   // NULL because we only want the len of the str
        0                       // set to zero in order to request only the size
        );

    // check for errors.
    // There is an ambiguous case.
    //  1) the api call returns 0 if there is an error; therefore s_len = 0
    //  2) The string is an empty string, therefore s_len = 0
    // Therefore we must check to see if the call was successful or not.
    if(s_len == 0){
        DWORD err = GetLastError();
        if(err != ERROR_SUCCESS){
            return L"";
        }
    }

    // now do the conversion of the string
    std::wstring utf16_str;
    utf16_str.resize(s_len);
    s_len = MultiByteToWideChar(
        CP_UTF8,                // convert from UTF-8
        WC_ERR_INVALID_CHARS,   // safely fail if an invalid UTF-8 char is found

        utf8_str,               // the UTF-8 string to convert into UTF-16
        s_len,                  // obtain the length of the string

        const_cast<wchar_t*>(utf16_str.data()),  // where we put the result str
        utf16_str.size()                         // num byte to read in
        );


    if(s_len == 0){
        // Check for same ambiguous error case as sepcified above.
        DWORD err = GetLastError();
        if(err != ERROR_SUCCESS){
            return L"";
        }
    }

    // set the success flag as appropriate and return the converted string.
    if(success != NULL){ *success = 1; }
    return utf16_str;
}