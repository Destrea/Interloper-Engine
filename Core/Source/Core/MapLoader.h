
#pragma once


#include <glm/glm.hpp>
#include <string>

class MapData
{
public:
    MapData(std::string const &path);
    glm::vec3 playerSpawn;
    glm::vec3 PlayerLocation(std::string const &path);
private:
    //Entity locations

};
