// Interfaz.cpp : Define el punto de entrada de la aplicación.
//
#include <windows.h>
#include <windowsx.h>
#include "framework.h"
#include "Interfaz.h"
#include "parsermet_yac.h"

#pragma comment(lib, "comctl32.lib")
#define MAX_LOADSTRING 100
#define UNICODE

#define MAX_LOADSTRING 100

// Variables globales:
HINSTANCE hInst;                                // instancia actual
WCHAR szTitle[MAX_LOADSTRING];                  // Texto de la barra de título
WCHAR szWindowClass[MAX_LOADSTRING];            // nombre de clase de la ventana principal

HWND hWndToolBar;                               //Variable para la barra de herramientas
const int NUMBUTTONS = 3;                       //Numero de botones en la barra,
extern int yyparse();
extern FILE* yyin;
extern FILE* yyout;
int analisis_lexico_exitoso = 0;

//Esta función determina el tamaño del archivo
long PopFileLength(FILE* file)
{
    int iCurrentPos, iFileLength;

    iCurrentPos = ftell(file);
    fseek(file, 0, SEEK_END);
    iFileLength = ftell(file);
    fseek(file, iCurrentPos, SEEK_SET);

    return iFileLength;
}


// Declaraciones de funciones adelantadas incluidas en este módulo de código:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int extraerLineaDeError(const wchar_t* mensaje)
{
    int linea = -1;
    swscanf(mensaje, L"Error (Linea %d):", &linea);
    return linea;
}

int extraerLineaDeAdvertencia(const wchar_t* mensaje)
{
    int linea = -1;
    swscanf(mensaje, L"Advertencia (Linea %d):", &linea);
    return linea;
}


void navegarALineaEnEdit(HWND hWndEdit, int linea)
{
    int pos = SendMessage(hWndEdit, EM_LINEINDEX, (WPARAM)(linea - 1), 0);
    if (pos != -1)
    {
        SendMessage(hWndEdit, EM_SETSEL, pos, pos);
        SetFocus(hWndEdit);
    }
}

void ConvertirCharAWcharT(const char* cadena, wchar_t* resultado, int tamanoResultado) {
    int longitud = MultiByteToWideChar(CP_UTF8, 0, cadena, -1, resultado, tamanoResultado);
    if (longitud > 0) {
        resultado[longitud - 1] = L'\0'; // Asegura que la cadena terminada en nulo
    }
    else {
        wcscpy(resultado, L"Error"); // Manejo de errores
    }
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);


    // Inicializar cadenas globales
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_INTERFAZ, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Realizar la inicialización de la aplicación:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_INTERFAZ));

    MSG msg;

    // Bucle principal de mensajes:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}



//
//  FUNCIÓN: MyRegisterClass()
//
//  PROPÓSITO: Registra la clase de ventana.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_INTERFAZ));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_INTERFAZ);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCIÓN: InitInstance(HINSTANCE, int)
//
//   PROPÓSITO: Guarda el identificador de instancia y crea la ventana principal
//
//   COMENTARIOS:
//
//        En esta función, se guarda el identificador de instancia en una variable común y
//        se crea y muestra la ventana principal del programa.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Almacenar identificador de instancia en una variable global

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1350, 900, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    //Codigo para crear la barra de herramientas:

    INITCOMMONCONTROLSEX InitCtrlEx;

    InitCtrlEx.dwSize = sizeof(INITCOMMONCONTROLSEX);
    InitCtrlEx.dwICC = ICC_BAR_CLASSES;
    InitCommonControlsEx(&InitCtrlEx);  //---------------------------------P O S I B L E   E R R O R -----

    TBBUTTON tbrButtons[3];

    tbrButtons[0].iBitmap = 0;
    tbrButtons[0].idCommand = ID_BTNANALIZAR;
    tbrButtons[0].fsState = TBSTATE_ENABLED;
    tbrButtons[0].fsStyle = TBSTYLE_BUTTON;
    tbrButtons[0].dwData = 0L;
    tbrButtons[0].iString = (INT_PTR)L"Analizar";

    tbrButtons[1].iBitmap = 1;
    tbrButtons[1].idCommand = ID_BTNEJECUTAR;
    tbrButtons[1].fsState = TBSTATE_ENABLED;
    tbrButtons[1].fsStyle = TBSTYLE_BUTTON;
    tbrButtons[1].dwData = 0L;
    tbrButtons[1].iString = (INT_PTR)L"Ejecutar";

    tbrButtons[2].iBitmap = 2;
    tbrButtons[2].idCommand = IDM_EXIT;
    tbrButtons[2].fsState = TBSTATE_ENABLED;
    tbrButtons[2].fsStyle = TBSTYLE_BUTTON;
    tbrButtons[2].dwData = 0L;
    tbrButtons[2].iString = (INT_PTR)L"Salir";

    hWndToolBar = CreateToolbarEx(hWnd, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | CCS_TOP | CCS_NODIVIDER | TBSTYLE_FLAT, IDB_TOOLBAR, NUMBUTTONS, hInstance, IDB_TOOLBAR, tbrButtons, NUMBUTTONS, 0, 0, 0, 0, sizeof(TBBUTTON));
    //Se establece el tamanio de los iconos para cada boton:
    SendMessage(hWndToolBar, TB_SETBITMAPSIZE, 0, MAKELPARAM(50, 45));

    //Se reajusta el tamanio de la barra de herramientas:
    SendMessage(hWndToolBar, TB_AUTOSIZE, 0, 0L);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  FUNCIÓN: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PROPÓSITO: Procesa mensajes de la ventana principal.
//
//  WM_COMMAND  - procesar el menú de aplicaciones
//  WM_PAINT    - Pintar la ventana principal
//  WM_DESTROY  - publicar un mensaje de salida y volver
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hWndEdit;
    static HWND hWndResultados;
    static HWND hStatus;
    static HWND hWndErrores;
    DWORD dwEVM;
    HFONT hFont;
    HFONT hFontR; //Para resultado
    HFONT hFontER; //Para errores
    HFONT hFontStatus;

    TCHAR* ptchBuffer = NULL;
    static FILE* entrada;
    static FILE* salida;
    static wchar_t palabra_w[64];

    switch (message)
    {
    case WM_CREATE:
    {
        LoadLibrary(L"riched20.dll");

        //Area de edicion
        hWndEdit = CreateWindowEx(
            WS_EX_CLIENTEDGE,
            RICHEDIT_CLASS, 
            L"",
            WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_WANTRETURN,
            0, 0, 0, 0, 
            hWnd,
            (HMENU)ID_EDITRICH,
            hInst,
            NULL
        );

        //Area de resultados
        hWndResultados = CreateWindowEx(
            WS_EX_CLIENTEDGE,       // Estilo extendido
            RICHEDIT_CLASS,         // Clase del control (RichEdit)
            L"",                    // Texto inicial (vacío)
            WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_READONLY | ES_NOHIDESEL, // Estilos
            0, 0, 0, 0,             // Posición y tamaño (se ajustará en WM_SIZE)
            hWnd,                   // Ventana padre
            (HMENU)ID_RESULTADOS,   // Identificador del control
            hInst,                  // Instancia de la aplicación
            NULL                    // Parámetros adicionales
        );

        //Area de errores
        hWndErrores = CreateWindow(L"listbox", NULL, WS_CHILDWINDOW | WS_VISIBLE
            | WS_VSCROLL | WS_HSCROLL | LBS_STANDARD,
            0, 0, 0, 0,
            hWnd, (HMENU)ID_ERRORES, hInst, NULL);

        //Barra de estado
        hStatus = CreateWindowEx(0, STATUSCLASSNAME, NULL, WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP, 0, 0, 0, 0, hWnd, (HMENU)IDB_STATUS, hInst, NULL);
        dwEVM = SendMessage(hWndEdit, EM_GETEVENTMASK, 0, 0);
        dwEVM |= ENM_UPDATE | ENM_CHANGE | ENM_SELCHANGE;
        SendMessage(hWndEdit, EM_SETEVENTMASK, 0, dwEVM);

        //Fuente de edicion
        hFont = CreateFont(24, 0, 0, 0, 0, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, L"Consolas");
        SendMessage(hWndEdit, WM_SETFONT, (WPARAM)hFont, 0);

        //Fuente para los resultados
        hFontR = CreateFont(12, 0, 0, 0, 0, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, L"Segoe UI");
        SendMessage(hWndResultados, WM_SETFONT, (WPARAM)hFont, 0);

        //Fuente para los errores
        hFontER = CreateFont(20, 0, 0, 0, 0, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            DEFAULT_PITCH, L"Courier New");

        SendMessage(hWndErrores, WM_SETFONT, (WPARAM)hFontER, 0);

        //Fuente para el status de la linea actual
        hFontStatus = CreateFont(22, 0, 0, 0, 0, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            DEFAULT_PITCH, L"Segoe UI");

        SendMessage(hStatus, WM_SETFONT, (WPARAM)hFontStatus, 0);

        SetFocus(hWndEdit); //Le da el foco al area de edicion
    }
    break;

    case WM_SIZE:
    {
        RECT rect;

        SendDlgItemMessage(hWnd, IDB_STATUS, WM_SIZE, 0, 0);
        GetWindowRect(hStatus, &rect);
        int width = LOWORD(lParam);
        int height = HIWORD(lParam);

        // Dividir la ventana en dos áreas (50% para cada una)
        int mitad = width / 2;

        // Redimensionar el área de edición
        MoveWindow(hWndEdit, 0, 70, mitad - (mitad/4), height / 2 - 50, TRUE);

        // Redimensionar el área de resultados
        MoveWindow(hWndResultados, mitad - (mitad/4), 70, mitad + (mitad/4), height, TRUE);
        MoveWindow(hWndErrores, 0, height / 2 + 20, mitad - (mitad/4), height / 2, TRUE);

        // Redimensionar la barra de estado
        SendMessage(hStatus, WM_SIZE, 0, 0);

        CHARFORMAT charFormat = { 0 }; 
        charFormat.cbSize = sizeof(CHARFORMAT);
        charFormat.dwMask = CFM_COLOR; 
        charFormat.crTextColor = RGB(40, 40, 40);

        // Colores para la ventana de edición (hWndEdit)
        SendMessage(hWndEdit, EM_SETBKGNDCOLOR, 0, (LPARAM)RGB(240, 240, 240));
        SendMessage(hWndEdit, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&charFormat); 

        // Colores para la ventana de edición (hWndEdit)
        SendMessage(hWndResultados, EM_SETBKGNDCOLOR, 0, (LPARAM)RGB(240, 240, 240));
        SendMessage(hWndResultados, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&charFormat);

        /*
        // Colores para el área de resultados (hWndResultados)
        SendMessage(hWndResultados, WM_CTLCOLORSTATIC, (WPARAM)GetDC(hWndResultados), (LPARAM)hWndResultados);
        SetTextColor(GetDC(hWndResultados), RGB(40, 40, 40));
        SetBkColor(GetDC(hWndResultados), RGB(255, 255, 255));
        */
        charFormat = { 0 };
        charFormat.cbSize = sizeof(CHARFORMAT);
        charFormat.dwMask = CFM_COLOR;
        charFormat.crTextColor = RGB(255, 69, 0);

        // Colores para la ventana de edición (hWndEdit)
        SendMessage(hWndErrores, EM_SETBKGNDCOLOR, 0, (LPARAM)RGB(240, 240, 240));
        SendMessage(hWndErrores, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&charFormat);

        /*
        // Colores para el área de errores (hWndErrores)
        SendMessage(hWndErrores, WM_CTLCOLORSTATIC, (WPARAM)GetDC(hWndErrores), (LPARAM)hWndErrores);
        SetTextColor(GetDC(hWndErrores), RGB(255, 69, 0));  // Texto en naranja para destacar errores
        SetBkColor(GetDC(hWndErrores), RGB(240, 240, 240));  // Fondo gris claro
        */

        // Configurar los colores del control de la barra de estado
        SendMessage(hStatus, SB_SETTEXT, 0, (LPARAM)L"Listo");
    }
    break;

    case WM_CTLCOLOREDIT:  // Manejador del mensaje de pintura para Edit Controls
    {
        // Cambiar el color de texto en el control de edición
        if ((HWND)lParam == hWndEdit)
        {
            HDC hdc = (HDC)wParam;

            // Establecer color de texto (gris oscuro)
            SetTextColor(hdc, RGB(40, 40, 40));  // Texto oscuro

            // Establecer color de fondo (gris claro)
            SetBkColor(hdc, RGB(240, 240, 240));  // Fondo gris claro

            // Retornar un pincel para el fondo
            return (LRESULT)GetStockObject(DC_BRUSH);
        }

        // Cambiar colores para la ventana de resultados
        if ((HWND)lParam == hWndResultados)
        {
            HDC hdc = (HDC)wParam;

            // Establecer color de texto (gris oscuro)
            SetTextColor(hdc, RGB(40, 40, 40));  // Texto oscuro

            // Establecer color de fondo (blanco)
            SetBkColor(hdc, RGB(255, 255, 255));  // Fondo blanco

            // Retornar un pincel para el fondo
            return (LRESULT)GetStockObject(DC_BRUSH);
        }

        // Cambiar colores para la ventana de errores
        if ((HWND)lParam == hWndErrores)
        {
            HDC hdc = (HDC)wParam;

            // Establecer color de texto (naranja)
            SetTextColor(hdc, RGB(255, 69, 0));  // Texto en naranja para errores

            // Establecer color de fondo (gris claro)
            SetBkColor(hdc, RGB(240, 240, 240));  // Fondo gris claro

            // Retornar un pincel para el fondo
            return (LRESULT)GetStockObject(DC_BRUSH);
        }
    }
    break;


    case WM_NOTIFY: //Aqui­ se detecta en que li­nea se esta
    {

        NMHDR* pHdr = (NMHDR*)lParam;
        TCHAR szPosCur[MAX_LOADSTRING];
        if (pHdr->hwndFrom == hWndEdit) {
            if (pHdr->code == EN_SELCHANGE) {
                wsprintf(szPosCur, L"Linea: %d", 1 + (WORD)SendMessage(hWndEdit, EM_LINEFROMCHAR, -1, 0L));
                SendDlgItemMessage(hWnd, IDB_STATUS, SB_SETTEXT, 0, (LPARAM)szPosCur);
            }
        }
    }
    break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Analizar las selecciones de menú:
        switch (wmId)
        {
        case ID_AYUDANT: {
            LPCWSTR rutaEjecutable = L"mupdf.exe";  // Ruta del .exe
            LPCWSTR rutaArchivo = L"ayuda_newton.pdf";      // Archivo como argumento

            ShellExecute(NULL, L"open", rutaEjecutable, rutaArchivo, NULL, SW_SHOW);
        }
        case ID_AYUDAPF: {
            LPCWSTR rutaEjecutable = L"mupdf.exe";  // Ruta del .exe
            LPCWSTR rutaArchivo = L"ayuda_newton.pdf";      // Archivo como argumento

            ShellExecute(NULL, L"open", rutaEjecutable, rutaArchivo, NULL, SW_SHOW);
        }
        break;
        case ID_AYUDAFUNC: {
            LPCWSTR rutaEjecutable = L"mupdf.exe";  // Ruta del .exe
            LPCWSTR rutaArchivo = L"ayuda_newton.pdf";      // Archivo como argumento

            ShellExecute(NULL, L"open", rutaEjecutable, rutaArchivo, NULL, SW_SHOW);
        }
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
        break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
        break;
        case ID_ERRORES:
        {
            if (HIWORD(wParam) == LBN_DBLCLK) // Verificar si es doble clic en la lista
            {
                wchar_t szItemLista[256];  // Buffer donde se almacenará el texto del error
                int i = SendMessage(hWndErrores, LB_GETCURSEL, 0, 0); // Obtener índice seleccionado

                if (i != LB_ERR) // Verificar que haya una selección válida
                {
                    SendMessage(hWndErrores, LB_GETTEXT, i, (LPARAM)szItemLista); // Obtener el texto del error
                    MessageBox(hWnd, szItemLista, L"Aviso", MB_OK);  // Mostrar el mensaje

                    int errorLine = extraerLineaDeError(szItemLista); // Extraer número de línea del error

                    if (errorLine != -1)
                    {
                        navegarALineaEnEdit(hWndEdit, errorLine);

                        DWORD dwStart = SendMessage(hWndEdit, EM_LINEINDEX, errorLine - 1, 0);
                        DWORD dwEnd = SendMessage(hWndEdit, EM_LINEINDEX, errorLine, 0);

                        // Seleccionar la línea
                        SendMessage(hWndEdit, EM_SETSEL, dwStart, dwEnd);

                        // Cambiar el color de fondo de la selección
                        CHARRANGE charRange;
                        charRange.cpMin = dwStart;
                        charRange.cpMax = dwEnd;
                        SendMessage(hWndEdit, EM_EXSETSEL, 0, (LPARAM)&charRange);
                    }
                    else{
                        errorLine = extraerLineaDeAdvertencia(szItemLista);
                        if (errorLine != -1) {
                            navegarALineaEnEdit(hWndEdit, errorLine);

                            DWORD dwStart = SendMessage(hWndEdit, EM_LINEINDEX, errorLine - 1, 0);
                            DWORD dwEnd = SendMessage(hWndEdit, EM_LINEINDEX, errorLine, 0);

                            // Seleccionar la línea
                            SendMessage(hWndEdit, EM_SETSEL, dwStart, dwEnd);

                            // Cambiar el color de fondo de la selección
                            CHARRANGE charRange;
                            charRange.cpMin = dwStart;
                            charRange.cpMax = dwEnd;
                            SendMessage(hWndEdit, EM_EXSETSEL, 0, (LPARAM)&charRange);
                        }
                    }
                }
            }
        }
        break;

        case ID_ARCHIVO_ABRIR:
        {
            TCHAR szFile[MAX_PATH], szCaption[64 + _MAX_FNAME + _MAX_EXT];
            ZeroMemory(szFile, MAX_PATH);
            OPENFILENAME ofn;
            ZeroMemory(&ofn, sizeof(OPENFILENAME));
            ofn.lStructSize = sizeof(OPENFILENAME);
            ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST |
                OFN_HIDEREADONLY | OFN_CREATEPROMPT;
            ofn.hwndOwner = hWnd;
            ofn.lpstrFilter = _T("Tipos de formatos soportados(*.txt)\0 * .txt\0Texto(*.txt)\0\0");
            ofn.lpstrTitle = _T("Abrir archivo de texto");
            ofn.lpstrFile = szFile; ofn.nMaxFile = MAX_PATH;
            if (IDOK == GetOpenFileName(&ofn)) {
                wsprintf(szCaption, _T("%s - %s"), szTitle, szFile[0] ? szFile : _T("Sin archivo abierto"));
                SetWindowText(hWnd, szCaption);
                FILE* file;
                int		iLength;
                PSTR	pstrBuffer;
                char	cFile[MAX_PATH];
                TCHAR* ptchBuffer;
                wcstombs(cFile, szFile, MAX_PATH);
                if (NULL == (file = fopen(cFile, "rb"))) {
                    MessageBox(hWnd, L"Error al leer el archivo", L"Error", MB_OK | MB_ICONERROR);
                }
                else {
                    iLength = PopFileLength(file);
                    if (NULL == (pstrBuffer = (PSTR)malloc(sizeof(char) * (iLength + 1))) ||
                        NULL == (ptchBuffer = (TCHAR*)malloc(sizeof(TCHAR) * (iLength + 1)))) {
                        fclose(file);
                        MessageBox(hWnd, L"Error al reservar memoria", L"Error", MB_OK | MB_ICONERROR);
                    }
                    else {
                        fread(pstrBuffer, 1, iLength, file);
                        fclose(file);
                        pstrBuffer[iLength] = '\0';
                        mbstowcs(ptchBuffer, pstrBuffer, iLength + 1);
                        SetWindowText(hWndEdit, ptchBuffer);
                        free(pstrBuffer);
                        free(ptchBuffer);
                    }
                }
            }
        }
        break;
        case ID_ARCHIVO_GUARDAR:
        {
            TCHAR szFile[MAX_PATH];
            ZeroMemory(szFile, MAX_PATH);
            OPENFILENAME ofn;
            ZeroMemory(&ofn, sizeof(OPENFILENAME));
            ofn.lStructSize = sizeof(OPENFILENAME);
            ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
            ofn.hwndOwner = hWnd;
            ofn.lpstrFilter = _T("Tipos de formatos soportados(*.txt)\0 * .txt\0Texto(*.txt)\0\0");
            ofn.lpstrTitle = _T("Guardar archivo de texto");
            ofn.lpstrFile = szFile;
            ofn.nMaxFile = MAX_PATH;
            if (IDOK == GetSaveFileName(&ofn)) {
                FILE* file;
                int     iLength;
                PSTR	pstrBuffer;
                char	cFile[MAX_PATH];
                TCHAR* ptchBuffer = NULL;
                wcstombs(cFile, szFile, MAX_PATH);

                if (NULL == (file = fopen(cFile, "wb"))) {
                    MessageBox(hWnd, L"Error al crear el archivo", L"Error", MB_OK | MB_ICONERROR);
                }
                else {
                    iLength = GetWindowTextLength(hWndEdit);
                    if (NULL == (pstrBuffer = (PSTR)malloc(sizeof(char) * (iLength + 1))) ||
                        NULL == (ptchBuffer = (TCHAR*)malloc(sizeof(TCHAR) * (iLength + 1))))
                    {
                        MessageBox(hWnd, L"Error al reservar memoria", L"Error", MB_OK | MB_ICONERROR);
                        fclose(file);
                    }
                    else {
                        GetWindowText(hWndEdit, ptchBuffer, iLength + 1);
                        wcstombs(pstrBuffer, ptchBuffer, iLength + 1);
                        fwrite(pstrBuffer, 1, iLength + 1, file);
                        free(pstrBuffer);
                        free(ptchBuffer);
                    }
                    fclose(file);
                }
            }
        }
        break;
        case ID_BTNEJECUTAR: {
            RECT rect;
            GetWindowRect(hWnd, &rect);

            // Abrir el archivo resultados.txt
            FILE* res = fopen("resultados.txt", "r");
            if (res == NULL) {
                MessageBox(hWnd, L"No se pudo abrir el archivo resultados.txt", L"Error", MB_OK | MB_ICONERROR);
                break;
            }

            // Obtener el tamaño del archivo
            fseek(res, 0, SEEK_END);
            long resSize = ftell(res);
            rewind(res);

            // Leer el contenido del archivo
            char* buffer = (char*)calloc(resSize + 1, sizeof(char));
            if (!buffer) {
                MessageBox(hWnd, L"Error al asignar memoria", L"Error", MB_OK | MB_ICONERROR);
                fclose(res);
                break;
            }

            fread(buffer, 1, resSize, res);
            buffer[resSize] = '\0'; // Asegurar terminación nula
            fclose(res);

            // Convertir el contenido a wchar_t
            wchar_t* wtext = (wchar_t*)calloc(resSize + 1, sizeof(wchar_t));
            if (!wtext) {
                MessageBox(hWnd, L"Error al asignar memoria", L"Error", MB_OK | MB_ICONERROR);
                free(buffer);
                break;
            }

            // Convertir de char* a wchar_t*
            ConvertirCharAWcharT(buffer, wtext, resSize + 1);

            // Mostrar el contenido en el control RichEdit de resultados
            SetWindowText(hWndResultados, wtext);

            // Liberar memoria
            free(buffer);
            free(wtext);

            InvalidateRect(hWnd, &rect, TRUE);
        }
            break;
        case ID_BTNANALIZAR: {
            FILE* file = fopen("entrada.txt", "w");

            if (file != NULL) {
                int length = GetWindowTextLength(hWndEdit);
                WCHAR* buffer = new WCHAR[length + 1];
                GetWindowText(hWndEdit, buffer, length + 1);

                // Crear un nuevo buffer sin '\r'
                WCHAR* cleanBuffer = new WCHAR[length + 1];
                int j = 0;
                for (int i = 0; i < length; i++) {
                    if (buffer[i] != L'\r') { // Copiar solo si no es '\r'
                        cleanBuffer[j++] = buffer[i];
                    }
                }
                cleanBuffer[j] = L'\0'; // Terminar correctamente la cadena

                fputws(cleanBuffer, file);

                // Liberar memoria
                delete[] buffer;
                delete[] cleanBuffer;
                fclose(file);
            }
            else {
                MessageBox(hWndEdit, L"Error al abrir el archivo.", L"Error", MB_OK | MB_ICONERROR);
            }


            // Limpiar errores
            SendMessage(hWndErrores, LB_RESETCONTENT, 0, 0);
            FILE* limpiar = fopen("errores.txt", "w+");
            if (limpiar) {
                fclose(limpiar); // Al cerrarlo inmediatamente, el archivo queda vacío
            }
            else {
                perror("Error al limpiar errores.txt");
            }

            // Limpiar advertencias
            limpiar = fopen("advertencias.txt", "w+");
            if (limpiar) {
                fclose(limpiar); // Al cerrarlo inmediatamente, el archivo queda vacío
            }
            else {
                perror("Error al limpiar advertencias.txt");
            }

            // Abrir archivos para `yyin` y `yyout`
            yyin = fopen("entrada.txt", "r");
            if (yyin == NULL) {
                MessageBox(hWnd, L"No es posible cargar yyin", L"Error", MB_OK | MB_ICONERROR);
                break;
            }

            yyout = fopen("resultados.txt", "w");
            freopen("errores.txt", "a", stderr); // Redirige stderr a errores.log

            if (yyout == NULL) {
                MessageBox(hWnd, L"No es posible crear yyout", L"Error", MB_OK | MB_ICONERROR);
                fclose(yyin);
                break;
            }

            // Ejecutar análisis
            parser();

            // Cerrar archivos
            fclose(yyin);
            fclose(yyout);



            // Abrir errores.txt en modo lectura
            FILE* errFile = fopen("errores.txt", "r");
            int hayErrores = 0;
            if (errFile) {
                fseek(errFile, 0, SEEK_END);
                hayErrores = (ftell(errFile) > 0);  // Si el tamaño del archivo es > 0, hay errores
            }

            // 📌 Verificar si "advertencias.txt" tiene contenido
            FILE* warnFile = fopen("advertencias.txt", "r");
            int hayAdvertencias = 0;
            if (warnFile) {
                fseek(warnFile, 0, SEEK_END);
                hayAdvertencias = (ftell(warnFile) > 0);  // Si hay contenido, hay advertencias
            }

            // Buffer para leer línea por línea
            if (hayErrores) {
                fseek(errFile, 0, SEEK_SET); 
                char errBuffer[256];
                // Leer línea por línea y agregar a la ListBox
                while (fgets(errBuffer, sizeof(errBuffer), errFile)) {
                    // Convertir la línea de char* a wchar_t*
                    wchar_t wErrLine[256];
                    ConvertirCharAWcharT(errBuffer, wErrLine, sizeof(wErrLine) / sizeof(wErrLine[0]));

                    // Agregar la línea de error a la ListBox
                    SendMessage(hWndErrores, LB_ADDSTRING, 0, (LPARAM)wErrLine);
                }

            }
            fclose(errFile);

            if (hayAdvertencias) {
                fseek(warnFile, 0, SEEK_SET);
                char wnBuffer[256];

                // Leer línea por línea y agregar a la ListBox
                while (fgets(wnBuffer, sizeof(wnBuffer), warnFile)) {
                    // Convertir la línea de char* a wchar_t*
                    wchar_t wWarnLine[256];
                    ConvertirCharAWcharT(wnBuffer, wWarnLine, sizeof(wWarnLine) / sizeof(wWarnLine[0]));

                    // Agregar la línea de error a la ListBox
                    SendMessage(hWndErrores, LB_ADDSTRING, 0, (LPARAM)wWarnLine);
                }

                // Cerrar el 
            }
            fclose(warnFile);
            if (hayErrores) {
                MessageBox(hWnd, L"Se encontraron errores en el análisis.", L"Error", MB_OK | MB_ICONERROR);
            }
            else {
                // Abrir el archivo resultados.txt
                FILE* res = fopen("resultados.txt", "r");
                if (res == NULL) {
                    MessageBox(hWnd, L"No se pudo abrir el archivo resultados.txt", L"Error", MB_OK | MB_ICONERROR);
                    break;
                }

                // Obtener el tamaño del archivo
                fseek(res, 0, SEEK_END);
                long resSize = ftell(res);
                rewind(res);

                // Leer el contenido del archivo
                char* buffer = (char*)calloc(resSize + 1, sizeof(char));
                if (!buffer) {
                    MessageBox(hWnd, L"Error al asignar memoria", L"Error", MB_OK | MB_ICONERROR);
                    fclose(res);
                    break;
                }

                fread(buffer, 1, resSize, res);
                buffer[resSize] = '\0'; // Asegurar terminación nula
                fclose(res);

                // Convertir el contenido a wchar_t
                wchar_t* wtext = (wchar_t*)calloc(resSize + 1, sizeof(wchar_t));
                if (!wtext) {
                    MessageBox(hWnd, L"Error al asignar memoria", L"Error", MB_OK | MB_ICONERROR);
                    free(buffer);
                    break;
                }

                // Convertir de char* a wchar_t*
                ConvertirCharAWcharT(buffer, wtext, resSize + 1);

                // Mostrar el contenido en el control RichEdit de resultados
                SetWindowText(hWndResultados, wtext);

                // Liberar memoria
                free(buffer);
                free(wtext);
                MessageBox(hWnd, L"El analisis se realizo correctamente, no se encontraron errores.", L"Éxito", MB_OK | MB_ICONINFORMATION);
            }
        }
        break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        EndPaint(hWnd, &ps);
        return 0;
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Controlador de mensajes del cuadro Acerca de.
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
