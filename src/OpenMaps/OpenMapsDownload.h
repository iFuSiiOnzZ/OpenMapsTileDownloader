//
// Created by andrei on 20/06/15.
//

#ifndef OPENMAPSTILEDOWNLOADER_OPENMAPSDOWNLOAD_H
#define OPENMAPSTILEDOWNLOADER_OPENMAPSDOWNLOAD_H

#include "./../defines.h"
#include "./../TaskPool/Task.h"

class OpenMapsDownload : public CTask
{
    private:
        int      m_Zoom;

        char    m_TileServer[256];
        char    m_PathToSave[256];

        Tile    m_Tile;
        LatLng  m_Coord;


    public:
        OpenMapsDownload    (char *l_TileServer, char *l_PathToSave, LatLng *l_Coord, int l_Zoom);
        OpenMapsDownload    (char *l_TileServer, char *l_PathToSave, Tile *l_Coord, int l_Zoom);

        void Coord2Tile     (LatLng *l_LatLng, Tile *l_Tile, int l_Zoom);
        void Execute        (void);
};


#endif //OPENMAPSTILEDOWNLOADER_OPENMAPSDOWNLOAD_H
