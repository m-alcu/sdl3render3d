#include <iostream>
#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>
#include <cstdint>
#include "objLoader.hpp"

void ObjLoader::setup(const std::string& filename) {
    loadVertices(filename);
    loadFaces();
    calculateNormals();
    calculateVertexNormals();
}

void ObjLoader::loadVertices(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Failed to open file.\n";
        return;
    }

    std::string line;
    bool readingVertices = false;
    bool readingFaces = false;

    std::vector<VertexData> vertices;
    std::vector<FaceData> faces;

    MaterialProperties properties = getMaterialProperties(MaterialType::Metal);

    slib::material material{};
    material.Ka = { properties.k_a * 0x00, properties.k_a * 0x58, properties.k_a * 0xfc };
    material.Kd = { properties.k_d * 0x00, properties.k_d * 0x58, properties.k_d * 0xfc }; 
    material.Ks = { properties.k_s * 0x00, properties.k_s * 0x58, properties.k_s * 0xfc };
    material.Ns = properties.shininess;
    materials.insert({"blue", material});

    material.Ka = { properties.k_a * 0xff, properties.k_a * 0xff, properties.k_a * 0xff };
    material.Kd = { properties.k_d * 0xff, properties.k_d * 0xff, properties.k_d * 0xff };
    material.Ks = { properties.k_s * 0xff, properties.k_s * 0xff, properties.k_s * 0xff };
    material.Ns = properties.shininess;
    materials.insert({"white", material});  

    while (std::getline(file, line)) {
        // Remove leading/trailing spaces
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);

        // Skip empty lines
        if (line.empty())
            continue;


        if (line.find("v") != std::string::npos) {
            // Example line: v -43.592037 7.219297 -21.717901
            VertexData vertexData;
            std::regex vertexRegex(R"(^v\s+([-+\.\dEe]+)\s+([-+\.\dEe]+)\s+([-+\.\dEe]+))");
            std::smatch match;

            if (std::regex_search(line, match, vertexRegex)) {
                vertexData.vertex.x = std::stof(match[1]);
                vertexData.vertex.y = std::stof(match[2]);
                vertexData.vertex.z = std::stof(match[3]);

                vertices.push_back(vertexData);
            }
        }

        if (line.find("f") != std::string::npos) {
            // Example line: f 791 763 645
            FaceData faceData;
            std::regex faceRegex(R"(^f\s+(\d+)\s+(\d+)\s+(\d+)$)");
            std::smatch match;

            if (std::regex_search(line, match, faceRegex)) {
                faceData.face.vertex1 = std::stoi(match[3])-1;
                faceData.face.vertex2 = std::stoi(match[2])-1;
                faceData.face.vertex3 = std::stoi(match[1])-1;
                faceData.face.materialKey = "blue"; // Default material key
                faces.push_back(faceData);
            }
        }
    }

    file.close();

    // Calculate total number of vertices and faces
    int num_vertex = vertices.size();
    int num_faces = faces.size();

    std::cout << "Total vertices: " << num_vertex << "\n";
    std::cout << "Total faces: " << num_faces << "\n";

    // Store vertices and faces in the class members
    ObjLoader::vertexData = vertices;
    ObjLoader::faceData = faces;
    ObjLoader::numVertices = num_vertex;
    ObjLoader::numFaces = num_faces;
}

void ObjLoader::loadFaces() {
    calculateNormals();
    calculateVertexNormals();
}

void ObjLoader::loadVertices() {
}