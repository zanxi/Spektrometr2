#include ".\CStaticExt.h"

void TColorText::setTransparent(bool ATransparent)
{
	MTransparent = ATransparent;
	UpdateSurface();
	//Invalidate();
}

void TColorText::SetBackgroundColor(COLORREF AColor)
{
	MBackgroundColor = AColor;
	MTransparent = false;
	UpdateSurface();
	//Invalidate();
}

void TColorText::UpdateSurface()
{
	CRect(rc);
	//rc = GetClientRect();
	GetWindowRect(rc);
	RedrawWindow();

	GetParent()->ScreenToClient(rc);
	GetParent()->InvalidateRect(rc, TRUE);
	GetParent()->UpdateWindow();
}


void TColorText::SetTextColor(COLORREF AColor)
{
	MTextColor = AColor;
	//Invalidate();
	UpdateSurface();
	//UpdateColors();
}

//LRESULT TColorText::OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//{

	//InvalidateRect(this->pdc,0,0);
//}


BEGIN_MESSAGE_MAP(TColorText, CStatic)
	ON_WM_CTLCOLOR_REFLECT()
	//ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


HBRUSH TColorText::CtlColor(CDC* pDC, UINT nCtlColor)
{
	pDC->SetTextColor(MTextColor);
	pDC->SetBkMode(TRANSPARENT);  // we do not want to draw background when drawing text. 
								  // background color comes from drawing the control background.
	if (MTransparent)
		return nullptr;  // return nullptr to indicate that the parent object 
						 // should supply the brush. it has the appropriate background color.
	else
		return (HBRUSH)CreateSolidBrush(MBackgroundColor);  // color for the empty area of the control
}