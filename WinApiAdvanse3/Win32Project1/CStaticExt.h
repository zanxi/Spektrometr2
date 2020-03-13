#pragma once
#include ".\stdwin.h"

class TColorText : public CStatic
{
protected:
	DECLARE_MESSAGE_MAP()

public:
	// make the background transparent (or if ATransparent == true, restore the previous background color)

	//TColorText() { notify = true; };
	void setTransparent(bool ATransparent = true);
	// set background color and make the background opaque
	void SetBackgroundColor(COLORREF);
	void SetTextColor(COLORREF);
	void UpdateSurface();

protected:
	HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg LRESULT OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

private:
	bool MTransparent = true;
	COLORREF MBackgroundColor = RGB(255, 255, 255);  // default is white (in case someone sets opaque without setting a color)
	COLORREF MTextColor = RGB(0, 0, 0);  // default is black. it would be more clean 
										 // to not use the color before set with SetTextColor(..), but whatever...
};