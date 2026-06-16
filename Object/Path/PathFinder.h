#pragma once
#include <vector>

class PathFinder
{
public:
    std::vector<std::pair<int, int>> FindPath(
        int startX, int startZ,
        int targetX, int targetZ);

private:
    int Heuristic(int x1, int z1, int x2, int z2);
};