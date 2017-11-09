# atlx

A lightweight library to reconstruct the core of <a href="https://msdn.microsoft.com/en-us/library/3ax346b7.aspx">ATL</a>(Active Template Library), and provide some foundational classes like MFC(Microsoft Foundational Classes) make you easily manage User Interfaces such as Window, Dialog, different kinds of control like Static box, Input box, Check box, Button, etc.
The aim is to simplify the usage for save time to develop User Interfaces in windows desktop application.
The aim is NOT to provide a framework like Model-View-Control mode, thus, the library is only facing the people who wants to build primary or middle-difficut User Interfaces as much easily as possible.

## Fundational Classes

#### CWndThunker
The core that any Window class been derived from
#### CWndSuper 
Super Window class derived from CWndThunker, helping you to create a simple Window easily, Subclass / Unsubclass Window functions are useful also.
#### CDialogx
Create model / modeless dialog and processing generic events.

## References
[Another new thunk copy from ATL](https://www.codeproject.com/Articles/348387/Another-new-thunk-copy-from-ATL) Authored by @[Simonchen](https://github.com/simonchen)
