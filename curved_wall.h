#ifndef CUBIC_CURVED_WALL_TEX_H
#define CUBIC_CURVED_WALL_TEX_H

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include "Shader.h" // Include your Shader class here

class CubicCurvedWallTex
{
public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;

    CubicCurvedWallTex(float outerRadius = 10.0f, float innerRadius = 9.8f, float height = 10.0f, float angle = 90.0f, int segmentCount = 50,
        glm::vec3 amb = glm::vec3(1.0f, 0.5f, 0.31f),
        glm::vec3 diff = glm::vec3(1.0f, 0.5f, 0.31f),
        glm::vec3 spec = glm::vec3(0.5f, 0.5f, 0.5f),
        float shiny = 32.0f)
        : verticesStride(32)
    {
        set(outerRadius, innerRadius, height, angle, segmentCount, amb, diff, spec, shiny);
        buildCoordinatesAndIndices();
        buildVertices();

        glGenVertexArrays(1, &wallVAO);
        glBindVertexArray(wallVAO);

        // Create VBO for vertex data
        unsigned int wallVBO;
        glGenBuffers(1, &wallVBO);
        glBindBuffer(GL_ARRAY_BUFFER, wallVBO);
        glBufferData(GL_ARRAY_BUFFER, getVertexSize(), getVertices(), GL_STATIC_DRAW);

        // Create EBO for index data
        unsigned int wallEBO;
        glGenBuffers(1, &wallEBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wallEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, getIndexSize(), getIndices(), GL_STATIC_DRAW);

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

    ~CubicCurvedWallTex() {}

    void set(float outerRadius, float innerRadius, float height, float angle, int segments, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny)
    {
        this->outerRadius = outerRadius;
        this->innerRadius = innerRadius;
        this->height = height;
        this->angle = glm::radians(angle); // Convert degrees to radians
        this->segmentCount = segments > 2 ? segments : 2; // Ensure at least two segments
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

    void drawCubicCurvedWall(Shader& lightingShader, unsigned int texture, glm::mat4 model) const
    {
        lightingShader.use();

        lightingShader.setVec3("material.ambient", this->ambient);
        lightingShader.setVec3("material.diffuse", this->diffuse);
        lightingShader.setVec3("material.specular", this->specular);
        lightingShader.setFloat("material.shininess", this->shininess);
        lightingShader.setMat4("model", model);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glBindVertexArray(wallVAO);
        glDrawElements(GL_TRIANGLES, getIndexCount(), GL_UNSIGNED_INT, (void*)0);
        glBindVertexArray(0);
    }

private:
    void buildCoordinatesAndIndices()
    {
        float thetaStep = angle / segmentCount;
        float halfHeight = height / 2.0f;

        // Outer and inner wall vertices
        for (int i = 0; i <= segmentCount; ++i)
        {
            float theta = i * thetaStep;

            // Outer wall
            float outerX = outerRadius * cosf(theta);
            float outerZ = outerRadius * sinf(theta);

            // Bottom vertex (outer)
            coordinates.push_back(outerX);
            coordinates.push_back(-halfHeight);
            coordinates.push_back(outerZ);
            normals.push_back(cosf(theta));
            normals.push_back(0.0f);
            normals.push_back(sinf(theta));
            texCoords.push_back((float)i / segmentCount);
            texCoords.push_back(0.0f);

            // Top vertex (outer)
            coordinates.push_back(outerX);
            coordinates.push_back(halfHeight);
            coordinates.push_back(outerZ);
            normals.push_back(cosf(theta));
            normals.push_back(0.0f);
            normals.push_back(sinf(theta));
            texCoords.push_back((float)i / segmentCount);
            texCoords.push_back(1.0f);

            // Inner wall
            float innerX = innerRadius * cosf(theta);
            float innerZ = innerRadius * sinf(theta);

            // Bottom vertex (inner)
            coordinates.push_back(innerX);
            coordinates.push_back(-halfHeight);
            coordinates.push_back(innerZ);
            normals.push_back(-cosf(theta));
            normals.push_back(0.0f);
            normals.push_back(-sinf(theta));
            texCoords.push_back((float)i / segmentCount);
            texCoords.push_back(0.0f);

            // Top vertex (inner)
            coordinates.push_back(innerX);
            coordinates.push_back(halfHeight);
            coordinates.push_back(innerZ);
            normals.push_back(-cosf(theta));
            normals.push_back(0.0f);
            normals.push_back(-sinf(theta));
            texCoords.push_back((float)i / segmentCount);
            texCoords.push_back(1.0f);
        }

        // Create indices for the outer and inner walls
        for (int i = 0; i < segmentCount; ++i)
        {
            int k1 = i * 4; // Outer bottom-left
            int k2 = k1 + 1; // Outer top-left
            int k3 = k1 + 2; // Inner bottom-left
            int k4 = k1 + 3; // Inner top-left

            // Outer wall
            indices.push_back(k1);
            indices.push_back(k2);
            indices.push_back(k1 + 4);
            indices.push_back(k2);
            indices.push_back(k1 + 5);
            indices.push_back(k1 + 4);

            // Inner wall
            indices.push_back(k3 + 4);
            indices.push_back(k4 + 4);
            indices.push_back(k3);
            indices.push_back(k4 + 4);
            indices.push_back(k4);
            indices.push_back(k3);

            // Connect inner and outer walls at the sides (caps)
            if (i == 0 || i == segmentCount - 1)
            {
                // Left cap
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k3);

                indices.push_back(k3);
                indices.push_back(k2);
                indices.push_back(k4);

                // Right cap
                indices.push_back(k1 + 4);
                indices.push_back(k3 + 4);
                indices.push_back(k1);

                indices.push_back(k3 + 4);
                indices.push_back(k3);
                indices.push_back(k1);
            }
        }

        // Top and bottom caps
        for (int i = 0; i < segmentCount; ++i)
        {
            int k1 = i * 4;     // Outer bottom-left
            int k2 = k1 + 4;   // Outer bottom-right
            int k3 = k1 + 2;
            // Bottom cap
            indices.push_back(k1);      // Outer bottom-left
            indices.push_back(k2);      // Outer bottom-right
            indices.push_back(k3);      // Inner bottom-left

            indices.push_back(k3);      // Inner bottom-left
            indices.push_back(k2);      // Inner bottom-right
            indices.push_back(k3 + 4);  // Inner bottom-right (next segment)

            // Top cap
            indices.push_back(k1 + 1);  // Outer top-left
            indices.push_back(k2 + 1);  // Outer top-right
            indices.push_back(k3 + 1);  // Inner top-left

            indices.push_back(k3 + 1);  // Inner top-left
            indices.push_back(k2 + 1);  // Outer top-right
            indices.push_back(k3 + 5);  // Inner top-right (next segment)
        }
    }

    void buildVertices()
    {
        // Combine coordinates, normals, and texture coordinates into a single array
        vertices.clear();
        for (size_t i = 0; i < coordinates.size() / 3; ++i)
        {
            vertices.push_back(coordinates[i * 3 + 0]); // x
            vertices.push_back(coordinates[i * 3 + 1]); // y
            vertices.push_back(coordinates[i * 3 + 2]); // z
            vertices.push_back(normals[i * 3 + 0]);     // nx
            vertices.push_back(normals[i * 3 + 1]);     // ny
            vertices.push_back(normals[i * 3 + 2]);     // nz
            vertices.push_back(texCoords[i * 2 + 0]);   // u
            vertices.push_back(texCoords[i * 2 + 1]);   // v
        }
    }

private:
    float outerRadius, innerRadius, height, angle;
    int segmentCount;
    std::vector<float> coordinates;
    std::vector<float> normals;
    std::vector<float> texCoords;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    unsigned int wallVAO;
    int verticesStride;
};

#endif // CUBIC_CURVED_WALL_TEX_H
