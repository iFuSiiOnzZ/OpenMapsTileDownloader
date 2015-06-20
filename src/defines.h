//
// Created by andrei on 19/06/15.
//

#ifndef OPENMAPSTILEDOWNLOADER_DEFINES_H
#define OPENMAPSTILEDOWNLOADER_DEFINES_H

    typedef struct LatLng
    {
        double m_Lat;
        double m_Lng;
    }LatLng;

    typedef struct Tile
    {
        unsigned int x;
        unsigned int y;
    }Tile;

#endif //OPENMAPSTILEDOWNLOADER_DEFINES_H
