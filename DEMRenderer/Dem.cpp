#include "stdafx.h"
#include "Dem.h"


Dem::Dem()
{
}


Dem::~Dem()
{
    Close();
}

void Dem::Open(const std::string& path) {

	std::function<bool(const std::string&)> isDemFile =
		[&](const std::string& path) -> bool {
		std::string extension = path.substr(path.find_last_of("."));
        std::transform(extension.begin(), extension.end(), extension.begin(),  ::tolower);
		return extension == ".dem";
	};

	if (!isDemFile(path)) {
		throw FileTypeException();
	}

	m_file.open(path);
	if (!m_file.is_open()) {
		throw FileOpenException();
	}
}

float Dem::ToFloat(std::string& value) {
    std::replace(value.begin(), value.end(), 'D', 'E');
    return std::stof(value);
}

void Dem::ParseHeader() {
    m_file.get(m_header.name, std::extent<decltype(m_header.name)>::value);
    m_file >> m_header.levelCode;
    m_file >> m_header.patternCode;
    m_file >> m_header.planimetricCode;
    m_file >> m_header.zoneCode;

    //ignore next 15 values of 0
    std::string unused;
    for (int i = 0; i < 15; ++i) {
        m_file >> unused;
    }

    m_file >> m_header.unitCodePlanimCoord;
    m_file >> m_header.unitCodeElevation;
    m_file >> m_header.sides;

    std::string coord1;
    std::string coord2;
    for (int i = 0; i < m_header.sides; ++i) {
        m_file >> coord1;
        m_file >> coord2;
        m_header.corners.push_back(std::make_pair<float, float>(ToFloat(coord1), ToFloat(coord2)));
    }

    std::string strMaxElevation;
    std::string strMinElevation;

    m_file >> strMinElevation;
    m_file >> strMaxElevation;

    m_header.minElevation = ToFloat(strMinElevation);
    m_header.maxElevation = ToFloat(strMaxElevation);

    std::string value;

    m_file >> value;
    m_header.primaryAngle = ToFloat(value);

    std::string value2;
    //m_file >> m_header.accuracyCode;

    m_file >> value2;
    m_header.spatialResolution[0] = ToFloat(value);
    m_header.spatialResolution[1] = ToFloat(value);
    m_header.spatialResolution[2] = ToFloat(value);

    m_file >> m_header.rows;
    m_file >> m_header.columns;

    m_file >> unused;
    m_file >> unused;

    LogHeader();
}

void Dem::ParseMap() {
    for (int i = 0; i < m_header.columns; ++i) {
        DemElevationVector elevationVector;
        m_file >> elevationVector.x;
        m_file >> elevationVector.y;
        m_file >> elevationVector.rows;
        m_file >> elevationVector.columns;
        std::string value;
        m_file >> value;
        elevationVector.groundPlanCoordX = ToFloat(value);
        m_file >> value;
        elevationVector.groundPlanCoordY = ToFloat(value);
        m_file >> value;
        elevationVector.localDatum = ToFloat(value);
        m_file >> value;
        elevationVector.minElevation = ToFloat(value);
        m_file >> value;
        elevationVector.maxElevation = ToFloat(value);

        int aux;
        for (int j = 0; j < elevationVector.rows; ++j) {
            m_file >> aux;
            elevationVector.elevations.push_back(aux);
        }

        m_elevations.push_back(std::move(elevationVector));
    }

    CreateNormalizedMap();

    //LogMap();
}

void Dem::Parse() {
    if (!m_file.is_open()) {
        throw FileOpenException();
    }
    ParseHeader();
    ParseMap();
}

void Dem::CreateNormalizedVector() {
    std::function<float(float, float, float)> normalize = [&](float val, float min, float max) {
        return (val - min) / (max - min);
    };
    size_t counter = 0;
    for (int i = 0; i < m_header.columns; ++i) {
        for (int j = 0; j < m_elevations.at(i).rows; ++j) {
            m_surfacePoints.push_back(std::move(glm::vec3(
                normalize(i, 0.0f, m_header.columns),
                normalize(m_elevations.at(i).elevations.at(j), m_header.minElevation, m_header.maxElevation),
                normalize(j, 0.0f, m_elevations.at(i).rows))));
        }
    }
}

void Dem::CreateNormalizedMap() {
    std::function<float(float, float, float)> normalize = [&](float val, float min, float max) {
        return (val - min) / (max - min);
    };
    long counter = 0;
    for (int i = 0; i < m_header.columns - 1; ++i) {
        for (int j = 0; j < m_elevations.at(i).rows - 1 && j < m_elevations.at(i + 1) .rows- 1; ++j) {
            glm::vec3 point1 = glm::vec3(
                normalize(i, 0.0f, m_header.columns),
                normalize(m_elevations.at(i).elevations.at(j), m_header.minElevation, m_header.maxElevation),
                normalize(j, 0.0f, m_elevations.at(i).rows));
            glm::vec3 point2 = glm::vec3(
                normalize(i, 0.0f, m_header.columns),
                normalize(m_elevations.at(i).elevations.at(j + 1), m_header.minElevation, m_header.maxElevation),
                normalize(j + 1, 0.0f, m_elevations.at(i).rows));
            glm::vec3 point3 = glm::vec3(
                normalize(i + 1, 0.0f, m_header.columns),
                normalize(m_elevations.at(i + 1).elevations.at(j), m_header.minElevation, m_header.maxElevation),
                normalize(j, 0.0f, m_elevations.at(i + 1).rows));
            glm::vec3 point4 = glm::vec3(
                normalize(i + 1, 0.0f, m_header.columns),
                normalize(m_elevations.at(i + 1).elevations.at(j + 1), m_header.minElevation, m_header.maxElevation),
                normalize(j + 1, 0.0f, m_elevations.at(i + 1).rows));

            m_surfaceTriangles.push_back(std::move(point1));
            m_surfaceTriangles.push_back(std::move(point2));
            m_surfaceTriangles.push_back(std::move(point3));

            m_surfaceTriangles.push_back(std::move(point3));
            m_surfaceTriangles.push_back(std::move(point4));
            m_surfaceTriangles.push_back(std::move(point2));
        }
    }
}

void Dem::LogHeader() {
    std::cout << "Name: " << m_header.name << std::endl;
    std::cout << "DEM Level code:" << m_header.levelCode << std::endl;
    std::cout << "Planimetric reference code:" << m_header.planimetricCode << std::endl;
    std::cout << "Zone code:" << m_header.zoneCode << std::endl;
    std::cout << "Unit code for ground planimtetric coordinates:" << m_header.unitCodePlanimCoord << std::endl;
    std::cout << "Unit code for elevation:" << m_header.unitCodeElevation << std::endl;
    std::cout << "Number of sides:" << m_header.sides << std::endl;

    std::cout << "Corner coordinates:" << std::endl;
    for (int i = 0; i < m_header.sides; ++i) {
        std::cout << m_header.corners.at(i).first << " " << m_header.corners.at(i).second << std::endl;
    }

    std::cout << "Minimum elevation:" << m_header.minElevation << std::endl;
    std::cout << "Maximum elevation:" << m_header.maxElevation << std::endl;
    std::cout << "Counterclockwise angle from primary axis:" << m_header.primaryAngle << std::endl;
    std::cout << "Accuracy code:" << m_header.accuracyCode << std::endl;
    std::cout << "Dem spatial resolution: " << m_header.spatialResolution[0] << " " << m_header.spatialResolution[1] 
        << m_header.spatialResolution[2] << std::endl;
    std::cout << "Rows:" << m_header.rows << std::endl;
    std::cout << "Columns:" << m_header.columns << std::endl;
}

void Dem::LogMap() {
    int size = GetElevationMapSize();
    std::cout << "Vertex vector size:" << size << std::endl;
    for (int i = 0; i < m_header.columns; ++i) {
        for (int j = 0; j < m_elevations.at(i).rows; ++j) {
            std::cout << m_elevations.at(i).elevations.at(j) << " ";
        }
        std::cout << std::endl;
    }
}

long Dem::GetElevationMapSize() {
    return m_surfacePoints.size();
}

long Dem::GetSurfaceMapSize() {
    return m_surfaceTriangles.size();
}

glm::vec3* Dem::GetElevationMap() {
    return m_surfacePoints.data();
}

glm::vec3* Dem::GetSurfaceMap() {
    return m_surfaceTriangles.data();
}

void Dem::Close() {
    m_file.close();
}
