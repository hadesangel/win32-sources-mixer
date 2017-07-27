#pragma once
#include <memory>
#include <string>
#include <vector>
#include <iostream>

#define MAX_32BIT_INT 2147483647

class TString;

class TStringList : public std::vector<TString>
{
public:
    friend std::ostream& operator<<(std::ostream& o, const TStringList& l);

    inline TStringList& operator<<(const TString& s)
    {std::vector<TString>::push_back(s); return *this;}

    inline TStringList& push_back(const TString& s)
    { std::vector<TString>::push_back(s); return *this; }

    TString join(const TString& sep);
};



class TString
{
public:
    TString();
    TString(int number);
    TString(long long number);
    TString(double number);
    TString(char ch);
    TString(const char* str);
    TString(const std::string& str);
    TString(const TString& str) = default;
    TString& truncate(size_t index);
    TString& append(const TString& str);
    TString& insert(int pos, const TString& str);
    TString  replace(const TString& newstr, const TString& oldstr,int count=-1);
    TStringList split_whitespace(int maxsplit)const;
    TStringList split(const TString & sep, int maxsplit = -1)const;
    TString& sprintf(const char* format, ...);
    int find(const std::string & sub, int start = 0, int end = MAX_32BIT_INT);
    int rfind(const std::string & sub, int start = 0, int end = MAX_32BIT_INT);
    TString mid(int begin, int end);

    char& operator[](size_t index) { return m_string->at(index); }
    inline char& at(size_t index) { return m_string->at(index); }
    friend bool operator==(const TString& l, const TString& r) { return *(l.m_string) == *(r.m_string); }
    friend bool operator<(const TString& l, const TString& r) { return *(l.m_string) < *(r.m_string); }
    friend bool operator>(const TString& l, const TString& r) { return *(l.m_string) > *(r.m_string); }
    friend bool operator>=(const TString& l, const TString& r) { return *(l.m_string) >= *(r.m_string); }
    friend bool operator<=(const TString& l, const TString& r) { return *(l.m_string) <= *(r.m_string); }
    friend std::ostream& operator<<(std::ostream& o, const TString& str) { return o << str.m_string->c_str(); }

    operator const std::string&()const { return (*m_string); }
    operator std::string&(){ return (*m_string); }

    TString& operator+=(const TString& str);
    TString& operator<<(const TString& str);
    friend TString operator+(const TString& l, const TString& r);


    inline void swap(TString &other) { m_string.swap(other.m_string); }
    inline int size() const { return m_string->size(); }
    inline int count() const { return m_string->size(); }
    inline int length() const { return m_string->size(); };
    inline bool isEmpty() const { return m_string->empty(); };
    inline bool empty()const{return m_string->empty();}
    inline void clear() { m_string->clear(); }
    void resize(int size) { m_string->resize(size); }

    inline char* data() {return (char*)m_string->c_str();}
    inline const char* c_str()const { return m_string->c_str(); }
    inline const char* constdata()const { return m_string->c_str();}

    int toInt()const;
    unsigned int toUInt()const;
    double toDouble()const;
    TString toLower()const;
    TString toUpper()const;

    std::wstring& toUnicode(std::wstring& outputUnicode)const;
    std::string& toUtf8(std::string& outputUtf8)const;
    TString& fromUnicode(const std::wstring& str);
    TString& fromUtf8(const std::string& utf8);
    //operator const const char*()const { return m_string->c_str(); }
private:
    std::shared_ptr<std::string> m_string;

};



