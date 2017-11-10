#pragma once
namespace ATLX{
	
	// Basic string template class, usage is more likely close to STL std::basic_string
	template <class _Elem>
	class basic_string
	{
	public:
		basic_string() { _Ptr = NULL; m_dataSize = m_maxSize = 0; }
		basic_string(size_t size) : _Ptr(NULL), m_dataSize(0), m_maxSize(0)
		{
			//assert(size > 0);
			_free();
			_alloc(size);
		}

		~basic_string()
		{
			_free();
		}

		size_t size() { return m_dataSize; }

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
		void format(_Elem* fmt, ...)
		{
			va_list args;
			va_start(args, fmt);

			if (sizeof(_Elem) == sizeof(WCHAR))
			{
				int lens = vswprintf_s(NULL, 0, fmt, args);
				if (lens > 0)
				{
					if (lens > m_maxSize){
						_realloc(lens);
					}
					vswprintf_s(_Ptr, sizeof(szBuff), fmt, args);
				}
			}
			else if (sizeof(_Elem) == sizeof(char))
			{
				int lens = _vsnprintf(NULL, 0, fmt, args);
				if (lens > 0)
				{
					if (lens > m_maxSize){
						_realloc(lens);
					}
					_vsnprintf(_Ptr, sizeof(szBuff), fmt, args);
				}

			}
		}
	protected:
		_Elem* _Ptr;
		size_t m_dataSize;
		size_t m_maxSize;

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

	// CStringA, CStringW would be the good name that make you easily remember CString in MFC
	typedef basic_string<char> CStringA;
	typedef basic_string<wchar_t> CStringW;

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