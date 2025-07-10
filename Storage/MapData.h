#pragma once

class MapData
{
public:
	bool LoadMapFromTxt(const std::string& path);
	bool IsWalkable(int x, int z);

private:
	concurrent_vector<concurrent_vector<int>> _walkableMap;
};