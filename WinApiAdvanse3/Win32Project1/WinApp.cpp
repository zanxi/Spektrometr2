#include ".\resource.h"
#include ".\TimerWorkSystem.h"
#include ".\GraphTaylor.h"
#include ".\stdwin.h"

#define MAX_LOADSTRING 100

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];
map<HWND, HACCEL> l_mAccelTable;

////////////////////////////////////////

ATOM MyRegisterClass(HINSTANCE hInst);
HWND InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

BOOL AddAccelerators(HWND hWnd, HACCEL hAccel)
{
	if (IsWindow(hWnd))
	{
		l_mAccelTable[hWnd] = hAccel;
		return TRUE;
	}
	return FALSE;
}

BOOL AddAccelerators(HWND hWnd, LPCTSTR accel)
{
	return AddAccelerators(hWnd, LoadAccelerators(hInst, accel));
}

BOOL AddAccelerators(HWND hWnd, int accel)
{
	return AddAccelerators(hWnd, MAKEINTRESOURCE(accel));
}

BOOL AddAccelerators(HWND hWnd)
{
	return AddAccelerators(hWnd, HACCEL(NULL));
}

void DelAccel(HWND hWnd)
{
	map<HWND, HACCEL>::iterator end = l_mAccelTable.end();
	map<HWND, HACCEL>::iterator me = l_mAccelTable.find(hWnd);

	if (me != end)
	{
		if (me->second)
		{
			DestroyAcceleratorTable(me->second);
		}

		l_mAccelTable.erase(me);
	}
}

BOOL HandleAccelWindow(map<HWND, HACCEL>::const_iterator mh, MSG& msg)
{
	const HWND& hWnd = mh->first;
	const HACCEL& hAccel = mh->second;

	if (!TranslateAccelerator(hWnd, hAccel, &msg))
	{
		if (!IsDialogMessage(hWnd, &msg))
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL HandleAccelArray(HWND hActive, MSG& msg)
{
	if (!hActive) return FALSE;
	map<HWND, HACCEL>::const_iterator mh = l_mAccelTable.find(hActive);
	if (mh != l_mAccelTable.end())
	{
		return HandleAccelWindow(mh, msg);
	}
	return FALSE;
}

WPARAM Loop()
{
	MSG msg;
	BOOL bRet = 0;
	while (bRet = GetMessage(&msg, nullptr, 0, 0))
	{
		if (bRet == -1) return 0;
		if (!HandleAccelArray(GetActiveWindow(), msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}

HICON hIcon1;               // icon handle 
POINT ptOld;                // previous cursor location 
UINT uResult;               // SetTimer's return value 
HINSTANCE hinstance;        // handle to current instance 

int APIENTRY wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_TESTUI, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);



	HWND hMainWnd = InitInstance(hInstance, nCmdShow);
	if (!hMainWnd)
	{
		return FALSE;
	}

	AddAccelerators(hMainWnd, IDC_TESTUI);
	WPARAM ret = Loop();
	DelAccel(hMainWnd);
	return (int)ret;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	
	
	//wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESTUI));
	//wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);

	wcex.hIcon = LoadIcon(hinstance, MAKEINTRESOURCE(400));
	wcex.hCursor = LoadCursor(hinstance, MAKEINTRESOURCE(200));


	wcex.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_TESTUI);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));


	return RegisterClassExW(&wcex);
}

VOID CALLBACK MyTimerProc(
	HWND hwnd,        // handle to window for timer messages 
	UINT message,     // WM_TIMER message 
	UINT idTimer,     // timer identifier 
	DWORD dwTime)     // current system time 
{

	RECT rc;
	POINT pt;

	// If the window is minimized, compare the current 
	// cursor position with the one from 10 seconds earlier. 
	// If the cursor position has not changed, move the 
	// cursor to the icon. 

	::x_end += step_x;
	TimerWorkSystem::OnTimer(::x_end);
	InvalidateRect(hwnd, NULL, FALSE);

	//if (IsIconic(hwnd))
	MessageBeep(1);
	{
		GetCursorPos(&pt);

		if ((pt.x == ptOld.x) && (pt.y == ptOld.y))
		{
			GetWindowRect(hwnd, &rc);
			//SetCursorPos(rc.left, rc.top);
		}
		else
		{
			ptOld.x = pt.x;
			ptOld.y = pt.y;
			//SetCursorPos(pt.x+100, pt.y+100);
		}
	}
}

HWND InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;

	HWND hWnd = CreateWindowW(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		nullptr,
		nullptr,
		hInstance,
		nullptr);

	if (!hWnd)
	{
		return FALSE;
	}
	
	////////////   TIMER   /////////////////
	GetCursorPos(&ptOld);
	uResult = SetTimer(hWnd,             // handle to main window 
		IDT_MOUSETRAP,                   // timer identifier 
		400,                           // 10-second interval 
		(TIMERPROC)MyTimerProc);               // no timer callback 

	if (uResult == 0)
	{
		//ErrorHandler("No timer is available.");
	}
	///////////////////////////////////////



	SendMessage(
		hWnd,
		WM_SETFONT,
		(WPARAM)GetStockObject(DEFAULT_GUI_FONT),
		1);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

INT_PTR CALLBACK DlgProc1(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		AddAccelerators(hWnd, IDR_ACCELERATOR1);
		break;
	}
	case WM_COMMAND:
	{
		if (BN_CLICKED == HIWORD(wParam) || 1 == HIWORD(wParam))
		{
			if (ID_F9 == LOWORD(wParam))
			{
				SendDlgItemMessage(hWnd, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM)TEXT("F9 PRESSED!"));
			}
			else if (IDOK == LOWORD(wParam) || IDCANCEL == LOWORD(wParam))
			{
				DestroyWindow(hWnd);
				break;
			}
		}
		break;
	}
	case WM_DESTROY:
	{
		DelAccel(hWnd);
		break;
	}

	default:
		break;
	}
	return 0;
}

INT_PTR CALLBACK DlgProc2(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		AddAccelerators(hWnd, IDR_ACCELERATOR2);
		break;
	}
	case WM_COMMAND:
	{
		if (BN_CLICKED == HIWORD(wParam) || 1 == HIWORD(wParam))
		{
			if (ID_F10 == LOWORD(wParam))
			{
				SendDlgItemMessage(hWnd, IDC_LIST2, LB_ADDSTRING, 0, (LPARAM)TEXT("F10 PRESSED!"));
			}
			else if (IDOK == LOWORD(wParam) || IDCANCEL == LOWORD(wParam))
			{
				DestroyWindow(hWnd);
				break;
			}
		}
		break;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		DelAccel(hWnd);
		break;
	}

	default:
		break;
	}
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		LPCREATESTRUCT lpcs = (LPCREATESTRUCT)lParam;
		CreateWindow(
			TEXT("BUTTON"),
			TEXT("Data from UART"),
			WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
			10, 70, 350, 20,
			hWnd, (HMENU)0x200,
			lpcs->hInstance,
			NULL
		);
		CreateWindow(
			TEXT("BUTTON"),
			TEXT("Settings"),
			WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
			360, 70, 500, 20,
			hWnd, (HMENU)0x201,
			lpcs->hInstance,
			NULL
		);
		break;
	}
	case WM_SETFONT:
	{
		EnumChildWindows(
			hWnd,
			[](HWND hwnd, LPARAM lparam)->BOOL CALLBACK
		{
			SendMessage(hwnd, WM_SETFONT, (WPARAM)lparam, TRUE);
			return TRUE;
		},
			(LPARAM)wParam
			);
		break;
	}
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst,
				MAKEINTRESOURCE(IDD_ABOUTBOX),
				hWnd,
				About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case 0x200:
		{
			HWND hDlg = CreateDialog(
				hInst,
				MAKEINTRESOURCE(IDD_DIALOG1),
				hWnd,
				DlgProc1
			);
			ShowWindow(hDlg, SW_SHOW);
			break;
		}
		case 0x201:
		{
			BOOL fIsWindowEnabled = IsWindowEnabled(hWnd);
			HWND hDlg = CreateDialog(
				hInst,
				MAKEINTRESOURCE(IDD_DIALOG2),
				hWnd,
				DlgProc2
			);
			if (hDlg)
			{
				ShowWindow(hDlg, SW_NORMAL);
				EnableWindow(hWnd, FALSE);
				Loop();
				EnableWindow(hWnd, fIsWindowEnabled);
			}
			break;
		}
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		OnPaint(hWnd);
		//InvalidateRect(hWnd, NULL, FALSE);
		break;
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		KillTimer(hWnd, IDT_MOUSETRAP);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}