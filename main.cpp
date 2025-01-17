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
unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax);

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
    glm::vec3(4.50f,  2.50f,  1.5f),
    glm::vec3(4.50f,  2.50f,  -1.5f),
    glm::vec3(-10.50f, 2.50f,  20.5f),
    glm::vec3(-10.50f, 2.50f,  -20.5f)
};

PointLight pointlight1(

    pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1       // light number
);
PointLight pointlight2(

    pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    2       // light number
);
PointLight pointlight3(

    pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    3       // light number
);
PointLight pointlight4(

    pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    4       // light number
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

    string diffuseMapPath = "container2.png";
    string specularMapPath = "container2_specular.png";
    string laughEmoPath = "world_map.png";
    string bitfestPath = "bitfest.jpg";
    string floor_tiles_path = "Images/floor_tiles.jpg";

    unsigned int diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int laughEmoji = loadTexture(laughEmoPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int bitfest = loadTexture(bitfestPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int floor_tiles = loadTexture(floor_tiles_path.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    //unsigned int laughEmojiv2 = loadTexture(laughEmoPath.c_str(), GL_REPEAT, GL_MIRRORED_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    Cube cube = Cube(bitfest, bitfest, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    Cube floor_tiles_cube = Cube(floor_tiles, floor_tiles, 50.0f, 0.0f, 0.0f, 10.0f, 10.0f);
    Cube2 floor_tiles_steps = Cube2(floor_tiles, floor_tiles, 50.0f, 0.0f, 0.0f, 10.0f, 10.0f);
    SphereTex spheretex = SphereTex();

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
        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,     // 4 - 2
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,     // |   |     diagonal / from top
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,     // 3 - 1
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,

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


        lightingShader.setVec3("directionalLight.directiaon", 0.5f, -3.0f, -3.0f);
        lightingShader.setVec3("directionalLight.ambient", .5f, .5f, .5f);
        lightingShader.setVec3("directionalLight.diffuse", .8f, .8f, .8f);
        lightingShader.setVec3("directionalLight.specular", 1.0f, 1.0f, 1.0f);


        lightingShader.setBool("directionLightOn", directionalLightOn);


        lightingShader.setVec3("directionalLight.direction", 0.5f, -3.0f, -3.0f);
        if (AmbientON) {
            lightingShader.setVec3("directionalLight.ambient", 0.2f, 0.2f, 0.2f);
            lightingShader.setVec3("spotLight.ambient", 0.2f, 0.2f, 0.2f);
        }
        if (DiffusionON) {
            lightingShader.setVec3("directionalLight.diffuse", 0.8f, 0.8f, 0.8f);
            lightingShader.setVec3("spotLight.diffuse", 0.8f, 0.8f, 0.8f);
        }
        if (SpecularON) {

            lightingShader.setVec3("directionalLight.specular", 1.0f, 1.0f, 1.0f);
            lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        }



        lightingShader.setBool("directionalLightON", directionalLightOn);
        lightingShader.setBool("SpotLightON", SpotLightOn);

        lightingShader.setVec3("spotLight.direction", 0.0f, -1.0f, 0.0f);
        lightingShader.setVec3("spotLight.position", 3.5f, 3.0f, 0.0f);



        lightingShader.setFloat("spotLight.k_c", 1.0f);
        lightingShader.setFloat("spotLight.k_l", 0.09f);
        lightingShader.setFloat("spotLight.k_q", 0.032f);
        lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(35.5f)));
        lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(40.5f)));

        // pass projection matrix to shader (note that in this case it could change every frame)
       // glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);

        //glm::mat4 projection = myPerspective(
        //    glm::radians(camera.Zoom),         // fov: field of view in radians
        //    (float)SCR_WIDTH / (float)SCR_HEIGHT, // aspect: aspect ratio
        //    0.1f,                              // near: near clipping plane
        //    100.0f                             // far: far clipping plane
        //);

        

        lightingShader.setMat4("projection", projection);

        // camera/view transformation
       /* glm::mat4 view = camera.GetViewMatrix();*/
        //glm::mat4 view = basic_camera.createViewMatrix();
        lightingShader.setMat4("view", view);

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

        lightingShaderWithTexture.setVec3("directionalLight.directiaon", 0.5f, -3.0f, -3.0f);
        lightingShaderWithTexture.setVec3("directionalLight.ambient", .5f, .5f, .5f);
        lightingShaderWithTexture.setVec3("directionalLight.diffuse", .8f, .8f, .8f);
        lightingShaderWithTexture.setVec3("directionalLight.specular", 1.0f, 1.0f, 1.0f);


        lightingShaderWithTexture.setBool("directionLightOn", directionalLightOn);


        lightingShaderWithTexture.setVec3("directionalLight.direction", 0.5f, -3.0f, -3.0f);
        if (AmbientON) {
            lightingShaderWithTexture.setVec3("directionalLight.ambient", 0.2f, 0.2f, 0.2f);
            lightingShaderWithTexture.setVec3("spotLight.ambient", 0.2f, 0.2f, 0.2f);
        }
        if (DiffusionON) {
            lightingShaderWithTexture.setVec3("directionalLight.diffuse", 0.8f, 0.8f, 0.8f);
            lightingShaderWithTexture.setVec3("spotLight.diffuse", 0.8f, 0.8f, 0.8f);
        }
        if (SpecularON) {

            lightingShaderWithTexture.setVec3("directionalLight.specular", 1.0f, 1.0f, 1.0f);
            lightingShaderWithTexture.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        }



        lightingShaderWithTexture.setBool("directionalLightON", directionalLightOn);

        lightingShaderWithTexture.setBool("SpotLightON", SpotLightOn);

        lightingShaderWithTexture.setVec3("spotLight.direction", 0.0f, -1.0f, 0.0f);
        lightingShaderWithTexture.setVec3("spotLight.position", 3.5f, 3.0f, 0.0f);



        lightingShaderWithTexture.setFloat("spotLight.k_c", 1.0f);
        lightingShaderWithTexture.setFloat("spotLight.k_l", 0.09f);
        lightingShaderWithTexture.setFloat("spotLight.k_q", 0.032f);
        lightingShaderWithTexture.setFloat("spotLight.cutOff", glm::cos(glm::radians(35.5f)));
        lightingShaderWithTexture.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(40.5f)));
        

        glm::mat4 modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(modelMatrixForContainer, glm::vec3(2.0f, 1.4f, 3.8f));
        
        spheretex.drawSphere(lightingShaderWithTexture,laughEmoji ,modelMatrixForContainer);

        glm::mat4 modelMatrixForContainer2 = glm::mat4(1.0f);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(1.2f, 0.7f, 0.4f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.2f, 0.8f, -1.8f));
        model = translateMatrix * scaleMatrix;
        cube.drawCubeWithTexture(lightingShaderWithTexture, model);

        
        glm::mat4 translate = glm::mat4(1.0f);
        //glm::mat4 translate2 = glm::mat4(1.0f);
        glm::mat4 scale = glm::mat4(1.0f);

        //Entire floor
        scale = glm::scale(identityMatrix, glm::vec3(28.0, 0.2, 40.0));
        translate = glm::translate(identityMatrix, glm::vec3(-9.0, -0.9, 0.0));
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
        for (unsigned int i = 0; i < 4; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            ourShader.setMat4("model", model);
            ourShader.setVec3("color", glm::vec3(0.8f, 0.8f, 0.8f));
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
    /*
    scale = glm::scale(identityMatrix, glm::vec3(-28.0, 0.2, 40.0));
    translate = glm::translate(identityMatrix, glm::vec3(5.0, -1.0, -20.0));
    
*/    
    glm::mat4 model = translate * scale;
    //drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 1.0, 32.0);

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

    //wall
    scale = glm::scale(identityMatrix, glm::vec3(0.1, 5.0, 10.0));
    translate = glm::translate(identityMatrix, glm::vec3(4.9, -0.8, -5.0));
    model = translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.5, 0.5, 0.5, 32.0);

    scale = glm::scale(identityMatrix, glm::vec3(-1.0, 1.8, 1.0));
    translate = glm::translate(identityMatrix, glm::vec3(3.0, 0.0, -3.0));
    model = translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.112, 0.167, 0.231, 32.0);

    //stage
    scale = glm::scale(identityMatrix, glm::vec3(6.0, 1.2, 10.0));
    translate = glm::translate(identityMatrix, glm::vec3(-1.0, -0.8, -5.0));
    model = translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.112, 0.167, 0.231, 32.0);

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
    scale = glm::scale(identityMatrix, glm::vec3(6.0, 1.2, 6.0));
    translate = glm::translate(identityMatrix, glm::vec3(-1.0, -0.8, -11.0));
    model = translate * scale;
    drawCube(triangleVAO, lightingShader, model, 0.112, 0.167, 0.231, 32.0);

    scale = glm::scale(identityMatrix, glm::vec3(6.0, 1.2, 6.0));
    translate = glm::translate(identityMatrix, glm::vec3(-1.0, -0.8, 11.0));
    rotation = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = translate * scale * rotation;
    drawCube(triangleVAO, lightingShader, model, 0.112, 0.167, 0.231, 32.0);
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
void processInput(GLFWwindow* window)
{
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
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
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
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        camera.ProcessKeyboard(P_UP, deltaTime);

    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
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

    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        if (pointLightOn)
        {
            pointlight1.turnOff();
            pointlight2.turnOff();
            pointlight3.turnOff();
            pointlight4.turnOff();
            pointLightOn = !pointLightOn;
        }
        else
        {
            pointlight1.turnOn();
            pointlight2.turnOn();
            pointlight3.turnOn();
            pointlight4.turnOn();
            pointLightOn = !pointLightOn;
        }
    }
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        if (directionalLightOn)
        {
            directionalLightOn = !directionalLightOn;
        }
        else
        {
            directionalLightOn = !directionalLightOn;
        }
    }
    if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    {
        if (SpotLightOn)
        {
            SpotLightOn = !SpotLightOn;
        }
        else
        {
            SpotLightOn = !SpotLightOn;
        }
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