#include <windows.h>
#include <Snmp.h>
#include <mgmtapi.h>
#include "resource.h"
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
    
   
    HWND newWnd = CreateDialog(NULL, MAKEINTRESOURCE(IDD_DIALOG1),NULL,NULL);
    ShowWindow(newWnd, SW_RESTORE);


    int operation = 0;
    AsnObjectIdentifier reqObject;
    RFC1157VarBindList variableBindings;
    LPSNMP_MGR_SESSION session;
    BYTE requestType;
    AsnInteger errorStatus;
    AsnInteger errorIndex;
    int i;

    variableBindings.list = NULL;
    variableBindings.len = 0;

    if (!SnmpMgrStrToOid(LPSTR(".1.3.6.1.2.1.25.3.3.1.2"), &reqObject))
    {
        return 1;
    }
    else
    {
        variableBindings.len++;
        if ((variableBindings.list = (RFC1157VarBind*)SNMP_realloc(
            variableBindings.list, sizeof(RFC1157VarBind) *
            variableBindings.len)) == NULL)
        {
            return 1;
        }

        variableBindings.list[variableBindings.len - 1].name = reqObject;
        variableBindings.list[variableBindings.len - 1].value.asnType = ASN_NULL;
    }
    if ((session = SnmpMgrOpen(LPSTR("208.87.96.30"), LPSTR("public"), 6000, 3)) == NULL)
    {
        return 1;
    }

    if (operation == 0)
        requestType = ASN_RFC1157_GETREQUEST;
    else
        requestType = ASN_RFC1157_GETNEXTREQUEST;

    i = 0;
    while (SnmpMgrRequest(session, requestType, &variableBindings, &errorStatus, &errorIndex))
    {
        // The API is indicating an error.
        if (errorStatus > 0)
        {
            break;
        }
        else
        {
            // Display the resulting variable bindings.
            UINT i;
            char* string = NULL;
            for (i = 0; i < variableBindings.len; i++)
            {
                SnmpMgrOidToStr(&variableBindings.list[i].name, &string);
                //printf("Variable = %s\n", string);
                if (string) SNMP_free(string);
                //printf("Value = ");
                SnmpUtilPrintAsnAny(&variableBindings.list[i].value);
            } // end for()
        } // end else
        i++;
        if (i == 1000)
            break;
    } // end while

    SnmpUtilVarBindListFree(&variableBindings);

    if (!SnmpMgrClose(session))
    {
        return 1;
    }


    MSG msg = { };


    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}