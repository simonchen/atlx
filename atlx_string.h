/**********************************************************************
*
* Copyright (c) 2017 Simon Chen
*
***********************************************************************/

#pragma once
namespace ATLX{

	// Basic string template class, usage is more likely close to STL std::basic_string
	// Note: it doesn't support COW (Copy on write), and it's non thread-safe.
	template <class _Elem>
	class basic_string
	{
	public:
		// Constructs string with empty.
		basic_string() { _Ptr = NULL; m_dataSize = m_maxSize = 0; }

		// Constructs string by certain size, it won't fill zeroes throughout string.
		basic_string(size_t size) : _Ptr(NULL), m_dataSize(0), m_maxSize(0)
		{
			//assert(size > 0);
			_free(); // be safely more
			_alloc(size);
		}

		// Constructs string by passing generic characters at ending of zero.
		basic_string(const _Elem* ptr) : _Ptr(NULL), m_dataSize(0), m_maxSize(0)
		{
			_assign(ptr);
		}

		// Contructs on copy of same basic_string<_Elem>
		basic_string(const basic_string<_Elem>& str) : _Ptr(NULL), m_dataSize(0), m_maxSize(0)
		{
			_copy(str);
		}

		basic_string<_Elem>& operator=(const _Elem* ptr)
		{
			_assign(ptr);
			return *this;
		}

		basic_string<_Elem>& operator=(const basic_string<_Elem>& str)
		{
			_copy(str);
			return *this;
		}

		_Elem& operator [](int index){
			if (index < 0 || index >= m_maxSize || !_Ptr){
				static _Elem nullchar(NULL);
				return nullchar;
			}

			return _Ptr[index];
		}

		~basic_string()
		{
			_free();
		}

		// Returns actual size to store data.
		size_t size() const { return m_dataSize; }

		// Returns actual size to store data.
		size_t length() const { return m_dataSize; }

		// Returns Max size allocated.
		size_t max_size() const { return m_maxSize; }

		// Append new elements to current string, 
		// For TEXT type of string, the size of last zero terminator should be counted.
		inline bool append(const _Elem* ptr, size_t count)
		{
			if ((m_dataSize + count) > m_maxSize)
			{
				if (!_realloc(m_dataSize + count))
					return false;
			}

			memcpy(_Ptr + m_dataSize, ptr, count*sizeof(_Elem));
			m_dataSize += count;
		}

		// Size of elements are reset to zero, but this won't free memory!
		void erase()
		{
			m_dataSize = 0;
		}

		// Empty string and free memory
		void empty()
		{
			_free();
		}

		// Get buffer by specified size
		_Elem* get_buffer(size_t size)
		{
			reserve(size);
			return _Ptr;
		}

		// Adjusts the max size of elements that the string can place
		// it's identical with reserve()
		bool resize(size_t size)
		{
			return reserve(size);
		}

		// Adjusts the max size of elements that the string can place
		// it's identical with resize()
		bool reserve(size_t size)
		{
			bool ret = true;
			if (size > m_maxSize){
				ret = _realloc(size);
				if (ret) m_maxSize = size;
			}

			return ret;
		}

		// Returns internal Ptr of _Elem
		const _Elem* c_str() const { return _Ptr; }

		operator _Elem*() const { return _Ptr; }

		// Reads formatted data from the standard input stream
		void sscanf(const _Elem* fmt, ...)
		{
			if (!_Ptr || m_dataSize <= 0)
				return;

			va_list args;
			va_start(args, fmt);

			if (sizeof(_Elem) == sizeof(WCHAR))
			{
				T2A _str(_Ptr);
				T2A _fmt(fmt);
				vsscanf_s(_str, _fmt, args);
			}
			if (sizeof(_Elem) == sizeof(char))
			{
				vsscanf_s((LPSTR)_Ptr, (LPCSTR)fmt, args);
			}

			va_end(args);
		}

		// Write formatted data by standard c++ format specification.
		void format(const _Elem* fmt, ...)
		{
			va_list args;
			va_start(args, fmt);

			if (sizeof(_Elem) == sizeof(WCHAR))
			{
				// If locale points to an empty string, the locale is the implementation-defined native environment
				// https://msdn.microsoft.com/en-us/library/x99tb11d.aspx
				// vwprintf_s will use ANSI encoding to convert unicode args, this will make sure that output length is correct.
				char* cur_locale = setlocale(LC_ALL, "");
				size_t lens = vwprintf_s((LPCWSTR)fmt, args);
				setlocale(LC_ALL, cur_locale);
				if (lens > 0)
				{
					lens += 1; // vwprintf_s doesn't count terminating L'\0' 
					if (lens > m_maxSize){
						if (!_realloc(lens))
							return;
						m_maxSize = lens;
					}
					lens = vswprintf_s((LPWSTR)_Ptr, lens, (LPCWSTR)fmt, args);
					m_dataSize = lens;
				}
			}
			else if (sizeof(_Elem) == sizeof(char))
			{
				size_t lens = _vscprintf((LPCSTR)fmt, args);
				if (lens > 0)
				{
					lens += 1; // _vscprintf doesn't count terminating '\0' 
					if (lens > m_maxSize){
						if (!_realloc(lens))
							return;
						m_maxSize = lens;
					}
					lens = _vsnprintf((LPSTR)_Ptr, lens, (LPCSTR)fmt, args);
					m_dataSize = lens;
				}

			}

			va_end(args);
		}

	protected:
		_Elem* _Ptr;
		size_t m_dataSize;
		size_t m_maxSize;

		// Copy TEXT string with zero terminator
		void _assign(const _Elem* ptr)
		{
			if (ptr){
				size_t lens = 0;
				if (sizeof(_Elem) == sizeof(char))
					lens = strlen((LPCSTR)ptr);
				if (sizeof(_Elem) == sizeof(WCHAR))
					lens = wcslen((LPCWSTR)ptr);
				if (_alloc(lens + 1))
				{
					memcpy(_Ptr, ptr, (lens + 1)*sizeof(_Elem));
					m_dataSize = lens;
					m_maxSize = lens + 1;
				}
			}
		}

		// Copy generic string
		void _copy(const basic_string<_Elem>& str)
		{
			if (str.c_str()){
				size_t lens = str.max_size();
				_free();
				if (_alloc(lens))
				{
					memcpy(_Ptr, str.c_str(), lens*sizeof(_Elem));
					m_dataSize = str.size();
					m_maxSize = lens;
				}
			}
		}

		// Just freeing memory
		void _free()
		{
			if (_Ptr)
			{
				delete _Ptr;
				_Ptr = NULL;
			}

			m_dataSize = 0;
			m_maxSize = 0;
		}

		// Force re-allocate memory and copy current data.
		bool _realloc(size_t size)
		{
			_Elem* temp = new _Elem[size];
			if (!temp) return false;
			if (_Ptr){
				memcpy(temp, _Ptr, m_dataSize*sizeof(_Elem));
				delete _Ptr;
			}
			_Ptr = temp;
			m_maxSize = size;
			return true;
		}

		// Allocate memory by size of elements, typically, it's used for initialization.
		bool _alloc(size_t size)
		{
			_Ptr = new _Elem[size];
			if (!_Ptr) return false;
			m_maxSize = size;
			return true;
		}
	};

	// CString, CStringA, CStringW
	// They intend to be used in processing of TEXT string at ending of zero
	typedef basic_string<char> CStringA;
	typedef basic_string<wchar_t> CStringW;
#ifdef UNICODE
	typedef CStringW CString;
#else
	typedef CStringA CString;
#endif
	// string
	// This intends to be used in processing of SINGLE BYTE(CHAR)
	typedef basic_string<char> string;

	// A2T, T2A for convert multi-bytes & unicode chars.
	class A2T
	{
	public:
		A2T(LPCSTR psz){
			Init(psz, 0);
		}

		~A2T(){
			if (m_psz) delete m_psz;
		}

		operator LPWSTR() const
		{
			return m_psz;
		}

	private:
		void Init(
			LPCSTR psz,
			UINT nCodePage)
		{
			if (psz == NULL)
			{
				m_psz = NULL;
				return;
			}
			int nLengthA = static_cast<int>(strlen(psz)) + 1;
			int nLengthW = ::MultiByteToWideChar(nCodePage, 0, psz, nLengthA, NULL, 0);
			m_psz = new WCHAR[nLengthW];
			::MultiByteToWideChar(nCodePage, 0, psz, nLengthA, m_psz, nLengthW);
		}

		LPWSTR m_psz;
	};

	class T2A
	{
	public:
		T2A(LPCWSTR psz){
			Init(psz, 0);
		}

		~T2A(){
			if (m_psz) delete m_psz;
		}

		operator LPSTR() const
		{
			return m_psz;
		}

	private:
		void Init(
			LPCWSTR psz,
			UINT nCodePage)
		{
			if (psz == NULL)
			{
				m_psz = NULL;
				return;
			}
			int nLengthW = static_cast<int>(wcslen(psz)) + 1;
			int nLengthA = WideCharToMultiByte(nCodePage, 0, psz, nLengthW, NULL, 0, NULL, NULL);
			m_psz = new char[nLengthA];
			::WideCharToMultiByte(nCodePage, 0, psz, nLengthW, m_psz, nLengthA, NULL, NULL);
		}

		LPSTR m_psz;
	};

}