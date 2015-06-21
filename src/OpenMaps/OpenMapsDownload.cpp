//
// Created by andrei on 20/06/15.
//

#include <netdb.h>
#include <signal.h>
#include <unistd.h>

#include <tgmath.h>
#include <string.h>

#include "./OpenMapsDownload.h"

#define BUFFER_SIZE 2048

void signal_callback_handler(int signum)
{
    printf("Caught signal SIGPIPE %d\n", signum);
}

OpenMapsDownload::OpenMapsDownload(char *l_TileServer, char *l_PathToSave, LatLng *l_Coord, int l_Zoom)
{
    m_Zoom = l_Zoom;

    strncpy(m_TileServer, l_TileServer, 256);
    strncpy(m_PathToSave, l_PathToSave, 256);

    m_Coord.Lat = l_Coord->Lat;
    m_Coord.Lng = l_Coord->Lng;

    Coord2Tile(&m_Coord, &m_Tile, m_Zoom);
}

OpenMapsDownload::OpenMapsDownload(char *l_TileServer, char *l_PathToSave, Tile *l_Coord, int l_Zoom)
{
    m_Zoom = l_Zoom;

    strncpy(m_TileServer, l_TileServer, 256);
    strncpy(m_PathToSave, l_PathToSave, 256);

    m_Tile.x = l_Coord->x;
    m_Tile.y = l_Coord->y;
}

void OpenMapsDownload::Coord2Tile(LatLng *l_LatLng, Tile *l_Tile, int l_Zoom)
{
    l_Tile->x = (int)(floor((l_LatLng->Lng + 180.0) / 360.0 * pow(2.0, l_Zoom)));
    l_Tile->y = (int)(floor((1.0 - log(tan(l_LatLng->Lat * M_PI/180.0) + 1.0 / cos(l_LatLng->Lat * M_PI/180.0)) / M_PI) / 2.0 * pow(2.0, l_Zoom)));
}

void OpenMapsDownload::Execute()
{
    struct sockaddr_in  l_ServerSockAddr = { 0 };
    struct hostent *l_ServerHostent = 0;

    char l_Buffer[BUFFER_SIZE] = {0};
    char l_GET[256] = {0};

    int l_SocketID = 0;
    signal(SIGPIPE, signal_callback_handler);

    if((l_SocketID = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("ERROR: Socket creation failed\n");
        return;
    }

    if((l_ServerHostent = gethostbyname(m_TileServer)) == 0)
    {
        printf("ERROR: Host %s not found\n", m_TileServer);
        close(l_SocketID); return;
    }

    l_ServerSockAddr.sin_family = AF_INET;
    l_ServerSockAddr.sin_port = htons(80);
    bcopy(l_ServerHostent->h_addr, &l_ServerSockAddr.sin_addr.s_addr, l_ServerHostent->h_length);

    if(connect(l_SocketID, (struct sockaddr *) &l_ServerSockAddr, sizeof(l_ServerSockAddr)) < 0)
    {
        printf("ERROR: Can not connect with %s\n", m_TileServer);
        close(l_SocketID); return;
    }

    int l_SZ = sprintf(l_GET, "GET /%d/%d/%d.png  HTTP/1.0\r\n\r\n", m_Zoom, m_Tile.x, m_Tile.y);
    write(l_SocketID, l_GET, l_SZ);

    char l_PathToSave[256] = { 0 };
    char *l_LastSlash = strrchr(m_PathToSave, '/');
    if(l_LastSlash != 0 && *(l_LastSlash + 1) == '\0') *l_LastSlash = '\0';

    sprintf(l_PathToSave, "%s/%d/%d/", m_PathToSave, m_Zoom, m_Tile.x);

    char l_MakeDir[256] = { 0 }; sprintf(l_MakeDir, "mkdir -p %s", l_PathToSave);
    system(l_MakeDir);

    char l_FileName[32] = { 0 }; sprintf(l_FileName, "%d.png", m_Tile.y);
    strncat(l_PathToSave, l_FileName, 256 - strlen(l_PathToSave));

    if(FILE *l_Found = fopen(l_PathToSave, "r"))
    {
        fclose(l_Found); close(l_SocketID); return;
    }

    FILE *l_pFile = fopen(l_PathToSave, "wb");
    if(l_pFile == 0)
    {
        printf("ERROR: Error opening %s\n", l_PathToSave);
        close(l_SocketID); return;
    }

    printf("Downloading: %s/%d/%d/%d.png\n", m_TileServer,m_Zoom, m_Tile.x, m_Tile.y);
    bool l_HeaderFound = false;

    for(int r = read(l_SocketID, l_Buffer, BUFFER_SIZE); r > 0; r = read(l_SocketID, l_Buffer, BUFFER_SIZE))
    {
        char *l_NoHeader = (!l_HeaderFound) ? strstr(l_Buffer, "\r\n\r\n") : 0;
        l_NoHeader = (l_NoHeader > 0)? l_NoHeader + 4 : l_Buffer;
        if(l_NoHeader - l_Buffer) l_HeaderFound = true;

        fwrite(l_NoHeader, sizeof(char), r - (l_NoHeader - l_Buffer), l_pFile);
        bzero(l_Buffer, 256);
    }

    close(l_SocketID);
    fclose(l_pFile);
}