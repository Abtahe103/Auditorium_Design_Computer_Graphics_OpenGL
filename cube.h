//
//  cube.h
//  test
//
//  Created by Nazirul Hasan on 4/10/23.
//

#ifndef cube_h
#define cube_h

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

using namespace std;

class Cube {
public:

    // materialistic property
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    // texture property
    float TXmin = 0.0f;
    float TXmax = 1.0f;
    float TYmin = 0.0f;
    float TYmax = 1.0f;
    unsigned int diffuseMap;
    unsigned int specularMap;

    // common property
    float shininess;

    // constructors
    Cube()
    {
        setUpCubeVertexDataAndConfigureVertexAttribute();
    }

    Cube(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny)
    {
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;

        setUpCubeVertexDataAndConfigureVertexAttribute();
    }

    Cube(unsigned int dMap, unsigned int sMap, float shiny, float textureXmin, float textureYmin, float textureXmax, float textureYmax)
    {
        this->diffuseMap = dMap;
        this->specularMap = sMap;
        this->shininess = shiny;
        this->TXmin = textureXmin;
        this->TYmin = textureYmin;
        this->TXmax = textureXmax;
        this->TYmax = textureYmax;

        setUpCubeVertexDataAndConfigureVertexAttribute();
    }

    // destructor
    ~Cube()
    {
        glDeleteVertexArrays(1, &cubeVAO);
        glDeleteVertexArrays(1, &lightCubeVAO);
        glDeleteVertexArrays(1, &lightTexCubeVAO);
        glDeleteBuffers(1, &cubeVBO);
        glDeleteBuffers(1, &cubeEBO);
    }

    void drawCubeWithTexture(Shader& lightingShaderWithTexture, glm::mat4 model = glm::mat4(1.0f))
    {
        lightingShaderWithTexture.use();

        lightingShaderWithTexture.setInt("material.diffuse", 0);
        lightingShaderWithTexture.setInt("material.specular", 1);
        lightingShaderWithTexture.setFloat("material.shininess", this->shininess);


        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->diffuseMap);
        // bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, this->specularMap);

        lightingShaderWithTexture.setMat4("model", model);

        glBindVertexArray(lightTexCubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    void drawCubeWithMaterialisticProperty(Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f))
    {
        lightingShader.use();

        lightingShader.setVec3("material.ambient", this->ambient);
        lightingShader.setVec3("material.diffuse", this->diffuse);
        lightingShader.setVec3("material.specular", this->specular);
        lightingShader.setFloat("material.shininess", this->shininess);

        lightingShader.setMat4("model", model);

        glBindVertexArray(lightCubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    void drawCube(Shader& shader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
    {
        shader.use();

        shader.setVec3("color", glm::vec3(r, g, b));
        shader.setMat4("model", model);

        glBindVertexArray(cubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    void setMaterialisticProperty(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny)
    {
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;
    }

    void setTextureProperty(unsigned int dMap, unsigned int sMap, float shiny)
    {
        this->diffuseMap = dMap;
        this->specularMap = sMap;
        this->shininess = shiny;
    }

private:
    unsigned int cubeVAO;
    unsigned int lightCubeVAO;
    unsigned int lightTexCubeVAO;
    unsigned int cubeVBO;
    unsigned int cubeEBO;

    void setUpCubeVertexDataAndConfigureVertexAttribute()
    {
        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------

        float cube_vertices[] = {
            // positions      // normals         // texture
            // back
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, TXmin, TYmin,
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, TXmax, TYmin,
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, TXmax, TYmax,
            -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, TXmin, TYmin,

            // right
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0, 0,
            0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0, 0,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0, 0,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0, 0,

            // front
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, TXmin, TYmin,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, TXmax, TYmin,
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, TXmax, TYmax,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, TXmin, TYmin,

            // left
            -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, TXmin, TYmin,
            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, TXmin, TYmax,
            -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, TXmax, TYmax,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, TXmax, TYmin,

            // top
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, TXmin, TYmin,
            0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, TXmin, TYmax,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, TXmax, TYmax,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, TXmax, TYmin,

            // bottom
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0, 0,
            0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0, 0,
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0, 0,
            -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0, 0
        };

        unsigned int cube_indices[] = {
            0, 3, 2,
            2, 1, 0,

            4, 5, 7,
            7, 6, 4,

            8, 9, 10,
            10, 11, 8,

            12, 13, 14,
            14, 15, 12,

            16, 17, 18,
            18, 19, 16,

            20, 21, 22,
            22, 23, 20
        };

        glGenVertexArrays(1, &cubeVAO);
        glGenVertexArrays(1, &lightCubeVAO);
        glGenVertexArrays(1, &lightTexCubeVAO);
        glGenBuffers(1, &cubeVBO);
        glGenBuffers(1, &cubeEBO);


        glBindVertexArray(lightTexCubeVAO);

        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // vertex normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)12);
        glEnableVertexAttribArray(1);

        // texture coordinate attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)24);
        glEnableVertexAttribArray(2);


        glBindVertexArray(lightCubeVAO);

        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)12);
        glEnableVertexAttribArray(1);


        glBindVertexArray(cubeVAO);

        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

};



class Cube2 {
public:

    // materialistic property
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    // texture property
    float TXmin = 0.0f;
    float TXmax = 1.0f;
    float TYmin = 0.0f;
    float TYmax = 1.0f;
    unsigned int diffuseMap;
    unsigned int specularMap;

    // common property
    float shininess;

    // constructors
    Cube2()
    {
        setUpCubeVertexDataAndConfigureVertexAttribute();
    }

    Cube2(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny)
    {
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;

        setUpCubeVertexDataAndConfigureVertexAttribute();
    }

    Cube2(unsigned int dMap, unsigned int sMap, float shiny, float textureXmin, float textureYmin, float textureXmax, float textureYmax)
    {
        this->diffuseMap = dMap;
        this->specularMap = sMap;
        this->shininess = shiny;
        this->TXmin = textureXmin;
        this->TYmin = textureYmin;
        this->TXmax = textureXmax;
        this->TYmax = textureYmax;

        setUpCubeVertexDataAndConfigureVertexAttribute();
    }

    // destructor
    ~Cube2()
    {
        glDeleteVertexArrays(1, &cubeVAO);
        glDeleteVertexArrays(1, &lightCubeVAO);
        glDeleteVertexArrays(1, &lightTexCubeVAO);
        glDeleteBuffers(1, &cubeVBO);
        glDeleteBuffers(1, &cubeEBO);
    }

    void drawCubeWithTexture(Shader& lightingShaderWithTexture, glm::mat4 model = glm::mat4(1.0f))
    {
        lightingShaderWithTexture.use();

        lightingShaderWithTexture.setInt("material.diffuse", 0);
        lightingShaderWithTexture.setInt("material.specular", 1);
        lightingShaderWithTexture.setFloat("material.shininess", this->shininess);


        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->diffuseMap);
        // bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, this->specularMap);

        lightingShaderWithTexture.setMat4("model", model);

        glBindVertexArray(lightTexCubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    void drawCubeWithMaterialisticProperty(Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f))
    {
        lightingShader.use();

        lightingShader.setVec3("material.ambient", this->ambient);
        lightingShader.setVec3("material.diffuse", this->diffuse);
        lightingShader.setVec3("material.specular", this->specular);
        lightingShader.setFloat("material.shininess", this->shininess);

        lightingShader.setMat4("model", model);

        glBindVertexArray(lightCubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    void drawCube(Shader& shader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
    {
        shader.use();

        shader.setVec3("color", glm::vec3(r, g, b));
        shader.setMat4("model", model);

        glBindVertexArray(cubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    void setMaterialisticProperty(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny)
    {
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;
    }

    void setTextureProperty(unsigned int dMap, unsigned int sMap, float shiny)
    {
        this->diffuseMap = dMap;
        this->specularMap = sMap;
        this->shininess = shiny;
    }

private:
    unsigned int cubeVAO;
    unsigned int lightCubeVAO;
    unsigned int lightTexCubeVAO;
    unsigned int cubeVBO;
    unsigned int cubeEBO;

    void setUpCubeVertexDataAndConfigureVertexAttribute()
    {
        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------

        float cube_vertices[] = {
            // positions      // normals         // texture
            // back
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0, 0,
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0, 0,
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0, 0,
            -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0, 0,

            // right
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0, 0,
            0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0, 0,
            0.25f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0, 0,
            0.25f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0, 0,

            // front
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0, 0,
            0.25f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0, 0,
            0.25f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0, 0,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0, 0,

            // left
            -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, TXmin, TYmin,
            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, TXmin, TYmax,
            -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, TXmax, TYmax,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, TXmax, TYmin,

            // top
            0.25f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, TXmin, TYmin,
            0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, TXmin, TYmax,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, TXmax, TYmax,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, TXmax, TYmin,

            // bottom
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, TXmin, TYmin,
            0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, TXmin, TYmax,
            0.25f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, TXmax, TYmax,
            -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, TXmax, TYmin
        };

        unsigned int cube_indices[] = {
            0, 3, 2,
            2, 1, 0,

            4, 5, 7,
            7, 6, 4,

            8, 9, 10,
            10, 11, 8,

            12, 13, 14,
            14, 15, 12,

            16, 17, 18,
            18, 19, 16,

            20, 21, 22,
            22, 23, 20
        };

        glGenVertexArrays(1, &cubeVAO);
        glGenVertexArrays(1, &lightCubeVAO);
        glGenVertexArrays(1, &lightTexCubeVAO);
        glGenBuffers(1, &cubeVBO);
        glGenBuffers(1, &cubeEBO);


        glBindVertexArray(lightTexCubeVAO);

        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // vertex normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)12);
        glEnableVertexAttribArray(1);

        // texture coordinate attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)24);
        glEnableVertexAttribArray(2);


        glBindVertexArray(lightCubeVAO);

        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)12);
        glEnableVertexAttribArray(1);


        glBindVertexArray(cubeVAO);

        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

};


class Roof {
public:

    // Materialistic properties
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    // Texture properties
    float TXmin = 0.0f;
    float TXmax = 1.0f;
    float TYmin = 0.0f;
    float TYmax = 1.0f;
    unsigned int diffuseMap;
    unsigned int specularMap;

    // Common properties
    float shininess;

    // Constructors
    Roof()
    {
        setUpRoofVertexDataAndConfigureVertexAttribute();
    }

    Roof(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny)
    {
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;

        setUpRoofVertexDataAndConfigureVertexAttribute();
    }

    Roof(unsigned int dMap, unsigned int sMap, float shiny, float textureXmin, float textureYmin, float textureXmax, float textureYmax)
    {
        this->diffuseMap = dMap;
        this->specularMap = sMap;
        this->shininess = shiny;
        this->TXmin = textureXmin;
        this->TYmin = textureYmin;
        this->TXmax = textureXmax;
        this->TYmax = textureYmax;

        setUpRoofVertexDataAndConfigureVertexAttribute();
    }

    // Destructor
    ~Roof()
    {
        glDeleteVertexArrays(1, &roofVAO);
        glDeleteVertexArrays(1, &lightRoofVAO);
        glDeleteVertexArrays(1, &lightTexRoofVAO);
        glDeleteBuffers(1, &roofVBO);
        glDeleteBuffers(1, &roofEBO);
    }

    void drawRoofWithTexture(Shader& lightingShaderWithTexture, glm::mat4 model = glm::mat4(1.0f))
    {
        lightingShaderWithTexture.use();

        lightingShaderWithTexture.setInt("material.diffuse", 0);
        lightingShaderWithTexture.setInt("material.specular", 1);
        lightingShaderWithTexture.setFloat("material.shininess", this->shininess);

        // Bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->diffuseMap);
        // Bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, this->specularMap);

        lightingShaderWithTexture.setMat4("model", model);

        glBindVertexArray(lightTexRoofVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    void drawRoofWithMaterialisticProperty(Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f))
    {
        lightingShader.use();

        lightingShader.setVec3("material.ambient", this->ambient);
        lightingShader.setVec3("material.diffuse", this->diffuse);
        lightingShader.setVec3("material.specular", this->specular);
        lightingShader.setFloat("material.shininess", this->shininess);

        lightingShader.setMat4("model", model);

        glBindVertexArray(lightRoofVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    void drawRoof(Shader& shader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
    {
        shader.use();

        shader.setVec3("color", glm::vec3(r, g, b));
        shader.setMat4("model", model);

        glBindVertexArray(roofVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    void setMaterialisticProperty(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny)
    {
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;
    }

    void setTextureProperty(unsigned int dMap, unsigned int sMap, float shiny)
    {
        this->diffuseMap = dMap;
        this->specularMap = sMap;
        this->shininess = shiny;
    }

private:
    unsigned int roofVAO;
    unsigned int lightRoofVAO;
    unsigned int lightTexRoofVAO;
    unsigned int roofVBO;
    unsigned int roofEBO;

    void setUpRoofVertexDataAndConfigureVertexAttribute()
    {
        // Vertex data for the roof (customize this as needed for your roof object)
        float roof_vertices[] = {
            // positions      // normals         // texture
            // back
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0, 0,
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0, 0,
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0, 0,
            -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0, 0,

            // right
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0, 0,
            0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0, 0,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0, 0,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0, 0,

            // front
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0, 0,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0, 0,
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0, 0,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0, 0,

            // left
            -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, TXmin, TYmin,
            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, TXmin, TYmax,
            -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, TXmax, TYmax,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, TXmax, TYmin,

            // top
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, TXmin, TYmin,
            0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, TXmin, TYmax,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, TXmax, TYmax,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, TXmax, TYmin,

            // bottom
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0, 0,
            0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0, 0,
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0, 0,
            -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0, 0
        };

        unsigned int roof_indices[] = {
            0, 3, 2,
            2, 1, 0,

            4, 5, 7,
            7, 6, 4,

            8, 9, 10,
            10, 11, 8,

            12, 13, 14,
            14, 15, 12,

            16, 17, 18,
            18, 19, 16,

            20, 21, 22,
            22, 23, 20
        };

        glGenVertexArrays(1, &roofVAO);
        glGenVertexArrays(1, &lightRoofVAO);
        glGenVertexArrays(1, &lightTexRoofVAO);
        glGenBuffers(1, &roofVBO);
        glGenBuffers(1, &roofEBO);

        glBindVertexArray(lightTexRoofVAO);

        glBindBuffer(GL_ARRAY_BUFFER, roofVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(roof_vertices), roof_vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, roofEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(roof_indices), roof_indices, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)12);
        glEnableVertexAttribArray(1);

        // Texture coordinate attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)24);
        glEnableVertexAttribArray(2);

        glBindVertexArray(lightRoofVAO);

        glBindBuffer(GL_ARRAY_BUFFER, roofVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, roofEBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)12);
        glEnableVertexAttribArray(1);

        glBindVertexArray(roofVAO);

        glBindBuffer(GL_ARRAY_BUFFER, roofVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, roofEBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }
};


class Angular_roof {
public:

    // materialistic property
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    // texture property
    float TXmin = 0.0f;
    float TXmax = 1.0f;
    float TYmin = 0.0f;
    float TYmax = 1.0f;
    unsigned int diffuseMap;
    unsigned int specularMap;

    // common property
    float shininess;

    // constructors
    Angular_roof()
    {
        setUpCubeVertexDataAndConfigureVertexAttribute();
    }

    Angular_roof(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny)
    {
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;

        setUpCubeVertexDataAndConfigureVertexAttribute();
    }

    Angular_roof(unsigned int dMap, unsigned int sMap, float shiny, float textureXmin, float textureYmin, float textureXmax, float textureYmax)
    {
        this->diffuseMap = dMap;
        this->specularMap = sMap;
        this->shininess = shiny;
        this->TXmin = textureXmin;
        this->TYmin = textureYmin;
        this->TXmax = textureXmax;
        this->TYmax = textureYmax;

        setUpCubeVertexDataAndConfigureVertexAttribute();
    }

    // destructor
    ~Angular_roof()
    {
        glDeleteVertexArrays(1, &cubeVAO);
        glDeleteVertexArrays(1, &lightCubeVAO);
        glDeleteVertexArrays(1, &lightTexCubeVAO);
        glDeleteBuffers(1, &cubeVBO);
        glDeleteBuffers(1, &cubeEBO);
    }

    void drawCubeWithTexture(Shader& lightingShaderWithTexture, glm::mat4 model = glm::mat4(1.0f))
    {
        lightingShaderWithTexture.use();

        lightingShaderWithTexture.setInt("material.diffuse", 0);
        lightingShaderWithTexture.setInt("material.specular", 1);
        lightingShaderWithTexture.setFloat("material.shininess", this->shininess);


        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->diffuseMap);
        // bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, this->specularMap);

        lightingShaderWithTexture.setMat4("model", model);

        glBindVertexArray(lightTexCubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    void drawCubeWithMaterialisticProperty(Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f))
    {
        lightingShader.use();

        lightingShader.setVec3("material.ambient", this->ambient);
        lightingShader.setVec3("material.diffuse", this->diffuse);
        lightingShader.setVec3("material.specular", this->specular);
        lightingShader.setFloat("material.shininess", this->shininess);

        lightingShader.setMat4("model", model);

        glBindVertexArray(lightCubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    void drawCube(Shader& shader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
    {
        shader.use();

        shader.setVec3("color", glm::vec3(r, g, b));
        shader.setMat4("model", model);

        glBindVertexArray(cubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    void setMaterialisticProperty(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny)
    {
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;
    }

    void setTextureProperty(unsigned int dMap, unsigned int sMap, float shiny)
    {
        this->diffuseMap = dMap;
        this->specularMap = sMap;
        this->shininess = shiny;
    }

private:
    unsigned int cubeVAO;
    unsigned int lightCubeVAO;
    unsigned int lightTexCubeVAO;
    unsigned int cubeVBO;
    unsigned int cubeEBO;

    void setUpCubeVertexDataAndConfigureVertexAttribute()
    {
        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------

        float cube_vertices[] = {
            // positions      // normals         // texture
            // back
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0, 0,
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0, 0,
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0, 0,
            -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0, 0,

            // right
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0, 0,
            0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0, 0,
            0.25f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0, 0,
            0.25f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0, 0,

            // front
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0, 0,
            0.25f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0, 0,
            0.25f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0, 0,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0, 0,

            // left
            -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, TXmin, TYmin,
            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, TXmin, TYmax,
            -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, TXmax, TYmax,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, TXmax, TYmin,

            // top
            0.25f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, TXmin, TYmin,
            0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, TXmin, TYmax,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, TXmax, TYmax,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, TXmax, TYmin,

            // bottom
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, TXmin, TYmin,
            0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, TXmin, TYmax,
            0.25f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, TXmax, TYmax,
            -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, TXmax, TYmin
        };

        unsigned int cube_indices[] = {
            0, 3, 2,
            2, 1, 0,

            4, 5, 7,
            7, 6, 4,

            8, 9, 10,
            10, 11, 8,

            12, 13, 14,
            14, 15, 12,

            16, 17, 18,
            18, 19, 16,

            20, 21, 22,
            22, 23, 20
        };

        glGenVertexArrays(1, &cubeVAO);
        glGenVertexArrays(1, &lightCubeVAO);
        glGenVertexArrays(1, &lightTexCubeVAO);
        glGenBuffers(1, &cubeVBO);
        glGenBuffers(1, &cubeEBO);


        glBindVertexArray(lightTexCubeVAO);

        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // vertex normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)12);
        glEnableVertexAttribArray(1);

        // texture coordinate attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)24);
        glEnableVertexAttribArray(2);


        glBindVertexArray(lightCubeVAO);

        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)12);
        glEnableVertexAttribArray(1);


        glBindVertexArray(cubeVAO);

        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

};

class RightWall {
public:

    // materialistic property
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    // texture property
    float TXmin = 0.0f;
    float TXmax = 1.0f;
    float TYmin = 0.0f;
    float TYmax = 1.0f;
    unsigned int diffuseMap;
    unsigned int specularMap;

    // common property
    float shininess;

    // constructors
    RightWall()
    {
        setUpRightWallVertexDataAndConfigureVertexAttribute();
    }

    RightWall(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny)
    {
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;

        setUpRightWallVertexDataAndConfigureVertexAttribute();
    }

    RightWall(unsigned int dMap, unsigned int sMap, float shiny, float textureXmin, float textureYmin, float textureXmax, float textureYmax)
    {
        this->diffuseMap = dMap;
        this->specularMap = sMap;
        this->shininess = shiny;
        this->TXmin = textureXmin;
        this->TYmin = textureYmin;
        this->TXmax = textureXmax;
        this->TYmax = textureYmax;

        setUpRightWallVertexDataAndConfigureVertexAttribute();
    }

    // destructor
    ~RightWall()
    {
        glDeleteVertexArrays(1, &rightWallVAO);
        glDeleteVertexArrays(1, &lightRightWallVAO);
        glDeleteVertexArrays(1, &lightTexRightWallVAO);
        glDeleteBuffers(1, &rightWallVBO);
        glDeleteBuffers(1, &rightWallEBO);
    }

    void drawRightWallWithTexture(Shader& lightingShaderWithTexture, glm::mat4 model = glm::mat4(1.0f))
    {
        lightingShaderWithTexture.use();

        lightingShaderWithTexture.setInt("material.diffuse", 0);
        lightingShaderWithTexture.setInt("material.specular", 1);
        lightingShaderWithTexture.setFloat("material.shininess", this->shininess);

        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->diffuseMap);
        // bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, this->specularMap);

        lightingShaderWithTexture.setMat4("model", model);

        glBindVertexArray(lightTexRightWallVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    void drawRightWallWithMaterialisticProperty(Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f))
    {
        lightingShader.use();

        lightingShader.setVec3("material.ambient", this->ambient);
        lightingShader.setVec3("material.diffuse", this->diffuse);
        lightingShader.setVec3("material.specular", this->specular);
        lightingShader.setFloat("material.shininess", this->shininess);

        lightingShader.setMat4("model", model);

        glBindVertexArray(lightRightWallVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    void drawRightWall(Shader& shader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
    {
        shader.use();

        shader.setVec3("color", glm::vec3(r, g, b));
        shader.setMat4("model", model);

        glBindVertexArray(rightWallVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    void setMaterialisticProperty(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny)
    {
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;
    }

    void setTextureProperty(unsigned int dMap, unsigned int sMap, float shiny)
    {
        this->diffuseMap = dMap;
        this->specularMap = sMap;
        this->shininess = shiny;
    }

private:
    unsigned int rightWallVAO;
    unsigned int lightRightWallVAO;
    unsigned int lightTexRightWallVAO;
    unsigned int rightWallVBO;
    unsigned int rightWallEBO;

    void setUpRightWallVertexDataAndConfigureVertexAttribute()
    {
        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------

        float rightWall_vertices[] = {
            // positions      // normals         // texture
            // back
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, TXmax, TYmin,
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, TXmin, TYmin,
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, TXmin, TYmax,
            -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, TXmax, TYmax,

            // right
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, TXmax, TYmin,
            0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, TXmax, TYmax,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, TXmin, TYmin,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, TXmin, TYmax,

            // front
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, TXmin, TYmin,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, TXmax, TYmin,
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, TXmax, TYmax,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, TXmin, TYmax,

            // left
            -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, TXmin, TYmin,
            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, TXmin, TYmax,
            -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, TXmax, TYmax,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, TXmax, TYmin,

            // top
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, TXmin, TYmin,
            0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, TXmin, TYmax,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, TXmax, TYmax,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, TXmax, TYmin,

            // bottom
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, TXmin, TYmin,
            0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, TXmin, TYmax,
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, TXmax, TYmax,
            -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,TXmax, TYmin,
        };

        unsigned int rightWall_indices[] = {
            0, 3, 2,
            2, 1, 0,

            4, 5, 7,
            7, 6, 4,

            8, 9, 10,
            10, 11, 8,

            12, 13, 14,
            14, 15, 12,

            16, 17, 18,
            18, 19, 16,

            20, 21, 22,
            22, 23, 20
        };

        glGenVertexArrays(1, &rightWallVAO);
        glGenVertexArrays(1, &lightRightWallVAO);
        glGenVertexArrays(1, &lightTexRightWallVAO);
        glGenBuffers(1, &rightWallVBO);
        glGenBuffers(1, &rightWallEBO);

        glBindVertexArray(lightTexRightWallVAO);

        glBindBuffer(GL_ARRAY_BUFFER, rightWallVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(rightWall_vertices), rightWall_vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rightWallEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rightWall_indices), rightWall_indices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // vertex normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)12);
        glEnableVertexAttribArray(1);

        // texture coordinate attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)24);
        glEnableVertexAttribArray(2);

        glBindVertexArray(lightRightWallVAO);

        glBindBuffer(GL_ARRAY_BUFFER, rightWallVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rightWallEBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)12);
        glEnableVertexAttribArray(1);

        glBindVertexArray(rightWallVAO);

        glBindBuffer(GL_ARRAY_BUFFER, rightWallVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rightWallEBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }
};

class Door {
public:

    // materialistic property
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    // texture property
    float TXmin = 0.0f;
    float TXmax = 1.0f;
    float TYmin = 0.0f;
    float TYmax = 1.0f;
    unsigned int diffuseMap;
    unsigned int specularMap;

    // common property
    float shininess;

    // constructors
    Door()
    {
        setUpDoorVertexDataAndConfigureVertexAttribute();
    }

    Door(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny)
    {
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;

        setUpDoorVertexDataAndConfigureVertexAttribute();
    }

    Door(unsigned int dMap, unsigned int sMap, float shiny, float textureXmin, float textureYmin, float textureXmax, float textureYmax)
    {
        this->diffuseMap = dMap;
        this->specularMap = sMap;
        this->shininess = shiny;
        this->TXmin = textureXmin;
        this->TYmin = textureYmin;
        this->TXmax = textureXmax;
        this->TYmax = textureYmax;

        setUpDoorVertexDataAndConfigureVertexAttribute();
    }

    // destructor
    ~Door()
    {
        glDeleteVertexArrays(1, &doorVAO);
        glDeleteBuffers(1, &doorVBO);
        glDeleteBuffers(1, &doorEBO);
    }

    void drawDoorWithTexture(Shader& lightingShaderWithTexture, glm::mat4 model = glm::mat4(1.0f))
    {
        lightingShaderWithTexture.use();

        lightingShaderWithTexture.setInt("material.diffuse", 0);
        lightingShaderWithTexture.setInt("material.specular", 1);
        lightingShaderWithTexture.setFloat("material.shininess", this->shininess);

        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->diffuseMap);
        // bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, this->specularMap);

        lightingShaderWithTexture.setMat4("model", model);

        glBindVertexArray(doorVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    void drawDoorWithMaterialisticProperty(Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f))
    {
        lightingShader.use();

        lightingShader.setVec3("material.ambient", this->ambient);
        lightingShader.setVec3("material.diffuse", this->diffuse);
        lightingShader.setVec3("material.specular", this->specular);
        lightingShader.setFloat("material.shininess", this->shininess);

        lightingShader.setMat4("model", model);

        glBindVertexArray(doorVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    void setMaterialisticProperty(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny)
    {
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;
    }

    void setTextureProperty(unsigned int dMap, unsigned int sMap, float shiny)
    {
        this->diffuseMap = dMap;
        this->specularMap = sMap;
        this->shininess = shiny;
    }

private:
    unsigned int doorVAO;
    unsigned int doorVBO;
    unsigned int doorEBO;

    void setUpDoorVertexDataAndConfigureVertexAttribute()
    {
        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------

        float door_vertices[] = {
            // positions      // normals         // texture
            // back
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, TXmin, TYmin,
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, TXmax, TYmin,
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, TXmax, TYmax,
            -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, TXmin, TYmin,

            // right
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0, 0,
            0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0, 0,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0, 0,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0, 0,

            // front
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, TXmin, TYmin,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, TXmax, TYmin,
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, TXmax, TYmax,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, TXmin, TYmin,

            // left
            -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, TXmin, TYmin,
            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, TXmin, TYmax,
            -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, TXmax, TYmax,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, TXmax, TYmin,

            // top
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, TXmin, TYmin,
            0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, TXmin, TYmax,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, TXmax, TYmax,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, TXmax, TYmin,

            // bottom
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0, 0,
            0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0, 0,
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0, 0,
            -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0, 0
        };

        unsigned int door_indices[] = {
            0, 3, 2,
            2, 1, 0,

            4, 5, 7,
            7, 6, 4,

            8, 9, 10,
            10, 11, 8,

            12, 13, 14,
            14, 15, 12,

            16, 17, 18,
            18, 19, 16,

            20, 21, 22,
            22, 23, 20
        };

        glGenVertexArrays(1, &doorVAO);
        glGenBuffers(1, &doorVBO);
        glGenBuffers(1, &doorEBO);

        glBindVertexArray(doorVAO);

        glBindBuffer(GL_ARRAY_BUFFER, doorVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(door_vertices), door_vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, doorEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(door_indices), door_indices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // vertex normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)12);
        glEnableVertexAttribArray(1);

        // texture coordinate attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)24);
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
    }
};

#endif /* cube_h */
