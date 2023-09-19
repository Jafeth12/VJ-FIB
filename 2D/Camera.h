#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera();
    Camera(float left, float right, float bottom, float top);
    ~Camera();

    glm::vec2 getPosition();
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();

    void setPosition(glm::vec2 pos);
    void setXPosition(float x);
    void setYPosition(float y);
    void addToPosition(float x, float y);
    void setVelocity(glm::vec2 vel);


private:
    glm::vec2 position;
    glm::vec2 velocity;

    glm::mat4 projection;
};

#endif
