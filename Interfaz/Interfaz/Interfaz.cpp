// INTERFAZ.cpp : Define el punto de entrada de la aplicación.
//
#include <windows.h>
#include "framework.h>
#include "Interfaz.h"

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
extern int yylex();
extern FILE* yyin;
extern FILE* yyout;
int analisis_lexico_exitoso = 0;

//Esta función determina el tamaño del archivo
//Debería estar después de las variables globales
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


void EscribirTexto(HDC hDC, int x, int y, int tf1, const wchar_t texto[128], COLORREF color)
{
    HFONT fuente1, fanterior;
    LOGFONT lf1 = { tf1, 0, 0, 900, 300, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,PROOF_QUALITY, DEFAULT_PITCH | FF_ROMAN, L"Arial" };
    COLORREF coloriginal;

    fuente1 = CreateFontIndirect(&lf1);
    SetBkMode(hDC, TRANSPARENT);

    fanterior = (HFONT)SelectObject(hDC, fuente1);
    coloriginal = SetTextColor(hDC, color);
    TextOut(hDC, x, y, texto, _tcsclen(texto));

    SelectObject(hDC, fanterior);
    SetTextColor(hDC, coloriginal);
    DeleteObject(fuente1);
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


void DibujarPalabrasRepetidas(HDC hdc, int xInicial, int yInicial, int xLim, int yLim) {
    int maxConteo = 0, proporcion = 0;
    int maxConteo_max = 0;
    int banderaPos = 0;
    int yPosSup = yInicial, yPosInf = yInicial;
    int xPos = xInicial;
    int yPosSel = yInicial; //Cual de las dos yPos se escoge
    int tamLetra = 45;
    COLORREF colorAleatorio = RGB(0, 0, 0);
    int i;

    maxConteo = repetidas.lista[0].conteo;
    maxConteo_max = maxConteo;

    wchar_t palabraWchar[100];
    wchar_t conteoWchar[50]; // Buffer para el conteo

    // Calcular la posición de cada palabra
    for (i = 0; i < repetidas.tam && yPosSup > xLim && yPosInf < yLim; i++) {
        colorAleatorio = RGB(rand() % 256, rand() % 256, rand() % 256);

        ConvertirCharAWcharT(repetidas.lista[i].texto, palabraWchar, sizeof(palabraWchar) / sizeof(palabraWchar[0]));
        swprintf(conteoWchar, sizeof(conteoWchar) / sizeof(conteoWchar[0]), L" : %d", repetidas.lista[i].conteo);

        // Dibujar la palabra
        EscribirTexto(hdc, xPos, yPosSel, tamLetra, palabraWchar, colorAleatorio);

        // Dibujar el conteo al lado de la palabra
        EscribirTexto(hdc, xPos + 100, yPosSel, tamLetra, conteoWchar, colorAleatorio);

        banderaPos = ~banderaPos;
        if (repetidas.lista[i].conteo < maxConteo) {
            tamLetra = tamLetra - 2 - (maxConteo_max / repetidas.lista[i].conteo);
            maxConteo = repetidas.lista[i].conteo;
        }
        if (banderaPos) { // Va pa arriba
            yPosSup -= 25 + tamLetra;
            yPosSel = yPosSup;
        }
        else { // Va pa abajo
            yPosInf += 25 + tamLetra;
            yPosSel = yPosInf;
        }
    }

}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Colocar código aquí.

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

    wcex.style = CS_HREDRAW | CS_VREDRAW;
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
    static HWND hWndEdit;	//Manejador de ventana
    static HWND hStatus;	//Manejador de la barra de estado
    DWORD dwEVM;	//Evento de captura
    HFONT hFont;	//Manejador de fuente

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
        hWndEdit = CreateWindowEx(WS_EX_CLIENTEDGE, RICHEDIT_CLASS, L"", WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL, 0, 0, 0, 0, hWnd, (HMENU)ID_EDITRICH, hInst, NULL);

        //barra de estado
        hStatus = CreateWindowEx(0, STATUSCLASSNAME, NULL, WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP, 0, 0, 0, 0, hWnd, (HMENU)IDB_STATUS, hInst, NULL);
        dwEVM = SendMessage(hWndEdit, EM_GETEVENTMASK, 0, 0);
        dwEVM |= ENM_UPDATE | ENM_CHANGE | ENM_SELCHANGE;
        SendMessage(hWndEdit, EM_SETEVENTMASK, 0, dwEVM);

        //Fuente de edicion
        hFont = CreateFont(18, 0, 0, 0, 0, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, L"Arial");
        SendMessage(hWndEdit, WM_SETFONT, (WPARAM)hFont, 0);

        SetFocus(hWndEdit); //Le da el foco al area de edicion
    }
    break;

    case WM_SIZE:
    {
        RECT rect;

        SendDlgItemMessage(hWnd, IDB_STATUS, WM_SIZE, 0, 0);
        GetWindowRect(hStatus, &rect);
        MoveWindow(hWndEdit, 0, 70, LOWORD(lParam) / 2, HIWORD(lParam) - (rect.bottom - rect.top) - 70, TRUE);
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

    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Analizar las selecciones de menú:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
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
        case ID_BTNEJECUTAR:
            if (analisis_lexico_exitoso) {
                //Por defecto se mostraran las palabras que quepan, pero aca se puede poner un numero remplazando
                //la variable t.cant_palabras, siempre y cuando ese valor sea menor o igual que la cantidad de palabras analizadas.
                //para una futura version, en la interfaz se podra escoger cuantas palabras ver graficamente
                repetidas = obtenerMasRepetidas(&t, t.cant_palabras);
                RECT rect;
                GetWindowRect(hWnd, &rect);
                InvalidateRect(hWnd, &rect, TRUE);
                analisis_lexico_exitoso = 0;
                eliminarTabla();
            }
            else {
                MessageBox(hWnd, L"No se ha realizado un analisis", L"Error", MB_OK | MB_ICONERROR);
            }
            break;
        case ID_BTNANALIZAR: {
            const WCHAR* nombre_archivo = L"entrada.txt";
            FILE* file;
            _wfopen_s(&file, nombre_archivo, L"w, ccs=UTF-8");

            if (file != NULL) {
                setlocale(LC_ALL, "es_ES.utf8"); //ACENTOS

                int length = GetWindowTextLength(hWndEdit);
                WCHAR* buffer = new WCHAR[length + 1];
                GetWindowText(hWndEdit, buffer, length + 1);

                // Convertir el texto a minúsculas
                for (int i = 0; i < length; ++i)
                {
                    buffer[i] = towlower(buffer[i]);
                }

                fputws(buffer, file);
                delete[] buffer;

                fclose(file);
                //MessageBox(hWndEdit, L"Archivo guardado exitosamente como Texto.txt.", L"Exitosamente", MB_OK | MB_ICONINFORMATION);
            }
            else
            {
                MessageBox(hWndEdit, L"Error al abrir el archivo.", L"Error", MB_OK | MB_ICONERROR);
            }
            // Abrir archivos para `yyin` y `yyout`
            yyin = fopen("entrada.txt", "r");  // Abrir en modo binario para leer UTF-8 sin interpretación
            if (yyin == NULL) {
                MessageBox(hWnd, L"No es posible cargar yyin", L"Error", MB_OK | MB_ICONERROR);
                analisis_lexico_exitoso = 0;
            }
            else {
                yyout = fopen("salida.txt", "w");
                if (yyout == NULL) {
                    MessageBox(hWnd, L"No es posible crear yyout", L"Error", MB_OK | MB_ICONERROR);
                    analisis_lexico_exitoso = 0;
                }
                else {
                    iniciarTabla();
                    yylex();
                    MessageBox(hWnd, L"Se analizo correctamente :)", L"Exito", MB_OK | MB_ICONINFORMATION);
                    analisis_lexico_exitoso = 1;

                    fclose(yyin);
                    fclose(yyout);
                }
            }

        }
                           break;
                           /*case ID_BTNINFO:
                               DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                               break;*/
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        RECT rect;
        GetWindowRect(hWnd, &rect);

        int limXizquierda = (rect.right - rect.left) / 2 + ((rect.right - rect.left) / 4);
        int ycentro = ((rect.bottom - rect.top) / 2) - 60;

        if (repetidas.tam > 0)
            DibujarPalabrasRepetidas(hdc, limXizquierda - 50, ycentro, 60, rect.bottom - 40);


        EndPaint(hWnd, &ps);
        return 0;  // Devuelve un valor válido
    }
    break;
    case WM_DESTROY:
        eliminarTabla();
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
