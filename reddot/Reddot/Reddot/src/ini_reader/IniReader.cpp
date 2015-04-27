#include <string>
#include <fstream>      // ifstream,ofstream
#include <cstdlib>      // atoi,atof
#include <cstdio>       // sprintf

#include "IniReader.h"

IniReader::IniReader() {}

bool IniReader::Load(std::string filename)
{
    _tokens.clear();
    std::ifstream fin(filename.c_str(),std::ifstream::in);
    if( fin.is_open() == false){return false;}

    std::string line;
    std::string current_section = "";
    while(!fin.eof() && fin.good())
    {
        std::getline(fin,line);

        // set based on the type of the token...
        if(line.length() == 0){
            _set_empty_line();
        } else if(line[0] == ';'){
            _set_comment(line);
        }else if(line[0]=='['){
            _set_section(line);

            // extract between the '[' and ']'; why length() - 2???
            current_section = line.substr(1,line.length()-2);
        }else {
            _set_token(current_section,line);
        }
    }

    fin.close();
    return true;
}

bool IniReader::Save(std::string path)
{
    std::ofstream fout(path.c_str(),std::ofstream::out);
    if( fout.is_open() == false){return false;}

    std::vector<Token>::iterator it;
    for( it = _tokens.begin(); it != _tokens.end(); ++it)
    {
        fout << it->ToString() << std::endl;
    }

    fout.close();
    return true;
}

// retrieve values from keys
std::string IniReader::GetString(std::string key,std::string section)
{
    if(!Has(key,section)){return "";}
    return _find(key,section)->value;
}

int IniReader::GetInt(std::string key,std::string section)
{
    if(!Has(key,section)){return 0;}
    return atoi(_find(key,section)->value.c_str());
}

float IniReader::GetFloat(std::string key,std::string section)
{
    if(!Has(key,section)){return 0.0f;}
    return static_cast<float>(atof(_find(key,section)->value.c_str()));
}
bool  IniReader::GetBool(std::string key,std::string section)
{
    if(!Has(key,section)){return false;}
    std::string rs = _find(key,section)->value;
    return ( rs == "true" || rs == "1");
}

bool IniReader::SetString(std::string key,std::string value,std::string section)
{
    if(!Has(key,section)){return false;}
    Token* tok = _find(key,section);
    tok->value = value;
    return true;
}

bool IniReader::SetInt(std::string key,int value,std::string section)
{
    if(!Has(key,section)){return false;}
    Token* tok = _find(key,section);
    // hopefully long enough...
    char buffer[120];
    sprintf(buffer,"%d",value);
    tok->value = std::string(buffer);
    return true;
}

bool IniReader::SetFloat(std::string key,float value,std::string section)
{
    if(!Has(key,section)){return false;}
    Token* tok = _find(key,section);

    // kind arbitrary digit. might be wasting a ton of space but idk
    char buffer[120];
    sprintf(buffer,"%f",value);
    tok->value = std::string(buffer);
    return true;
}
bool IniReader::SetBool(std::string key,bool value,std::string section)
{
    if(!Has(key,section)){return false;}
    Token* tok = _find(key,section);
    if( value == true){
        tok->value = "true";
    }else {
        tok->value = "false";
    }
    return true;
}


bool IniReader::Has(std::string key, std::string section)
{
    Token* t = _find(key,section);
    return (t != NULL);
}


////////////////////////////////////////////////////////////////////////////////

/*
 * @param section - a string denoting the section of the key-value pair.
 *      we note that this will not have the '[',']' around the string.
 * @param line- a string denothing the key-value pair.
 *      i.e key_thing=holy grill
 */
void IniReader::_set_token(std::string& section, std::string& line)
{
    Token tok;

    size_t w = line.find_first_of("=");
    if( w == std::string::npos) {
        // edge case in which we have malformed key-value pair
        tok.is_invalid = true;
        tok.value = line;
    }else{
        tok.section = section;
        // extract out the key before the '='
        tok.key = line.substr(0,w);

        // extract out the key after the '='
        tok.value = line.substr(w+1);
    }

    _tokens.push_back(tok);
}
void IniReader::_set_comment(std::string& line)
{
    Token tok;
    tok.is_comment = true;
    tok.value = line;
    _tokens.push_back(tok);
}
void IniReader::_set_section(std::string& line)
{
    Token tok;
    tok.is_section = true;
    tok.section = line;
    tok.value = line;
    _tokens.push_back(tok);
}
void IniReader::_set_empty_line()
{
    Token tok;
    tok.is_empty_line = true;
    //tok.value = std::endl;
    _tokens.push_back(tok);
}

/**
 * @param key - the key string for which to retrieve the Token object.
 * @param section [optional] - the section in which the key-value pair belongs
 *      to. Must explicitely match the section as written in the .ini file.
 *      Default section is the global section. It is represented by the "".
 * @return NULL if a token with the given key + section cannot be found, else
 *      returns a pointer to the internal Token objet.
 */
IniReader::Token* IniReader::_find(std::string& key, std::string& section)
{
    std::vector<Token>::iterator it;
    for(it = _tokens.begin(); it != _tokens.end(); ++it)
    {
        // not a token...
        if(it->is_invalid || it->is_comment || it->is_empty_line || it->is_section){
            continue;
        }

        if(it->section == section && it->key == key){
            return &(*it);
        }
    }
    return NULL;
}

////////////////////////////////////////////////////////////////////////////////
IniReader::Token::Token()
{
    is_invalid = false;
    is_comment = false;
    is_empty_line = false;
    is_section = false;
}

std::string IniReader::Token::ToString()
{
    if(is_invalid){
        return value;
    }else if( is_comment){
        return value;
    }else if( is_empty_line){
        return "";
    }else if(is_section){
        return section;
    }else{
        return key + "=" + value;
    }
}