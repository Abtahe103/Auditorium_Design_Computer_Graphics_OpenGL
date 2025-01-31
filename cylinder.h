//
//  cylinder.h
//  test
//
//  Created by Nazirul Hasan on 26/9/23.
//

#ifndef cylinder_h
#define cylinder_h

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

# define PI 3.1416

using namespace std;

class Cylinder
{
public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;

    // Constructor
    Cylinder(float radius = 1.0f, float height = 2.0f, int sectorCount = 18,
        glm::vec3 amb = glm::vec3(0.05f, 0.05f, 0.05f),
        glm::vec3 diff = glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3 spec = glm::vec3(0.5f, 0.5f, 0.5f),
        float shiny = 32.0f)
        : verticesStride(32) // Updated stride for position, normal, and texture coordinates
    {
        set(radius, height, sectorCount, amb, diff, spec, shiny);
        buildCoordinatesAndIndices();
        buildVertices();

        glGenVertexArrays(1, &cylinderVAO);
        glBindVertexArray(cylinderVAO);

        // Create VBO for vertex data
        unsigned int cylinderVBO;
        glGenBuffers(1, &cylinderVBO);
        glBindBuffer(GL_ARRAY_BUFFER, cylinderVBO);
        glBufferData(GL_ARRAY_BUFFER,
            this->getVertexSize(),
            this->getVertices(),
            GL_STATIC_DRAW);

        // Create EBO for index data
        unsigned int cylinderEBO;
        glGenBuffers(1, &cylinderEBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cylinderEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            this->getIndexSize(),
            this->getIndices(),
            GL_STATIC_DRAW);

        // Activate attribute arrays
        glEnableVertexAttribArray(0); // Position
        glEnableVertexAttribArray(1); // Normal
        glEnableVertexAttribArray(2); // Texture coordinates

        // Set attribute pointers
        int stride = this->getVerticesStride();
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);                        // Position
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 3));      // Normal
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 6));      // Texture

        // Unbind VAO and buffers
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    ~Cylinder() {}

    // Setters
    void set(float radius, float height, int sectors, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny)
    {
        if (radius > 0)
            this->radius = radius;
        this->height = height;
        this->sectorCount = sectors;
        if (sectors < 3)
            this->sectorCount = 3; // Minimum sector count
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;
    }

    // Getters
    unsigned int getVertexCount() const { return (unsigned int)coordinates.size() / 3; }
    unsigned int getVertexSize() const { return (unsigned int)vertices.size() * sizeof(float); }
    int getVerticesStride() const { return verticesStride; }
    const float* getVertices() const { return vertices.data(); }
    unsigned int getIndexSize() const { return (unsigned int)indices.size() * sizeof(unsigned int); }
    const unsigned int* getIndices() const { return indices.data(); }
    unsigned int getIndexCount() const { return (unsigned int)indices.size(); }

    // Draw the cylinder
    void drawCylinder(Shader& lightingShader, unsigned int texture, glm::mat4 model) const
    {
        lightingShader.use();

        lightingShader.setVec3("material.ambient", this->ambient);
        lightingShader.setVec3("material.diffuse", this->diffuse);
        lightingShader.setVec3("material.specular", this->specular);
        lightingShader.setFloat("material.shininess", this->shininess);

        lightingShader.setMat4("model", model);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glBindVertexArray(cylinderVAO);
        glDrawElements(GL_TRIANGLES, this->getIndexCount(), GL_UNSIGNED_INT, (void*)0);
        glBindVertexArray(0);
    }

private:
    // Build geometry
    void buildCoordinatesAndIndices()
    {
        float x, z; // Vertex position
        float nx, nz; // Vertex normal
        float sectorStep = 2 * PI / sectorCount;
        float sectorAngle;

        // Generate coordinates, normals, and texture coordinates
        for (int i = 0; i <= sectorCount; ++i)
        {
            sectorAngle = i * sectorStep;

            x = radius * cosf(sectorAngle);
            z = radius * sinf(sectorAngle);

            // Bottom circle vertex
            coordinates.push_back(x);
            coordinates.push_back(-height / 2.0f);
            coordinates.push_back(z);

            // Top circle vertex
            coordinates.push_back(x);
            coordinates.push_back(height / 2.0f);
            coordinates.push_back(z);

            // Normals (pointing outwards)
            nx = cosf(sectorAngle);
            nz = sinf(sectorAngle);
            normals.push_back(nx);
            normals.push_back(0.0f);
            normals.push_back(nz);
            normals.push_back(nx);
            normals.push_back(0.0f);
            normals.push_back(nz);

            // Texture coordinates
            float u = (float)i / sectorCount; // Horizontal wrapping
            texCoords.push_back(u);
            texCoords.push_back(0.0f); // Bottom
            texCoords.push_back(u);
            texCoords.push_back(1.0f); // Top
        }

        // Center point for bottom and top circles
        coordinates.push_back(0.0f); // Bottom center
        coordinates.push_back(-height / 2.0f);
        coordinates.push_back(0.0f);

        coordinates.push_back(0.0f); // Top center
        coordinates.push_back(height / 2.0f);
        coordinates.push_back(0.0f);

        normals.push_back(0.0f); // Bottom center normal
        normals.push_back(-1.0f);
        normals.push_back(0.0f);

        normals.push_back(0.0f); // Top center normal
        normals.push_back(1.0f);
        normals.push_back(0.0f);

        texCoords.push_back(0.5f); // Bottom center texture coordinates
        texCoords.push_back(0.5f);

        texCoords.push_back(0.5f); // Top center texture coordinates
        texCoords.push_back(0.5f);

        // Indices for the side surface
        for (int i = 0; i < sectorCount; ++i)
        {
            int k1 = i * 2;     // Bottom vertex index
            int k2 = k1 + 1;    // Top vertex index

            // Two triangles per sector
            indices.push_back(k1);
            indices.push_back(k2);
            indices.push_back(k1 + 2);

            indices.push_back(k2);
            indices.push_back(k2 + 2);
            indices.push_back(k1 + 2);
        }

        // Indices for the bottom circle
        int bottomCenterIndex = (int)coordinates.size() / 3 - 2; // Index of the bottom center
        for (int i = 0; i < sectorCount; ++i)
        {
            int k1 = i * 2;     // Bottom vertex index
            int k2 = (i + 1) * 2; // Next bottom vertex index

            indices.push_back(bottomCenterIndex);
            indices.push_back(k1);
            indices.push_back(k2);
        }

        // Indices for the top circle
        int topCenterIndex = (int)coordinates.size() / 3 - 1; // Index of the top center
        for (int i = 0; i < sectorCount; ++i)
        {
            int k1 = i * 2 + 1;     // Top vertex index
            int k2 = (i + 1) * 2 + 1; // Next top vertex index

            indices.push_back(topCenterIndex);
            indices.push_back(k2);
            indices.push_back(k1);
        }
    }


    void buildVertices()
    {
        for (size_t i = 0, j = 0; i < coordinates.size(); i += 3, j += 2)
        {
            vertices.push_back(coordinates[i]);
            vertices.push_back(coordinates[i + 1]);
            vertices.push_back(coordinates[i + 2]);

            vertices.push_back(normals[i]);
            vertices.push_back(normals[i + 1]);
            vertices.push_back(normals[i + 2]);

            vertices.push_back(texCoords[j]);
            vertices.push_back(texCoords[j + 1]);
        }
    }

    // Member variables
    unsigned int cylinderVAO;
    float radius;
    float height;
    int sectorCount; // Longitude, # of slices
    vector<float> vertices;
    vector<float> normals;
    vector<unsigned int> indices;
    vector<float> texCoords; // Texture coordinates
    vector<float> coordinates;
    int verticesStride; // 32 bytes for position, normal, and texture coordinates
};


class CylinderNoTex
{
public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;

    // Constructor
    CylinderNoTex(float radius = 1.0f, float height = 2.0f, int sectorCount = 18,
        glm::vec3 amb = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 diff = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 spec = glm::vec3(0.0f, 0.0f, 0.0f),
        float shiny = 32.0f)
        : verticesStride(24) // Updated stride for position and normal
    {
        set(radius, height, sectorCount, amb, diff, spec, shiny);
        buildCoordinatesAndIndices();
        buildVertices();

        glGenVertexArrays(1, &cylinderVAO);
        glBindVertexArray(cylinderVAO);

        // Create VBO for vertex data
        unsigned int cylinderVBO;
        glGenBuffers(1, &cylinderVBO);
        glBindBuffer(GL_ARRAY_BUFFER, cylinderVBO);
        glBufferData(GL_ARRAY_BUFFER,
            this->getVertexSize(),
            this->getVertices(),
            GL_STATIC_DRAW);

        // Create EBO for index data
        unsigned int cylinderEBO;
        glGenBuffers(1, &cylinderEBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cylinderEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            this->getIndexSize(),
            this->getIndices(),
            GL_STATIC_DRAW);

        // Activate attribute arrays
        glEnableVertexAttribArray(0); // Position
        glEnableVertexAttribArray(1); // Normal

        // Set attribute pointers
        int stride = this->getVerticesStride();
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);                        // Position
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 3));      // Normal

        // Unbind VAO and buffers
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    ~CylinderNoTex() {}

    // Setters
    void set(float radius, float height, int sectors, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny)
    {
        if (radius > 0)
            this->radius = radius;
        this->height = height;
        this->sectorCount = sectors;
        if (sectors < 3)
            this->sectorCount = 3; // Minimum sector count
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;
    }

    // Getters
    unsigned int getVertexCount() const { return (unsigned int)coordinates.size() / 3; }
    unsigned int getVertexSize() const { return (unsigned int)vertices.size() * sizeof(float); }
    int getVerticesStride() const { return verticesStride; }
    const float* getVertices() const { return vertices.data(); }
    unsigned int getIndexSize() const { return (unsigned int)indices.size() * sizeof(unsigned int); }
    const unsigned int* getIndices() const { return indices.data(); }
    unsigned int getIndexCount() const { return (unsigned int)indices.size(); }

    // Draw the cylinder
    void drawCylinderNoTex(Shader& lightingShader, glm::mat4 model) const
    {
        lightingShader.use();

        lightingShader.setVec3("material.ambient", this->ambient);
        lightingShader.setVec3("material.diffuse", this->diffuse);
        lightingShader.setVec3("material.specular", this->specular);
        lightingShader.setFloat("material.shininess", this->shininess);

        lightingShader.setMat4("model", model);

        glBindVertexArray(cylinderVAO);
        glDrawElements(GL_TRIANGLES, this->getIndexCount(), GL_UNSIGNED_INT, (void*)0);
        glBindVertexArray(0);
    }

private:
    // Build geometry
    void buildCoordinatesAndIndices()
    {
        float x, z; // Vertex position
        float nx, nz; // Vertex normal
        float sectorStep = 2 * PI / sectorCount;
        float sectorAngle;

        // Generate coordinates and normals
        for (int i = 0; i <= sectorCount; ++i)
        {
            sectorAngle = i * sectorStep;

            x = radius * cosf(sectorAngle);
            z = radius * sinf(sectorAngle);

            // Bottom circle vertex
            coordinates.push_back(x);
            coordinates.push_back(-height / 2.0f);
            coordinates.push_back(z);

            // Top circle vertex
            coordinates.push_back(x);
            coordinates.push_back(height / 2.0f);
            coordinates.push_back(z);

            // Normals (pointing outwards)
            nx = cosf(sectorAngle);
            nz = sinf(sectorAngle);
            normals.push_back(nx);
            normals.push_back(0.0f);
            normals.push_back(nz);
            normals.push_back(nx);
            normals.push_back(0.0f);
            normals.push_back(nz);
        }

        // Indices for the side surface
        for (int i = 0; i < sectorCount; ++i)
        {
            int k1 = i * 2;     // Bottom vertex index
            int k2 = k1 + 1;    // Top vertex index

            // Two triangles per sector
            indices.push_back(k1);
            indices.push_back(k2);
            indices.push_back(k1 + 2);

            indices.push_back(k2);
            indices.push_back(k2 + 2);
            indices.push_back(k1 + 2);
        }

        // Indices for the bottom circle
        int bottomCenterIndex = (int)coordinates.size() / 3 - 2; // Index of the bottom center
        for (int i = 0; i < sectorCount; ++i)
        {
            int k1 = i * 2;     // Bottom vertex index
            int k2 = (i + 1) * 2; // Next bottom vertex index

            indices.push_back(bottomCenterIndex);
            indices.push_back(k1);
            indices.push_back(k2);
        }

        // Indices for the top circle
        int topCenterIndex = (int)coordinates.size() / 3 - 1; // Index of the top center
        for (int i = 0; i < sectorCount; ++i)
        {
            int k1 = i * 2 + 1;     // Top vertex index
            int k2 = (i + 1) * 2 + 1; // Next top vertex index

            indices.push_back(topCenterIndex);
            indices.push_back(k2);
            indices.push_back(k1);
        }
    }

    void buildVertices()
    {
        for (size_t i = 0; i < coordinates.size(); i += 3)
        {
            vertices.push_back(coordinates[i]);
            vertices.push_back(coordinates[i + 1]);
            vertices.push_back(coordinates[i + 2]);

            vertices.push_back(normals[i]);
            vertices.push_back(normals[i + 1]);
            vertices.push_back(normals[i + 2]);
        }
    }

    // Member variables
    unsigned int cylinderVAO;
    float radius;
    float height;
    int sectorCount; // Longitude, # of slices
    vector<float> vertices;
    vector<float> normals;
    vector<unsigned int> indices;
    vector<float> coordinates;
    int verticesStride; // 24 bytes for position and normal
};

#endif /* cylinder_h */
