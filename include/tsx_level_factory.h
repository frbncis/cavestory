#include <level.h>

#include <string>

#pragma once

class Graphics;

class TsxLevelFactory {
public:
    TsxLevelFactory(Graphics &graphics);
    ~TsxLevelFactory();

    Level load_map(std::string map_name);

private:
    Graphics graphics;
};