#include <vector>
#include <tgmath.h>
#include <unistd.h>

#include "./TaskPool/Pool.h"
#include "./OpenMaps/OpenMapsDownload.h"

void Coord2Tile(LatLng *l_LatLng, Tile *l_Tile, int l_Zoom)
{
    l_Tile->x = (int)(floor((l_LatLng->m_Lng + 180.0) / 360.0 * pow(2.0, l_Zoom)));
    l_Tile->y = (int)(floor((1.0 - log(tan(l_LatLng->m_Lat * M_PI/180.0) + 1.0 / cos(l_LatLng->m_Lat * M_PI/180.0)) / M_PI) / 2.0 * pow(2.0, l_Zoom)));
}

int main(int argc, char *argv[])
{
    std::vector<CTask *> l_Tasks;

    CPool l_TaskPool;
    l_TaskPool.Init(sysconf(_SC_NPROCESSORS_ONLN));

    LatLng l_StartPoint = {41.50443, 2.09167};
    LatLng l_EndPoint = {41.49723, 2.11897};

    Tile l_Start = {0};
    Tile l_End = {0};


    for(unsigned int zoom = 10; zoom <= 19; ++zoom)
    {
        Coord2Tile(&l_StartPoint, &l_Start, zoom);
        Coord2Tile(&l_EndPoint, &l_End, zoom);

        for(unsigned int i = l_Start.x; i <= l_End.x; i++)
        {
            for(unsigned int j = l_Start.y; j <= l_End.y; ++j)
            {
                Tile l_AuxTile = {i, j};

                CTask *l_Task = new OpenMapsDownload((char *) "a.tile.openstreetmap.org", (char *) "/home/andrei/MapTiles/", &l_AuxTile, zoom);
                l_Tasks.push_back(l_Task); l_TaskPool.AddTask(l_Task);
            }
        }
    }

    l_TaskPool.WaitForWorkers();

    for(unsigned int i = 0; i < l_Tasks.size(); ++i) delete(l_Tasks.at(i));
    l_Tasks.clear();

    return 0;
}