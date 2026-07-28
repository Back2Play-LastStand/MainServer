#include "pch.h"
#include "PathFinder.h"
#include "Storage/MapData.h"

#include <queue>
#include <unordered_map>
#include <algorithm>

using namespace std;

struct Node
{
    int x, z;
    int g;
    int f;

    bool operator>(const Node& other) const
    {
        return f > other.f;
    }
};

int PathFinder::Heuristic(int x1, int z1, int x2, int z2)
{
    return abs(x1 - x2) + abs(z1 - z2);
}

vector<pair<int, int>> PathFinder::FindPath(int startX, int startZ, int targetX, int targetZ)
{
    if (!GMapData)
        return {};

    // Fail if start or target position is blocked
    if (!GMapData->IsWalkable(startX, startZ) || !GMapData->IsWalkable(targetX, targetZ))
        return {};

    auto Key = [](int x, int z)
        {
            return ((int64_t)x << 32) | (uint32_t)z;
        };

    priority_queue<Node, vector<Node>, greater<Node>> open;
    unordered_map<int64_t, int64_t> parent;
    unordered_map<int64_t, int> gScore;

    int64_t startKey = Key(startX, startZ);

    open.push({ startX, startZ, 0, Heuristic(startX, startZ, targetX, targetZ) });
    gScore[startKey] = 0;

    vector<pair<int, int>> dirs =
    {
        {1,0}, {-1,0}, {0,1}, {0,-1}
    };

    while (!open.empty())
    {
        Node cur = open.top();
        open.pop();

        // Reached target
        if (cur.x == targetX && cur.z == targetZ)
        {
            vector<pair<int, int>> path;

            int64_t curKey = Key(cur.x, cur.z);

            while (curKey != startKey)
            {
                int x = (int)(curKey >> 32);
                int z = (int)(curKey & 0xFFFFFFFF);

                path.push_back({ x, z });
                curKey = parent[curKey];
            }

            reverse(path.begin(), path.end());
            return path;
        }

        for (auto& d : dirs)
        {
            int nx = cur.x + d.first;
            int nz = cur.z + d.second;

            // Check walkable tile using MapData
            if (!GMapData->IsWalkable(nx, nz))
                continue;

            int64_t nextKey = Key(nx, nz);
            int newG = cur.g + 1;

            if (!gScore.count(nextKey) || newG < gScore[nextKey])
            {
                gScore[nextKey] = newG;
                parent[nextKey] = Key(cur.x, cur.z);

                int f = newG + Heuristic(nx, nz, targetX, targetZ);
                open.push({ nx, nz, newG, f });
            }
        }
    }

    return {}; // No path found
}