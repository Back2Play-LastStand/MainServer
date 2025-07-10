#include "pch.h"
#include "MapData.h"
#include <sstream>
#include <fstream>

constexpr int MAX_WIDTH = 200;
constexpr int MAX_HEIGHT = 200;

shared_ptr<MapData> GMapData = make_shared<MapData>();

bool MapData::LoadMapFromTxt(const std::string& path)
{
	ifstream file(path);
	if (!file.is_open())
	{
		cerr << "Failed to open map file: " << path << endl;
		return false;
	}

	string line;
	int z = 0;
	while (getline(file, line))
	{
		stringstream ss(line);
		string cell;
		concurrent_vector<int> row;
		int x = 0;

		while (getline(ss, cell, ','))
		{
			int value = stoi(cell);
			row.push_back(stoi(cell));

			if (value == 1)
				_walkablePosition.push_back({ x, z });

			++x;
		}

		_walkableMap.push_back(row);
		++z;
	}

	cout << "Map loaded: " << _walkableMap.size() << endl;
	return true;
}

bool MapData::IsWalkable(int x, int z)
{
	if (x < 0 || z < 0 || z >= _walkableMap.size() || x >= _walkableMap[z].size())
		return false;
	return _walkableMap[z][x] == 1;
}
