#ifndef INI_READER_H
#define INI_READER_H

/**
 * A utility object used to load-in and manipulate a .ini file.Simple operations
 * for readeing/setting basic primitive values( i.e string, bool, int,float)
 *
 * Specific notes:
 *  1. If a section string is not provided, it is assumed the global default
 *      section. In this case it is represented by the empty string.
 *  2. The parser is not very smart. Please ensure that your ini files are
 *      well-formed. No in-line comments. Key-value pairs are seperated on first
 *      '=' character, section title must be enclosed in '[',']'
 *
 * An example ini file:
 *  key1=1
 *  [section 1]
 *  ;comment line
 *  key1=one
 *  key2=1.0
 *
 * Usage:
 *  IniReader reader;
 *  reader.Load("example.ini");
 *  int val1 = reader.GetInt("key1");
 *  std::string val2 = reader.GetString("key1","section1");
 *  reader.SetFloat("key2",5.0f,"section2");
 *  reader.Save();
 */

#include <string>
#include <vector>

class IniReader
{
public:
    IniReader();

    bool Load(std::string filename);
    bool Save(std::string path);

    bool Has(std::string key, std::string section = "");

    // retrieve values from keys
    std::string GetString (std::string key,std::string section="",std::string defaultValue="");
    int         GetInt    (std::string key,std::string section="",int defaultValue= 0);
    float       GetFloat  (std::string key,std::string section="",float defaultValue=0.0f);
    bool        GetBool   (std::string key,std::string section="",bool defaultValue=false);

    // set value for the particular key.
    bool SetString (std::string key,std::string value,std::string section="");
    bool SetInt    (std::string key,int value,std::string section="");
    bool SetFloat  (std::string key,float value,std::string section="");
    bool SetBool   (std::string key,bool value,std::string section="");

protected:

    // Internal class used to represent an ini token
    class Token
    {
    public:
        Token();
        std::string ToString();

        std::string section;
        std::string key;
        std::string value;

        bool is_invalid;
        bool is_comment;
        bool is_section;
        bool is_empty_line;
    };

    // we use a vector,so that we we do a 'save' we can recreate the entire ini-file
    // as the user left it. Efficiency wise this is terrible choice,but atleast it
    // will preserve the file...
    std::vector<Token> _tokens;

    void _set_token(std::string& section, std::string& line);
    void _set_comment(std::string& line);
    void _set_section(std::string& line);
    void _set_empty_line();
    Token* _find(std::string& key, std::string& section);
};

#endif