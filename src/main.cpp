#include <vector>
#include <tgmath.h>
#include <unistd.h>
#include <string.h>

#include "./TaskPool/Pool.h"
#include "./OpenMaps/OpenMapsDownload.h"

void Coord2Tile(LatLng *l_LatLng, Tile *l_Tile, int l_Zoom)
{
    l_Tile->x = (int)(floor((l_LatLng->Lng + 180.0) / 360.0 * pow(2.0, l_Zoom)));
    l_Tile->y = (int)(floor((1.0 - log(tan(l_LatLng->Lat * M_PI/180.0) + 1.0 / cos(l_LatLng->Lat * M_PI/180.0)) / M_PI) / 2.0 * pow(2.0, l_Zoom)));
}

void Help(void)
{
    printf("OpenMapsTileDownload -s Lat,Lng -e Lat,Lng -z Min,Max\n");
    printf("\t\t\"s\" : start point\n");
    printf("\t\t\"e\" : end point\n");
    printf("\t\t\"z\" : zoom\n\n");

    printf("Example: OpenMapsTileDownloader -s 41.50443,2.09167 -e 41.49723,2.11897 -z 10,19\n");
    printf("         Don\'t leave space after comma\n");

}

bool ArgsParser(int argc, char *argv[],  LatLng *l_StartPoint, LatLng *l_EndPoint, Zoom *l_Zoom)
{
    if(argc < 7) return false;
    bool l_S = false, l_E = false, l_Z = false;

    for(int i = 1; i < argc; ++i)
    {
        if(!strcmp("-s", *(argv + i)))
        {
            char *l_Comma = strchr(*(argv + i + 1), ',');
            if(l_Comma)
            {
                l_StartPoint->Lng = atof(l_Comma + 1);

                l_Comma = '\0';
                l_StartPoint->Lat = atof(*(argv + i + 1));

                l_S = true;
            }
        }

        if(!strcmp("-e", *(argv + i)))
        {
            char *l_Comma = strchr(*(argv + i + 1), ',');
            if(l_Comma)
            {
                l_EndPoint->Lng = atof(l_Comma + 1);

                l_Comma = '\0';
                l_EndPoint->Lat = atof(*(argv + i + 1));

                l_E = true;
            }
        }

        if(!strcmp("-z", *(argv + i)))
        {
            char *l_Comma = strchr(*(argv + i + 1), ',');
            if(l_Comma)
            {
                l_Zoom->Max = atoi(l_Comma + 1);

                l_Comma = '\0';
                l_Zoom->Min = atoi(*(argv + i + 1));

                l_Z = true;
            }
        }
    }

    return l_S & l_E & l_Z;
}

int main(int argc, char *argv[])
{
    LatLng l_StartPoint = { 0.0, 0.0 };
    LatLng l_EndPoint = { 0.0, 0.0 };
    Zoom l_Zoom = { 0, 0 };

    if(!ArgsParser(argc, argv, &l_StartPoint, &l_EndPoint, &l_Zoom))
    {
        Help();
        return EXIT_SUCCESS;
    }

    std::vector<CTask *> l_Tasks;
    CPool l_TaskPool;

    Tile l_Start = {0};
    Tile l_End = {0};

    l_TaskPool.Init(sysconf(_SC_NPROCESSORS_ONLN));


    for(unsigned int zoom = l_Zoom.Min; !(zoom > l_Zoom.Max); ++zoom)
    {
        Coord2Tile(&l_StartPoint, &l_Start, zoom);
        Coord2Tile(&l_EndPoint, &l_End, zoom);

        for(unsigned int i = l_Start.x; !(i > l_End.x); ++i)
        {
            for(unsigned int j = l_Start.y; !(j > l_End.y); ++j)
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

    return EXIT_FAILURE;
}