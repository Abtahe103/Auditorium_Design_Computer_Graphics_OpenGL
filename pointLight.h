#pragma once
//
//  pointLight.h
//  test
//
//  Created by Nazirul Hasan on 22/9/23.
//

#ifndef pointLight_h
#define pointLight_h

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include "shader.h"

using namespace std;

class PointLight {
public:
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float k_c;
    float k_l;
    float k_q;
    int Number;

    PointLight(float posX, float posY, float posZ, float ambR, float ambG, float ambB, float diffR, float diffG, float diffB, float specR, float specG, float specB, float constant, float linear, float quadratic, int num) {
        position = glm::vec3(posX, posY, posZ);
        ambient = glm::vec3(ambR, ambG, ambB);
        diffuse = glm::vec3(diffR, diffG, diffB);
        specular = glm::vec3(specR, specG, specB);
        k_c = constant;
        k_l = linear;
        k_q = quadratic;
        Number = num - 1;
    }

    void setUpPointLight(Shader& lightingShader)
    {
        lightingShader.use();
        string lightNumber = to_string(Number);
        lightingShader.setVec3("pointLights[" + lightNumber + "].position", position);
        lightingShader.setVec3("pointLights[" + lightNumber + "].ambient", ambient * ambientOn);
        lightingShader.setVec3("pointLights[" + lightNumber + "].diffuse", diffuse * diffuseOn);
        lightingShader.setVec3("pointLights[" + lightNumber + "].specular", specular * specularOn);
        lightingShader.setFloat("pointLights[" + lightNumber + "].k_c", k_c);
        lightingShader.setFloat("pointLights[" + lightNumber + "].k_l", k_l);
        lightingShader.setFloat("pointLights[" + lightNumber + "].k_q", k_q);
    }

    void turnOff()
    {
        ambientOn = 0.0;
        diffuseOn = 0.0;
        specularOn = 0.0;
    }

    void turnOn()
    {
        ambientOn = 1.0;
        diffuseOn = 1.0;
        specularOn = 1.0;
    }

    void turnAmbientOn()
    {
        ambientOn = 1.0;
    }

    void turnAmbientOff()
    {
        ambientOn = 0.0;
    }

    void turnDiffuseOn()
    {
        diffuseOn = 1.0;
    }

    void turnDiffuseOff()
    {
        diffuseOn = 0.0;
    }

    void turnSpecularOn()
    {
        specularOn = 1.0;
    }

    void turnSpecularOff()
    {
        specularOn = 0.0;
    }

private:
    float ambientOn = 1.0;
    float diffuseOn = 1.0;
    float specularOn = 1.0;
};

#endif /* pointLight_h */