#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string>
#include <iostream>
#include <mswsock.h>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define SCREENSIZE (24*40)
#define DEFAULT_BUFLEN SCREENSIZE+1
#define DEFAULT_PORT "6502"

int getStatus(const SOCKET a_socket, int status);

const int   STATUS_READ   = 0x1, 
            STATUS_WRITE  = 0x2, 
            STATUS_EXCEPT = 0x4;

unsigned char screen_to_ascii[256];
unsigned char ascii_to_screen[256];
unsigned char atascii_to_ascii[256];
unsigned char ascii_to_atascii[256];
unsigned char ascii_screen[SCREENSIZE];

unsigned char atascii_special[256];
unsigned char ascii_special[256];

HANDLE StdOut;
SOCKET ConnectSocket = INVALID_SOCKET;


SOCKET Blocking(char *host)
{
    WSADATA wsaData;
    ULONG iResult;
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;



    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return SOCKET_ERROR;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;



    // Resolve the server address and port
    iResult = getaddrinfo(host, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return SOCKET_ERROR;
    }


    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return INVALID_SOCKET;
        }

        // Connect to server.
        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        WSACleanup();
        return INVALID_SOCKET;
    }

    return ConnectSocket;
}

void Position(int x, int y)
{
    COORD	Coord = { 0 , 0 };
    Coord.X = x;
    Coord.Y = y;
    SetConsoleCursorPosition(StdOut, Coord);

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

    Position(0, 0);

    return;
}

void convert(char* io)
{
    char* c = io;

    while (*c)
    {
       
        if (((unsigned char ) *c & 0xFF) == 0x13)
            *c = (char) 0x9B;
        c++;
    }

}


void init(void)
{
    StdOut = GetStdHandle(STD_OUTPUT_HANDLE);


    // SCREEN TO ASCII

    for (int i = 0; i < 255; i++)
        screen_to_ascii[i] = '?';

    int c = 0;
    for (int i = ' '; i < '_'+1; i++)
        screen_to_ascii[c++] = (char)(i);
    // ???
    c = 97;
    for (int i = 'a'; i < 'z'+1; i++)
        screen_to_ascii[c++] = (char)(i);
    
    screen_to_ascii['z' + 1]  = 6; // spade
    screen_to_ascii['_' + 1]  = 3; // heart
    screen_to_ascii['a' - 1]  = 4; // diamond
    screen_to_ascii['a' - 17] = 5; // club
    screen_to_ascii[65] = (char) 195;     // |-
    screen_to_ascii[66] = (char)179;     //  |
    screen_to_ascii[67] = (char)217;     // _|
    screen_to_ascii[68] = (char)180;     // -|
    screen_to_ascii[69] = (char)191;
    screen_to_ascii[70] = 47;
    screen_to_ascii[71] = 92;
    screen_to_ascii[72] = '?';
    screen_to_ascii[73] = (char)220;
    screen_to_ascii[74] = '?';
    screen_to_ascii[75] = (char)223;
    screen_to_ascii[76] = '?';
    screen_to_ascii[77] = (char)196;
    screen_to_ascii[78] = (char)196;
    screen_to_ascii[79] = '?';
    screen_to_ascii[81] = (char)218;
    screen_to_ascii[82] = (char)196;
    screen_to_ascii[83] = (char)197;
    screen_to_ascii[84] = 9;
    screen_to_ascii[85] = (char)220;
    screen_to_ascii[86] = (char)204;
    screen_to_ascii[87] = (char)194;
    screen_to_ascii[88] = (char)193;
    screen_to_ascii[89] = (char)221;
    screen_to_ascii[90] = (char)192;
    screen_to_ascii[91] = '?';
    screen_to_ascii[92] = 30;
    screen_to_ascii[93] = 31;
    screen_to_ascii[94] = 17;
    screen_to_ascii[95] = 16;
    screen_to_ascii[96] = 7;

    for (int i = 0; i < 128; i++)
        screen_to_ascii[i + 128] = screen_to_ascii[i] | 128;

    // ASCII TO SCREEN

    for (int i = 0; i < 255; i++)
        ascii_to_screen[i] = '?';

    c = 0;
    for (int i = ' '; i < '_'+1; i++)
        ascii_to_screen[i] = (char)(c++);

    c = 97;
    for (int i = 'a'; i < 'z'+1; i++)
        ascii_to_screen[i] = (char)(c++);

    for (int i = 0; i < 128; i++)
        ascii_to_screen[i+128] = ascii_to_screen[i];


    // ATASCII TO ASCII

    for (int i = 0; i < 256; i++)
        atascii_to_ascii[i] = (char) i;

    atascii_to_ascii[126] = 8;  // BACKSPACE
    atascii_to_ascii[127] = 9;  // TAB

    for (int i = 0; i < 127; i++)
    {
        unsigned int index = atascii_to_ascii[i] & 0xFF;

        atascii_to_ascii[i + 128] = ((atascii_to_ascii[i] & 0xFF) | 128);
    }

    // special above 128
    atascii_to_ascii[155] = 13; // RETURN
    atascii_to_ascii[253] = 7;  // BELL

    unsigned int index;
    for (int i = 0; i < 256; i++)
    {
        index = atascii_to_ascii[i] & 0xFF;
        ascii_to_atascii[index] = i;
    }

    // #special#
    atascii_special[72] = 28; // arrow up 
    atascii_special[80] = 29; // arrow down
    atascii_special[77] = 31; // arrow right
    atascii_special[75] = 30; // arrow left
    // atascii_special[0] = 83; // break FN-DELETE

    for (int i = 0; i < 256; i++)
    {
        index = atascii_special[i] & 0xFF;
        ascii_special[index] = i;
    }

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
    SetConsoleMode(NULL, TRUE);
   
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
            if ((unsigned int) in[i] > 127)
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

char translateToAscii(char c)
{
    return screen_to_ascii[c];
}

// status: 0x1 for read, 0x2 for write, 0x4 for exception
int getStatus(const SOCKET a_socket, int status)
{
    // zero seconds, zero milliseconds. max time select call allowd to block
    static timeval instantSpeedPlease = { 0,0 };
    fd_set a = { 1, {a_socket} };
    fd_set* read = ((status & 0x1) != 0) ? &a : NULL;
    fd_set* write = ((status & 0x2) != 0) ? &a : NULL;
    fd_set* except = ((status & 0x4) != 0) ? &a : NULL;
    /*
    select returns the number of ready socket handles in the fd_set structure, zero if the time limit expired, or SOCKET_ERROR if an error occurred. WSAGetLastError can be used to retrieve a specific error code.
    */
    int result = select(0, read, write, except, &instantSpeedPlease);
    if (result == SOCKET_ERROR)
    {
        result = WSAGetLastError();
    }
    if (result < 0 || result > 3)
    {
        printf("select(read) error %d\n", result);
        return SOCKET_ERROR;
    }
    return result;
}

//#define SHOW_KEY_CODE

DWORD WINAPI keyboard_thread(LPVOID lpParam)
{
    char sendbuf[1];
    bool special;


    while (true)
    {
        if (_kbhit())
        {
            special = false;
            sendbuf[0] = _getch();
            unsigned int ascii   = (unsigned int)(sendbuf[0] & 0xFF);
            unsigned int atascii = (unsigned int)(ascii_to_atascii[ascii] & 0xFF);

            if (ascii == 224)
            {
                special = true;
#ifdef SHOW_KEY_CODE
                printf("<%02X> ", ascii);
#endif
                sendbuf[0] = _getch();
                ascii = (unsigned int)(sendbuf[0] & 0xFF);
                atascii = (unsigned int) (atascii_special[ascii]);

            }
#ifdef SHOW_KEY_CODE
            printf("[%02X / %02X / %c]", ascii, atascii, (char) ascii);
#else
            if (special)
            {
                switch (ascii)
                {
                case 72: // UP  Esc[ValueA
                    printf("%c[1A", 27);
                    break;
                case 80: // DOWN
                    printf("%c[1B", 27);
                    break;
                case 75: // LEFT
                    printf("%c[1D", 27);
                    break;
                case 77: // RIGHT
                    printf("%c[1C", 27);
                    break;
                default:
                    break;
                }
            }
            else
            {

                switch (ascii)
                {
                case 8:
                    printf("%c %c", ascii, ascii);
                    break;
                case 13:
                    printf("%c%c", ascii, 10);
                    break;
                default:
                    printf("%c", ascii);
                }
            }

#endif
            sendbuf[0] = (char) (atascii & 0xFF);
            send(ConnectSocket, sendbuf, 1, 0);
        }
    }
    return 0;
}


int __cdecl main(int argc, char** argv)
{
    char recvbuf[DEFAULT_BUFLEN];
    char atariscreen[SCREENSIZE + 1];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;
    int sendbuflen = DEFAULT_BUFLEN;
    char host[80] = { "" };
    SOCKET ConnectSocket = INVALID_SOCKET;
    int dots = 0;



    init();
    cls(StdOut);

    // Validate the parameters
    if (argc != 2) {
        printf("usage: %s FujiNet-name\n", argv[0]);
        return 1;
    }
    else
        strcpy_s(host, sizeof(host), argv[1]);

    printf("Waiting for connection to: %s", host);
    bool connected = false;
    while (! connected)
    {
        ConnectSocket = Blocking(host);
        if ((ConnectSocket == INVALID_SOCKET) || (ConnectSocket == SOCKET_ERROR))
        {
            printf(".");
            fflush(stdout);
            Sleep(2000);
        }
        else
        {
            if (ConnectSocket > 0)
                connected = true;
        }
    }
    printf("\nConnection: %ld!\n", ConnectSocket);

    DWORD dwThreadID;
    HANDLE threadHandle = CreateThread(NULL, 0, keyboard_thread, 0, 0, &dwThreadID);

    // Receive until the peer closes the connection
    int fullSize = 960;

    int Collected = 0;
    for (int i = 0; i < SCREENSIZE; i++)
        atariscreen[i] = 0;

    Collected = 0;
    do
    {

        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if (iResult < 0)
        {
            int i = iResult;
            iResult = WSAGetLastError();

            if (iResult == WSAEWOULDBLOCK)
                continue;
            else
                if (iResult == WSAENOTCONN)
                    continue;
                else
                    iResult = i;
        }

        if (iResult > 0)
        {
            for (int i = 0; i < iResult; i++)
            {
                atariscreen[Collected++] = recvbuf[i];
            }


            if (Collected >= SCREENSIZE)
            {
                display_as_ascii(atariscreen);
                int c = SCREENSIZE;
                for (int i = 0; i < Collected; i++)
                    atariscreen[i] = recvbuf[c++];
                Collected = Collected - SCREENSIZE;
            }

        }
        else if (iResult == 0)
            printf("Connection closed\n");
        else
            printf("recv failed with error: %d\n", WSAGetLastError());

        if (iResult == -1)
            break;
    } while (iResult != 0);
    
    printf("done\n");

    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}