#pragma once

#include "WndSuper.h"
#include "atlx_tooltip_ctrl.h"

namespace ATLX{

	class CSliderCtrl : public CWndSuper
	{
		// Constructors
	public:
		CSliderCtrl();

		// Generic creator
		virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWndSuper* pParentWnd, UINT nID = 0xFFFF);

		// Generic creator allowing extended style bits
		virtual BOOL CreateEx(DWORD dwExStyle, DWORD dwStyle, const RECT& rect,
			CWndSuper* pParentWnd, UINT nID = 0xFFFF);

		// Attributes
		// Retrieves the number of logical positions the trackbar control's slider
		// moves in response to keyboard input from the up/down arrow keys.
		int GetLineSize() const;

		// Sets the number of logical positions the trackbar control's slider
		// moves in response to keyboard input from the up/down arrow keys.
		int SetLineSize(int nSize);

		// Retrieves the number of logical positions the trackbar control's slider
		// moves in response to keyboard input from the PgUp/PgDn keys.
		int GetPageSize() const;

		// Sets the number of logical positions the trackbar control's slider
		// moves in response to keyboard input from the PgUp/PgDn keys.
		int SetPageSize(int nSize);

		// Retrieves the maximum position for the slider in the trackbar control.
		int GetRangeMax() const;

		// Retrieves the minimum position for the slider in the trackbar control.
		int GetRangeMin() const;

		// Retrieves the minimum and maximum positions for the slider in the trackbar control.
		void GetRange(int& nMin, int& nMax) const;

		// Sets the minimum position for the slider in the trackbar control.
		void SetRangeMin(int nMin, BOOL bRedraw = FALSE);

		// Sets the maximum position for the slider in the trackbar control.
		void SetRangeMax(int nMax, BOOL bRedraw = FALSE);

		// Sets the minimum and maximum positions for the slider in the trackbar control.
		void SetRange(int nMin, int nMax, BOOL bRedraw = FALSE);

		// Retrieves the starting and ending positions of the selection in the trackbar control.
		void GetSelection(int& nMin, int& nMax) const;

		// Sets the starting and ending positions of the selection in the trackbar control.
		void SetSelection(int nMin, int nMax);

		// Retrieves the bounding rectangle for the trackbar control's channel.
		void GetChannelRect(LPRECT lprc) const;

		// Retrieves the bounding rectangle for the slider in the trackbar control.
		void GetThumbRect(LPRECT lprc) const;

		// Retrieves the current logical position of the slider in the trackbar control.
		int GetPos() const;

		// Sets the current logical position of the slider in the trackbar control.
		void SetPos(int nPos);

		// Retrieves the number of tick marks in a trackbar control.
		UINT GetNumTics() const;

		// Retrieves an array of positions of tick marks in the trackbar control.
		DWORD* GetTicArray() const;

		// Retrieves the logical position of the specified tick mark in the trackbar control.
		int GetTic(int nTic) const;

		// Retrieves the current physical position of the specified tick mark in the trackbar control.
		int GetTicPos(int nTic) const;

		// Sets a tick mark in the trackbar control at the specified logical position.
		BOOL SetTic(int nTic);

		// Sets the interval frequency for tick marks in the trackbar control.
		void SetTicFreq(int nFreq);

		// Retrieves the trackbar control buddy window at the specified location.
		CWndSuper* GetBuddy(BOOL fLocation = TRUE) const;

		// Assigns the specified window as the buddy window for the trackbar control.
		CWndSuper* SetBuddy(CWndSuper* pWndBuddy, BOOL fLocation = TRUE);

		// Retrieves the ToolTip control assigned to the trackbar control.
		CToolTipCtrl* GetToolTips() const;

		// Assigns a ToolTip control to the trackbar control.
		void SetToolTips(CToolTipCtrl* pWndTip);

		// Positions the ToolTip control used by the trackbar control.
		int SetTipSide(int nLocation);

		// Sets the length of the slider in the trackbar control.
		void SetThumbLength(int nLength);

		// Retrieves the length of the slider in the trackbar control.
		int GetThumbLength() const;

		// Operations
		// Clears the current selection range in the trackbar control.
		void ClearSel(BOOL bRedraw = FALSE);

		// Removes the current tick marks from the trackbar control.
		void ClearTics(BOOL bRedraw = FALSE);

		// Implementation
	public:
		virtual ~CSliderCtrl();
	};

}