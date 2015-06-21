//
// Created by andrei on 19/06/15.
//

#ifndef OPENMAPSTILEDOWNLOADER_DEFINES_H
#define OPENMAPSTILEDOWNLOADER_DEFINES_H

    typedef struct LatLng
    {
        double Lat;
        double Lng;
    }LatLng;

    typedef struct Tile
    {
        unsigned int x;
        unsigned int y;
    }Tile;

    typedef struct Zoom
    {
        unsigned int Min;
        unsigned int Max;
    }Zoom;

#endif //OPENMAPSTILEDOWNLOADER_DEFINES_H
