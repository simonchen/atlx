# atlx

A lightweight library that reconstruct the core of [ATL](https://msdn.microsoft.com/en-us/library/3ax346b7.aspx), and provide some foundational classes like [MFC](https://en.wikipedia.org/wiki/Microsoft_Foundation_Class_Library)(Microsoft Foundational Classes) make you easily manage User Interfaces such as Window, Dialog, View(and Scrollable View), other **Limited** kinds of control like Static box, Input box, Check box, Radio box, Button, Progress bar, Slider control, Hotkey control.

The aim **IS** to simplify the usage for save time to develop User Interfaces for windows desktop application.
The aim is **NOT** to provide a complete development framework like [Model-View-Controller mode](https://en.wikipedia.org/wiki/Model%E2%80%93view%E2%80%93controller).

The people who ever been familar with MFC/ATL/[STL](https://en.wikipedia.org/wiki/Standard_Template_Library)(Standard Template Library) will be more easily to use this library.

## Fundational Window Classes

#### CWndThunker
The core that any Window class been derived from
#### CWndSuper 
Super Window class derived from CWndThunker creates a basic Window, Subclass / Unsubclass Window functions are supplied also.
#### CDialogx
Create model / modeless dialog and processing generic events.

## Utility Classes

#### String

- CString, CStringA, CStringW

They intend to be used in processing of TEXT string at ending of zero

- string

This intends to be used in processing of SINGLE BYTE(CHAR)
  
## Compilation environment
- VisualStudio 2005 or higher version (2013,2015,2017)
- Windows SDK
- Multi-Byte(UTF-8) / Unicode charset
- C Run-time secure

## Code rules
- No exception handler (such as Memory allocation failures)
- Naming rules are almost close to [MFC](https://en.wikipedia.org/wiki/Microsoft_Foundation_Class_Library) 

## References
[Another new thunk copy from ATL](https://www.codeproject.com/Articles/348387/Another-new-thunk-copy-from-ATL) Authored by @[Simonchen](https://github.com/simonchen)
