#ifndef TORUS_H
#define TORUS_H

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

# define PI 3.1416

using namespace std;

const int MIN_MAJOR_SEGMENTS = 3;
const int MIN_MINOR_SEGMENTS = 3;

const int MIN_SECTOR_COUNT = 3;
const int MIN_STACK_COUNT = 2;

class Torus {
public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;

    // Constructor
    Torus(float majorRadius = 1.0f, float minorRadius = 0.3f, int majorSegments = 36, int minorSegments = 18,
        glm::vec3 amb = glm::vec3(1.0, 0.0, 0.0), glm::vec3 diff = glm::vec3(1.0, 0.0, 0.0),
        glm::vec3 spec = glm::vec3(1.0f, 0.0f, 0.0f), float shiny = 32.0f) : verticesStride(24) {
        set(majorRadius, minorRadius, majorSegments, minorSegments, amb, diff, spec, shiny);
        buildCoordinatesAndIndices();
        buildVertices();

        // Generate VAO, VBO, EBO
        glGenVertexArrays(1, &torusVAO);
        glBindVertexArray(torusVAO);

        unsigned int torusVBO, torusEBO;

        glGenBuffers(1, &torusVBO);
        glBindBuffer(GL_ARRAY_BUFFER, torusVBO);
        glBufferData(GL_ARRAY_BUFFER, this->getVertexSize(), this->getVertices(), GL_STATIC_DRAW);

        glGenBuffers(1, &torusEBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, torusEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->getIndexSize(), this->getIndices(), GL_STATIC_DRAW);

        // Enable attribute arrays
        glEnableVertexAttribArray(0); // Position
        glEnableVertexAttribArray(1); // Normal

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, verticesStride, (void*)0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, verticesStride, (void*)(3 * sizeof(float)));

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    ~Torus() {}

    // Set parameters
    void set(float majorRadius, float minorRadius, int majorSegments, int minorSegments, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny) {
        if (majorRadius > 0) this->majorRadius = majorRadius;
        if (minorRadius > 0) this->minorRadius = minorRadius;

        this->majorSegments = max(majorSegments, MIN_MAJOR_SEGMENTS);
        this->minorSegments = max(minorSegments, MIN_MINOR_SEGMENTS);

        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;
    }

    // Accessors
    unsigned int getVertexCount() const { return (unsigned int)coordinates.size() / 3; }
    unsigned int getVertexSize() const { return (unsigned int)vertices.size() * sizeof(float); }
    int getVerticesStride() const { return verticesStride; }
    const float* getVertices() const { return vertices.data(); }

    unsigned int getIndexCount() const { return (unsigned int)indices.size(); }
    unsigned int getIndexSize() const { return (unsigned int)indices.size() * sizeof(unsigned int); }
    const unsigned int* getIndices() const { return indices.data(); }

    // Draw torus
    void drawTorus(Shader& shader, glm::mat4 model) const {
        shader.use();
        shader.setVec3("material.ambient", this->ambient);
        shader.setVec3("material.diffuse", this->diffuse);
        shader.setVec3("material.specular", this->specular);
        shader.setFloat("material.shininess", this->shininess);
        shader.setMat4("model", model);

        glBindVertexArray(torusVAO);
        glDrawElements(GL_TRIANGLES, this->getIndexCount(), GL_UNSIGNED_INT, (void*)0);
        glBindVertexArray(0);
    }

private:
    unsigned int torusVAO;
    float majorRadius, minorRadius;
    int majorSegments, minorSegments;
    vector<float> vertices;
    vector<float> normals;
    vector<unsigned int> indices;
    vector<float> coordinates;
    int verticesStride;

    // Generate torus coordinates and indices
    void buildCoordinatesAndIndices() {
        float majorStep = 2.0f * PI / majorSegments;
        float minorStep = 2.0f * PI / minorSegments;

        // Generate vertices
        for (int i = 0; i <= majorSegments; ++i) {
            float majorAngle = i * majorStep;
            glm::vec3 majorCircle(cos(majorAngle), 0.0f, sin(majorAngle));

            for (int j = 0; j <= minorSegments; ++j) {
                float minorAngle = j * minorStep;
                float x = (majorRadius + minorRadius * cos(minorAngle)) * majorCircle.x;
                float y = minorRadius * sin(minorAngle);
                float z = (majorRadius + minorRadius * cos(minorAngle)) * majorCircle.z;

                coordinates.push_back(x);
                coordinates.push_back(y);
                coordinates.push_back(z);

                glm::vec3 normal = glm::normalize(glm::vec3(cos(minorAngle) * majorCircle.x, sin(minorAngle), cos(minorAngle) * majorCircle.z));
                normals.push_back(normal.x);
                normals.push_back(normal.y);
                normals.push_back(normal.z);
            }
        }

        // Generate indices
        for (int i = 0; i < majorSegments; ++i) {
            for (int j = 0; j < minorSegments; ++j) {
                int current = i * (minorSegments + 1) + j;
                int next = (i + 1) * (minorSegments + 1) + j;

                indices.push_back(current);
                indices.push_back(next);
                indices.push_back(current + 1);

                indices.push_back(current + 1);
                indices.push_back(next);
                indices.push_back(next + 1);
            }
        }
    }

    // Interleave vertices and normals
    void buildVertices() {
        for (size_t i = 0; i < coordinates.size(); i += 3) {
            vertices.push_back(coordinates[i]);
            vertices.push_back(coordinates[i + 1]);
            vertices.push_back(coordinates[i + 2]);

            vertices.push_back(normals[i]);
            vertices.push_back(normals[i + 1]);
            vertices.push_back(normals[i + 2]);
        }
    }
};

class SphereTex
{
public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;

    SphereTex(float radius = 1.0f, int sectorCount = 36, int stackCount = 18,
        glm::vec3 amb = glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3 diff = glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3 spec = glm::vec3(0.5f, 0.5f, 0.5f),
        float shiny = 32.0f) : verticesStride(32)
    {
        set(radius, sectorCount, stackCount, amb, diff, spec, shiny);
        buildCoordinatesAndIndices();
        buildVertices();

        glGenVertexArrays(1, &sphereVAO);
        glBindVertexArray(sphereVAO);

        // Create VBO for vertex data
        unsigned int sphereVBO;
        glGenBuffers(1, &sphereVBO);
        glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
        glBufferData(GL_ARRAY_BUFFER, this->getVertexSize(), this->getVertices(), GL_STATIC_DRAW);

        // Create EBO for index data
        unsigned int sphereEBO;
        glGenBuffers(1, &sphereEBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->getIndexSize(), this->getIndices(), GL_STATIC_DRAW);

        // Enable vertex attributes
        glEnableVertexAttribArray(0); // Position
        glEnableVertexAttribArray(1); // Normal
        glEnableVertexAttribArray(2); // Texture Coordinates

        // Set attribute pointers
        int stride = this->getVerticesStride();
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);                        // Position
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 3));      // Normal
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 6));      // Texture Coordinates

        // Unbind VAO and buffers
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    ~SphereTex() {}

    void set(float radius, int sectors, int stacks, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny)
    {
        if (radius > 0)
            this->radius = radius;
        this->sectorCount = sectors;
        if (sectors < MIN_SECTOR_COUNT)
            this->sectorCount = MIN_SECTOR_COUNT;
        this->stackCount = stacks;
        if (stacks < MIN_STACK_COUNT)
            this->stackCount = MIN_STACK_COUNT;
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;
    }

    unsigned int getVertexCount() const
    {
        return (unsigned int)coordinates.size() / 3;
    }

    unsigned int getVertexSize() const
    {
        return (unsigned int)vertices.size() * sizeof(float);
    }

    int getVerticesStride() const
    {
        return verticesStride; // 32 bytes: 3 (position) + 3 (normal) + 2 (texture coordinates)
    }

    const float* getVertices() const
    {
        return vertices.data();
    }

    unsigned int getIndexSize() const
    {
        return (unsigned int)indices.size() * sizeof(unsigned int);
    }

    const unsigned int* getIndices() const
    {
        return indices.data();
    }

    unsigned int getIndexCount() const
    {
        return (unsigned int)indices.size();
    }

    void drawSphere(Shader& lightingShader, unsigned int texture, glm::mat4 model) const
    {
        lightingShader.use();

        lightingShader.setVec3("material.ambient", this->ambient);
        lightingShader.setVec3("material.diffuse", this->diffuse);
        lightingShader.setVec3("material.specular", this->specular);
        lightingShader.setFloat("material.shininess", this->shininess);
        lightingShader.setMat4("model", model);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glBindVertexArray(sphereVAO);
        glDrawElements(GL_TRIANGLES, this->getIndexCount(), GL_UNSIGNED_INT, (void*)0);
        glBindVertexArray(0);
    }

private:
    void buildCoordinatesAndIndices()
    {
        float x, y, z, xz;
        float nx, ny, nz, lengthInv = 1.0f / radius;
        float s, t;

        float sectorStep = 2 * PI / sectorCount;
        float stackStep = PI / stackCount;
        float sectorAngle = -sectorStep;
        float stackAngle = PI / 2 + stackStep;

        for (int i = 0; i <= stackCount; ++i)
        {
            stackAngle -= stackStep;
            xz = radius * cosf(stackAngle);
            y = radius * sinf(stackAngle);

            for (int j = 0; j <= sectorCount; ++j)
            {
                sectorAngle += sectorStep;

                z = xz * cosf(sectorAngle);
                x = xz * sinf(sectorAngle);
                coordinates.push_back(x);
                coordinates.push_back(y);
                coordinates.push_back(z);

                nx = x * lengthInv;
                ny = y * lengthInv;
                nz = z * lengthInv;
                normals.push_back(nx);
                normals.push_back(ny);
                normals.push_back(nz);

                s = (float)j / sectorCount; // U-coordinate
                t = (float)i / stackCount; // V-coordinate
                texCoords.push_back(s);
                texCoords.push_back(t);
            }
        }

        int k1, k2;
        for (int i = 0; i < stackCount; ++i)
        {
            k1 = i * (sectorCount + 1);
            k2 = k1 + sectorCount + 1;

            for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
            {
                if (i != 0)
                {
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);
                }

                if (i != (stackCount - 1))
                {
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }
            }
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

            size_t texIndex = i / 3 * 2;
            vertices.push_back(texCoords[texIndex]);
            vertices.push_back(texCoords[texIndex + 1]);
        }
    }

    unsigned int sphereVAO;
    float radius;
    int sectorCount;
    int stackCount;
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texCoords;
    std::vector<unsigned int> indices;
    std::vector<float> coordinates;
    int verticesStride;
};

#endif // TORUS_H
