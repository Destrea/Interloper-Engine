
#include "MapLoader.h"
#include <stdio.h>

MapData::MapData(std::string const &path)
{
    playerSpawn = PlayerLocation(path);
}

glm::vec3 MapData::PlayerLocation(std::string const &path)
{
    glm::vec3 origin{0.0f};
    FILE *f = fopen(path.c_str(), "r");
    if(!f)
    {
        //std::cout << "ERROR::level File not loaded!" << std::endl;
        //return -1;
    }

    char line[1024], buff[64];
    while(fgets(line, sizeof(line), f))
    {
        if(sscanf(line, "\"origin\" \"%f %f %f\"", &origin.x, &origin.y, &origin.z))
        {
            origin.x /= 32.0f;
            origin.y /= 32.0f;
            origin.z /= 32.0f;
            return origin;
        }
    }

    return origin;
}
