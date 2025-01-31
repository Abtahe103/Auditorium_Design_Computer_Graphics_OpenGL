#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "basic_camera.h"
#include "pointLight.h"
#include "sphere.h"
#include "stb_image.h"
#include "cube.h"
#include "SpotLight.h"
#include "curved_wall.h"
#include "fractal.h"
#include "cylinder.h"
#include "BezierCurve.h"

#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model, float r, float g, float b, float shininess);
void axis(unsigned int& cubeVAO, Shader& lightingShader);
void bed(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether);
void floor(unsigned int& cubeVAO, Shader& lightingShader);
void frontWall(unsigned int& cubeVAO, Shader& lightingShader);
void triangleStage(unsigned int& triangleVAO, Shader& lightingShader);
void chair_center(unsigned int& cubeVAO, Shader& lightingShader);
void chair_left(unsigned int& cubeVAO, Shader& lightingShader, glm::vec3 position);
void drawRowOfChairs(unsigned int& cubeVAO, Shader& lightingShader);
void rightWall(unsigned int& cubeVAO, Shader& lightingShader);
void ambienton_off(Shader& lightingShader);
void diffuse_on_off(Shader& lightingShader);
void specular_on_off(Shader& lightingShader);
void drawCar(Shader& lightingShader, unsigned int& cubeVAO, unsigned int& triangleVAO);
void drawDoors(Shader& lightingShaderWithTexture, Door& door, glm::mat4 identityMatrix);
unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax);



glm::vec3 carPosition = glm::vec3(0.0f, 0.0f, 0.0f); // Initial car position
float carRotation = 0.0f; // Rotation angle in degrees

bool leftDoor1Open = false;
bool leftDoor2Open = false;
bool rightDoor1Open = false;
bool rightDoor2Open = false;

float doorOpenAngle = glm::radians(90.0f);

float left = -5.0f;
float right = 5.0f;
float bottom = -5.0f;
float top = 5.0f;
float near = 0.1f; 
float far = 100.0f;


glm::mat4 myProjection(float left, float right, float bottom, float top, float near, float far) {
    glm::mat4 result(0.0f); // Initialize to a zero matrix

    // Fill the perspective projection matrix
    result[0][0] = (2.0f * near) / (right - left);  // Scale x
    result[1][1] = (2.0f * near) / (top - bottom);  // Scale y
    result[2][0] = (right + left) / (right - left); // Translate x
    result[2][1] = (top + bottom) / (top - bottom); // Translate y
    result[2][2] = -(far + near) / (far - near);    // Scale z
    result[2][3] = -1.0f;                           // Perspective division
    result[3][2] = -(2.0f * far * near) / (far - near); // Translate z

    return result;
}

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Time management
double lastKeyPressTime = 0.0;
const double keyPressDelay = 0.2; // delay in seconds

// modelling transform
float rotationAngle = 0.0f;
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;

// camera
Camera camera(glm::vec3(0.0f, 1.1f, -5.2f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 1.0, eyeZ = 3.0;
float lookAtX = 0.0, lookAtY = 0.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);


// positions of the point lights
glm::vec3 pointLightPositions[] = {
    
    ///4 street lamp front

    glm::vec3(-30.5f, 4.0f, 14.5f),
    glm::vec3(-30.5f, 4.0f, -15.5f),
    glm::vec3(-48.5f, 4.0f, 14.5f),
    glm::vec3(-48.5f,  4.0f, -15.5f),


    //// center inside lights
    glm::vec3(-7.5, 10.7, 0.0),
    glm::vec3(-7.5, 10.7, 5.0),
    glm::vec3(-7.5, 10.7, -5.0),


    glm::vec3(-9.8, 10.5, 0.0),
    glm::vec3(-9.8, 10.5, 5.0),
    glm::vec3(-9.8, 10.5, -5.0),

    glm::vec3(-11.9, 10.3, 0.0),
    glm::vec3(-11.9, 10.3, 5.0),
    glm::vec3(-11.9, 10.3, -5.0),

    glm::vec3(-14.0, 10.1, 0.0),
    glm::vec3(-14.0, 10.1, 5.0),
    glm::vec3(-14.0, 10.1, -5.0),

    glm::vec3(-16.1, 9.9, 0.0),
    glm::vec3(-16.1, 9.9, 5.0),
    glm::vec3(-16.1, 9.9, -5.0),

    glm::vec3(-18.2, 9.7, 0.0),
    glm::vec3(-18.2, 9.7, 5.0),
    glm::vec3(-18.2, 9.7, -5.0),


    // right inside lights
    glm::vec3(-7.2, 10.7, -8.5),
    glm::vec3(-5.2, 10.7, -13.5),
    glm::vec3(-3.2, 10.7, -18.5),


    glm::vec3(-9.6, 10.5, -8.5),
    glm::vec3(-7.9, 10.5, -13.5),
    glm::vec3(-5.9, 10.5, -18.5),

    glm::vec3(-12.0, 10.3, -8.5),
    glm::vec3(-10.6, 10.3, -13.5),
    glm::vec3(-8.6, 10.3, -18.5),

    glm::vec3(-14.0, 10.1, -8.5),
    glm::vec3(-13.0, 10.1, -13.5),
    glm::vec3(-11.5, 10.1, -18.5),

    glm::vec3(-16.5, 9.9, -8.5),
    glm::vec3(-15.5, 9.9, -13.5),
    glm::vec3(-14.3, 9.9, -18.5),

    glm::vec3(-19.0, 9.7, -8.5),
    glm::vec3(-18.2, 9.7, -13.5),
    glm::vec3(-17.0, 9.7, -18.5),


    // left inside lights
    glm::vec3(-7.2, 10.7, 8.5),
    glm::vec3(-5.2, 10.7, 13.5),
    glm::vec3(-3.2, 10.7, 18.5),


    glm::vec3(-9.6, 10.5, 8.5),
    glm::vec3(-7.9, 10.5, 13.5),
    glm::vec3(-5.9, 10.5, 18.5),

    glm::vec3(-12.0, 10.3, 8.5),
    glm::vec3(-10.6, 10.3, 13.5),
    glm::vec3(-8.6, 10.3, 18.5),

    glm::vec3(-14.0, 10.1, 8.5),
    glm::vec3(-13.0, 10.1, 13.5),
    glm::vec3(-11.5, 10.1, 18.5),

    glm::vec3(-16.5, 9.9, 8.5),
    glm::vec3(-15.5, 9.9, 13.5),
    glm::vec3(-14.3, 9.9, 18.5),

    glm::vec3(-19.0, 9.7, 8.5),
    glm::vec3(-18.2, 9.7, 13.5),
    glm::vec3(-17.0, 9.7, 18.5),


    // left outside lamps
    glm::vec3(-40.5f, 4.0f, 45.0f),
    glm::vec3(-22.5f, 4.0f, 45.0f),
    glm::vec3(-4.5f, 4.0f, 45.0f),
    glm::vec3(13.5f, 4.0f, 45.0f),

    glm::vec3(-4.5f, 4.0f, 28.0f),
    glm::vec3(15.0f, 4.0f, 28.0f),


    

    //right outside lamps
  /*  glm::vec3(-30.5f, 4.0f, -30.5f),
    glm::vec3(-48.5f,  4.0f, -30.5f),*/


    
};



PointLight pointlight1(

    pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z,  // position
    0.5f, 0.5f, 0.5f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1       // light number
);
PointLight pointlight2(

    pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z,  // position
    0.5f, 0.5f, 0.5f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    2       // light number
);
PointLight pointlight3(

    pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z,  // position
    0.5f, 0.5f, 0.5f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    3       // light number
);
PointLight pointlight4(

    pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z,  // position
    0.5f, 0.5f, 0.5f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    4       // light number
);


PointLight pointlight5(
    pointLightPositions[4].x, pointLightPositions[4].y, pointLightPositions[4].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    5       // light number
);

PointLight pointlight6(
    pointLightPositions[5].x, pointLightPositions[5].y, pointLightPositions[5].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    6       // light number
);

PointLight pointlight7(
    pointLightPositions[6].x, pointLightPositions[6].y, pointLightPositions[6].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    7       // light number
);

PointLight pointlight8(
    pointLightPositions[7].x, pointLightPositions[7].y, pointLightPositions[7].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    8       // light number
);

PointLight pointlight9(
    pointLightPositions[8].x, pointLightPositions[8].y, pointLightPositions[8].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    9       // light number
);

PointLight pointlight10(
    pointLightPositions[9].x, pointLightPositions[9].y, pointLightPositions[9].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    10       // light number
);

PointLight pointlight11(
    pointLightPositions[10].x, pointLightPositions[10].y, pointLightPositions[10].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    11       // light number
);

PointLight pointlight12(
    pointLightPositions[11].x, pointLightPositions[11].y, pointLightPositions[11].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    12       // light number
);

PointLight pointlight13(
    pointLightPositions[12].x, pointLightPositions[12].y, pointLightPositions[12].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    13       // light number
);

PointLight pointlight14(
    pointLightPositions[13].x, pointLightPositions[13].y, pointLightPositions[13].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    14       // light number
);

PointLight pointlight15(
    pointLightPositions[14].x, pointLightPositions[14].y, pointLightPositions[14].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    15       // light number
);

PointLight pointlight16(
    pointLightPositions[15].x, pointLightPositions[15].y, pointLightPositions[15].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    16       // light number
);

PointLight pointlight17(
    pointLightPositions[16].x, pointLightPositions[16].y, pointLightPositions[16].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    17       // light number
);

PointLight pointlight18(
    pointLightPositions[17].x, pointLightPositions[17].y, pointLightPositions[17].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    18       // light number
);

PointLight pointlight19(
    pointLightPositions[18].x, pointLightPositions[18].y, pointLightPositions[18].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    19       // light number
);

PointLight pointlight20(
    pointLightPositions[19].x, pointLightPositions[19].y, pointLightPositions[19].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    20       // light number
);

PointLight pointlight21(
    pointLightPositions[20].x, pointLightPositions[20].y, pointLightPositions[20].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    21       // light number
);

PointLight pointlight22(
    pointLightPositions[21].x, pointLightPositions[21].y, pointLightPositions[21].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    22       // light number
);

PointLight pointlight23(
    pointLightPositions[22].x, pointLightPositions[22].y, pointLightPositions[22].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    23       // light number
);

PointLight pointlight24(
    pointLightPositions[23].x, pointLightPositions[23].y, pointLightPositions[23].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    24       // light number
);

PointLight pointlight25(
    pointLightPositions[24].x, pointLightPositions[24].y, pointLightPositions[24].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    25       // light number
);

PointLight pointlight26(
    pointLightPositions[25].x, pointLightPositions[25].y, pointLightPositions[25].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    26       // light number
);

PointLight pointlight27(
    pointLightPositions[26].x, pointLightPositions[26].y, pointLightPositions[26].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    27       // light number
);

PointLight pointlight28(
    pointLightPositions[27].x, pointLightPositions[27].y, pointLightPositions[27].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    28       // light number
);

PointLight pointlight29(
    pointLightPositions[28].x, pointLightPositions[28].y, pointLightPositions[28].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    29       // light number
);

PointLight pointlight30(

    pointLightPositions[29].x, pointLightPositions[29].y, pointLightPositions[29].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    30       // light number
);

PointLight pointlight31(

    pointLightPositions[30].x, pointLightPositions[30].y, pointLightPositions[30].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    31       // light number
);

PointLight pointlight32(

    pointLightPositions[31].x, pointLightPositions[31].y, pointLightPositions[31].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    32       // light number
);

PointLight pointlight33(

    pointLightPositions[32].x, pointLightPositions[32].y, pointLightPositions[32].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    33       // light number
);

PointLight pointlight34(

    pointLightPositions[33].x, pointLightPositions[33].y, pointLightPositions[33].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    34       // light number
);

PointLight pointlight35(

    pointLightPositions[34].x, pointLightPositions[34].y, pointLightPositions[34].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    35       // light number
);

PointLight pointlight36(

    pointLightPositions[35].x, pointLightPositions[35].y, pointLightPositions[35].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    36       // light number
);

PointLight pointlight37(

    pointLightPositions[36].x, pointLightPositions[36].y, pointLightPositions[36].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    37       // light number
);

PointLight pointlight38(

    pointLightPositions[37].x, pointLightPositions[37].y, pointLightPositions[37].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    38       // light number
);

PointLight pointlight39(

    pointLightPositions[38].x, pointLightPositions[38].y, pointLightPositions[38].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    39      // light number
);

PointLight pointlight40(

    pointLightPositions[39].x, pointLightPositions[39].y, pointLightPositions[39].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    40       // light number
);

PointLight pointlight41(
    pointLightPositions[40].x, pointLightPositions[40].y, pointLightPositions[40].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    41       // light number
);

PointLight pointlight42(
    pointLightPositions[41].x, pointLightPositions[41].y, pointLightPositions[41].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    42       // light number
);

PointLight pointlight43(
    pointLightPositions[42].x, pointLightPositions[42].y, pointLightPositions[42].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    43       // light number
);

PointLight pointlight44(
    pointLightPositions[43].x, pointLightPositions[43].y, pointLightPositions[43].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    44       // light number
);

PointLight pointlight45(
    pointLightPositions[44].x, pointLightPositions[44].y, pointLightPositions[44].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    45       // light number
);

PointLight pointlight46(
    pointLightPositions[45].x, pointLightPositions[45].y, pointLightPositions[45].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    46       // light number
);

PointLight pointlight47(
    pointLightPositions[46].x, pointLightPositions[46].y, pointLightPositions[46].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    47       // light number
);

PointLight pointlight48(
    pointLightPositions[47].x, pointLightPositions[47].y, pointLightPositions[47].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    48       // light number
);

PointLight pointlight49(
    pointLightPositions[48].x, pointLightPositions[48].y, pointLightPositions[48].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    49       // light number
);

PointLight pointlight50(
    pointLightPositions[49].x, pointLightPositions[49].y, pointLightPositions[49].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    50       // light number
);

PointLight pointlight51(
    pointLightPositions[50].x, pointLightPositions[50].y, pointLightPositions[50].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    51       // light number
);

PointLight pointlight52(
    pointLightPositions[51].x, pointLightPositions[51].y, pointLightPositions[51].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    52       // light number
);

PointLight pointlight53(
    pointLightPositions[52].x, pointLightPositions[52].y, pointLightPositions[52].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    53       // light number
);

PointLight pointlight54(
    pointLightPositions[53].x, pointLightPositions[53].y, pointLightPositions[53].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    54       // light number
);

PointLight pointlight55(
    pointLightPositions[54].x, pointLightPositions[54].y, pointLightPositions[54].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    55       // light number
);

PointLight pointlight56(
    pointLightPositions[55].x, pointLightPositions[55].y, pointLightPositions[55].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    56       // light number
);

PointLight pointlight57(
    pointLightPositions[56].x, pointLightPositions[56].y, pointLightPositions[56].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    57       // light number
);

PointLight pointlight58(
    pointLightPositions[57].x, pointLightPositions[57].y, pointLightPositions[57].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.2f, 0.2f, 0.2f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    58       // light number
);


/// left outside lamps

PointLight pointlight59(
    pointLightPositions[58].x, pointLightPositions[58].y, pointLightPositions[58].z,  // position
    0.5f, 0.5f, 0.5f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    59       // light number
);

PointLight pointlight60(
    pointLightPositions[59].x, pointLightPositions[59].y, pointLightPositions[59].z,  // position
    0.5f, 0.5f, 0.5f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    60       // light number
);

PointLight pointlight61(
    pointLightPositions[60].x, pointLightPositions[60].y, pointLightPositions[60].z,  // position
    0.5f, 0.5f, 0.5f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    61       // light number
);

PointLight pointlight62(
    pointLightPositions[61].x, pointLightPositions[61].y, pointLightPositions[61].z,  // position
    0.5f, 0.5f, 0.5f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    62       // light number
);

PointLight pointlight63(
    pointLightPositions[62].x, pointLightPositions[62].y, pointLightPositions[62].z,  // position
    0.5f, 0.5f, 0.5f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    63       // light number
);

PointLight pointlight64(
    pointLightPositions[63].x, pointLightPositions[63].y, pointLightPositions[63].z,  // position
    0.5f, 0.5f, 0.5f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    64       // light number
);




glm::vec3 spotPositions[] = {
    glm::vec3(5.0f, 3.0f, 0.5f),
    glm::vec3(8.5f, 3.0f, 11.8f),
    glm::vec3(8.5f, 3.0f, -11.8f),

    glm::vec3(15.0f, 10.0f, 2.0f),
    glm::vec3(15.0f, 10.0f, -2.0f),

};

SpotLight spotlight1(
    spotPositions[0].x, spotPositions[0].y, spotPositions[0].z,  // position
    1.0f, 1.0f, 1.0f,     // ambient
    1.0f, 1.0f, 1.0f,      // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1,       // light number
    glm::cos(glm::radians(35.5f)),
    glm::cos(glm::radians(40.0f)),
    0, -1, 0
);

SpotLight spotlight2(
    spotPositions[1].x, spotPositions[1].y, spotPositions[1].z,  // position
    1.0f, 1.0f, 1.0f,     // ambient
    1.0f, 1.0f, 1.0f,      // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    2,       // light number
    glm::cos(glm::radians(20.5f)),
    glm::cos(glm::radians(25.5f)),
    0.5, 1, 0
);

SpotLight spotlight3(
    spotPositions[2].x, spotPositions[2].y, spotPositions[2].z,  // position
    1.0f, 1.0f, 1.0f,     // ambient
    1.0f, 1.0f, 1.0f,      // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    3,       // light number
    glm::cos(glm::radians(20.5f)),
    glm::cos(glm::radians(25.5f)),
    0.5, 1, 0
);

SpotLight spotlight4(
    spotPositions[3].x, spotPositions[3].y, spotPositions[3].z,  // position
    1.0f, 1.0f, 1.0f,     // ambient
    1.0f, 1.0f, 1.0f,      // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    4,       // light number
    glm::cos(glm::radians(20.5f)),
    glm::cos(glm::radians(25.0f)),
    0, -1, 0
);

SpotLight spotlight5(
    spotPositions[4].x, spotPositions[4].y, spotPositions[4].z,  // position
    1.0f, 1.0f, 1.0f,     // ambient
    1.0f, 1.0f, 1.0f,      // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    5,       // light number
    glm::cos(glm::radians(20.5f)),
    glm::cos(glm::radians(25.0f)),
    0, -1, 0
);







// light settings
bool pointLightOn = true;
bool directionalLightOn = true;
bool SpotLightOn = true;
bool AmbientON = true;
bool DiffusionON = true;
bool SpecularON = true;
bool ambientToggle = true;
bool diffuseToggle = true;
bool specularToggle = true;
bool isRotating = false;


// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;

int main()
{

    float fov = glm::radians(45.0f);               // Field of view in radians
    float aspect = 16.0f / 9.0f;                  // Aspect ratio (e.g., 1920x1080 screen)
    float near = 0.1f;                            // Near clipping plane
    float far = 100.0f;                           // Far clipping plane

    // Calculate boundaries for the near clipping plane
    float top = tan(fov / 2.0f) * near;           // Top boundary
    float bottom = -top;                          // Bottom boundary
    float right = top * aspect;                   // Right boundary
    float left = -right;


    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    //glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShading.fs");
    Shader lightingShaderWithTexture("vertexShaderForPhongShadingWithTexture.vs", "fragmentShaderForPhongShadingWithTexture.fs");
    //Shader lightingShader("vertexShaderForGouraudShading.vs", "fragmentShaderForGouraudShading.fs");
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");


    GLfloat roof_points[] = {
   0.0,0.0,1.0,
0.25,0.25,1.0,
0.5,0.5,1.0,
0.75,0.25,1.0,
1.0,0.0,1.0,
1.25,-0.25,1.0,
1.50,-0.50,1.0,
1.75,-0.25,1.0,
2.0,0.0,1.0,
2.25,0.25,1.0,
2.50,0.50,1.0,
2.75,0.25,1.0,
3.0,0.0,1.0,
3.25,-0.25,1.0,
3.50,-0.50,1.0,
3.75,-0.25,1.0,
4.0,0.0,1.0,
3.75,-0.25,1.0,
3.5,-0.50,1.0,
3.25,-0.75,1.0,
3.0,-0.50,1.0,
2.75,-0.25,1.0,
2.5,0.0,1.0,
2.25,0.25,1.0,
2.0,0.0,1.0,
1.75,-0.25,1.0,
1.5,-0.50,1.0,
1.25,-0.75,1.0,
1.0,-0.50,1.0,
0.75,-0.25,1.0,
0.5,0.0,1.0,
0.25,0.25,1.0,
0.0,0.0,1.0,
-0.25,-0.25,1.0
    };


    GLfloat sculp_points[] = {
        -0.3, -1.8, 1.0,
        -0.8, -1.6, 1.0,
        -0.9, -1.2, 1.0,
        -0.8, -0.8, 1.0,
        -0.7, -0.4, 1.0,
        -0.2, -0.2, 1.0,
        0.0, -0.1, 1.0,
        0.2, 0.0, 1.0,
        0.7, 0.2, 1.0,
        0.8, 0.6, 1.0,
        0.9, 1.0, 1.0,
        0.8, 1.4, 1.0,
        0.3, 1.8, 1.0
    };

    


  /*  string diffuseMapPath = "container2.png";
    string specularMapPath = "container2_specular.png";*/
    string laughEmoPath = "world_map.png";
    string bitfestPath = "bitfest.jpg";
    string floor_tiles_path = "Images/floor_tiles_2.jpg";
    string brick_wall_path = "Images/Bricks_curve_wall.jpg";
    string tree_pot_path = "Images/tree_pot.jpg";
    string grass_path = "Images/grass.jpg";
    string wall_texture_path = "Images/wall_texture.jpg";
    string stage_texture_path = "Images/stage_texture.jpg";
    string curtain_texture_path = "Images/curtain_texture.jpg";
    string roof_texture_path = "Images/roof_texture.jpg";
    string inside_wall_texture_path = "Images/inside_wall_texture.jpg";
    string door_texture_path = "Images/door_mirror.png";
    string door_mirror_texture_path = "Images/door_mirror.png";
    string car_way_texture_path = "Images/car_way.jpg";

    /*unsigned int diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);*/
    unsigned int laughEmoji = loadTexture(laughEmoPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int bitfest = loadTexture(bitfestPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int floor_tiles = loadTexture(floor_tiles_path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int brick_curve_wall = loadTexture(brick_wall_path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int tree_pot = loadTexture(tree_pot_path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int grass = loadTexture(grass_path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int wall_texture = loadTexture(wall_texture_path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int stage_texture = loadTexture(stage_texture_path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int roof_texture = loadTexture(roof_texture_path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int curtain_texture = loadTexture(curtain_texture_path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int inside_wall_texture = loadTexture(inside_wall_texture_path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int door_texture = loadTexture(door_texture_path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int door_mirror_texture = loadTexture(door_mirror_texture_path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int car_way_texture = loadTexture(car_way_texture_path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    //unsigned int laughEmojiv2 = loadTexture(laughEmoPath.c_str(), GL_REPEAT, GL_MIRRORED_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    Cube cube = Cube(bitfest, bitfest, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    Cube floor_tiles_cube = Cube(floor_tiles, floor_tiles, 32.0f, 0.0f, 0.0f, 5.0f, 5.0f);
    Cube stage_design = Cube(stage_texture, stage_texture, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    Cube curtain_design = Cube(curtain_texture, curtain_texture, 10.0f, 0.0f, 0.0f, 1.0f, 10.0f);
    Cube2 floor_tiles_steps = Cube2(floor_tiles, floor_tiles, 32.0f, 0.0f, 0.0f, 20.0f, 20.0f);
    Cube2 wall_tex = Cube2(wall_texture, wall_texture, 32.0f, 0.0f, 0.0f, 10.0f, 10.0f);
    Cube side_wall = Cube(wall_texture, wall_texture, 32.0f, 0.0f, 0.0f, 10.0f, 10.0f);
    RightWall right_side_wall = RightWall(inside_wall_texture, inside_wall_texture, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    RightWall front_side_wall = RightWall(inside_wall_texture, inside_wall_texture, 32.0f, 0.0f, 0.0f, 2.0f, 1.0f);

    


    

    Roof center_roof = Roof(roof_texture, roof_texture, 32.0f, 0.0f, 0.0f, 1.0f, 10.0f);
    Angular_roof right_roof = Angular_roof(roof_texture, roof_texture, 32.0f, 0.0f, 0.0f, 1.0f, 10.0f);
    Angular_roof left_roof = Angular_roof(roof_texture, roof_texture, 32.0f, 0.0f, 0.0f, 1.0f, 10.0f);

    CubicCurvedWallTex curve_wall_right = CubicCurvedWallTex();
    CubicCurvedWallTex curve_wall_left = CubicCurvedWallTex();

    FractalTree tree = FractalTree();

    SphereTex spheretex = SphereTex();

    Cylinder treepot = Cylinder();

    Cylinder treepot_grass = Cylinder(0.8f);

    BezierCurve roof_design = BezierCurve(roof_points, 34 * 3, wall_texture);

    BezierSculpt sculpure_design = BezierSculpt(sculp_points, 13 * 3, laughEmoji);

    

    

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    float cube_vertices[] = {
        // positions      // normals
        //back
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,    // 3 - 4
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,    // |   |
        1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,    // 2 - 1
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        
        //right
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,      // 4 - 2
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,      // |   |
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,      // 3 - 1             
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        //front
        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,      // 4 - 3
        1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,      // |   |   
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,      // 1 - 2
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        //left
        0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,     // 3 - 2
        0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,     // |   |
        0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,     // 4 - 1   
        0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        //up
        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,      // 3 - 2
        1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,      // |   |
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,      // 4 - 1
        0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

        //down
        0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,     // 1 - 2
        1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,     // |   |   
        1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,     // 4 - 3
        0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f
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


    float triangle_3d_vertices[] = {
        // positions      // normals

        //right
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,      // 4 - 2
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,      // |   |
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,      // 3 - 1             
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        //front
        1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,      // 4 - 3
                                                 // |   |   
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,      // 2 - 1
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        //top
        1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,      // 3 - 1
                                                 // |   |
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,      // 4 - 2
        0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

        //bottom
        1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,     // 2 - 1
                                                 // |   |   top view
        1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,     // 4 - 3
        0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,

        //diagonal
        0.0f, 0.0f, 1.0f, -1.0f, 0.0f, -1.0f,     // 4 - 2
        0.0f, 1.0f, 1.0f, -1.0f, 0.0f, -1.0f,     // |   |     diagonal / from top
        1.0f, 0.0f, 0.0f, -1.0f, 0.0f, -1.0f,     // 3 - 1
        1.0f, 1.0f, 0.0f, -1.0f, 0.0f, -1.0f,

    };

    unsigned int triangle_3d_indices[] = {
        0,1,2,
        1,2,3,

        4,5,6,
        5,6,7,

        8,9,11,

        12,14,15,


        16,17,18,
        17,18,19
    };



    unsigned int cubeVAO, cubeVBO, cubeEBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);


    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);



    unsigned int triangleVAO, triangleVBO, triangleEBO;
    glGenVertexArrays(1, &triangleVAO);
    glGenBuffers(1, &triangleVBO);
    glGenBuffers(1, &triangleEBO);

    glBindVertexArray(triangleVAO);

    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_3d_vertices), triangle_3d_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangle_3d_indices), triangle_3d_indices, GL_STATIC_DRAW);


    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightTriangleVAO;
    glGenVertexArrays(1, &lightTriangleVAO);
    glBindVertexArray(lightTriangleVAO);

    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleEBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    /*Cone cone = Cone();*/

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);



    //ourShader.use();
    //lightingShader.use();

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = myProjection(left, right, bottom, top, near, far);

        glm::mat4 view = camera.GetViewMatrix();

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // pass projection matrix to shader (note that in this case it could change every frame)
        
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
       

        // camera/view transformation
        
        //glm::mat4 view = basic_camera.createViewMatrix();
        


        // point light 1
        pointlight1.setUpPointLight(lightingShader);
        // point light 2
        pointlight2.setUpPointLight(lightingShader);
        // point light 3
        pointlight3.setUpPointLight(lightingShader);
        //// point light 4
        pointlight4.setUpPointLight(lightingShader);
        pointlight5.setUpPointLight(lightingShader);
        pointlight6.setUpPointLight(lightingShader);
        pointlight7.setUpPointLight(lightingShader);
        pointlight8.setUpPointLight(lightingShader);
        pointlight9.setUpPointLight(lightingShader);
        pointlight10.setUpPointLight(lightingShader);
        pointlight11.setUpPointLight(lightingShader);
        pointlight12.setUpPointLight(lightingShader);
        pointlight13.setUpPointLight(lightingShader);
        pointlight14.setUpPointLight(lightingShader);
        pointlight15.setUpPointLight(lightingShader);
        pointlight16.setUpPointLight(lightingShader);
        pointlight17.setUpPointLight(lightingShader);
        pointlight18.setUpPointLight(lightingShader);
        pointlight19.setUpPointLight(lightingShader);
        pointlight20.setUpPointLight(lightingShader);
        pointlight21.setUpPointLight(lightingShader);
        pointlight22.setUpPointLight(lightingShader);
        pointlight23.setUpPointLight(lightingShader);
        pointlight24.setUpPointLight(lightingShader);
        pointlight25.setUpPointLight(lightingShader);
        pointlight26.setUpPointLight(lightingShader);
        pointlight27.setUpPointLight(lightingShader);
        pointlight28.setUpPointLight(lightingShader);
        pointlight29.setUpPointLight(lightingShader);
        pointlight30.setUpPointLight(lightingShader);
        pointlight31.setUpPointLight(lightingShader);
        pointlight32.setUpPointLight(lightingShader);
        pointlight33.setUpPointLight(lightingShader);
        pointlight34.setUpPointLight(lightingShader);
        pointlight35.setUpPointLight(lightingShader);
        pointlight36.setUpPointLight(lightingShader);
        pointlight37.setUpPointLight(lightingShader);
        pointlight38.setUpPointLight(lightingShader);
        pointlight39.setUpPointLight(lightingShader);
        pointlight40.setUpPointLight(lightingShader);
        pointlight41.setUpPointLight(lightingShader);
        pointlight42.setUpPointLight(lightingShader);
        pointlight43.setUpPointLight(lightingShader);
        pointlight44.setUpPointLight(lightingShader);
        pointlight45.setUpPointLight(lightingShader);
        pointlight46.setUpPointLight(lightingShader);
        pointlight47.setUpPointLight(lightingShader);
        pointlight48.setUpPointLight(lightingShader);
        pointlight49.setUpPointLight(lightingShader);
        pointlight50.setUpPointLight(lightingShader);
        pointlight51.setUpPointLight(lightingShader);
        pointlight52.setUpPointLight(lightingShader);
        pointlight53.setUpPointLight(lightingShader);
        pointlight54.setUpPointLight(lightingShader);
        pointlight55.setUpPointLight(lightingShader);
        pointlight56.setUpPointLight(lightingShader);
        pointlight57.setUpPointLight(lightingShader);
        pointlight58.setUpPointLight(lightingShader);
        pointlight59.setUpPointLight(lightingShader);
        pointlight60.setUpPointLight(lightingShader);
        pointlight61.setUpPointLight(lightingShader);
        pointlight62.setUpPointLight(lightingShader);
        pointlight63.setUpPointLight(lightingShader);
        pointlight64.setUpPointLight(lightingShader);



        spotlight1.setUpspotLight(lightingShader);
        spotlight2.setUpspotLight(lightingShader);
        spotlight3.setUpspotLight(lightingShader);
        spotlight4.setUpspotLight(lightingShader);
        spotlight5.setUpspotLight(lightingShader);

        lightingShader.setVec3("directionalLight.direction", 0.0f, -1.0f, 0.0f);
        lightingShader.setVec3("directionalLight.ambient", .2f, .2f, .2f);
        lightingShader.setVec3("directionalLight.diffuse", .8f, .8f, .8f);
        lightingShader.setVec3("directionalLight.specular", 1.0f, 1.0f, 1.0f);       

        lightingShader.setBool("directionalLightON", directionalLightOn);
        
       

        // Modelling Transformation
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, rotation, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);


        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setVec3("viewPos", camera.Position);
        lightingShaderWithTexture.setMat4("projection", projection);
        lightingShaderWithTexture.setMat4("view", view);
        //// point light 1
        pointlight1.setUpPointLight(lightingShaderWithTexture);
        //// point light 2
        pointlight2.setUpPointLight(lightingShaderWithTexture);
        //// point light 3
        pointlight3.setUpPointLight(lightingShaderWithTexture);
        ////// point light 4
        pointlight4.setUpPointLight(lightingShaderWithTexture);
        pointlight5.setUpPointLight(lightingShaderWithTexture);
        pointlight6.setUpPointLight(lightingShaderWithTexture);
        pointlight7.setUpPointLight(lightingShaderWithTexture);
        pointlight8.setUpPointLight(lightingShaderWithTexture);
        pointlight9.setUpPointLight(lightingShaderWithTexture);
        pointlight10.setUpPointLight(lightingShaderWithTexture);
        pointlight11.setUpPointLight(lightingShaderWithTexture);
        pointlight12.setUpPointLight(lightingShaderWithTexture);
        pointlight13.setUpPointLight(lightingShaderWithTexture);
        pointlight14.setUpPointLight(lightingShaderWithTexture);
        pointlight15.setUpPointLight(lightingShaderWithTexture);
        pointlight16.setUpPointLight(lightingShaderWithTexture);
        pointlight17.setUpPointLight(lightingShaderWithTexture);
        pointlight18.setUpPointLight(lightingShaderWithTexture);
        pointlight19.setUpPointLight(lightingShaderWithTexture);
        pointlight20.setUpPointLight(lightingShaderWithTexture);
        pointlight21.setUpPointLight(lightingShaderWithTexture);
        pointlight22.setUpPointLight(lightingShaderWithTexture);
        pointlight23.setUpPointLight(lightingShaderWithTexture);
        pointlight24.setUpPointLight(lightingShaderWithTexture);
        pointlight25.setUpPointLight(lightingShaderWithTexture);
        pointlight26.setUpPointLight(lightingShaderWithTexture);
        pointlight27.setUpPointLight(lightingShaderWithTexture);
        pointlight28.setUpPointLight(lightingShaderWithTexture);
        pointlight29.setUpPointLight(lightingShaderWithTexture);
        pointlight30.setUpPointLight(lightingShaderWithTexture);
        pointlight31.setUpPointLight(lightingShaderWithTexture);
        pointlight32.setUpPointLight(lightingShaderWithTexture);
        pointlight33.setUpPointLight(lightingShaderWithTexture);
        pointlight34.setUpPointLight(lightingShaderWithTexture);
        pointlight35.setUpPointLight(lightingShaderWithTexture);
        pointlight36.setUpPointLight(lightingShaderWithTexture);
        pointlight37.setUpPointLight(lightingShaderWithTexture);
        pointlight38.setUpPointLight(lightingShaderWithTexture);
        pointlight39.setUpPointLight(lightingShaderWithTexture);
        pointlight40.setUpPointLight(lightingShaderWithTexture);
        pointlight41.setUpPointLight(lightingShaderWithTexture);
        pointlight42.setUpPointLight(lightingShaderWithTexture);
        pointlight43.setUpPointLight(lightingShaderWithTexture);
        pointlight44.setUpPointLight(lightingShaderWithTexture);
        pointlight45.setUpPointLight(lightingShaderWithTexture);
        pointlight46.setUpPointLight(lightingShaderWithTexture);
        pointlight47.setUpPointLight(lightingShaderWithTexture);
        pointlight48.setUpPointLight(lightingShaderWithTexture);
        pointlight49.setUpPointLight(lightingShaderWithTexture);
        pointlight50.setUpPointLight(lightingShaderWithTexture);
        pointlight51.setUpPointLight(lightingShaderWithTexture);
        pointlight52.setUpPointLight(lightingShaderWithTexture);
        pointlight53.setUpPointLight(lightingShaderWithTexture);
        pointlight54.setUpPointLight(lightingShaderWithTexture);
        pointlight55.setUpPointLight(lightingShaderWithTexture);
        pointlight56.setUpPointLight(lightingShaderWithTexture);
        pointlight57.setUpPointLight(lightingShaderWithTexture);
        pointlight58.setUpPointLight(lightingShaderWithTexture);
        pointlight59.setUpPointLight(lightingShaderWithTexture);
        pointlight60.setUpPointLight(lightingShaderWithTexture);
        pointlight61.setUpPointLight(lightingShaderWithTexture);
        pointlight62.setUpPointLight(lightingShaderWithTexture);
        pointlight63.setUpPointLight(lightingShaderWithTexture);
        pointlight64.setUpPointLight(lightingShaderWithTexture);



        spotlight1.setUpspotLight(lightingShaderWithTexture);
        spotlight2.setUpspotLight(lightingShaderWithTexture);
        spotlight3.setUpspotLight(lightingShaderWithTexture);
        spotlight4.setUpspotLight(lightingShaderWithTexture);
        spotlight5.setUpspotLight(lightingShaderWithTexture);


        lightingShaderWithTexture.setVec3("directionalLight.direction", 0.0f, -1.0f, 0.0f);
        lightingShaderWithTexture.setVec3("directionalLight.ambient", .2f, .2f, .2f);
        lightingShaderWithTexture.setVec3("directionalLight.diffuse", .8f, .8f, .8f);
        lightingShaderWithTexture.setVec3("directionalLight.specular", 1.0f, 1.0f, 1.0f);

        lightingShaderWithTexture.setBool("directionLightOn", directionalLightOn);

       




        ///............................... Object drawing....................................////

        ///......................stage design................./////

        glm::mat4 modelMatrixForContainer2 = glm::mat4(1.0f);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(12.0f, 4.5f, -12.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(6.0f, 11.0f, 4.0f));
        model = translateMatrix * scaleMatrix;
        stage_design.drawCubeWithTexture(lightingShaderWithTexture, model);


        translateMatrix = glm::translate(identityMatrix, glm::vec3(12.0f, 4.5f, 12.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(6.0f, 11.0f, 4.0f));
        model = translateMatrix * scaleMatrix;
        stage_design.drawCubeWithTexture(lightingShaderWithTexture, model);


        translateMatrix = glm::translate(identityMatrix, glm::vec3(12.0f, 10.0f, 0.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(6.0f, 28.0f, 2.0f));
        rotation = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = translateMatrix * rotation * scaleMatrix;
        stage_design.drawCubeWithTexture(lightingShaderWithTexture, model); 


        ///......................stage curtain draw......................./////

        // front right curtain
        translateMatrix = glm::translate(identityMatrix, glm::vec3(10.0f, 4.8f, -9.7f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01f, 8.5f, 0.8f));
        rotation = glm::rotate(identityMatrix, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = translateMatrix * rotation * scaleMatrix;
        curtain_design.drawCubeWithTexture(lightingShaderWithTexture, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(10.0f, 4.8f, -9.2f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01f, 8.5f, 0.8f));
        rotation = glm::rotate(identityMatrix, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = translateMatrix * rotation * scaleMatrix;
        curtain_design.drawCubeWithTexture(lightingShaderWithTexture, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(10.0f, 4.8f, -8.7f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01f, 8.5f, 0.8f));
        rotation = glm::rotate(identityMatrix, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = translateMatrix * rotation * scaleMatrix;
        curtain_design.drawCubeWithTexture(lightingShaderWithTexture, model);
       
        translateMatrix = glm::translate(identityMatrix, glm::vec3(10.0f, 4.8f, -8.2f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01f, 8.5f, 0.8f));
        rotation = glm::rotate(identityMatrix, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = translateMatrix * rotation * scaleMatrix;
        curtain_design.drawCubeWithTexture(lightingShaderWithTexture, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(10.0f, 4.8f, -7.7f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01f, 8.5f, 0.8f));
        rotation = glm::rotate(identityMatrix, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = translateMatrix * rotation * scaleMatrix;
        curtain_design.drawCubeWithTexture(lightingShaderWithTexture, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(10.0f, 4.8f, -7.2f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01f, 8.5f, 0.8f));
        rotation = glm::rotate(identityMatrix, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = translateMatrix * rotation * scaleMatrix;
        curtain_design.drawCubeWithTexture(lightingShaderWithTexture, model);





        //front left curtain

        translateMatrix = glm::translate(identityMatrix, glm::vec3(10.0f, 4.8f, 9.7f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01f, 8.5f, 0.8f));
        rotation = glm::rotate(identityMatrix, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = translateMatrix * rotation * scaleMatrix;
        curtain_design.drawCubeWithTexture(lightingShaderWithTexture, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(10.0f, 4.8f, 9.2f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01f, 8.5f, 0.8f));
        rotation = glm::rotate(identityMatrix, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = translateMatrix * rotation * scaleMatrix;
        curtain_design.drawCubeWithTexture(lightingShaderWithTexture, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(10.0f, 4.8f, 8.7f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01f, 8.5f, 0.8f));
        rotation = glm::rotate(identityMatrix, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = translateMatrix * rotation * scaleMatrix;
        curtain_design.drawCubeWithTexture(lightingShaderWithTexture, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(10.0f, 4.8f, 8.2f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01f, 8.5f, 0.8f));
        rotation = glm::rotate(identityMatrix, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = translateMatrix * rotation * scaleMatrix;
        curtain_design.drawCubeWithTexture(lightingShaderWithTexture, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(10.0f, 4.8f, 7.7f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01f, 8.5f, 0.8f));
        rotation = glm::rotate(identityMatrix, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = translateMatrix * rotation * scaleMatrix;
        curtain_design.drawCubeWithTexture(lightingShaderWithTexture, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(10.0f, 4.8f, 7.2f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01f, 8.5f, 0.8f));
        rotation = glm::rotate(identityMatrix, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = translateMatrix * rotation * scaleMatrix;
        curtain_design.drawCubeWithTexture(lightingShaderWithTexture, model);





        // back right curtain
        translateMatrix = glm::translate(identityMatrix, glm::vec3(12.0f, 4.8f, -9.4f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01f, 8.5f, 0.8f));
        rotation = glm::rotate(identityMatrix, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = translateMatrix * rotation * scaleMatrix;
        curtain_design.drawCubeWithTexture(lightingShaderWithTexture, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(12.0f, 4.8f, -8.9f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01f, 8.5f, 0.8f));
        rotation = glm::rotate(identityMatrix, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = translateMatrix * rotation * scaleMatrix;
        curtain_design.drawCubeWithTexture(lightingShaderWithTexture, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(12.0f, 4.8f, -8.4f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01f, 8.5f, 0.8f));
        rotation = glm::rotate(identityMatrix, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = translateMatrix * rotation * scaleMatrix;
        curtain_design.drawCubeWithTexture(lightingShaderWithTexture, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(12.0f, 4.8f, -7.9f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01f, 8.5f, 0.8f));
        rotation = glm::rotate(identityMatrix, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = translateMatrix * rotation * scaleMatrix;
        curtain_design.drawCubeWithTexture(lightingShaderWithTexture, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(12.0f, 4.8f, -7.4f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01f, 8.5f, 0.8f));
        rotation = glm::rotate(identityMatrix, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = translateMatrix * rotation * scaleMatrix;
        curtain_design.drawCubeWithTexture(lightingShaderWithTexture, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(12.0f, 4.8f, -6.9f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01f, 8.5f, 0.8f));
        rotation = glm::rotate(identityMatrix, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = translateMatrix * rotation * scaleMatrix;
        curtain_design.drawCubeWithTexture(lightingShaderWithTexture, model);




        // back left curtain
        translateMatrix = glm::translate(identityMatrix, glm::vec3(12.0f, 4.8f, 9.4f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01f, 8.5f, 0.8f));
        rotation = glm::rotate(identityMatrix, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = translateMatrix * rotation * scaleMatrix;
        curtain_design.drawCubeWithTexture(lightingShaderWithTexture, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(12.0f, 4.8f, 8.9f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01f, 8.5f, 0.8f));
        rotation = glm::rotate(identityMatrix, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = translateMatrix * rotation * scaleMatrix;
        curtain_design.drawCubeWithTexture(lightingShaderWithTexture, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(12.0f, 4.8f, 8.4f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01f, 8.5f, 0.8f));
        rotation = glm::rotate(identityMatrix, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = translateMatrix * rotation * scaleMatrix;
        curtain_design.drawCubeWithTexture(lightingShaderWithTexture, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(12.0f, 4.8f, 7.9f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01f, 8.5f, 0.8f));
        rotation = glm::rotate(identityMatrix, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = translateMatrix * rotation * scaleMatrix;
        curtain_design.drawCubeWithTexture(lightingShaderWithTexture, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(12.0f, 4.8f, 7.4f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01f, 8.5f, 0.8f));
        rotation = glm::rotate(identityMatrix, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = translateMatrix * rotation * scaleMatrix;
        curtain_design.drawCubeWithTexture(lightingShaderWithTexture, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(12.0f, 4.8f, 6.9f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01f, 8.5f, 0.8f));
        rotation = glm::rotate(identityMatrix, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = translateMatrix * rotation * scaleMatrix;
        curtain_design.drawCubeWithTexture(lightingShaderWithTexture, model);






        glm::mat4 translate = glm::mat4(1.0f);
        //glm::mat4 translate2 = glm::mat4(1.0f);
        glm::mat4 scale = glm::mat4(1.0f);



        /// sphere draw

        if (isRotating) {
            rotationAngle += 1.0f;  // Adjust this value to control rotation speed
            if (rotationAngle >= 360.0f) {
                rotationAngle -= 360.0f;  // Reset to prevent overflow
            }
        }



        glm::mat4 modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(6.0f, 1.4f, 3.8f));
        rotation = glm::rotate(identityMatrix, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrixForContainer = modelMatrixForContainer * rotation;
        spheretex.drawSphere(lightingShaderWithTexture,laughEmoji ,modelMatrixForContainer);

        ///roof design

        modelMatrixForContainer = glm::mat4(1.0f);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-4.0f, 16.0f, 0.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(9.0f, 15.0f, 9.0f));
        modelMatrixForContainer = translateMatrix * scaleMatrix;
        roof_design.drawBezierCurve(lightingShaderWithTexture, modelMatrixForContainer);


        ///roof drawing and light placement

        //Center 1st step
        scale = glm::scale(identityMatrix, glm::vec3(16.6, 0.2, 15.0));
        translate = glm::translate(identityMatrix, glm::vec3(-14.7, 11.0, 0.0));
        model = translate * scale;
        center_roof.drawRoofWithTexture(lightingShaderWithTexture, model);



        //center 2nd step
        scale = glm::scale(identityMatrix, glm::vec3(14.4, 0.2, 15.0));
        translate = glm::translate(identityMatrix, glm::vec3(-15.8, 10.8, 0.0));
        model = translate * scale;
        center_roof.drawRoofWithTexture(lightingShaderWithTexture, model);

        //center 3rd step
        scale = glm::scale(identityMatrix, glm::vec3(12.2, 0.2, 15.0));
        translate = glm::translate(identityMatrix, glm::vec3(-16.9, 10.6, 0.0));
        model = translate * scale;
        center_roof.drawRoofWithTexture(lightingShaderWithTexture, model);

        //center 4th step
        scale = glm::scale(identityMatrix, glm::vec3(10.0, 0.2, 15.0));
        translate = glm::translate(identityMatrix, glm::vec3(-18.0, 10.4, 0.0));
        model = translate * scale;
        center_roof.drawRoofWithTexture(lightingShaderWithTexture, model);

        //center 5th step
        scale = glm::scale(identityMatrix, glm::vec3(7.8, 0.2, 15.0));
        translate = glm::translate(identityMatrix, glm::vec3(-19.1, 10.2, 0.0));
        model = translate * scale;
        center_roof.drawRoofWithTexture(lightingShaderWithTexture, model);

        //center 6th step
        scale = glm::scale(identityMatrix, glm::vec3(5.6, 0.2, 15.0));
        translate = glm::translate(identityMatrix, glm::vec3(-20.2, 10.0, 0.0));
        model = translate * scale;
        center_roof.drawRoofWithTexture(lightingShaderWithTexture, model);




        //right angular 1st step
        scale = glm::scale(identityMatrix, glm::vec3(22.13, 0.2, 12.5));
        translate = glm::translate(identityMatrix, glm::vec3(-11.94, 11.0, -13.75));
        /*rotation = glm::rotate(identityMatrix, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));*/
        model = translate * scale;
        right_roof.drawCubeWithTexture(lightingShaderWithTexture, model);

        //right angular 2nd step
        scale = glm::scale(identityMatrix, glm::vec3(19.2, 0.2, 12.5));
        translate = glm::translate(identityMatrix, glm::vec3(-13.40, 10.8, -13.75));

        model = translate * scale;
        right_roof.drawCubeWithTexture(lightingShaderWithTexture, model);

        //right angular 3rd step
        scale = glm::scale(identityMatrix, glm::vec3(16.27, 0.2, 12.5));
        translate = glm::translate(identityMatrix, glm::vec3(-14.87, 10.6, -13.75));

        model = translate * scale;
        right_roof.drawCubeWithTexture(lightingShaderWithTexture, model);

        //right angular 4th step
        scale = glm::scale(identityMatrix, glm::vec3(13.34, 0.2, 12.5));
        translate = glm::translate(identityMatrix, glm::vec3(-16.34, 10.4, -13.75));

        model = translate * scale;
        right_roof.drawCubeWithTexture(lightingShaderWithTexture, model);

        //right angular 5th step
        scale = glm::scale(identityMatrix, glm::vec3(10.41, 0.2, 12.5));
        translate = glm::translate(identityMatrix, glm::vec3(-17.805, 10.2, -13.75));

        model = translate * scale;
        right_roof.drawCubeWithTexture(lightingShaderWithTexture, model);

        //right angular 6th step
        scale = glm::scale(identityMatrix, glm::vec3(7.48, 0.2, 12.5));
        translate = glm::translate(identityMatrix, glm::vec3(-19.27, 10.0, -13.75));

        model = translate * scale;
        right_roof.drawCubeWithTexture(lightingShaderWithTexture, model);





        //left angular 1st step
        scale = glm::scale(identityMatrix, glm::vec3(22.13, 0.2, 12.5));
        translate = glm::translate(identityMatrix, glm::vec3(-11.92, 11.0, 13.75));
        rotation = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = translate * scale * rotation;
        left_roof.drawCubeWithTexture(lightingShaderWithTexture, model);

        //left angular 2nd step
        scale = glm::scale(identityMatrix, glm::vec3(19.2, 0.2, 12.5));
        translate = glm::translate(identityMatrix, glm::vec3(-13.39, 10.8, 13.75));
        rotation = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = translate * scale * rotation;
        left_roof.drawCubeWithTexture(lightingShaderWithTexture, model);

        //left angular 3rd step
        scale = glm::scale(identityMatrix, glm::vec3(16.27, 0.2, 12.5));
        translate = glm::translate(identityMatrix, glm::vec3(-14.86, 10.6, 13.75));
        rotation = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = translate * scale * rotation;
        left_roof.drawCubeWithTexture(lightingShaderWithTexture, model);

        //left angular 4th step
        scale = glm::scale(identityMatrix, glm::vec3(13.34, 0.2, 12.5));
        translate = glm::translate(identityMatrix, glm::vec3(-16.33, 10.4, 13.75));
        rotation = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = translate * scale * rotation;
        left_roof.drawCubeWithTexture(lightingShaderWithTexture, model);

        //left angular 5th step
        scale = glm::scale(identityMatrix, glm::vec3(10.41, 0.2, 12.5));
        translate = glm::translate(identityMatrix, glm::vec3(-17.80, 10.2, 13.75));
        rotation = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = translate * scale * rotation;
        left_roof.drawCubeWithTexture(lightingShaderWithTexture, model);

        //left angular 6th step
        scale = glm::scale(identityMatrix, glm::vec3(7.48, 0.2, 12.5));
        translate = glm::translate(identityMatrix, glm::vec3(-19.27, 10.0, 13.75));
        rotation = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = translate * scale * rotation;
        left_roof.drawCubeWithTexture(lightingShaderWithTexture, model);






        // right curved wall
        
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-18.0f, 3.0f, -10.0f));
        rotation = glm::rotate(identityMatrix, glm::radians(150.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.2f, 0.8f, 1.8f));
        modelMatrixForContainer = translateMatrix * rotation * scaleMatrix;
        curve_wall_right.drawCubicCurvedWall(lightingShaderWithTexture,brick_curve_wall, modelMatrixForContainer);


        // left curved wall
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-10.0f, 3.0f, 13.0f));
        rotation = glm::rotate(identityMatrix, glm::radians(100.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateXMatrix = glm::rotate(rotation, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.45f, 0.8f, 1.8f));
        modelMatrixForContainer = translateMatrix * rotateXMatrix * scaleMatrix;
        curve_wall_right.drawCubicCurvedWall(lightingShaderWithTexture, brick_curve_wall, modelMatrixForContainer);



       



        //right wall straight outside 1st
        scale = glm::scale(identityMatrix, glm::vec3(10.0, 0.3, 6.5));
        translate = glm::translate(identityMatrix, glm::vec3(-8.0, 2.2, -24.5));
        rotation = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateXMatrix = glm::rotate(rotation, glm::radians(-10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = translate * rotateXMatrix * scale;
        wall_tex.drawCubeWithTexture(lightingShaderWithTexture, model);

        //right wall straight outside 2nd
        scale = glm::scale(identityMatrix, glm::vec3(10.0, 0.3, 4.5));
        translate = glm::translate(identityMatrix, glm::vec3(-4.0, 1.2, -23.0));
        rotation = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateXMatrix = glm::rotate(rotation, glm::radians(-5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = translate * rotateXMatrix * scale;
        wall_tex.drawCubeWithTexture(lightingShaderWithTexture, model);



        //left wall straight outside 1st
        scale = glm::scale(identityMatrix, glm::vec3(10.0, 0.3, 6.5));
        translate = glm::translate(identityMatrix, glm::vec3(-8.0, 2.2, 24.5));
        rotation = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateXMatrix = glm::rotate(rotation, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = translate * rotateXMatrix * scale;
        wall_tex.drawCubeWithTexture(lightingShaderWithTexture, model);

        //left wall straight outside 2nd
        scale = glm::scale(identityMatrix, glm::vec3(10.0, 0.3, 4.5));
        translate = glm::translate(identityMatrix, glm::vec3(-4.0, 1.2, 23));
        rotation = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateXMatrix = glm::rotate(rotation, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = translate * rotateXMatrix * scale;
        wall_tex.drawCubeWithTexture(lightingShaderWithTexture, model);



        ///**********Doors**********/////

        Door door = Door(door_texture, door_texture, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
        drawDoors(lightingShaderWithTexture, door, identityMatrix);


        /*/// left door1
        scale = glm::scale(identityMatrix, glm::vec3(4.0, 4.8, 0.2));
        translate = glm::translate(identityMatrix, glm::vec3(3.0, 1.6, 19.9));
        model = translate * scale;

        door.drawDoorWithTexture(lightingShaderWithTexture, model);

        ///left door2
        scale = glm::scale(identityMatrix, glm::vec3(4.0, 4.8, 0.2));
        translate = glm::translate(identityMatrix, glm::vec3(7.0, 1.6, 19.9));
        rotation = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = translate * rotation * scale;

        door.drawDoorWithTexture(lightingShaderWithTexture, model);


        /// right door1
        scale = glm::scale(identityMatrix, glm::vec3(4.0, 4.8, 0.2));
        translate = glm::translate(identityMatrix, glm::vec3(3.0, 1.6, -19.9));
        model = translate * scale;

        door.drawDoorWithTexture(lightingShaderWithTexture, model);

        ///right door2
        scale = glm::scale(identityMatrix, glm::vec3(4.0, 4.8, 0.2));
        translate = glm::translate(identityMatrix, glm::vec3(7.0, 1.6, 19.9));
        rotation = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = translate * rotation * scale;

        door.drawDoorWithTexture(lightingShaderWithTexture, model);*/

        

        ////......side walls....../////

        ///right
        scale = glm::scale(identityMatrix, glm::vec3(24.0, 0.2, 12.0));
        translate = glm::translate(identityMatrix, glm::vec3(-11.0, 5.0, -20.0));
        rotation = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = translate * rotation * scale;
        side_wall.drawCubeWithTexture(lightingShaderWithTexture, model);

        //texture
        scale = glm::scale(identityMatrix, glm::vec3(24.0, 0.2, 12.0));
        translate = glm::translate(identityMatrix, glm::vec3(-11.0, 5.0, -19.9));
        rotation = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = translate * rotation * scale;
        right_side_wall.drawRightWallWithTexture(lightingShaderWithTexture, model);

        scale = glm::scale(identityMatrix, glm::vec3(8.0, 0.2, 7.0));
        translate = glm::translate(identityMatrix, glm::vec3(5.0, 7.5, -20.0));
        rotation = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = translate * rotation * scale;
        side_wall.drawCubeWithTexture(lightingShaderWithTexture, model);

        scale = glm::scale(identityMatrix, glm::vec3(6.5, 0.2, 12.0));
        translate = glm::translate(identityMatrix, glm::vec3(12.2, 5.0, -20.0));
        rotation = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = translate * rotation * scale;
        side_wall.drawCubeWithTexture(lightingShaderWithTexture, model);




        ///left
        scale = glm::scale(identityMatrix, glm::vec3(24.0, 0.2, 12.0));
        translate = glm::translate(identityMatrix, glm::vec3(-11.0, 5.0, 20.0));
        rotation = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = translate * rotation * scale;
        side_wall.drawCubeWithTexture(lightingShaderWithTexture, model);

        //texture
        scale = glm::scale(identityMatrix, glm::vec3(24.0, 0.2, 12.0));
        translate = glm::translate(identityMatrix, glm::vec3(-11.0, 5.0, 19.9));
        rotation = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotation = glm::rotate(rotation, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = translate * rotation * scale;
        right_side_wall.drawRightWallWithTexture(lightingShaderWithTexture, model);


        scale = glm::scale(identityMatrix, glm::vec3(8.0, 0.2, 7.0));
        translate = glm::translate(identityMatrix, glm::vec3(5.0, 7.5, 20.0));
        rotation = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = translate * rotation * scale;
        side_wall.drawCubeWithTexture(lightingShaderWithTexture, model);

        scale = glm::scale(identityMatrix, glm::vec3(6.5, 0.2, 12.0));
        translate = glm::translate(identityMatrix, glm::vec3(12.2, 5.0, 20.0));
        rotation = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = translate * rotation * scale;
        side_wall.drawCubeWithTexture(lightingShaderWithTexture, model);



        ///front

        scale = glm::scale(identityMatrix, glm::vec3(40.2, 0.2, 12.0));
        translate = glm::translate(identityMatrix, glm::vec3(-23.0, 5.0, 0.0));
        rotation = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotation = glm::rotate(rotation, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = translate * rotation * scale;
        side_wall.drawCubeWithTexture(lightingShaderWithTexture, model);

        //texture
        scale = glm::scale(identityMatrix, glm::vec3(40.2, 0.2, 5.0));
        translate = glm::translate(identityMatrix, glm::vec3(-22.9, 8.0, 0.0));
        rotation = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotation = glm::rotate(rotation, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        rotation = glm::rotate(rotation, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = translate * rotation * scale;
        front_side_wall.drawRightWallWithTexture(lightingShaderWithTexture, model);


        /// back

        scale = glm::scale(identityMatrix, glm::vec3(40.2, 0.2, 12.0));
        translate = glm::translate(identityMatrix, glm::vec3(15.5, 5.0, 0.0));
        rotation = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotation = glm::rotate(rotation, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = translate * rotation * scale;
        side_wall.drawCubeWithTexture(lightingShaderWithTexture, model);

        ////*******outside floor*************/////

        Cube floor_outside = Cube(floor_tiles, floor_tiles, 32.0f, 0.0f, 0.0f, 5.0f, 5.0f);



        scale = glm::scale(identityMatrix, glm::vec3(100.0, 0.2, 100.0));
        translate = glm::translate(identityMatrix, glm::vec3(0.0, -1.4, 0.0));

        model = translate * scale;

        floor_outside.drawCubeWithTexture(lightingShaderWithTexture, model);

        ///*****car way******////

        Cube car_way = Cube(car_way_texture, car_way_texture, 32.0f, 0.0f, 0.0f, 5.0f, 5.0f);



        scale = glm::scale(identityMatrix, glm::vec3(10.0, 0.2, 80.0));
        translate = glm::translate(identityMatrix, glm::vec3(-40.0, -1.2, 0.0));

        model = translate * scale;

        car_way.drawCubeWithTexture(lightingShaderWithTexture, model);


        scale = glm::scale(identityMatrix, glm::vec3(10.0, 0.2, 70.0));
        translate = glm::translate(identityMatrix, glm::vec3(0.0, -1.2, 35.0));
        rotation = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        

        model = translate * rotation * scale;

        car_way.drawCubeWithTexture(lightingShaderWithTexture, model);


        
        /// ..... 1st floor........//////

        ///bitfest
        
        translateMatrix = glm::translate(identityMatrix, glm::vec3(5.2f, 0.7f, 0.4f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.8f, -1.8f));
        model = translateMatrix * scaleMatrix;
        cube.drawCubeWithTexture(lightingShaderWithTexture, model);

        //Entire floor
        scale = glm::scale(identityMatrix, glm::vec3(37.0, 0.2, 40.0));
        translate = glm::translate(identityMatrix, glm::vec3(-3.0, -0.9, 0.0));
        model = translate * scale;
        floor_tiles_cube.drawCubeWithTexture(lightingShaderWithTexture, model);



        //Center 1st step
        scale = glm::scale(identityMatrix, glm::vec3(16.6, 0.2, 15.0));
        translate = glm::translate(identityMatrix, glm::vec3(-14.7, -0.7, 0.0));
        model = translate * scale;
        floor_tiles_cube.drawCubeWithTexture(lightingShaderWithTexture, model);

        //center 2nd step
        scale = glm::scale(identityMatrix, glm::vec3(14.4, 0.2, 15.0));
        translate = glm::translate(identityMatrix, glm::vec3(-15.8, -0.5, 0.0));
        model = translate * scale;
        floor_tiles_cube.drawCubeWithTexture(lightingShaderWithTexture, model);

        //center 3rd step
        scale = glm::scale(identityMatrix, glm::vec3(12.2, 0.2, 15.0));
        translate = glm::translate(identityMatrix, glm::vec3(-16.9, -0.3, 0.0));
        model = translate * scale;
        floor_tiles_cube.drawCubeWithTexture(lightingShaderWithTexture, model);

        //center 4th step
        scale = glm::scale(identityMatrix, glm::vec3(10.0, 0.2, 15.0));
        translate = glm::translate(identityMatrix, glm::vec3(-18.0, -0.1, 0.0));
        model = translate * scale;
        floor_tiles_cube.drawCubeWithTexture(lightingShaderWithTexture, model);

        //center 5th step
        scale = glm::scale(identityMatrix, glm::vec3(7.8, 0.2, 15.0));
        translate = glm::translate(identityMatrix, glm::vec3(-19.1, 0.1, 0.0));
        model = translate * scale;
        floor_tiles_cube.drawCubeWithTexture(lightingShaderWithTexture, model);

        //center 6th step
        scale = glm::scale(identityMatrix, glm::vec3(5.6, 0.2, 15.0));
        translate = glm::translate(identityMatrix, glm::vec3(-20.2, 0.3, 0.0));
        model = translate * scale;
        floor_tiles_cube.drawCubeWithTexture(lightingShaderWithTexture, model);
        



        //right angular 1st step
        scale = glm::scale(identityMatrix, glm::vec3(22.13, 0.2, 12.5));
        translate = glm::translate(identityMatrix, glm::vec3(-11.94, -0.7, -13.75));
        /*rotation = glm::rotate(identityMatrix, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));*/
        model = translate * scale;
        floor_tiles_steps.drawCubeWithTexture(lightingShaderWithTexture, model);

        //right angular 2nd step
        scale = glm::scale(identityMatrix, glm::vec3(19.2, 0.2, 12.5));
        translate = glm::translate(identityMatrix, glm::vec3(-13.40, -0.5, -13.75));
        
        model = translate * scale ;
        floor_tiles_steps.drawCubeWithTexture(lightingShaderWithTexture, model);

        //right angular 3rd step
        scale = glm::scale(identityMatrix, glm::vec3(16.27, 0.2, 12.5));
        translate = glm::translate(identityMatrix, glm::vec3(-14.87, -0.3, -13.75));
        
        model = translate * scale;
        floor_tiles_steps.drawCubeWithTexture(lightingShaderWithTexture, model);

        //right angular 4th step
        scale = glm::scale(identityMatrix, glm::vec3(13.34, 0.2, 12.5));
        translate = glm::translate(identityMatrix, glm::vec3(-16.34, -0.1, -13.75));
        
        model = translate * scale;
        floor_tiles_steps.drawCubeWithTexture(lightingShaderWithTexture, model);

        //right angular 5th step
        scale = glm::scale(identityMatrix, glm::vec3(10.41, 0.2, 12.5));
        translate = glm::translate(identityMatrix, glm::vec3(-17.805, 0.1, -13.75));
        
        model = translate * scale;
        floor_tiles_steps.drawCubeWithTexture(lightingShaderWithTexture, model);

        //right angular 6th step
        scale = glm::scale(identityMatrix, glm::vec3(7.48, 0.2, 12.5));
        translate = glm::translate(identityMatrix, glm::vec3(-19.27, 0.3, -13.75));
        
        model = translate * scale;
        floor_tiles_steps.drawCubeWithTexture(lightingShaderWithTexture, model);





        //left angular 1st step
        scale = glm::scale(identityMatrix, glm::vec3(22.13, 0.2, 12.5));
        translate = glm::translate(identityMatrix, glm::vec3(-11.92, -0.7, 13.75));
        rotation = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = translate * scale * rotation;
        floor_tiles_steps.drawCubeWithTexture(lightingShaderWithTexture, model);

        //left angular 2nd step
        scale = glm::scale(identityMatrix, glm::vec3(19.2, 0.2, 12.5));
        translate = glm::translate(identityMatrix, glm::vec3(-13.39, -0.5, 13.75));
        rotation = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = translate * scale * rotation;
        floor_tiles_steps.drawCubeWithTexture(lightingShaderWithTexture, model);

        //left angular 3rd step
        scale = glm::scale(identityMatrix, glm::vec3(16.27, 0.2, 12.5));
        translate = glm::translate(identityMatrix, glm::vec3(-14.86, -0.3, 13.75));
        rotation = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = translate * scale * rotation;
        floor_tiles_steps.drawCubeWithTexture(lightingShaderWithTexture, model);

        //left angular 4th step
        scale = glm::scale(identityMatrix, glm::vec3(13.34, 0.2, 12.5));
        translate = glm::translate(identityMatrix, glm::vec3(-16.33, -0.1, 13.75));
        rotation = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = translate * scale * rotation;
        floor_tiles_steps.drawCubeWithTexture(lightingShaderWithTexture, model);

        //left angular 5th step
        scale = glm::scale(identityMatrix, glm::vec3(10.41, 0.2, 12.5));
        translate = glm::translate(identityMatrix, glm::vec3(-17.80, 0.1, 13.75));
        rotation = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = translate * scale * rotation;
        floor_tiles_steps.drawCubeWithTexture(lightingShaderWithTexture, model);

        //left angular 6th step
        scale = glm::scale(identityMatrix, glm::vec3(7.48, 0.2, 12.5));
        translate = glm::translate(identityMatrix, glm::vec3(-19.27, 0.3, 13.75));
        rotation = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = translate * scale * rotation;
        floor_tiles_steps.drawCubeWithTexture(lightingShaderWithTexture, model);



        ////...........2nd floor..........//////

        //center 4th step
        scale = glm::scale(identityMatrix, glm::vec3(10.0, 0.2, 15.0));
        translate = glm::translate(identityMatrix, glm::vec3(-18.0, 5.0, 0.0));
        model = translate * scale;
        floor_tiles_cube.drawCubeWithTexture(lightingShaderWithTexture, model);

        //center 5th step
        scale = glm::scale(identityMatrix, glm::vec3(7.8, 0.2, 15.0));
        translate = glm::translate(identityMatrix, glm::vec3(-19.1, 5.1, 0.0));
        model = translate * scale;
        floor_tiles_cube.drawCubeWithTexture(lightingShaderWithTexture, model);

        //center 6th step
        scale = glm::scale(identityMatrix, glm::vec3(5.6, 0.2, 15.0));
        translate = glm::translate(identityMatrix, glm::vec3(-20.2, 5.3, 0.0));
        model = translate * scale;
        floor_tiles_cube.drawCubeWithTexture(lightingShaderWithTexture, model);


        //right angular 4th step
        scale = glm::scale(identityMatrix, glm::vec3(13.34, 0.2, 12.5));
        translate = glm::translate(identityMatrix, glm::vec3(-16.34, 5.0, -13.75));

        model = translate * scale;
        floor_tiles_steps.drawCubeWithTexture(lightingShaderWithTexture, model);

        //right angular 5th step
        scale = glm::scale(identityMatrix, glm::vec3(10.41, 0.2, 12.5));
        translate = glm::translate(identityMatrix, glm::vec3(-17.805, 5.1, -13.75));

        model = translate * scale;
        floor_tiles_steps.drawCubeWithTexture(lightingShaderWithTexture, model);

        //right angular 6th step
        scale = glm::scale(identityMatrix, glm::vec3(7.48, 0.2, 12.5));
        translate = glm::translate(identityMatrix, glm::vec3(-19.27, 5.3, -13.75));

        model = translate * scale;
        floor_tiles_steps.drawCubeWithTexture(lightingShaderWithTexture, model);




        //left angular 4th step
        scale = glm::scale(identityMatrix, glm::vec3(13.34, 0.2, 12.5));
        translate = glm::translate(identityMatrix, glm::vec3(-16.33, 5.0, 13.75));
        rotation = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = translate * scale * rotation;
        floor_tiles_steps.drawCubeWithTexture(lightingShaderWithTexture, model);

        //left angular 5th step
        scale = glm::scale(identityMatrix, glm::vec3(10.41, 0.2, 12.5));
        translate = glm::translate(identityMatrix, glm::vec3(-17.80, 5.1, 13.75));
        rotation = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = translate * scale * rotation;
        floor_tiles_steps.drawCubeWithTexture(lightingShaderWithTexture, model);

        //left angular 6th step
        scale = glm::scale(identityMatrix, glm::vec3(7.48, 0.2, 12.5));
        translate = glm::translate(identityMatrix, glm::vec3(-19.27, 5.3, 13.75));
        rotation = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = translate * scale * rotation;
        floor_tiles_steps.drawCubeWithTexture(lightingShaderWithTexture, model);


        ////.............Outside auditorium...................///////////////
        /// tree draw left
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-30.0f, 1.0f, 20.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 1.0f, 1.8f));
        model = translateMatrix * scaleMatrix;
        tree.drawTree(lightingShader, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-30.0f, 1.0f, 20.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 1.0f, 1.8f));
        rotation = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = translateMatrix * rotation * scaleMatrix;
        tree.drawTree(lightingShader, model);


        ///treepot draw left

        modelMatrixForContainer = glm::translate(identityMatrix, glm::vec3(-30.0f, 0.0f, 20.0f));

        treepot.drawCylinder(lightingShaderWithTexture, tree_pot, modelMatrixForContainer);

        modelMatrixForContainer = glm::translate(identityMatrix, glm::vec3(-30.0f, 0.1f, 20.0f));

        treepot_grass.drawCylinder(lightingShaderWithTexture, grass, modelMatrixForContainer);


        /// tree draw right
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-30.0f, 1.0f, -20.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 1.0f, 1.8f));
        model = translateMatrix * scaleMatrix;
        tree.drawTree(lightingShader, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-30.0f, 1.0f, -20.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 1.0f, 1.8f));
        rotation = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = translateMatrix * rotation * scaleMatrix;
        tree.drawTree(lightingShader, model);


        ///treepot draw right

        modelMatrixForContainer = glm::translate(identityMatrix, glm::vec3(-30.0f, 0.0f, -20.0f));

        treepot.drawCylinder(lightingShaderWithTexture, tree_pot, modelMatrixForContainer);

        modelMatrixForContainer = glm::translate(identityMatrix, glm::vec3(-30.0f, 0.1f, -20.0f));

        treepot_grass.drawCylinder(lightingShaderWithTexture, grass, modelMatrixForContainer);



        /// *********Street lamp*******************////

        ///left lamp1 front

        Cylinder street_lamp_base = Cylinder(0.4,1.0);
        modelMatrixForContainer = glm::translate(identityMatrix, glm::vec3(-30.0f, -1.0f, 15.0f));
        street_lamp_base.drawCylinder(lightingShaderWithTexture, tree_pot, modelMatrixForContainer);

        Cylinder street_lamp_stand = Cylinder(0.1, 5.5);
        modelMatrixForContainer = glm::translate(identityMatrix, glm::vec3(-30.0f, 1.5f, 15.0f));
        street_lamp_stand.drawCylinder(lightingShaderWithTexture, tree_pot, modelMatrixForContainer);

        ///left lamp2 front
      
        modelMatrixForContainer = glm::translate(identityMatrix, glm::vec3(-48.0f, -1.0f, 15.0f));
        street_lamp_base.drawCylinder(lightingShaderWithTexture, tree_pot, modelMatrixForContainer);

        modelMatrixForContainer = glm::translate(identityMatrix, glm::vec3(-48.0f, 1.5f, 15.0f));
        street_lamp_stand.drawCylinder(lightingShaderWithTexture, tree_pot, modelMatrixForContainer);

        ///******** outside left lamps ***********//
        //lamp1
        modelMatrixForContainer = glm::translate(identityMatrix, glm::vec3(-40.0f, -1.0f, 45.5f));
        street_lamp_base.drawCylinder(lightingShaderWithTexture, tree_pot, modelMatrixForContainer);

        modelMatrixForContainer = glm::translate(identityMatrix, glm::vec3(-40.0f, 1.5f, 45.5f));
        street_lamp_stand.drawCylinder(lightingShaderWithTexture, tree_pot, modelMatrixForContainer);

        //lamp2
        modelMatrixForContainer = glm::translate(identityMatrix, glm::vec3(-22.0f, -1.0f, 45.5f));
        street_lamp_base.drawCylinder(lightingShaderWithTexture, tree_pot, modelMatrixForContainer);

        modelMatrixForContainer = glm::translate(identityMatrix, glm::vec3(-22.0f, 1.5f, 45.5f));
        street_lamp_stand.drawCylinder(lightingShaderWithTexture, tree_pot, modelMatrixForContainer);

        //lamp3
        modelMatrixForContainer = glm::translate(identityMatrix, glm::vec3(-4.0f, -1.0f, 45.5f));
        street_lamp_base.drawCylinder(lightingShaderWithTexture, tree_pot, modelMatrixForContainer);

        modelMatrixForContainer = glm::translate(identityMatrix, glm::vec3(-4.0f, 1.5f, 45.5f));
        street_lamp_stand.drawCylinder(lightingShaderWithTexture, tree_pot, modelMatrixForContainer);

        //lamp4
        modelMatrixForContainer = glm::translate(identityMatrix, glm::vec3(14.0f, -1.0f, 45.5f));
        street_lamp_base.drawCylinder(lightingShaderWithTexture, tree_pot, modelMatrixForContainer);

        modelMatrixForContainer = glm::translate(identityMatrix, glm::vec3(14.0f, 1.5f, 45.5f));
        street_lamp_stand.drawCylinder(lightingShaderWithTexture, tree_pot, modelMatrixForContainer);


        //lamp5

        modelMatrixForContainer = glm::translate(identityMatrix, glm::vec3(-4.0f, -1.0f, 28.5f));
        street_lamp_base.drawCylinder(lightingShaderWithTexture, tree_pot, modelMatrixForContainer);

        modelMatrixForContainer = glm::translate(identityMatrix, glm::vec3(-4.0f, 1.5f, 28.5f));
        street_lamp_stand.drawCylinder(lightingShaderWithTexture, tree_pot, modelMatrixForContainer);

        //lamp6

        modelMatrixForContainer = glm::translate(identityMatrix, glm::vec3(15.5f, -1.0f, 28.5f));
        street_lamp_base.drawCylinder(lightingShaderWithTexture, tree_pot, modelMatrixForContainer);

        modelMatrixForContainer = glm::translate(identityMatrix, glm::vec3(15.5f, 1.5f, 28.5f));
        street_lamp_stand.drawCylinder(lightingShaderWithTexture, tree_pot, modelMatrixForContainer);


        ///right lamp1 front

        modelMatrixForContainer = glm::translate(identityMatrix, glm::vec3(-30.0f, -1.0f, -15.0f));
        street_lamp_base.drawCylinder(lightingShaderWithTexture, tree_pot, modelMatrixForContainer);

        modelMatrixForContainer = glm::translate(identityMatrix, glm::vec3(-30.0f, 1.5f, -15.0f));
        street_lamp_stand.drawCylinder(lightingShaderWithTexture, tree_pot, modelMatrixForContainer);

        ///right lamp2 front
        modelMatrixForContainer = glm::translate(identityMatrix, glm::vec3(-48.0f, -1.0f, -15.0f));
        street_lamp_base.drawCylinder(lightingShaderWithTexture, tree_pot, modelMatrixForContainer);

        modelMatrixForContainer = glm::translate(identityMatrix, glm::vec3(-48.0f, 1.5f, -15.0f));
        street_lamp_stand.drawCylinder(lightingShaderWithTexture, tree_pot, modelMatrixForContainer);



        



        /// Sculpture draw

        

        /*translateMatrix = glm::translate(identityMatrix, glm::vec3(-10.0f, 2.0f, 40.0f));
        rotation = glm::rotate(identityMatrix, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 1.0f, 2.0f));
        model = translateMatrix * rotation * scaleMatrix;

        sculpure_design.drawBezierSculpt(lightingShaderWithTexture, model);*/


        ///******************* draw car****************////

        drawCar(lightingShader, cubeVAO, triangleVAO);
        


        //glBindVertexArray(cubeVAO);
        //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, 36);

        //bed(cubeVAO, lightingShader, model);
        //draw floor
        floor(cubeVAO, lightingShader);
        //axis(cubeVAO, lightingShader);
        frontWall(cubeVAO, lightingShader);
        triangleStage(triangleVAO, lightingShader);
        /*rightWall(cubeVAO, lightingShader);*/

        drawRowOfChairs(cubeVAO, lightingShader);

        // also draw the lamp object(s)
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // we now draw as many light bulbs as we have point lights.
        glBindVertexArray(lightCubeVAO);
        for (unsigned int i = 0; i < 64; i++)
        {
            if (i == 0 || i==1 || i==2 || i==3 || i==58 || i==59 || i==60 || i==61 ||i == 62 || i == 63) {
                model = glm::mat4(1.0f);
                model = glm::translate(model, pointLightPositions[i]);
                model = glm::scale(model, glm::vec3(1.0f)); // Make it a smaller cube
                ourShader.setMat4("model", model);
                ourShader.setVec3("color", glm::vec3(1.0f, 1.0f, 1.0f));
            }
            else {


                model = glm::mat4(1.0f);
                model = glm::translate(model, pointLightPositions[i]);
                model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
                ourShader.setMat4("model", model);
                ourShader.setVec3("color", glm::vec3(0.8f, 0.8f, 0.8f));
            }
            
            /*cone.drawCone(lightingShader, model);*/
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        {
            ambienton_off(lightingShader);
        }
        if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
        {
            diffuse_on_off(lightingShader);
        }
        if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
        {
            specular_on_off(lightingShader);
        }


        //toras....

        /*glm::mat4 model = glm::mat4(1.0f);*/
        model = glm::translate(model, glm::vec3(-10.0f, -8.19f, -0.2f)); // Adjust position
        model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // 90 degrees about X-axis
        model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f)); // 45 degrees about Z-axis
        model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // 45 degrees about Z-axis
        /*torus.drawTorus(lightingShader, model);*/


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &cubeEBO);

    glDeleteVertexArrays(1, &triangleVAO);
    glDeleteVertexArrays(1, &lightTriangleVAO);
    glDeleteBuffers(1, &triangleVBO);
    glDeleteBuffers(1, &triangleEBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f, float shininess = 32.0f)
{
    lightingShader.use();

    lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
    lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
    lightingShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    lightingShader.setFloat("material.shininess", shininess);

    lightingShader.setMat4("model", model);

    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void drawTriangle(unsigned int& triangleVAO, Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f, float shininess = 32.0f)
{
    lightingShader.use();

    lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
    lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
    lightingShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    lightingShader.setFloat("material.shininess", shininess);

    lightingShader.setMat4("model", model);

    glBindVertexArray(triangleVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void floor(unsigned int& cubeVAO, Shader& lightingShader)
{
    lightingShader.use();
    //base
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    //glm::mat4 translate2 = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    
    //scale = glm::scale(identityMatrix, glm::vec3(100.0, 0.2, 100.0));
    //translate = glm::translate(identityMatrix, glm::vec3(-50.0, -1.5, -50.0));
    //
    //
    //glm::mat4 model = translate * scale;
    //drawCube(cubeVAO, lightingShader, model, 0.8, 0.8, 0.8, 100.0);

    /*scale = glm::scale(identityMatrix, glm::vec3(-16.6, 0.2, 15.0));
    translate = glm::translate(identityMatrix, glm::vec3(-6.4, -0.8, -7.0));
    model = translate * scale;
    drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 1.0, 32.0);

    scale = glm::scale(identityMatrix, glm::vec3(-14.4, 0.2, 15.0));
    translate = glm::translate(identityMatrix, glm::vec3(-8.6, -0.6, -7.0));
    model = translate * scale;
    drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 1.0, 32.0);

    scale = glm::scale(identityMatrix, glm::vec3(-12.2, 0.2, 15.0));
    translate = glm::translate(identityMatrix, glm::vec3(-10.8, -0.4, -7.0));
    model = translate * scale;
    drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 1.0, 32.0);*/

    /*scale = glm::scale(identityMatrix, glm::vec3(0.1, 2.0, 0.1));
    translate = glm::translate(identityMatrix, glm::vec3(-5.0, -0.8, -5.0));
    model = translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.1, 0.1, 0.1, 32.0);*/

    //wall
    /*scale = glm::scale(identityMatrix, glm::vec3(-5.0, 5.0, 0.1));
    translate = glm::translate(identityMatrix, glm::vec3(4.9, -0.8, 2.0));
    model = translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.5, 0.5, 0.5, 32.0);*/
}
void frontWall(unsigned int& cubeVAO, Shader& lightingShader)
{
    lightingShader.use();
    //base
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    glm::mat4 rotation = glm::mat4(1.0f);
    glm::mat4 model = translate * scale;

    //stage wall
    scale = glm::scale(identityMatrix, glm::vec3(0.1, 10.0, 22.0));
    translate = glm::translate(identityMatrix, glm::vec3(15.0, 0.0, -11.0));
    model = translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.5, 0.5, 0.5, 32.0);
    
    //dais
    scale = glm::scale(identityMatrix, glm::vec3(1.0, 1.8, 1.0));
    translate = glm::translate(identityMatrix, glm::vec3(5.0, 0.0, -3.0));
    model = translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.112, 0.167, 0.231, 32.0);

    //stage
    scale = glm::scale(identityMatrix, glm::vec3(12.0, 1.2, 10.0));
    translate = glm::translate(identityMatrix, glm::vec3(3.0, -0.8, -5.0));
    model = translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.112, 0.167, 0.231, 32.0);


    ///*******car*********////
    /*scale = glm::scale(identityMatrix, glm::vec3(5.0, 1.0, 4.0));
    translate = glm::translate(identityMatrix, glm::vec3(-43.0, 0.0+1.0, 2.0));
    model = translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.9, 0.1, 0.1, 32.0);


    scale = glm::scale(identityMatrix, glm::vec3(5.0, 1.5, 14.0));
    translate = glm::translate(identityMatrix, glm::vec3(-43.0, -1.5+1.0, -2.5));
    model = translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.9, 0.1, 0.1, 32.0);*/

    /*scale = glm::scale(identityMatrix, glm::vec3(-1.0, 0.8, 10.0));
    translate = glm::translate(identityMatrix, glm::vec3(1.0, -0.8, -5.0));
    model = translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.112, 0.167, 0.231, 32.0);

    scale = glm::scale(identityMatrix, glm::vec3(-1.0, 0.4, 10.0));
    translate = glm::translate(identityMatrix, glm::vec3(0.0, -0.8, -5.0));
    model = translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.112, 0.167, 0.231, 32.0);*/

    ////lamp holder
    //scale = glm::scale(identityMatrix, glm::vec3(0.6, 0.05, 0.05));
    //translate = glm::translate(identityMatrix, glm::vec3(4.4f, 2.9f, 1.475f));
    //model = translate * scale;
    //drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0, 32.0);

    //translate = glm::translate(identityMatrix, glm::vec3(4.4f, 2.9f, -1.525f));
    //model = translate * scale;
    //drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 0.0, 32.0);

}

void triangleStage(unsigned int& triangleVAO, Shader& lightingShader) {
    lightingShader.use();
    //base
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    glm::mat4 rotation = glm::mat4(1.0f);
    glm::mat4 model = translate * scale;
    //stage
    scale = glm::scale(identityMatrix, glm::vec3(12.0, 1.2, 7.0));
    translate = glm::translate(identityMatrix, glm::vec3(3.0, -0.8, -12.0));
    model = translate * scale;
    drawCube(triangleVAO, lightingShader, model, 0.112, 0.167, 0.231, 32.0);

    scale = glm::scale(identityMatrix, glm::vec3(12.0, 1.2, 7.0));
    translate = glm::translate(identityMatrix, glm::vec3(3.0, -0.8, 12.0));
    rotation = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = translate * scale * rotation;
    drawCube(triangleVAO, lightingShader, model, 0.112, 0.167, 0.231, 32.0);


    ///*******car********////
    /*scale = glm::scale(identityMatrix, glm::vec3(1.0, 5.0, 2.0));
    translate = glm::translate(identityMatrix, glm::vec3(-43.0, 1.0+1.0, 0.0));
    rotation = glm::rotate(identityMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translate * rotation * scale;
    drawCube(triangleVAO, lightingShader, model, 0.9, 0.1, 0.1, 32.0);

    scale = glm::scale(identityMatrix, glm::vec3(1.0, 5.0, 2.0));
    translate = glm::translate(identityMatrix, glm::vec3(-38.0, 1.0+1.0, 8.0));
    rotation = glm::rotate(identityMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    rotation = glm::rotate(rotation, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = translate * rotation * scale;
    drawCube(triangleVAO, lightingShader, model, 0.9, 0.1, 0.1, 32.0);*/

}

void chair_center(unsigned int& cubeVAO, Shader& lightingShader, glm::vec3 position) {
    lightingShader.use();

    // Identity matrix
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 translate;
    glm::mat4 scale;
    glm::mat4 rotation;
    glm::mat4 model;

    // Chair base
    scale = glm::scale(identityMatrix, glm::vec3(-1.0, 0.4, 1.0));
    translate = glm::translate(identityMatrix, position + glm::vec3(-5.0, -0.8, -5.0)); // Adjust position for the base
    model = translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.112, 0.167, 0.231, 32.0);

    // Chair back support
    scale = glm::scale(identityMatrix, glm::vec3(-0.4, 1.0, 1.0));
    translate = glm::translate(identityMatrix, position + glm::vec3(-6.0, -0.8, -5.0)); // Adjust position for the back support
    rotation = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translate * scale * rotation;
    drawCube(cubeVAO, lightingShader, model, 0.112, 0.167, 0.231, 32.0);
}

void chair_left(unsigned int& cubeVAO, Shader& lightingShader, glm::vec3 position) {
    lightingShader.use();

    // Identity matrix
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 translate;
    glm::mat4 scale;
    glm::mat4 rotation;
    glm::mat4 model;

    // Chair base
    scale = glm::scale(identityMatrix, glm::vec3(1.0, 0.4, 1.0));
    translate = glm::translate(identityMatrix, position + glm::vec3(-5.7, -0.8, 8.5)); // Adjust position for the base
    rotation = glm::rotate(identityMatrix, glm::radians(25.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = translate * rotation * scale ;
    drawCube(cubeVAO, lightingShader, model, 0.112, 0.167, 0.231, 32.0);

    // Chair back support
    scale = glm::scale(identityMatrix, glm::vec3(0.4, 1.0, 1.0));
    translate = glm::translate(identityMatrix, position + glm::vec3(-5.9, -0.8, 8.6)); // Adjust position for the back support
    rotation = glm::rotate(identityMatrix, glm::radians(25.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = translate * rotation * scale ;
    drawCube(cubeVAO, lightingShader, model, 0.112, 0.167, 0.231, 32.0);
}

void chair_right(unsigned int& cubeVAO, Shader& lightingShader, glm::vec3 position) {
    lightingShader.use();

    // Identity matrix
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 translate;
    glm::mat4 scale;
    glm::mat4 rotation;
    glm::mat4 model;

    // Chair base
    scale = glm::scale(identityMatrix, glm::vec3(1.0, 0.4, 1.0));
    translate = glm::translate(identityMatrix, position + glm::vec3(-5.7, -0.8, -9.3)); // Adjust position for the base
    rotation = glm::rotate(identityMatrix, glm::radians(-25.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = translate * rotation * scale;
    drawCube(cubeVAO, lightingShader, model, 0.112, 0.167, 0.231, 32.0);

    // Chair back support
    scale = glm::scale(identityMatrix, glm::vec3(0.4, 1.0, 1.0));
    translate = glm::translate(identityMatrix, position + glm::vec3(-5.9, -0.8, -9.4)); // Adjust position for the back support
    rotation = glm::rotate(identityMatrix, glm::radians(-25.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = translate * rotation * scale;
    drawCube(cubeVAO, lightingShader, model, 0.112, 0.167, 0.231, 32.0);
}


void drawRowOfChairs(unsigned int& cubeVAO, Shader& lightingShader) {
    float angleDegrees = 66.0f;
    float angleRadians = glm::radians(angleDegrees);
    float spacing = 1.5f; // Distance between chairs along the angled line


/////////..................1st floor........................./////////


    for (int i = 0; i < 7; ++i) {
        // Calculate the position for each chair along a 25-degree line
        glm::vec3 position = glm::vec3(i * spacing * cos(angleRadians),
            0.0f,
            i * spacing * sin(angleRadians)); // Adjust z based on sin of the angle
        chair_left(cubeVAO, lightingShader, position);
    }

    for (int i = 0; i < 7; ++i) {
        // Calculate the position for each chair along a 25-degree line
        glm::vec3 position = glm::vec3(i * spacing * cos(glm::radians(68.5f)) - 2.25f,
            0.2f,
            i * spacing * sin(glm::radians(68.5f))); // Adjust z based on sin of the angle
        chair_left(cubeVAO, lightingShader, position);
    }

    for (int i = 0; i < 7; ++i) {
        // Calculate the position for each chair along a 25-degree line
        glm::vec3 position = glm::vec3(i * spacing * cos(glm::radians(72.0f)) - 4.5f,
            0.4f,
            i * spacing * sin(glm::radians(72.0f))); // Adjust z based on sin of the angle
        chair_left(cubeVAO, lightingShader, position);
    }

    for (int i = 0; i < 7; ++i) {
        // Calculate the position for each chair along a 25-degree line
        glm::vec3 position = glm::vec3(i * spacing * cos(glm::radians(75.5f)) - 6.75f,
            0.6f,
            i * spacing * sin(glm::radians(75.5f))); // Adjust z based on sin of the angle
        chair_left(cubeVAO, lightingShader, position);
    }

    for (int i = 0; i < 7; ++i) {
        // Calculate the position for each chair along a 25-degree line
        glm::vec3 position = glm::vec3(i * spacing * cos(glm::radians(78.5f)) - 9.0f,
            0.8f,
            i * spacing * sin(glm::radians(78.5f))); // Adjust z based on sin of the angle
        chair_left(cubeVAO, lightingShader, position);
    }

    for (int i = 0; i < 7; ++i) {
        // Calculate the position for each chair along a 25-degree line
        glm::vec3 position = glm::vec3(i * spacing * cos(glm::radians(82.0f)) - 11.25f,
            1.0f,
            i * spacing * sin(glm::radians(82.0f))); // Adjust z based on sin of the angle
        chair_left(cubeVAO, lightingShader, position);
    }

    for (int i = 0; i < 7; ++i) {
        // Calculate the position for each chair along a 25-degree line
        glm::vec3 position = glm::vec3(i * spacing * cos(glm::radians(85.5f)) - 13.5f,
            1.2f,
            i * spacing * sin(glm::radians(85.5f))); // Adjust z based on sin of the angle
        chair_left(cubeVAO, lightingShader, position);
    }









    //right chairs

     angleDegrees = -66.0f;
     angleRadians = glm::radians(angleDegrees);
     spacing = 1.5f; // Distance between chairs along the angled line

    for (int i = 0; i < 7; ++i) {
        // Calculate the position for each chair along a 25-degree line
        glm::vec3 position = glm::vec3(i * spacing * cos(angleRadians),
            0.0f,
            i * spacing * sin(angleRadians)); // Adjust z based on sin of the angle
        chair_right(cubeVAO, lightingShader, position);
    }

    for (int i = 0; i < 7; ++i) {
        // Calculate the position for each chair along a 25-degree line
        glm::vec3 position = glm::vec3(i * spacing * cos(glm::radians(-68.5f)) - 2.23f,
            0.2f,
            i * spacing * sin(glm::radians(-68.5f))); // Adjust z based on sin of the angle
        chair_right(cubeVAO, lightingShader, position);
    }

    for (int i = 0; i < 7; ++i) {
        // Calculate the position for each chair along a 25-degree line
        glm::vec3 position = glm::vec3(i * spacing * cos(glm::radians(-72.0f)) - 4.3f,
            0.4f,
            i * spacing * sin(glm::radians(-72.0f))); // Adjust z based on sin of the angle
        chair_right(cubeVAO, lightingShader, position);
    }

    for (int i = 0; i < 7; ++i) {
        // Calculate the position for each chair along a 25-degree line
        glm::vec3 position = glm::vec3(i * spacing * cos(glm::radians(-74.5f)) - 6.71f,
            0.6f,
            i * spacing * sin(glm::radians(-75.5f))); // Adjust z based on sin of the angle
        chair_right(cubeVAO, lightingShader, position);
    }

    for (int i = 0; i < 7; ++i) {
        // Calculate the position for each chair along a 25-degree line
        glm::vec3 position = glm::vec3(i * spacing * cos(glm::radians(-78.5f)) - 8.8f,
            0.8f,
            i * spacing * sin(glm::radians(-78.5f))); // Adjust z based on sin of the angle
        chair_right(cubeVAO, lightingShader, position);
    }

    for (int i = 0; i < 7; ++i) {
        // Calculate the position for each chair along a 25-degree line
        glm::vec3 position = glm::vec3(i * spacing * cos(glm::radians(-81.0f)) - 11.2f,
            1.0f,
            i * spacing * sin(glm::radians(-82.0f))); // Adjust z based on sin of the angle
        chair_right(cubeVAO, lightingShader, position);
    }

    for (int i = 0; i < 7; ++i) {
        // Calculate the position for each chair along a 25-degree line
        glm::vec3 position = glm::vec3(i * spacing * cos(glm::radians(-85.5f)) - 13.2f,
            1.2f,
            i * spacing * sin(glm::radians(-85.5f))); // Adjust z based on sin of the angle
        chair_right(cubeVAO, lightingShader, position);
    }








    for (int i = 0; i < 7; ++i) {
        glm::vec3 position = glm::vec3(0.0f, 0.0f, i*1.5f); // Space chairs by 3 units along the x-axis
        chair_center(cubeVAO, lightingShader, position);
    }

    for (int i = 0; i < 7; ++i) {
        glm::vec3 position = glm::vec3(-2.2f, 0.2f, i * 1.5f); // Space chairs by 3 units along the x-axis
        chair_center(cubeVAO, lightingShader, position);
    }

    for (int i = 0; i < 7; ++i) {
        glm::vec3 position = glm::vec3(-4.4f, 0.4f, i * 1.5f); // Space chairs by 3 units along the x-axis
        chair_center(cubeVAO, lightingShader, position);
    }

    for (int i = 0; i < 7; ++i) {
        glm::vec3 position = glm::vec3(-6.6f, 0.6f, i * 1.5f); // Space chairs by 3 units along the x-axis
        chair_center(cubeVAO, lightingShader, position);
    }

    for (int i = 0; i < 7; ++i) {
        glm::vec3 position = glm::vec3(-8.8f, 0.8f, i * 1.5f); // Space chairs by 3 units along the x-axis
        chair_center(cubeVAO, lightingShader, position);
    }

    for (int i = 0; i < 7; ++i) {
        glm::vec3 position = glm::vec3(-11.0f, 1.0f, i * 1.5f); // Space chairs by 3 units along the x-axis
        chair_center(cubeVAO, lightingShader, position);
    }

    for (int i = 0; i < 7; ++i) {
        glm::vec3 position = glm::vec3(-13.2f, 1.2f, i * 1.5f); // Space chairs by 3 units along the x-axis
        chair_center(cubeVAO, lightingShader, position);
    }


//////.............2nd floor..............///////

    /// center chairs
    for (int i = 0; i < 7; ++i) {
        glm::vec3 position = glm::vec3(-8.8f, 0.8f+5.0f, i * 1.5f); // Space chairs by 3 units along the x-axis
        chair_center(cubeVAO, lightingShader, position);
    }

    for (int i = 0; i < 7; ++i) {
        glm::vec3 position = glm::vec3(-11.0f, 1.0f+5.0f, i * 1.5f); // Space chairs by 3 units along the x-axis
        chair_center(cubeVAO, lightingShader, position);
    }

    for (int i = 0; i < 7; ++i) {
        glm::vec3 position = glm::vec3(-13.2f, 1.2f+5.0f, i * 1.5f); // Space chairs by 3 units along the x-axis
        chair_center(cubeVAO, lightingShader, position);
    }


    /// right chairs
    for (int i = 0; i < 7; ++i) {
        // Calculate the position for each chair along a 25-degree line
        glm::vec3 position = glm::vec3(i * spacing * cos(glm::radians(-78.5f)) - 8.8f,
            0.8f+5.0f,
            i * spacing * sin(glm::radians(-78.5f))); // Adjust z based on sin of the angle
        chair_right(cubeVAO, lightingShader, position);
    }

    for (int i = 0; i < 7; ++i) {
        // Calculate the position for each chair along a 25-degree line
        glm::vec3 position = glm::vec3(i * spacing * cos(glm::radians(-81.0f)) - 11.2f,
            1.0f+5.0f,
            i * spacing * sin(glm::radians(-82.0f))); // Adjust z based on sin of the angle
        chair_right(cubeVAO, lightingShader, position);
    }

    for (int i = 0; i < 7; ++i) {
        // Calculate the position for each chair along a 25-degree line
        glm::vec3 position = glm::vec3(i * spacing * cos(glm::radians(-85.5f)) - 13.2f,
            1.2f+5.0f,
            i * spacing * sin(glm::radians(-85.5f))); // Adjust z based on sin of the angle
        chair_right(cubeVAO, lightingShader, position);
    }


    /// left chairs

    for (int i = 0; i < 7; ++i) {
        // Calculate the position for each chair along a 25-degree line
        glm::vec3 position = glm::vec3(i * spacing * cos(glm::radians(78.5f)) - 9.0f,
            0.8f+5.0f,
            i * spacing * sin(glm::radians(78.5f))); // Adjust z based on sin of the angle
        chair_left(cubeVAO, lightingShader, position);
    }

    for (int i = 0; i < 7; ++i) {
        // Calculate the position for each chair along a 25-degree line
        glm::vec3 position = glm::vec3(i * spacing * cos(glm::radians(82.0f)) - 11.25f,
            1.0f+5.0f,
            i * spacing * sin(glm::radians(82.0f))); // Adjust z based on sin of the angle
        chair_left(cubeVAO, lightingShader, position);
    }

    for (int i = 0; i < 7; ++i) {
        // Calculate the position for each chair along a 25-degree line
        glm::vec3 position = glm::vec3(i * spacing * cos(glm::radians(85.5f)) - 13.5f,
            1.2f+5.0f,
            i * spacing * sin(glm::radians(85.5f))); // Adjust z based on sin of the angle
        chair_left(cubeVAO, lightingShader, position);
    }
    
}



void axis(unsigned int& cubeVAO, Shader& lightingShader)
{
    lightingShader.use();
    //base
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    //glm::mat4 translate2 = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    //x
    scale = glm::scale(identityMatrix, glm::vec3(2.0, 0.1, 0.1));
    translate = glm::translate(identityMatrix, glm::vec3(0.0, 0.0, 0.0));
    glm::mat4 model = translate * scale;
    drawCube(cubeVAO, lightingShader, model, 1.0, 0.0, 0.0, 32.0);
    //y
    scale = glm::scale(identityMatrix, glm::vec3(0.1, 2.0, 0.1));
    translate = glm::translate(identityMatrix, glm::vec3(0.0, 0.0, 0.0));
    model = translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.0, 1.0, 0.0, 32.0);
    //z
    scale = glm::scale(identityMatrix, glm::vec3(0.1, 0.1, 2.0));
    translate = glm::translate(identityMatrix, glm::vec3(0.0, 0.0, 0.0));
    model = translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.0, 0.0, 1.0, 32.0);
}



// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------

void drawDoors(Shader& lightingShaderWithTexture, Door& door, glm::mat4 identityMatrix) {
    glm::mat4 scale, translate, rotation, model;

    /// left door1
    scale = glm::scale(identityMatrix, glm::vec3(4.0, 4.8, 0.2));
    translate = glm::translate(identityMatrix, glm::vec3(3.0, 1.6, 19.9));
    if (leftDoor1Open) {
        rotation = glm::rotate(identityMatrix, doorOpenAngle, glm::vec3(0.0f, 1.0f, 0.0f));
        translate = glm::translate(identityMatrix, glm::vec3(1.0, 1.6, 17.9));
    }
    else {
        rotation = glm::mat4(1.0f); // No rotation
    }
    model = translate * rotation * scale;
    door.drawDoorWithTexture(lightingShaderWithTexture, model);

    /// left door2
    scale = glm::scale(identityMatrix, glm::vec3(4.0, 4.8, 0.2));
    translate = glm::translate(identityMatrix, glm::vec3(7.0, 1.6, 19.9));
    
    if (leftDoor2Open) {
        rotation = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        translate = glm::translate(identityMatrix, glm::vec3(8.0, 1.6, 17.9));
    }
    else {
        rotation = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }
    model = translate * rotation * scale;
    door.drawDoorWithTexture(lightingShaderWithTexture, model);

    /// right door1
    scale = glm::scale(identityMatrix, glm::vec3(4.0, 4.8, 0.2));
    translate = glm::translate(identityMatrix, glm::vec3(3.0, 1.6, -19.9));
    if (rightDoor1Open) {
        rotation = glm::rotate(identityMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        translate = glm::translate(identityMatrix, glm::vec3(1.0, 1.6, -17.9));
    }
    else {
        rotation = glm::mat4(1.0f); // No rotation
    }
    model = translate * rotation * scale;
    door.drawDoorWithTexture(lightingShaderWithTexture, model);

    /// right door2
    scale = glm::scale(identityMatrix, glm::vec3(4.0, 4.8, 0.2));
    translate = glm::translate(identityMatrix, glm::vec3(7.0, 1.6, -19.9));
    rotation = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    if (rightDoor2Open) {
        rotation = glm::rotate(identityMatrix, -doorOpenAngle, glm::vec3(0.0f, 1.0f, 0.0f));
        translate = glm::translate(identityMatrix, glm::vec3(9.0, 1.6, -17.9));
    }
    else {
        rotation = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }
    model = translate * rotation * scale;
    door.drawDoorWithTexture(lightingShaderWithTexture, model);
}




void drawCar(Shader& lightingShader, unsigned int& cubeVAO, unsigned int& triangleVAO) {
    // Create the car's overall transformation matrix
    glm::mat4 carTransform = glm::mat4(1.0f);
    carTransform = glm::translate(carTransform, carPosition);
    carTransform = glm::rotate(carTransform, glm::radians(carRotation), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 translate, rotation, scale, model;

    // Front triangle - reduced scale and adjusted position
    scale = glm::scale(identityMatrix, glm::vec3(0.6, 3.0, 1.2));
    translate = glm::translate(identityMatrix, glm::vec3(-43.0, 1.1, 0.0));
    rotation = glm::rotate(identityMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = carTransform * translate * rotation * scale;
    drawCube(triangleVAO, lightingShader, model, 0.9, 0.1, 0.1, 32.0);

    // Back triangle - reduced scale and adjusted position
    scale = glm::scale(identityMatrix, glm::vec3(0.6, 3.0, 1.2));
    translate = glm::translate(identityMatrix, glm::vec3(-40.0, 1.1, 4.8));
    rotation = glm::rotate(identityMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    rotation = glm::rotate(rotation, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = carTransform * translate * rotation * scale;
    drawCube(triangleVAO, lightingShader, model, 0.9, 0.1, 0.1, 32.0);

    // Upper body - reduced scale and adjusted position
    scale = glm::scale(identityMatrix, glm::vec3(3.0, 0.6, 2.4));
    translate = glm::translate(identityMatrix, glm::vec3(-43.0, 0.0 + 0.5, 1.2));
    model = carTransform * translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.9, 0.1, 0.1, 32.0);

    // Lower body - reduced scale and adjusted position
    scale = glm::scale(identityMatrix, glm::vec3(3.0, 0.9, 8.4));
    translate = glm::translate(identityMatrix, glm::vec3(-43.0, -0.4, -1.5));
    model = carTransform * translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.9, 0.1, 0.1, 32.0);

    // Create wheel object once
    CylinderNoTex wheel = CylinderNoTex();

    // Front left wheel - reduced scale and adjusted position
    scale = glm::scale(identityMatrix, glm::vec3(0.5, 0.25, 0.5));
    translate = glm::translate(identityMatrix, glm::vec3(-42.8, -0.5, 0.0));
    rotation = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = carTransform * translate * rotation * scale;
    wheel.drawCylinderNoTex(lightingShader, model);

    // Front right wheel - reduced scale and adjusted position
    scale = glm::scale(identityMatrix, glm::vec3(0.5, 0.25, 0.5));
    translate = glm::translate(identityMatrix, glm::vec3(-40.2, -0.5, 0.0));
    rotation = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = carTransform * translate * rotation * scale;
    wheel.drawCylinderNoTex(lightingShader, model);

    // Back left wheel - reduced scale and adjusted position
    scale = glm::scale(identityMatrix, glm::vec3(0.5, 0.25, 0.5));
    translate = glm::translate(identityMatrix, glm::vec3(-42.8, -0.5, 5.4));
    rotation = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = carTransform * translate * rotation * scale;
    wheel.drawCylinderNoTex(lightingShader, model);

    // Back right wheel - reduced scale and adjusted position
    scale = glm::scale(identityMatrix, glm::vec3(0.5, 0.25, 0.5));
    translate = glm::translate(identityMatrix, glm::vec3(-40.2, -0.5, 5.4));
    rotation = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = carTransform * translate * rotation * scale;
    wheel.drawCylinderNoTex(lightingShader, model);
}


void processInput(GLFWwindow* window)
{
    float moveSpeed = 0.5f;
    float rotateSpeed = 0.5f;


    static bool oKeyPressed_left = false; // To avoid multiple toggles on a single key press
    static bool oKeyPressed_right = false;

    // Check if 'O' key is pressed to toggle doors
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
        if (!oKeyPressed_left) { // Key just pressed
            // Toggle the door states
            leftDoor1Open = !leftDoor1Open;
            leftDoor2Open = !leftDoor2Open;

            oKeyPressed_left = true; // Mark key as pressed
        }
    }
    else if (glfwGetKey(window, GLFW_KEY_M) == GLFW_RELEASE) {
        // Reset the flag when the key is released
        oKeyPressed_left = false;
    }

    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
        if (!oKeyPressed_right) { // Key just pressed
            // Toggle the door states
            rightDoor1Open = !rightDoor1Open;
            rightDoor2Open = !rightDoor2Open;

            oKeyPressed_right = true; // Mark key as pressed
        }
    }
    else if (glfwGetKey(window, GLFW_KEY_M) == GLFW_RELEASE) {
        // Reset the flag when the key is released
        oKeyPressed_right = false;
    }


    // Forward/Backward movement
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        float radians = glm::radians(carRotation);
        carPosition.x += moveSpeed * sin(radians);
        carPosition.z += moveSpeed * cos(radians);
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        float radians = glm::radians(carRotation);
        carPosition.x -= moveSpeed * sin(radians);
        carPosition.z -= moveSpeed * cos(radians);
    }

    // Rotation
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        carRotation += rotateSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
        carRotation -= rotateSpeed;
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        if (rotateAxis_X) rotateAngle_X -= 0.1;
        else if (rotateAxis_Y) rotateAngle_Y -= 0.1;
        else rotateAngle_Z -= 0.1;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        camera.ProcessKeyboard(UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        camera.ProcessKeyboard(DOWN, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        camera.ProcessKeyboard(Y_LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
        camera.ProcessKeyboard(Y_RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        camera.ProcessKeyboard(R_LEFT, deltaTime);

    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        camera.ProcessKeyboard(R_RIGHT, deltaTime);

    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        camera.ProcessKeyboard(P_UP, deltaTime);

    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        camera.ProcessKeyboard(P_DOWN, deltaTime);
    }
    //if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) translate_Y += 0.01;
    //if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) translate_Y -= 0.01;
    //if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) translate_X += 0.01;
    //if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) translate_X -= 0.01;
    //if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) translate_Z += 0.01;
    //if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) translate_Z -= 0.01;
    //if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) scale_X += 0.001;
    //if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) scale_X -= 0.001;
    //if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) scale_Y += 0.001;
    //if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) scale_Y -= 0.001;
    //if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) scale_Z += 0.001;
    //if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) scale_Z -= 0.001;

    //if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    //{
    //    rotateAngle_X += 0.1;
    //    rotateAxis_X = 1.0;
    //    rotateAxis_Y = 0.0;
    //    rotateAxis_Z = 0.0;
    //}
    //if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
    //{
    //    rotateAngle_Y += 0.1;
    //    rotateAxis_X = 0.0;
    //    rotateAxis_Y = 1.0;
    //    rotateAxis_Z = 0.0;
    //}
    //if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    //{
    //    rotateAngle_Z += 0.1;
    //    rotateAxis_X = 0.0;
    //    rotateAxis_Y = 0.0;
    //    rotateAxis_Z = 1.0;
    //}

    //if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
    //{
    //    eyeX += 2.5 * deltaTime;
    //    basic_camera.changeEye(eyeX, eyeY, eyeZ);
    //}
    //if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    //{
    //    eyeX -= 2.5 * deltaTime;
    //    basic_camera.changeEye(eyeX, eyeY, eyeZ);
    //}
    //if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    //{
    //    eyeZ += 2.5 * deltaTime;
    //    basic_camera.changeEye(eyeX, eyeY, eyeZ);
    //}
    //if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    //{
    //    eyeZ -= 2.5 * deltaTime;
    //    basic_camera.changeEye(eyeX, eyeY, eyeZ);
    //}
    //if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    //{
    //    eyeY += 2.5 * deltaTime;
    //    basic_camera.changeEye(eyeX, eyeY, eyeZ);
    //}
    //if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    //{
    //    eyeY -= 2.5 * deltaTime;
    //    basic_camera.changeEye(eyeX, eyeY, eyeZ);
    //}

}
void ambienton_off(Shader& lightingShader)
{
    double currentTime = glfwGetTime();
    if (currentTime - lastKeyPressTime < keyPressDelay) return;
    lightingShader.use();
    if (AmbientON)
    {
        pointlight1.turnAmbientOff();
        pointlight2.turnAmbientOff();
        lightingShader.setVec3("directionalLight.ambient", 0.0f, 0.0f, 0.0f);
        lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        AmbientON = !AmbientON;
        lastKeyPressTime = currentTime;
    }
    else
    {
        pointlight1.turnAmbientOn();
        pointlight2.turnAmbientOn();
        lightingShader.setVec3("directionalLight.ambient", 0.2f, 0.2f, 0.2f);
        lightingShader.setVec3("spotLight.ambient", 0.2f, 0.2f, 0.2f);
        AmbientON = !AmbientON;
        lastKeyPressTime = currentTime;
    }
}
void diffuse_on_off(Shader& lightingShader)
{
    double currentTime = glfwGetTime();
    if (currentTime - lastKeyPressTime < keyPressDelay) return;
    lightingShader.use();
    if (DiffusionON)
    {
        pointlight1.turnDiffuseOff();
        pointlight2.turnDiffuseOff();
        lightingShader.setVec3("directionalLight.diffuse", 0.0f, 0.0f, 0.0f);
        lightingShader.setVec3("spotLight.diffuse", 0.0f, 0.0f, 0.0f);
        DiffusionON = !DiffusionON;
        lastKeyPressTime = currentTime;
    }
    else
    {
        pointlight1.turnDiffuseOn();
        pointlight2.turnDiffuseOn();
        lightingShader.setVec3("directionalLight.diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("spotLight.diffuse", 0.8f, 0.8f, 0.8f);
        DiffusionON = !DiffusionON;
        lastKeyPressTime = currentTime;
    }
}
void specular_on_off(Shader& lightingShader)
{
    double currentTime = glfwGetTime();
    if (currentTime - lastKeyPressTime < keyPressDelay) return;
    lightingShader.use();
    if (SpecularON)
    {
        pointlight1.turnSpecularOff();
        pointlight2.turnSpecularOff();
        lightingShader.setVec3("directionalLight.specular", 0.0f, 0.0f, 0.0f);
        lightingShader.setVec3("spotLight.specular", 0.0f, 0.0f, 0.0f);
        SpecularON = !SpecularON;
        lastKeyPressTime = currentTime;
    }
    else
    {
        pointlight1.turnSpecularOn();
        pointlight2.turnSpecularOn();
        lightingShader.setVec3("directionalLight.specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        SpecularON = !SpecularON;
        lastKeyPressTime = currentTime;
    }
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    //{
    //    if (pointLightOn)
    //    {
    //        pointlight1.turnOff();
    //        pointlight2.turnOff();
    //        pointlight3.turnOff();
    //        pointlight4.turnOff();
    //        pointLightOn = !pointLightOn;
    //    }
    //    else
    //    {
    //        pointlight1.turnOn();
    //        pointlight2.turnOn();
    //        pointlight3.turnOn();
    //        pointlight4.turnOn();
    //        pointLightOn = !pointLightOn;
    //    }
    //}

    // First, declare the array of point lights at the top with your other variables
    PointLight* pointLights[] = {
    &pointlight1, &pointlight2, &pointlight3, &pointlight4, &pointlight5,
    &pointlight6, &pointlight7, &pointlight8, &pointlight9, &pointlight10,
    &pointlight11, &pointlight12, &pointlight13, &pointlight14, &pointlight15,
    &pointlight16, &pointlight17, &pointlight18, &pointlight19, &pointlight20,
    &pointlight21, &pointlight22, &pointlight23, &pointlight24, &pointlight25,
    &pointlight26, &pointlight27, &pointlight28, &pointlight29, &pointlight30,
    &pointlight31, &pointlight32, &pointlight33, &pointlight34, &pointlight35,
    &pointlight36, &pointlight37, &pointlight38, &pointlight39, &pointlight40,
    &pointlight41, &pointlight42, &pointlight43, &pointlight44, &pointlight45,
    &pointlight46, &pointlight47, &pointlight48, &pointlight49, &pointlight50,
    &pointlight51, &pointlight52, &pointlight53, &pointlight54, &pointlight55,
    &pointlight56, &pointlight57, &pointlight58,& pointlight59,& pointlight60,
    & pointlight61,& pointlight62,& pointlight63,& pointlight64
    };
    const int numLights = 64;  // Updated number of point lights

    // In your key callback function:
    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        for (int i = 0; i < numLights; i++)
        {
            if (pointLightOn)
            {
                pointLights[i]->turnOff();
            }
            else
            {
                pointLights[i]->turnOn();
            }
        }
        pointLightOn = !pointLightOn;
    }


    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        directionalLightOn = !directionalLightOn;
    }
    if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    {
        if (SpotLightOn)
        {
            spotlight1.turnOff();
            spotlight2.turnOff();
            spotlight3.turnOff();
            spotlight4.turnOff();
            spotlight5.turnOff();
            SpotLightOn = !SpotLightOn;
        }
        else
        {
            spotlight1.turnOn();
            spotlight2.turnOn();
            spotlight3.turnOn();
            spotlight4.turnOn();
            spotlight5.turnOn();
            SpotLightOn = !SpotLightOn;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        isRotating = !isRotating;  // Toggle rotation state
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrappingModeS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrappingModeT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilteringModeMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilteringModeMax);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}