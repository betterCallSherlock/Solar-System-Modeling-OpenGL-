#ifndef GLTEST_CAMERA_H
#define GLTEST_CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum cameraMovement{
    FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN
};

class Camera {
public:
    int ViewType = 0;
    glm::vec3 Pos0 = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 Position, Front, Up, Right, WorldUp;
    float yaw, pitch, speed, sensitivity, zoom;

    explicit Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    glm::mat4 getViewMat();
    void processKey(cameraMovement move, float deltaTime);
    void processMouse(float xOffset, float yOffset);
    void processScroll(float yOffset);
    glm::vec3 getPosition();
    void setPosition(glm::vec3 p);
    void setTarget(const glm::vec3& target);
    void updateCameraVectors();
    // 模视矩阵
    glm::mat4 viewMatrix;
    glm::mat4 projMatrix;
    glm::mat4 getProjMat();

private:
    constexpr static float YAW = -90.0f;
    constexpr static float PITCH = 0.0f;
    constexpr static float SPEED = 1.5f;
    constexpr static float SENSITIVITY = 0.005f;
    constexpr static float ZOOM = 45.0f;


};


#endif //GLTEST_CAMERA_H
