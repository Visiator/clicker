/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.cc to edit this template
 */

#include <thread>

#ifdef _WIN32

#define _WINSOCKAPI_ 
#include <windows.h>
#undef _WINSOCKAPI_
#include <winsock2.h>
#include <stdlib.h>
#include <iphlpapi.h>
#include <stdio.h>
#undef _WINSOCKAPI_

#endif

#include "GUI.h"
#include "../tools.h"

extern bool GLOBAL_STOP;
extern GUI gui;

//#define _WIN32

#ifdef _WIN32

    extern HINSTANCE _hInstance_;

    HWND viewer_window_hwnd = 0;

    char msg [256]={"test"};
    
    LRESULT CALLBACK MainWinProcViewer(HWND hw, UINT msg, WPARAM wp, LPARAM lp);

    LRESULT WM_CREATE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
        return DefWindowProc(hw, msg, wp, lp);
    }

    LRESULT WM_NCHITTEST_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
        return DefWindowProc(hw, msg, wp, lp);
    };
    LRESULT WM_PAINT_(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
    void GUI::execute() {
        printf("GUI::execute...\n");
        DWORD err;
        
        
        HICON hi = nullptr;

	HICON hi_sm = nullptr;
	//hi_sm = LoadIcon(app_attributes.hInstance, MAKEINTRESOURCE(101));


	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_BYTEALIGNCLIENT | CS_BYTEALIGNWINDOW;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = MainWinProcViewer;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = _hInstance_;
	wc.hIcon = hi_sm; //  LoadIcon(app_attributes.hInstance, MAKEINTRESOURCE(1055));
	wc.hIconSm = nullptr; // LoadIcon(app_attributes.hInstance, MAKEINTRESOURCE(101));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(6);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"SNIFFER2";

	if (!RegisterClassEx(&wc)) {
		fatal_error("RegisterClass error");
		return;
	};
        
        int x, y, w, h;
        x = start_position.getx(); y = start_position.gety(); w = start_position.getw(); h = start_position.geth();
        
        
        
        viewer_window_hwnd = CreateWindow(L"SNIFFER2", L"SNIFFER3", WS_OVERLAPPEDWINDOW, x, y, w, h, NULL, NULL, _hInstance_, NULL);
	//app_attributes.desktop_window_hwnd = CreateWindowEx( 0, L"VISIATOR VIEWER", L"VISIATOR", WS_OVERLAPPEDWINDOW, x, y, w, h, NULL, NULL, app_attributes.hInstance, NULL);

	if (!viewer_window_hwnd) {
                err = GetLastError();
		fatal_error("CreateWindow error");
		return;
	};
        printf("11111 %x\n", viewer_window_hwnd);
        
        ShowWindow(viewer_window_hwnd, SW_SHOWNORMAL);
	UpdateWindow(viewer_window_hwnd);
        
                MSG msg;
                printf("w1\n");
        	
		//timer();
		while (GetMessage(&msg, NULL, 0, 0) > 0 && GLOBAL_STOP == false) {
		
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	
        
        set_GLOBAL_STOP(L"GUI::execute end");
        execute_is_run = false;
        
    }
    LRESULT WM_SIZING_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
    
        return TRUE;
    }
    
    LRESULT WM_SIZE_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
        
        RECT ClientRect;

	GetClientRect(hw, &ClientRect);

	int mx = (int)LOWORD(lp);
	int my = (int)HIWORD(lp);
        
        printf("WM_SIZE_ %d %d\n", mx, my);
	gui.win32.change_size(hw, mx, my);
        gui.screen.set_size(mx, my);
        
        
        return DefWindowProc(hw, msg, wp, lp);
    }
    
    LRESULT WM_LBUTTONDOWN_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {

	int mx, my;

	mx = (int)LOWORD(lp);
	my = (int)HIWORD(lp);

	gui.MouseButtonEvent(MOUSE_BUTTON_TYPE::left, mx, my );
        printf("ButtonPress left %d:%d\n", mx, my );
        return 0;
    }    
    bool GUI::low_level_init() {
        
        printf("WIN low_level_init...\n");
        
        start_position.set_xywh(50, 50, 1000, 900);
        screen.set_size(start_position.getw(), start_position.geth());
        
        /*
        screen.set_size(start_position.getw(), start_position.geth());
        
        linux.image = XCreateImage(linux.display_,DefaultVisual(linux.display_,DefaultScreen(linux.display_)),24,ZPixmap,0,(char*) screen.buffer,start_position.getw(), start_position.geth(),32,(start_position.getw())*4);
        */
        
        execute_is_run = true;
        execute_thread = new std::thread(&GUI::execute, this);
 
        return true;
    };
    
    void GUI::finish() {
        
        
    }
    
    LRESULT CALLBACK MainWinProcViewer(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	//int w_param;

	//int ww;

	//TRACKMOUSEEVENT tme;
	//MINMAXINFO *pInfo;
	//POINT Min, Max;
	RECT rr;
	//RECT ClientRect, WindowRect;

	switch (msg) {

	case WM_CREATE:				return WM_CREATE_(hw, msg, wp, lp);
        case WM_NCHITTEST:			return WM_NCHITTEST_(hw, msg, wp, lp);
        case WM_DESTROY:

                printf("WM_DESTROY...\n");
		//GetWindowRect(hw, &rr);

		//set_GLOBAL_STOP_true(); // GLOBAL_STOP = true;

		PostQuitMessage(0);
		return TRUE;

	case WM_CLOSE:				PostMessage(hw, WM_DESTROY, 0, 0);		return TRUE;
	case WM_PAINT:				return WM_PAINT_(hw, msg, wp, lp);
        case WM_SIZING:				return WM_SIZING_(hw, msg, wp, lp);
	case WM_SIZE:				return WM_SIZE_(hw, msg, wp, lp);
        case WM_LBUTTONDOWN:                    return WM_LBUTTONDOWN_(hw, msg, wp, lp);
        /*
	case WM_TIMER:				return viewer->WM_TIMER_(hw, msg, wp, lp);
	case WM_GETMINMAXINFO:		return viewer->WM_GETMINMAXINFO_(hw, msg, wp, lp);
	case WM_CAPTURECHANGED:		return viewer->WM_CAPTURECHANGED_(hw, msg, wp, lp);
	case WM_SYSCOMMAND:			return viewer->WM_SYSCOMMAND_(hw, msg, wp, lp);
	case WM_SETFOCUS:			return viewer->WM_SETFOCUS_(hw, msg, wp, lp);
	case WM_KILLFOCUS:			return viewer->WM_KILLFOCUS_(hw, msg, wp, lp);
	case WM_ACTIVATE:			return viewer->WM_ACTIVATE_(hw, msg, wp, lp);
	case WM_ACTIVATEAPP:		return viewer->WM_ACTIVATEAPP_(hw, msg, wp, lp);
	case WM_WINDOWPOSCHANGED:	return viewer->WM_WINDOWPOSCHANGED_(hw, msg, wp, lp);
	case WM_WINDOWPOSCHANGING:	return viewer->WM_WINDOWPOSCHANGING_(hw, msg, wp, lp);
	case WM_NCACTIVATE:			return viewer->WM_NCACTIVATE_(hw, msg, wp, lp);
	case WM_MOUSEACTIVATE:		return viewer->WM_MOUSEACTIVATE_(hw, msg, wp, lp);
	case WM_SETCURSOR:			return viewer->WM_SETCURSOR_(hw, msg, wp, lp);
	case WM_NCMOUSEMOVE:		return viewer->WM_NCMOUSEMOVE_(hw, msg, wp, lp);
	case WM_MOUSEMOVE:			return viewer->WM_MOUSEMOVE_(hw, msg, wp, lp);
	case WM_MOUSELEAVE:			return viewer->WM_MOUSELEAVE_(hw, msg, wp, lp);
	case WM_NCLBUTTONDOWN:		return viewer->WM_NCLBUTTONDOWN_(hw, msg, wp, lp);
	
	case WM_NCLBUTTONUP:		return viewer->WM_NCLBUTTONUP_(hw, msg, wp, lp);
	case WM_LBUTTONUP:			return viewer->WM_LBUTTONUP_(hw, msg, wp, lp);
	case WM_RBUTTONDOWN:		return viewer->WM_RBUTTONDOWN_(hw, msg, wp, lp);
	case WM_RBUTTONUP:			return viewer->WM_RBUTTONUP_(hw, msg, wp, lp);
	case WM_ERASEBKGND:			return viewer->WM_ERASEBKGND_(hw, msg, wp, lp);
	
	case WM_MOVE:				return viewer->WM_MOVE_(hw, msg, wp, lp);
	case WM_VSCROLL:			return viewer->WM_VSCROLL_(hw, msg, wp, lp);
	case WM_HSCROLL:			return viewer->WM_HSCROLL_(hw, msg, wp, lp);
	
	case WM_MOUSEWHEEL:			return viewer->WM_MOUSEWHEEL_(hw, msg, wp, lp);
	case WM_CHAR:				return viewer->WM_CHAR_(hw, msg, wp, lp);
	case WM_KEYDOWN:			return viewer->WM_KEYDOWN_(hw, msg, wp, lp);
	case WM_KEYUP:				return viewer->WM_KEYUP_(hw, msg, wp, lp);
	case WM_SYSKEYDOWN:			return viewer->WM_SYSKEYDOWN_(hw, msg, wp, lp);
	case WM_SYSKEYUP:			return viewer->WM_SYSKEYUP_(hw, msg, wp, lp);
	


	case WM_DRAWCLIPBOARD:		return viewer->WM_DRAWCLIPBOARD_(hw, msg, wp, lp);
	case WM_CHANGECBCHAIN:		return viewer->WM_CHANGECBCHAIN_(hw, msg, wp, lp);
        */
	};

	return DefWindowProc(hw, msg, wp, lp);

};
    
LRESULT WM_PAINT_(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
    
    gui.screen.fill_all(0xffffff);
                    
    gui.elements.paint(&gui.screen);
    
    PAINTSTRUCT ps;
    HDC hdc;

    hdc = BeginPaint(hw, &ps);
    
    int liness = SetDIBitsToDevice(hdc, 0, 0
		, gui.screen.w
		, gui.screen.h
		, 0, 0, 0
		, gui.screen.h
		, gui.screen.buffer //, buf2   - 2021 -
		, gui.win32.bmInfo
		, DIB_RGB_COLORS);
    
    EndPaint(hw, &ps);

    return 0;
}

#endif