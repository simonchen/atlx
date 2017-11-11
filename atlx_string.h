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

		// Constructs string by passing generic characters at ending zero.
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

		size_t size() { return m_dataSize; }
		size_t max_size() { return m_maxSize; }

		inline bool append(const _Elem* ptr, size_t count)
		{
			if ((m_dataSize + count) > m_maxSize)
			{
				if (!_realloc(m_dataSize + count))
					return false;
			}

			memcpy(_Ptr + m_dataSize, ptr, count);
			m_dataSize += count;
		}

		void erase()
		{
			m_dataSize = 0;
		}

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
		const _Elem* c_str() { return _Ptr; }

		operator _Elem*() const { return _Ptr; }

		// Format string by standard c++ format specification.
		void format(const _Elem* fmt, ...)
		{
			va_list args;
			va_start(args, fmt);

			if (sizeof(_Elem) == sizeof(WCHAR))
			{
				int lens = vswprintf_s(NULL, 0, (LPCWSTR)fmt, args);
				if (lens > 0)
				{
					if (lens > m_maxSize){
						_realloc(lens);
					}
					vswprintf_s((LPWSTR)_Ptr, lens, (LPCWSTR)fmt, args);
				}
			}
			else if (sizeof(_Elem) == sizeof(char))
			{
				int lens = _vsnprintf(NULL, 0, (LPCSTR)fmt, args);
				if (lens > 0)
				{
					if (lens > m_maxSize){
						_realloc(lens);
					}
					_vsnprintf((LPSTR)_Ptr, lens, (LPCSTR)fmt, args);
				}

			}
		}

	protected:
		_Elem* _Ptr;
		size_t m_dataSize;
		size_t m_maxSize;

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

		void _copy(const basic_string<_Elem>& str)
		{
			if (str.c_str()){
				size_t lens = str.max_size();
				if (_alloc(lens))
				{
					memcpy(_Ptr, str.c_str(), lens*sizeof(_Elem));
					m_dataSize = str.size();
					m_maxSize = lens;
				}
			}
		}

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

		bool _realloc(size_t size)
		{
			_Elem* temp = new _Elem[size];
			if (!temp) return false;
			if (_Ptr){
				memcpy(temp, _Ptr, m_dataSize);
				delete _Ptr;
			}
			_Ptr = temp;
			m_maxSize = size;
			return true;
		}

		bool _alloc(size_t size)
		{
			_Ptr = new _Elem[size];
			if (!_Ptr) return false;
			m_maxSize = size;
			return true;
		}
	};

	// CString, CStringA, CStringW
	typedef basic_string<char> CStringA;
	typedef basic_string<wchar_t> CStringW;
#ifdef UNICODE
	typedef CStringW CString;
#else
	typedef CStringA CString;
#endif

	// A2T, T2A for convert multi-bytes & unicode chars.
	class A2T
	{
	public:
		A2T(LPCSTR psz){
			Init(psz, CP_UTF8);
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
			Init(psz, CP_UTF8);
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
			int nLengthA = WideCharToMultiByte(nCodePage, 0, psz, nLengthA, NULL, 0, NULL, NULL);
			m_psz = new char[nLengthA];
			::WideCharToMultiByte(nCodePage, 0, psz, nLengthW, m_psz, nLengthA, NULL, NULL);
		}

		LPSTR m_psz;
	};

}