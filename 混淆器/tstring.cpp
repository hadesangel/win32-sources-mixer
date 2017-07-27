#include "stdafx.h"
#include "tstring.h"
#include <stdarg.h>
#include <windows.h>

TString::TString()
	:m_string(new std::string)
{
}

TString::TString(int number)
	:m_string(new std::string)
{
	resize(32);
	_itoa_s(number, data(),32, 10);
}

TString::TString(const char* str) : m_string(new std::string)
{
	(*m_string) = str;
}

// TString::TString(const TString& str) : m_string(new std::string)
// {
// 	m_string = str.m_string;
// }

TString::TString(double number)
	: m_string(new std::string)
{
	char buff[64];
	sprintf_s(buff, "%lf", number);
	(*m_string)= buff;
}

TString::TString(long long number)
	:m_string(new std::string)
{
	resize(32);
	_i64toa_s(number, data(), 64, 10);
}

TString::TString(char ch)
	:m_string(new std::string)
{
	m_string->push_back(ch);
}

TString::TString(const std::string& str)
	: m_string(new std::string)
{
	*m_string = str;
}

TString& TString::truncate(size_t index)
{
	m_string->at(index) = 0;
	return *this;
}

TString& TString::append(const TString& str)
{
	m_string->append((*str.m_string));
	return *this;
}

TString& TString::insert(int pos, const TString& str)
{
	m_string->insert(pos, (*str.m_string));
	return *this;
}

TString TString::replace(const TString& newstr, const TString& oldstr, int count)
{
	int sofar = 0;
	int cursor = 0;
	TString s(*this);

	std::string::size_type oldlen = oldstr.size(), newlen = newstr.size();

	cursor = s.m_string->find((*oldstr.m_string), cursor);

	while (cursor != -1 && cursor <= (int)s.size())
	{
		if (count > -1 && sofar >= count)
		{
			break;
		}

		s.m_string->replace(cursor, oldlen, (const std::string&)newstr);
		cursor += (int)newlen;

		if (oldlen != 0)
		{
			cursor = s.m_string->find((const std::string&)oldstr, cursor);
		}
		else
		{
			++cursor;
		}

		++sofar;
	}

	return s;
}


TStringList TString::split_whitespace(int maxsplit)const
{
	TStringList result;

	std::string::size_type i, j, len = size();
	for (i = j = 0; i < len; )
	{
		while (i < len && ::isspace(m_string->at(i))) i++;
		j = i;

		while (i < len && !::isspace(m_string->at(i))) i++;

		if (j < i)
		{
			if (maxsplit-- <= 0) break;
			
			result.push_back(m_string->substr(j, i - j));

			while (i < len && ::isspace(m_string->at(i))) i++;
			j = i;
		}
	}
	if (j < len)
	{
		result.push_back(m_string->substr(j, len - j));
	}

	return result;
}


TStringList TString::split(const TString & sep, int maxsplit)const
{
	TStringList result;
	size_t last = 0;
	size_t index = m_string->find_first_of(sep, last);
	while (index != std::string::npos)
	{
		result.push_back(m_string->substr(last, index - last));
		last = index + 1;
		index = m_string->find_first_of(sep, last);
	}
	if (index - last > 0)
	{
		result.push_back(m_string->substr(last, index - last));
	}
	return result;
}

TString& TString::sprintf(const char* format, ...)
{
	va_list args;
	
	m_string->clear();
	m_string->resize(4096);
	va_start(args, format);
	vsprintf_s(data(),4096, format, args);
	va_end(args);
	return *this;
}

int TString::find(const std::string & sub, int start /*= 0*/, int end /*= MAX_32BIT_INT*/)
{
	std::string::size_type result = m_string->find(sub, start);

	// If we cannot find the string, or if the end-point of our found substring is past
	// the allowed end limit, return that it can't be found.
	if (result == std::string::npos ||
		(result + sub.size() > (std::string::size_type)end))
	{
		return -1;
	}

	return (int)result;
}

int TString::rfind(const std::string & sub, int start /*= 0*/, int end /*= MAX_32BIT_INT*/)
{
	std::string::size_type result = m_string->rfind(sub, end);

	if (result == std::string::npos ||
		result < (std::string::size_type)start ||
		(result + sub.size() > (std::string::size_type)end))
		return -1;

	return (int)result;
}

TString TString::mid(int begin, int end)
{
	return m_string->substr(begin, end);
}

TString& TString::operator<<(const TString& str)
{
	return append(str);
}


TString& TString::operator+=(const TString& str)
{
	return append(str);
}

int TString::toInt() const
{
	return atol(constdata());
}

unsigned int TString::toUInt() const
{
	return strtoul(constdata(), nullptr, 10);
}

double TString::toDouble() const
{
	return strtod(constdata(), nullptr);
}

TString TString::toLower() const
{
	TString s(*this);
	std::string::size_type len = s.size(), i;

	for (i = 0; i < len; ++i)
	{
		if (::isupper(s[i])) s[i] = (char) ::tolower(s[i]);
	}

	return s;
}

TString TString::toUpper() const
{
	TString s(*this);
	std::string::size_type len = s.size(), i;

	int ch = 0;
	for (i = 0; i < len; ++i)
	{
		ch = s[i];
		if (ch >='a' && ch<='z')
			s[i] = (char) ::toupper(s[i]);
	}

	return s;
}



std::wstring& AsciiToUnicode(const std::string& str , std::wstring& wstr)
{
	// 预算-缓冲区中宽字节的长度    
	int unicodeLen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);

	wstr.reserve(unicodeLen);

	// 开始向缓冲区转换字节    
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, (wchar_t*)wstr.c_str(), unicodeLen);

	return wstr;
}
std::string& UnicodeToAscii(const std::wstring& wstr, std::string& astr)
{
	// 预算-缓冲区中多字节的长度    
	int ansiiLen = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	
	astr.reserve(ansiiLen);

	// 开始向缓冲区转换字节    
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, (char*)astr.c_str(), ansiiLen, nullptr, nullptr);
	

	return astr;
}
std::wstring& Utf8ToUnicode(const std::string& str , std::wstring& wstr)
{
	// 预算-缓冲区中宽字节的长度    
	int unicodeLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
	
	wstr.reserve(unicodeLen);

	// 开始向缓冲区转换字节    
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, (wchar_t*)wstr.c_str(), unicodeLen);


	return wstr;
}
std::string& UnicodeToUtf8(const std::wstring& wstr , std::string& astr)
{
	// 预算-缓冲区中多字节的长度    
	int ansiiLen = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	
	astr.reserve(ansiiLen);

	// 开始向缓冲区转换字节    
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, (char*)astr.c_str(), ansiiLen, nullptr, nullptr);


	return astr;
}

std::wstring& TString::toUnicode(std::wstring& outputUnicode) const
{

	return AsciiToUnicode(*m_string, outputUnicode);
}


std::string& TString::toUtf8(std::string& outputUtf8) const
{
	std::wstring  wstr;
	UnicodeToUtf8(AsciiToUnicode(*m_string, wstr), outputUtf8);
	return outputUtf8;
}

TString& TString::fromUnicode(const std::wstring& str)
{
	UnicodeToAscii(str,*m_string);
	return *this;
}

TString& TString::fromUtf8(const std::string& utf8)
{
	std::wstring wstr;
	UnicodeToAscii(Utf8ToUnicode(utf8, wstr),*m_string);
	return *this;
}

TString operator+(const TString& l, const TString& r)
{
	TString str(l);
	str.append(r);
	return str;
}

std::ostream& operator<<(std::ostream& o, const TStringList& l)
{
	o << '[';
	for (auto&i : l)
		o << i << ',';
	printf("\b]");
	return o;
}

TString TStringList::join(const TString& sep)
{
	TString str;
	for (auto&i : *this)
	{
		str.append(i) + sep;
	}
	str.truncate(str.rfind(sep));
	return str;
}


