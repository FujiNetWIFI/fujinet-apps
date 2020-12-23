#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 40*24+1
#define DEFAULT_PORT "6502"

#define SCREENSIZE (24*40)

char screen_to_ascii[255];
char ascii_to_screen[255];
char ascii_screen[SCREENSIZE];

HANDLE StdOut;

void init(void)
{
    StdOut = GetStdHandle(STD_OUTPUT_HANDLE);


    // SCREEN TO ASCII

    for (int i = 0; i < 255; i++)
        screen_to_ascii[i] = '?';

    int c = 0;
    for (int i = ' '; i < '_' + 1; i++)
        screen_to_ascii[c++] = (char)(i);
    // ???
    c = 97;
    for (int i = 'a'; i < 'z' + 1; i++)
        screen_to_ascii[c++] = (char)(i);

    screen_to_ascii['z' + 1] = 6; // spade
    screen_to_ascii['_' + 1] = 3; // heart
    screen_to_ascii['a' - 1] = 4; // diamond
    screen_to_ascii['a' - 17] = 5; // club

    for (int i = 0; i < 127; i++)
        screen_to_ascii[i + 128] = screen_to_ascii[i] | 128;

    // ASCII TO SCREEN

    for (int i = 0; i < 255; i++)
        ascii_to_screen[i] = '?';

    c = 0;
    for (int i = ' '; i < '_' + 1; i++)
        ascii_to_screen[i] = (char)(c++);

    c = 97;
    for (int i = 'a'; i < 'z' + 1; i++)
        ascii_to_screen[i] = (char)(c++);

    for (int i = 0; i < 127; i++)
        ascii_to_screen[i + 128] = ascii_to_screen[i];


    // Console font size
    CONSOLE_FONT_INFOEX cfi;

    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 0;                   // Width of each character in the font
    cfi.dwFontSize.Y = 24;                  // Height
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;

    wcscpy_s(cfi.FaceName, L"Consolas"); // Choose your font
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);



}

void update_screen(void)
{
    static char c = 'A';

    for (int i = 0; i < SCREENSIZE; i++)
    {
        ascii_screen[i] = c;
        c = c + 1;
        if (c > 255)
            c = 0;
    }

    int i = 0;
    for (int y = 0; y < 24; y++)
    {
        for (int x = 0; x < 40; x++)
        {
            printf("%c", ascii_screen[i]);
            i++;
        }
        printf("\n");
    }
}

void translate_ascii_to_screen(char* out, char* in, int size)
{
    for (int i = 0; i < size; i++)
        out[i] = ascii_to_screen[in[i]];
}

void cls(HANDLE hConsole)
{
    COORD coordScreen = { 0, 0 };    /* here's where we'll home the
                                       cursor */
    BOOL bSuccess;
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi; /* to get buffer info */
    DWORD dwConSize;                 /* number of character cells in
                                       the current buffer */

                                       /* get the number of character cells in the current buffer */

    bSuccess = GetConsoleScreenBufferInfo(hConsole, &csbi);
    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

    /* fill the entire screen with blanks */

    bSuccess = FillConsoleOutputCharacter(hConsole, (TCHAR)' ',
        dwConSize, coordScreen, &cCharsWritten);

    /* get the current text attribute */

    bSuccess = GetConsoleScreenBufferInfo(hConsole, &csbi);

    /* now set the buffer's attributes accordingly */

    bSuccess = FillConsoleOutputAttribute(hConsole, csbi.wAttributes,
        dwConSize, coordScreen, &cCharsWritten);

    return;
}

void display_as_ascii(char* in)
{
    DWORD   CharsWritten;
    COORD	Coord = { 0 , 0 };
    char    c[2];
    static  int     fullRefresh = 0;


    if (fullRefresh == 0)
    {
        cls(StdOut);
        fullRefresh = 100;
    }
    else
        fullRefresh--;

    int i = 0;
    Coord.Y = 0;
    for (int y = 0; y < 24; y++)
    {
        Coord.X = 0;
        SetConsoleCursorPosition(StdOut, Coord);
        char c1;
        for (int x = 0; x < 40; x++)
        {
            if (i > 126)
                int x = 0;
            c1 = in[i] & 0x7F;
            if ((unsigned int)in[i] > 127)
            {
                // inverse
                SetConsoleTextAttribute(StdOut, BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
            }
            else
            {
                SetConsoleTextAttribute(StdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            }
            sprintf_s(c, sizeof(c), "%c", screen_to_ascii[c1]);

            WriteConsole(StdOut,			// handle to a console screen buffer 
                &c,	                        // pointer to buffer to write from 
                (DWORD)1,	                // number of characters to write 
                &CharsWritten,	            // pointer to number of characters written 
                NULL); 		                // reserved 
            i++;
        }
        Coord.Y++;
    }
    Coord.Y++;
    Coord.X = 0;
    SetConsoleCursorPosition(StdOut, Coord);
    SetConsoleTextAttribute(StdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}


int __cdecl main(void)
{
    WSADATA wsaData;
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo* result = NULL;
    struct addrinfo hints;

    int iSendResult;
    //char recvbuf[DEFAULT_BUFLEN];
    char sendbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;

    init();

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }
    else
    {
        printf("WSAStartup successfull\n");
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }
    else
    {
        printf("get address info successfull\n");
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }
    else
    {
        printf("listenSocket successfull %d\n", ListenSocket);
    }

    // Setup the TCP listening socket
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
    else
    {
        printf("Bind successful\n");
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
    else
    {
        printf("listen successful\n");
    }

    printf("Waiting to accept...\n");
    // Accept a client socket
    ClientSocket = accept(ListenSocket, NULL, NULL);
    printf("after accept\n");
    if (ClientSocket == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
    else
    {
        printf("accept successfull\n");
    }

    // No longer need server socket
    printf("close socket\n");
    closesocket(ListenSocket);

    // Receive until the peer shuts down the connection

    do {

        update_screen();

        translate_ascii_to_screen(sendbuf, ascii_screen, SCREENSIZE);
        display_as_ascii(sendbuf);
        printf("sending %d bytes...\n", SCREENSIZE);
        
        iSendResult = send(ClientSocket, sendbuf, SCREENSIZE, 0);
        if (iSendResult == SOCKET_ERROR) {
            printf("send failed with error: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            return 1;
        }

        printf("Bytes sent: %d\n", iSendResult);

        Sleep(10000);

    } while (iSendResult > 0);

    // shutdown the connection since we're done
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }

    // cleanup
    closesocket(ClientSocket);
    WSACleanup();

    return 0;
}