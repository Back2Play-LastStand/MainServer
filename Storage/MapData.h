#pragma once

class MapData
{
public:
	bool LoadMapFromTxt(const std::string& path);
	bool IsWalkable(int x, int z);

	concurrent_vector<pair<int, int>> GetAllWalkablePositions() const { return _walkablePosition; }

private:
	concurrent_vector<concurrent_vector<int>> _walkableMap;
	concurrent_vector<pair<int, int>> _walkablePosition;
};

extern shared_ptr<MapData> GMapData;