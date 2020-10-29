#include "Built_Options.h"
#include "Platform.h"

#include "WindowManager.h"
#include <string>
#include <assert.h>
#include "VulkanManager.h"
using namespace VulkanCoreSpace;

#if VK_USE_PLATFORM_WIN32_KHR

// Microsoft Windows specific versions of window functions
LRESULT CALLBACK WindowsEventHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    WindowManager * window = reinterpret_cast<WindowManager*>(
        GetWindowLongPtrW(hWnd, GWLP_USERDATA));

    switch (uMsg) {
    case WM_CLOSE:
        window->Close();
        return 0;
    case WM_SIZE:
        // we get here if the window has changed size, we should rebuild most
        // of our window resources before rendering to this window again.
        // ( no need for this because our window sizing by hand is disabled )
        break;
    default:
        break;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

uint64_t	WindowManager::_win32_class_id_counter = 0;

void WindowManager::_InitOSWindow()
{
    WNDCLASSEX win_class{};
    assert(_surface_size_x > 0);
    assert(_surface_size_y > 0);

    _win32_instance = GetModuleHandle(nullptr);
    _win32_class_name = _window_name + "_" + std::to_string(_win32_class_id_counter);
    _win32_class_id_counter++;

    string message = _win32_class_name.c_str();
    std::wstring w_win32_class_name = std::wstring(message.begin(), message.end());

    // Initialize the window class structure:
    win_class.cbSize = sizeof(WNDCLASSEX);
    win_class.style = CS_HREDRAW | CS_VREDRAW;
    win_class.lpfnWndProc = WindowsEventHandler;
    win_class.cbClsExtra = 0;
    win_class.cbWndExtra = 0;
    win_class.hInstance = _win32_instance; // hInstance
    win_class.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    win_class.hCursor = LoadCursor(NULL, IDC_ARROW);
    win_class.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);// WHITE_BRUSH);
    win_class.lpszMenuName = NULL;
    win_class.lpszClassName = w_win32_class_name.c_str();
    win_class.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
    // Register window class:
    if (!RegisterClassEx(&win_class)) {
        // It didn't work, so try to give a useful error:
        assert(0 && "Cannot create a window in which to draw!\n");
        fflush(stdout);
        std::exit(-1);
    }

    DWORD ex_style = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

    message = _window_name.c_str();
    std::wstring w_window_name = std::wstring(message.begin(), message.end());

    // Create window with the registered class:
    RECT wr = { 0, 0, LONG(_surface_size_x), LONG(_surface_size_y) };
    AdjustWindowRectEx(&wr, style, FALSE, ex_style);
    _win32_window = CreateWindowEx(0,
        w_win32_class_name.c_str(),		// class name
        w_window_name.c_str(),			// app name
        style,							// window style
        CW_USEDEFAULT, CW_USEDEFAULT,	// x/y coords
        wr.right - wr.left,				// width
        wr.bottom - wr.top,				// height
        NULL,							// handle to parent
        NULL,							// handle to menu
        _win32_instance,				// hInstance
        NULL);							// no extra parameters
    if (!_win32_window) {
        // It didn't work, so try to give a useful error:
        assert(1 && "Cannot create a window in which to draw!\n");
        fflush(stdout);
        std::exit(-1);
    }
    SetWindowLongPtr(_win32_window, GWLP_USERDATA, (LONG_PTR)this);

    ShowWindow(_win32_window, SW_SHOW);
    SetForegroundWindow(_win32_window);
    SetFocus(_win32_window);
}

void WindowManager::_DeInitOSWindow()
{
    string message = _win32_class_name.c_str();
    std::wstring w_win32_class_name = std::wstring(message.begin(), message.end());

    DestroyWindow(_win32_window);
    UnregisterClass(w_win32_class_name.c_str(), _win32_instance);
}

void WindowManager::_UpdateOSWindow()
{
    MSG msg;
    if (PeekMessage(&msg, _win32_window, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void WindowManager::_InitOSSurface()
{
    VkWin32SurfaceCreateInfoKHR create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    create_info.hinstance = _win32_instance;
    create_info.hwnd = _win32_window;
    vkCreateWin32SurfaceKHR(VulkanManager::GetSingleton()->GetVulkanInstance(), &create_info, nullptr, &_surface);
}

#endif