#pragma once

#ifdef __windows__
#undef __windows__
#endif

#include ".\stdwin.h"
#include ".\DataStruct.h"

const double epsilon = 0.0000001;
const double pi = 355.0 / 113.0;

double step_x = 0.505;
double x_start = -2-0 * pi;
double x_end = x_start+step_x;
int pix_widthx = 500;
int pix_width_data = 350;
int pix_heighty = 400;


double sin_e(double arg, double eps)
{
	return sin(arg);

	double result = 0, rn = arg;
	for (long i = 1;; i++) 
	{
		if (rn<eps && rn>-eps) return result;
		result += rn;
		rn = -rn*arg*arg / (i + 1) / (i+2);
		i++;

	}

	return 0;
}

double cos_e(double arg, double eps)
{
	double result = 0, rn = 1;
	for (int i = 1;; i++)
	{
		if (rn<eps && rn>-eps) return result;
		result += rn;
		rn = -rn*arg*arg / (i + 1) / (i + 2);
		i++;

	}
}

void DrawAxis(HDC hdc, RECT rectClient) {
	HPEN penGraph = CreatePen(PS_SOLID,2,RGB(0,0,255));
	HGDIOBJ gdiOld = SelectObject(hdc, penGraph);
	MoveToEx(hdc, 0, rectClient.bottom/2,NULL);
	LineTo(hdc, rectClient.right, rectClient.bottom/2);
	LineTo(hdc, rectClient.right - 5, rectClient.bottom / 2 + 2);
	MoveToEx(hdc, rectClient.right / 2, rectClient.bottom/2, NULL);
	LineTo(hdc, rectClient.right - 5, rectClient.bottom / 2 - 2);
	MoveToEx(hdc, rectClient.right / 2, rectClient.bottom / 2, NULL);
	LineTo(hdc, rectClient.right/2, rectClient.top);
	LineTo(hdc, rectClient.right / 2 - 2, rectClient.top+5);
	MoveToEx(hdc, rectClient.right / 2, rectClient.top, NULL);
	LineTo(hdc, rectClient.right / 2 +2, rectClient.top + 5);
	SelectObject(hdc,gdiOld);
}

double f(double arg, double eps)
{
	double y=0;
	for (int i = 1; i < 4; i++) 
	{
		y += i*sin_e(i*pi*arg, eps)+ 0*cos_e(i*arg, eps);
	}
	//y = sin_e(arg, eps);
	return y;
	//return sin_e(arg, eps) + cos_e(arg,eps);
}

double minmax(double a, double b, double h, double& ymin, double& ymax)
{	
	double x_current = a;		
	ymin = f(a, epsilon); ymax = f(a, epsilon);
	while (x_current < b)
	{
		x_current += h;
		if(ymin>f(x_current,epsilon)) ymin = f(x_current, epsilon);
	    if(ymax < f(x_current, epsilon))ymax = f(x_current, epsilon);
	}
	return 0;	
}

double minmax(map<double, double> dataCom, double& ymin, double& ymax)
{
	std::map<double, double>::iterator it = dataCom.begin();

	ymin = it->second;
    ymax = it->second;

	for (it = dataCom.begin(); it != dataCom.end(); ++it)
	{		
		if (ymin > it->second) ymin = it->second;
		if (ymax < it->second)ymax = it->second;
	}
	return 0;
}


void DrawGraph(HDC hdc, RECT rectClient)
{
	BitBlt(hdc, 0, 0, rectClient.right, rectClient.bottom, 0, 0, 0, WHITENESS);

	TextOut(hdc, 30, 100, "a: ", strlen("a: "));
	TextOut(hdc, 170, 100, std::to_string(x_start).c_str(), strlen(std::to_string(x_start).c_str()));

	TextOut(hdc, 30, 120, "b: ", strlen("b: "));
	TextOut(hdc, 170, 120, std::to_string(x_end).c_str(), strlen(std::to_string(x_end).c_str()));

	HPEN penGraph = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	HGDIOBJ gdiOld = SelectObject(hdc, penGraph);

	//if (Data::DataRS.size < 3) 
	{
		//return;
	}

	
	double x_current = 0;
	//double step = (x_end - x_start) / pix_widthx;

	double ymin = 0, ymax = 0;
	//minmax(x_start, x_end, step_x, ymin, ymax);
	std::map<double, double>::iterator it = Data::DataRS.begin();
	if (it == Data::DataRS.end())return;
	x_start = it->first;
	//x_end = Data::DataRS.end()->first;
	minmax(Data::DataRS, ymin, ymax);
	double y_start = Data::DataRS.begin()->first;


	TextOut(hdc, 30, 140, "ymin: ", strlen("ymin: "));
	TextOut(hdc, 170, 140, std::to_string(ymin).c_str(), strlen(std::to_string(ymin).c_str()));

	TextOut(hdc, 30, 160, "ymax: ", strlen("ymax: "));
	TextOut(hdc, 170, 160, std::to_string(ymax).c_str(), strlen(std::to_string(ymax).c_str()));

	//TextOut(hdc, 30, 180, "num pixels: ", strlen("num pixels: "));
	//TextOut(hdc, 170, 180, std::to_string(pix_widthx).c_str(), strlen(std::to_string(pix_widthx).c_str()));
	//MoveToEx(hdc, 0, int(-y_start/step) + rectClient.bottom/2,NULL);
	MoveToEx(hdc, pix_width_data, int((y_start - ymin) / (ymax - ymin) * rectClient.bottom) + 1, NULL);
	long num_pix = 0;	
		
	for (it = Data::DataRS.begin(); it != Data::DataRS.end(); ++it)
	{		
		num_pix++;
		x_current += it->first;
		double y_next = it->second;
		int xc = pix_width_data + int((x_current - x_start) / (x_end - x_start) * (rectClient.right - pix_width_data)) + 1;
		int yc = int((y_next - ymin) / (ymax - ymin) * rectClient.bottom) + 1;
		LineTo(hdc, xc, yc);
	}

	TextOut(hdc, 30, 180, "num pixels: ", strlen("num pixels: "));
	TextOut(hdc, 170, 180, std::to_string(num_pix).c_str(), strlen(std::to_string(num_pix).c_str()));

	TextOut(hdc, 30, 200, "velocity (ms) - 1 value: ", strlen("velocity (ms) - 1 value: "));
	TextOut(hdc, 190, 200, std::to_string(100).c_str(), strlen(std::to_string(100).c_str()));
	
	SelectObject(hdc, gdiOld);
}


void OnPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	RECT rectClient;
	HDC hdc = BeginPaint(hwnd, &ps);
	GetClientRect(hwnd, &rectClient);	

	DrawGraph(hdc, rectClient);

	//DrawAxis(hdc, rectClient);
	//ValidateRect(hwnd, NULL);
	EndPaint(hwnd, &ps);

	/*
	CPaintDC clientDC(this);
	CDC memDC;
	memDC.CreateCompatibleDC(&clientDC);
	memDC.SelectObject(&backgroundBitmap);
	BITMAP bmp;
	backgroundBitmap.GetBitmap(&bmp);
	clientDC.BitBlt(0,0,bmp.bmWidth, bmp.bmHeight,&memDC,0,0,SRCCOPY);
	/**/

}


class CMainWnd : public CFrameWnd
{
public:
	CMainWnd();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP();

};

afx_msg void CMainWnd::OnPaint() 
{
	CPaintDC paintDC(this);
	paintDC.TextOutA(25,25,CString("Use OnPaint()"));
}

BEGIN_MESSAGE_MAP(CMainWnd, CFrameWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()
/**/

class A {
	int i;
public: 
	A(int i):i(i){}
};

/**/
