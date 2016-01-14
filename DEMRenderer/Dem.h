#pragma once

#include <string>
#include <exception>
#include <fstream>
#include <functional>
#include <iostream>
#include <algorithm>
#include <vector>
#include <glm.hpp>

class FileTypeException : std::exception {
public:
	virtual char *what() {
		return "Invalid file extension!";
	}
};

class FileOpenException : std::exception {
public:
	virtual char *what() {
		return ".dem file could not be opened!";
	}
};

struct DemHeader {
    char name[144];
    int levelCode;
    int patternCode;
    int planimetricCode;
    int zoneCode;
    int unitCodePlanimCoord;
    int unitCodeElevation;
    int sides;
    std::vector< std::pair<float, float> > corners;
    float minElevation;
    float maxElevation;
    float primaryAngle;
    int accuracyCode;
    float spatialResolution[3];
    int rows;
    int columns;
};

struct DemElevationVector {
    int x, y;
    int rows, columns;
    float groundPlanCoordX, groundPlanCoordY;
    float localDatum;
    float minElevation;
    float maxElevation;
    std::vector<int>elevations;
};

class Dem
{
public:
	Dem();
	virtual ~Dem();

	void Open(const std::string& path);
	void Parse();
    void Close();
    void CreateNormalizedVector();
    void CreateNormalizedMap();
    long GetElevationMapSize();
    long GetSurfaceMapSize();

    glm::vec3* GetElevationMap() const;
    glm::vec3* GetSurfaceMap() const;

private:
	std::ifstream m_file;
    DemHeader m_header;
    std::vector<DemElevationVector> m_elevations;
    glm::vec3 *m_vertices;
    glm::vec3* m_surface;
    std::vector<glm::vec3> m_vecSurface;

    //helper methods
    void LogHeader();
    void ParseHeader();
    void LogMap();
    void ParseMap();

    float ToFloat(std::string &value);
};

