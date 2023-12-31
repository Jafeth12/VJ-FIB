#include "Camera.h"
#include "Game.h"

Camera::Camera() {
    position = glm::vec2(0.0f);
    velocity = glm::vec2(0.0f);

    // TODO feo af (quitalo)
    projection = glm::ortho(0.f, 640.0f-1, 480.0f-1, 0.f);
}

Camera::Camera(float left, float right, float bottom, float top) {
    position = glm::vec2(0.0f);
    velocity = glm::vec2(0.0f);

    projection = glm::ortho(left, right, bottom, top);
}

glm::vec2 Camera::getPosition() {
    return position;
}

glm::mat4 Camera::getViewMatrix() {
    return glm::translate(glm::mat4(1.0f), glm::vec3(-position, 0.0f));
}

glm::mat4 Camera::getProjectionMatrix() {
    return projection;
}

// ------------- SETTERS -------------

void Camera::setPosition(glm::vec2 pos) {
    position = pos;
}

void Camera::setXPosition(float x) {
    position.x = x;
}

void Camera::setYPosition(float y) {
    position.y = y;
}

void Camera::addToPosition(float x, float y) {
    position.x += x;
    position.y += y;
}

bool Camera::isOnScreen(const glm::vec2 &pos, const glm::vec2 &size) {
    return (pos.x + size.x >= position.x && pos.x <= position.x + SCREEN_WIDTH) &&
           (pos.y + size.y >= position.y && pos.y <= position.y + SCREEN_HEIGHT);
}

Camera::~Camera() {
    // TODO
}


