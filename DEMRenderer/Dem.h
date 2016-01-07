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
    std::vector< std::pair<double, double> > corners;
    double minElevation;
    double maxElevation;
    double primaryAngle;
    int accuracyCode;
    double spatialResolution[3];
    int rows;
    int columns;
};

struct DemElevationVector {
    int x, y;
    int rows, columns;
    double groundPlanCoordX, groundPlanCoordY;
    double localDatum;
    double minElevation;
    double maxElevation;
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
    void CreateVerticeVector();
    long GetVertexMapSize();

    glm::vec3* GetVertexMap() const;

private:
	std::ifstream m_file;
    DemHeader m_header;
    std::vector<DemElevationVector> m_elevations;
    glm::vec3 *m_vertices;

    //helper methods
    void LogHeader();
    void ParseHeader();
    void LogMap();
    void ParseMap();

    double ToDouble(std::string &value);
};

