#ifndef FRACTAL_TREE_H
#define FRACTAL_TREE_H

#include <glm/glm.hpp>
#include <vector>
#include "Shader.h"

class FractalTree {
public:
    glm::vec3 branchColor; // Color of the branches
    float branchWidth;     // Width of the branches

    FractalTree(float branchLength = 1.0f, float branchAngle = 30.0f, int recursionDepth = 10,
        glm::vec3 color = glm::vec3(0.22f, 1.0f, 0.078f), float width = 5.0f)
    {
        this->branchLength = branchLength;
        this->branchAngle = branchAngle;
        this->recursionDepth = recursionDepth;
        this->branchColor = color;
        this->branchWidth = width;

        buildTree();

        // Generate VAO and VBO for rendering
        glGenVertexArrays(1, &treeVAO);
        glBindVertexArray(treeVAO);

        glGenBuffers(1, &treeVBO);
        glBindBuffer(GL_ARRAY_BUFFER, treeVBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        // Enable vertex attribute for position
        glEnableVertexAttribArray(0); // Position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // Unbind VAO and VBO
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    ~FractalTree() {
        glDeleteVertexArrays(1, &treeVAO);
        glDeleteBuffers(1, &treeVBO);
    }

    void drawTree(Shader& shader, glm::mat4 model) const {
        shader.use();
        shader.setVec3("color", branchColor);
        shader.setMat4("model", model);

        // Set line width for branches
        glLineWidth(branchWidth);

        glBindVertexArray(treeVAO);
        glDrawArrays(GL_LINES, 0, vertices.size() / 3);
        glBindVertexArray(0);

        // Reset line width to default for other drawings
        glLineWidth(1.0f);
    }

private:
    unsigned int treeVAO, treeVBO;
    float branchLength;    // Length of the branches
    float branchAngle;     // Angle between branches
    int recursionDepth;    // Maximum depth of recursion
    std::vector<float> vertices; // Stores the tree's vertices

    void buildTree() {
        glm::vec3 start(0.0f, 0.0f, 0.0f); // Start at origin
        glm::vec3 direction(0.0f, branchLength, 0.0f); // Initial upward direction
        generateBranches(start, direction, recursionDepth);
    }

    void generateBranches(const glm::vec3& start, const glm::vec3& direction, int depth) {
        if (depth == 0) return;

        // Compute the end point of the branch
        glm::vec3 end = start + direction;

        // Add the branch to the vertices list
        vertices.push_back(start.x);
        vertices.push_back(start.y);
        vertices.push_back(start.z);
        vertices.push_back(end.x);
        vertices.push_back(end.y);
        vertices.push_back(end.z);

        // Compute new branch directions
        float angleRadians = glm::radians(branchAngle);
        glm::mat4 rotationLeft = glm::rotate(glm::mat4(1.0f), angleRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 rotationRight = glm::rotate(glm::mat4(1.0f), -angleRadians, glm::vec3(0.0f, 0.0f, 1.0f));

        glm::vec3 leftDirection = glm::vec3(rotationLeft * glm::vec4(direction * 0.7f, 0.0f));
        glm::vec3 rightDirection = glm::vec3(rotationRight * glm::vec4(direction * 0.7f, 0.0f));

        // Recursively generate branches
        generateBranches(end, leftDirection, depth - 1);
        generateBranches(end, rightDirection, depth - 1);
    }
};

#endif // FRACTAL_TREE_H
