#include <windows.h>
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    const wchar_t CLASS_NAME[] = L"OpenEggplants";
    WNDCLASS wc = { };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;


    RegisterClass(&wc);


    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"OpenEggplants main",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    NOTIFYICONDATAW notify = {};
    notify.hWnd = hwnd;
    //LPCWSTR PATH = L"C:\\Users\\Eggplantsjz\\source\\repos\\start\\start\\1.jpg";
    notify.cbSize = sizeof(NOTIFYICONDATA);
    notify.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    notify.uFlags = NIF_INFO | NIF_ICON;
    notify.uTimeout = 10 * 1000;
    wcscpy_s(notify.szInfo, L"OpenEggplants Start");
    wcscpy_s(notify.szInfoTitle, L"Run");
    PNOTIFYICONDATA pnotify = &notify;
    int ret  =  Shell_NotifyIconW(NIM_ADD, pnotify);
    


    MSG msg = { };


    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}