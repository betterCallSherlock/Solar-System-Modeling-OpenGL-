#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.h"
#include "Shaders.h"
#include "Camera.h"
#include "Sphere.h"
#include "skyBox.h"
#include "TriMesh.h"
#include "MeshPainter.h"
#include <iostream>
#include <vector>
#include <random>

using namespace std;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow* windows, double xOffset, double yOffset);
bool getTextureID(unsigned int &ID, char *s);
bool stopEarthRotate = false;

// settings
const unsigned int SCR_WIDTH = 2400;
const unsigned int SCR_HEIGHT = 1800;

const float aroundTimeEarth = 10.0f * 3;
const float aroundTimeMars = 12.0f * 3;
const float aroundTimeJupiter = 13.0f * 3;
const float aroundTimeSaturn = 15.0f * 3;
const float aroundTimeUranus = 16.0f * 3;
const float aroundTimeNeptune = 18.0f * 3;
const float aroundTimeVenus = 6.0f * 3;
const float aroundTimeMecury = 5.0f * 3;
const float selfAroundTime = 10.0f * 5;
const float aroundTimeMoon = 8.0f * 3;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
Light* light = new Light();

float deltaTime = 0.0f;
float lastFrame = 0.0f;
MeshPainter *mesh_painter = new MeshPainter();
MeshPainter *shadow_painter = new MeshPainter();
TriMesh *shuffle = new TriMesh ();
TriMesh *plane = new TriMesh();
TriMesh *satellite = new TriMesh();
Sphere sun(0.5f);
unsigned int earthTexture, moonTexture, sunTexture,venusTexture,
marsTexture,mercuryTexture,jupiterTexture,saturnTexture,uranusTexture,neptuneTexture;
bool bindCameraShuffle = false;

unsigned int VBO, VAO, EBO, sunVAO;
void init(){
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // 设置光源位置
    light->setTranslation(glm::vec3(0.0, 10.0, 0.0));
    light->setAmbient(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 环境光
    light->setDiffuse(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 漫反射
    light->setSpecular(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 镜面反射

    shuffle->setNormalize(true);
    shuffle->readObj("asset/fly.obj",1);
    // 飞机 - 铝金属
    shuffle->setAmbient(glm::vec4( 0.25f, 0.20725f, 0.20725f, 0.922f));
    shuffle->setDiffuse(glm::vec4(1.0f, 0.829f, 0.829f, 0.922f));
    shuffle->setSpecular(glm::vec4(.296648f, 0.296648f, 0.296648f, 0.922f ));
    shuffle->setShininess(11.264f);
    shuffle->setRotation(glm::vec3 (-20,-140,0)); //飞机的参数
    shuffle->setTranslation(glm::vec3(0,-0.82,0));
    shuffle->setScale(glm::vec3 (0.9,0.9,0.9));
    mesh_painter->addMesh(shuffle, "shuffle", "asset/flySZU.jpg", "shaders/vshader.glsl", "shaders/fshader.glsl");
    shadow_painter->addMesh(shuffle, "shuffle", "asset/black.jpg", "shaders/vshader.glsl", "shaders/fshader.glsl");
    // 发射台 - 塑胶
    plane->setAmbient(glm::vec4 (0.05f,0.05f,0.05f,1.0f));
    plane->setDiffuse(glm::vec4(0.5f,0.5f,0.5f,1.0f));
    plane->setSpecular(glm::vec4( 0.7f,0.7f,0.7f,1.0f ));
    plane->setShininess(10.0f);
    plane->generateSquare(glm::vec3 (1,1,1));
    plane->setRotation(glm::vec3 (-90,0,0));
    plane->setTranslation(glm::vec3(0,-1.00001f,0));
    plane->setScale(glm::vec3(1.0,1.0,1.0));
    mesh_painter->addMesh(plane,"plane","asset/lauch.png","shaders/vshader.glsl", "shaders/fshader.glsl");
    // 人造卫星 - 铝金属
    satellite->setNormalize(true);
    satellite->readObj("asset/satellite.obj",1);
    satellite->setAmbient(glm::vec4(0.25f, 0.20725f, 0.20725f, 0.922f));
    satellite->setDiffuse(glm::vec4(1.0f, 0.829f, 0.829f, 0.922f));
    satellite->setSpecular(glm::vec4(.296648f, 0.296648f, 0.296648f, 0.922f ));
    satellite->setShininess(11.264f);
    satellite->setTranslation(glm::vec3(0,0,2)+glm::vec3(2.5f,0.0f,0.0f));
    satellite->setScale(glm::vec3 (0.1,0.1,0.1));
    mesh_painter->addMesh(satellite, "satellite", "asset/satellite.jpeg", "shaders/vshader.glsl", "shaders/fshader.glsl");



    // 材质
    getTextureID(earthTexture, "./img/earth.png");
    getTextureID(moonTexture,"./img/moon.jpg");
    getTextureID(venusTexture,"./img/venus.jpg");
    getTextureID(sunTexture, "./img/sun.jpg");
    getTextureID(marsTexture, "./img/mars.jpg");
    getTextureID(mercuryTexture,"./img/mercury.jpg");
    getTextureID(jupiterTexture,"./img/jupiter.jpg");
    getTextureID(saturnTexture,"./img/saturn.jpg");
    getTextureID(uranusTexture,"./img/uranus.jpg");
    getTextureID(neptuneTexture,"./img/neptune.jpg");

    //VAO VBO EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sun.getVertexSize()*sizeof(float ), sun.getVertexAddress(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sun.getIndicesSize()*sizeof(int ), sun.getIndicesAddress(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)) );
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // sunVAO
    glGenVertexArrays(1, &sunVAO);
    glBindVertexArray(sunVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float ), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float ), (void*)(3*sizeof(float)) );
    glEnableVertexAttribArray(1);

    // skyBox天空盒初始化
    skyBoxInit("./img/skybox");
}

void printHelp();
glm::mat4 earthRotate,venusRotate;
glm::mat4 marsRotate;
glm::mat4 mercuryRotate;
glm::mat4 jupiterRotate;
glm::mat4 saturnRotate;
glm::mat4 uranusRotate;
glm::mat4 neptuneRotate;
glm::mat4 sateRotate;
glm::mat4 selfRotate;

glm::vec3 venusPos(1.44f, 0.0f, 0.0f);
glm::vec3 earthPos(2.5f,0.0f,0.0f);
glm::vec3 marsPos(5.0f,0.0f,0.0f);
glm::vec3 satePos(0.6f, 0.7f, 0.9f);
glm::vec3 mercuryPos(0.93f,0,0);
glm::vec3 jupiterPos(6.231f,0,0);
glm::vec3 saturnPos(9.231f,0,0);
glm::vec3 uranusPos(12.231f,0,0);
glm::vec3 neptunePos(15.231f,0,0);
glm::vec3 sateAxis = glm::cross(satePos, glm::vec3(0,0,-1.0f));
glm::vec3 Positions[10]; // 星球位置
int lookAtPlanet = 0;
glm::mat4 earth_tmp;

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    static bool firstMouse = true;
    static float lastX, lastY;

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xOffset = xpos - lastX;
    float yOffset = lastY - ypos; // 注意方向相反
    lastX = xpos;
    lastY = ypos;

    // 动态更新摄像机方向
    camera.processMouse(xOffset, yOffset);

    // 如果绑定到飞机，更新飞机的旋转
    if (bindCameraShuffle) {
        if (bindCameraShuffle) {
            // 根据摄像机方向更新飞机旋转
            glm::vec3 front = glm::normalize(camera.Front);

            // 计算偏航角（yaw）和俯仰角（pitch）
            float yaw = glm::degrees(glm::atan(front.z, front.x)) + 90.0f; // +90 因为 atan 返回值范围
            float pitch = glm::degrees(glm::asin(front.y));

            yaw = - yaw - 140;
            pitch = - pitch - 20;

            if(yaw > 360) yaw -= 360;
            if(yaw < 360) yaw += 360;

            if(pitch > 360) pitch -= 360;
            if(pitch < 360) pitch += 360;

            // 将偏航和俯仰角直接应用到飞机旋转
            shuffle->setRotation(glm::vec3(pitch, yaw, 0.0f));
        }
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
    if(key == GLFW_KEY_E && action == GLFW_PRESS){
        glm::vec3 scale = satellite->getScale();
        scale = scale + glm::vec3(.1,.1,.1);
        if(scale.x <= 0.5) {
            satellite->setScale(scale);
        }
    }
    if(key == GLFW_KEY_R && action == GLFW_PRESS){
        glm::vec3 scale = satellite->getScale();
        scale = scale - glm::vec3(.1,.1,.1);
        if(scale.x >= 0.1) {
            satellite->setScale(scale);
        }
    }
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if(glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        bindCameraShuffle = true;
    }
    if(glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
        bindCameraShuffle = false;
    }
    if(glfwGetKey(window,GLFW_KEY_Z) == GLFW_PRESS) {
        bindCameraShuffle = false;
        camera.ViewType = 0;
        camera.updateCameraVectors();
    }
    if(glfwGetKey(window,GLFW_KEY_X) == GLFW_PRESS) {
        camera.updateCameraVectors();
        camera.ViewType = 1;
        bindCameraShuffle = false;
    }
    if(glfwGetKey(window,GLFW_KEY_H) == GLFW_PRESS)
        printHelp();
    if(glfwGetKey(window,GLFW_KEY_1) == GLFW_PRESS)
        lookAtPlanet = 0;
    if(glfwGetKey(window,GLFW_KEY_2) == GLFW_PRESS)
        lookAtPlanet = 2;
    if(glfwGetKey(window,GLFW_KEY_3) == GLFW_PRESS)
        lookAtPlanet = 3;
    if(glfwGetKey(window,GLFW_KEY_4) == GLFW_PRESS)
        lookAtPlanet = 4;
    if(glfwGetKey(window,GLFW_KEY_5) == GLFW_PRESS)
        lookAtPlanet = 5;
    if(glfwGetKey(window,GLFW_KEY_6) == GLFW_PRESS)
        lookAtPlanet = 6;
    if(glfwGetKey(window,GLFW_KEY_7) == GLFW_PRESS)
        lookAtPlanet = 7;
    if(glfwGetKey(window,GLFW_KEY_8) == GLFW_PRESS)
        lookAtPlanet = 8;
    if(glfwGetKey(window,GLFW_KEY_9) == GLFW_PRESS)
        lookAtPlanet = 9;
    if(glfwGetKey(window,GLFW_KEY_T) == GLFW_PRESS) {
        stopEarthRotate = true;
        earth_tmp = earthRotate;
    }
    if(glfwGetKey(window,GLFW_KEY_T) &&  mode == GLFW_MOD_SHIFT) {
        stopEarthRotate = false;
    }
}

void scroll_callback(GLFWwindow* windows, double xOffset, double yOffset){
    camera.processScroll(yOffset);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
void printHelp()
{
    std::cout << "=========================================================" << std::endl;
    std::cout << "SZU Computer Graphic 2024 Fall Final Project "<< std::endl;
    std::cout <<  "Solar System Scenario Modeling By Chuhao Zhou 2022150057" << std::endl;
    std::cout << "=========================================================" << std::endl;
    std::cout << "Use mouse scroll to Zoom! And Use Mouse To Control Camera's Direction!" << std::endl<<std::endl;
    std::cout << "Keyboard Usage" << std::endl;
    std::cout <<
              "[Window]" << std::endl <<
              "ESC:		Exit" << std::endl <<
              "h:		Print help message" << std::endl <<
              std::endl <<
              "[ViewMode]" << std::endl <<
              "Z:		Free Moving Mode" << std::endl <<
              "X:		Asteroid Observation Mode" << std::endl <<
              "C:		Driving Shuffle Mode" << std::endl <<
              std::endl <<
              "[Free Moving Mode - Camera Movements]" << std::endl <<
              "WASD:		Move Forward/Left/Backward/Right" << std::endl <<
              "SPACE:		Move Up" << std::endl <<
              "Left Shift:	Move Down" << std::endl <<
              std::endl <<
              "[Asteroid Observation Mode - Targets]" << std::endl <<
              "1:           Sun" << std::endl <<
              "2:           Moon" << std::endl <<
              "3:           Venus" << std::endl <<
              "4:           Mars" << std::endl <<
              "5:           Mercury" << std::endl <<
              "6:           Jupiter" << std::endl <<
              "7:           Saturn" << std::endl <<
              "8:           Uranus" << std::endl <<
              "9:           Neptune" << std::endl <<
              std::endl <<
              "[Driving Shuffle Mode - Shuffle & Camera Movements]" << std::endl <<
              "WASD:		Move Forward/Left/Backward/Right" << std::endl <<
              "SPACE:		Move Up" << std::endl <<
              "Left Shift:	Move Down" << std::endl <<
              std::endl <<
              "[Other Keyboard]" << std::endl <<
              "E:		    Enlarge Satellite Size" << std::endl <<
              "R:		    Shrink Satellite Size" << std::endl<<
              "T:		    Stop Earth Rotation"<< std::endl<<
              "Right Shift+T:		Continue Earth Rotation"<<std::endl;

}
int main()
{
    glfwInit();

    // 配置GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "2022150057_周楚皓_期末大作业", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    init();
    printHelp();
    glfwSetKeyCallback(window,key_callback);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEPTH_TEST);
    Shaders normalShader("./shaders/vertexShader.glsl", "./shaders/fragmentShader.glsl");
    Shaders sunShader("./shaders/sunVertexShader.glsl","./shaders/sunFragmentShader.glsl");
    Shaders skyBoxShader("./shaders/skyBoxVS.glsl", "./shaders/skyBoxFS.glsl");
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processInput(window);
        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Shaders *shader;
        // 太阳
        glBindVertexArray(sunVAO);
        glBindTexture(GL_TEXTURE_2D, sunTexture);

        // 投影矩阵和视图矩阵
        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.getViewMat();
        glm::mat4 model;
        model = glm::scale(model,glm::vec3 (1.4,1.4,1.4));
        // 自旋
        selfRotate = glm::rotate(selfRotate, glm::radians(deltaTime / selfAroundTime * 360),
                                 glm::vec3(0.0f, 1.0f, 0.0f));
        model = model * selfRotate;
        // 使用着色器
        shader = &sunShader;
        shader->useProgram();

        // 设置矩阵
        shader->setMat4("view", view);
        shader->setMat4("projection", projection);
        shader->setMat4("model", model);

        // 设置动态参数
        shader->setFloat("time", currentFrame);
        shader->setFloat("radius", sun.RADIUS);
        shader->setVec3("sunColor", glm::vec3(1.0f, 1.0f, 0.0f));
        shader->setVec3("glowColor", glm::vec3(1.0f, 0.5f, 0.0f));
        Positions[0] = glm::vec3(model[3]);
        glDrawElements(GL_TRIANGLES, sun.getIndicesSize(), GL_UNSIGNED_INT, 0);// 地球
        // 地球
        shader = &normalShader;
        shader->useProgram();
        shader->setFloat("spotlightInnerAngle",glm::radians(60.0f));
        shader->setFloat("spotlightOuterAngle",glm::radians(90.0f));
        shader->setMat4("view", view);
        shader->setMat4("projection", projection);
        shader->setMat4("model", model);
        shader->setVec3("lightColor",1.0f,1.0f,1.0f);
        shader->setVec3("lightPos",.0,.0,.0);
        shader->setVec3("cameraPos",camera.getPosition());
        shader->setFloat("roughness", 0.9);
        shader->setFloat("fresnel",.1);
        shader->setFloat("time", currentFrame);
        glBindVertexArray(VAO);
        glBindTexture(GL_TEXTURE_2D, earthTexture);
        // 地球公转
        if(!stopEarthRotate)
            earthRotate = glm::rotate(earthRotate, glm::radians(deltaTime/aroundTimeEarth*360),
                                      glm::vec3(0.0f, 1.0f, 0.0f));
        else
            earthRotate = earth_tmp;

        model = earthRotate;
        model = glm::translate(model, earthPos);
        // 加入自转
        glm::mat4 modelEarth = model * selfRotate;
        shader->setMat4("model", modelEarth);
        Positions[1] = modelEarth[3];
        glDrawElements(GL_TRIANGLES, sun.getIndicesSize(), GL_UNSIGNED_INT, 0);
        // 月球
        glBindTexture(GL_TEXTURE_2D, moonTexture);
        // 为地球下一层
        sateRotate = glm::rotate(sateRotate,glm::radians(deltaTime/aroundTimeMoon*360),sateAxis);
        model = model * sateRotate;// 月球公转
        model = glm::translate(model, satePos);// 与地球相对距离
        model = model * selfRotate;// 月球自转
        model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
        shader->setMat4("model", model);
        Positions[2] = model[3];
        glDrawElements(GL_TRIANGLES, sun.getIndicesSize(), GL_UNSIGNED_INT, 0);




        // 金星
        glBindTexture(GL_TEXTURE_2D, venusTexture);
        venusRotate = glm::rotate(venusRotate, glm::radians(deltaTime/aroundTimeVenus*360), glm::cross(venusPos, glm::vec3(0, 0.0f, -1.0f)));
        model = venusRotate;
        model = glm::translate(model, venusPos);
        model = model * selfRotate;
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        shader->setMat4("model", model);
        Positions[3] = model[3];
        glDrawElements(GL_TRIANGLES, sun.getIndicesSize(), GL_UNSIGNED_INT, 0);

        // 火星
        glBindTexture(GL_TEXTURE_2D, marsTexture);
        marsRotate = glm::rotate(marsRotate, glm::radians(deltaTime/aroundTimeMars*360), glm::cross(marsPos, glm::vec3(0, 0.0f, -1.0f)));
        model = marsRotate;
        model = glm::translate(model, marsPos);
        model = model * selfRotate;
        model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
        Positions[4] = model[3];
        shader->setMat4("model", model);
        glDrawElements(GL_TRIANGLES, sun.getIndicesSize(), GL_UNSIGNED_INT, 0);
        satellite->setTranslation(Positions[4]+glm::vec3 (0,0,1));
        satellite->setRotation(glm::vec3 (0,deltaTime/aroundTimeMoon*360,0));

        // 水星
        glBindTexture(GL_TEXTURE_2D, mercuryTexture);
        mercuryRotate = glm::rotate(mercuryRotate, glm::radians(deltaTime/aroundTimeMecury*360), glm::cross(mercuryPos, glm::vec3(0, 0.0f, -1.0f)));
        model = mercuryRotate;
        model = glm::translate(model, mercuryPos);
        model = model * selfRotate;
        model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
        Positions[5] = model[3];
        shader->setMat4("model", model);
        glDrawElements(GL_TRIANGLES, sun.getIndicesSize(), GL_UNSIGNED_INT, 0);

        // 木星
        glBindTexture(GL_TEXTURE_2D, jupiterTexture);
        jupiterRotate = glm::rotate(jupiterRotate, glm::radians(deltaTime/aroundTimeJupiter*360), glm::cross(jupiterPos, glm::vec3(0, 0.0f, -1.0f)));
        model = jupiterRotate;
        model = glm::translate(model, jupiterPos);
        model = model * selfRotate;
        model = glm::scale(model, glm::vec3(1.35f, 1.35f, 1.35f));
        Positions[6] = model[3];
        shader->setMat4("model", model);
        glDrawElements(GL_TRIANGLES, sun.getIndicesSize(), GL_UNSIGNED_INT, 0);

        // 土星
        glBindTexture(GL_TEXTURE_2D, saturnTexture);
        saturnRotate = glm::rotate(saturnRotate, glm::radians(deltaTime/aroundTimeSaturn*360), glm::cross(saturnPos, glm::vec3(0, 0.0f, -1.0f)));
        model = saturnRotate;
        model = glm::translate(model, saturnPos);
        model = model * selfRotate;
        model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
        Positions[7] = model[3];
        shader->setMat4("model", model);
        glDrawElements(GL_TRIANGLES, sun.getIndicesSize(), GL_UNSIGNED_INT, 0);

        // 天王星
        glBindTexture(GL_TEXTURE_2D, uranusTexture);
        uranusRotate = glm::rotate(uranusRotate, glm::radians(deltaTime/aroundTimeUranus*360), glm::cross(uranusPos, glm::vec3(0, 0.0f, -1.0f)));
        model = uranusRotate;
        model = glm::translate(model, uranusPos);
        model = model * selfRotate;
        model = glm::scale(model, glm::vec3(1.8f, 1.8f, 1.8f));
        Positions[8] = model[3];
        shader->setMat4("model", model);
        glDrawElements(GL_TRIANGLES, sun.getIndicesSize(), GL_UNSIGNED_INT, 0);

        // 海王星
        glBindTexture(GL_TEXTURE_2D, neptuneTexture);
        neptuneRotate = glm::rotate(neptuneRotate, glm::radians(deltaTime/aroundTimeNeptune*360), glm::cross(neptunePos, glm::vec3(0, 0.0f, -1.0f)));
        model = neptuneRotate;
        model = glm::translate(model, neptunePos);
        model = model * selfRotate;
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        Positions[9] = model[3];
        shader->setMat4("model", model);
        glDrawElements(GL_TRIANGLES, sun.getIndicesSize(), GL_UNSIGNED_INT, 0);

        // Phong模型下的物体
        if(camera.ViewType != 1) {
            mesh_painter->drawMeshes(light, &camera);
            shadow_painter->drawShadows(light, &camera);
        }

        // skybox 环境
        shader = &skyBoxShader;
        shader->useProgram();
        view = glm::mat4(glm::mat3(camera.getViewMat()));
        shader->setMat4("view", view);
        shader->setMat4("projection", projection);
        drawSkyBox();

        if(bindCameraShuffle) {
            glm::vec3 offset(0.0f, 0.1f, 0.0f); // 摄像机偏移
            camera.setPosition(shuffle->getTranslation() + offset);
            camera.updateCameraVectors();
        }
        if(camera.ViewType == 0) {	glClearColor(0.34, 0.34, 0.33, 1.0);

            glfwSetCursorPosCallback(window, mouse_callback);
        }
        else if(camera.ViewType == 1) {
            glfwSetCursorPosCallback(window, NULL);
            glm::vec3 earthPosition = Positions[1]; // 获取地球的世界位置
            glm::vec3 offset = glm::vec3(0.0f, 1.0f, 0.0f); // 摄像机与地球的偏移量
            camera.setPosition(earthPosition + offset); // 更新摄像机位置
            camera.updateCameraVectors();
            camera.setTarget(Positions[lookAtPlanet]);
            glm::mat4 view = camera.getViewMat(); // 根据更新后的摄像机位置计算视图矩阵
            shader->setMat4("view", view);
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);


    glfwTerminate();
    return 0;
}

bool getTextureID(unsigned int &ID, char *s){
    int width, height, nrChannels;
    unsigned char *data = stbi_load(s, &width, &height, &nrChannels, 0);
    if(!data){
        std::cerr << "Failed to open img." << std::endl;
        return false;
    }

    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    return true;
}
void processInput(GLFWwindow *window)
{
    if(bindCameraShuffle) {
        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            shuffle->setTranslation(shuffle->getTranslation() + camera.Front * deltaTime * camera.speed);
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            shuffle->setTranslation(shuffle->getTranslation() - camera.Front * deltaTime * camera.speed);
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            shuffle->setTranslation(shuffle->getTranslation() - camera.Right * deltaTime * camera.speed);
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            shuffle->setTranslation(shuffle->getTranslation() + camera.Right * deltaTime * camera.speed);
        if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            shuffle->setTranslation(shuffle->getTranslation() + glm::vec3(0.0f, 1.0f, 0.0f) * deltaTime * camera.speed);
        if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            shuffle->setTranslation(shuffle->getTranslation() - glm::vec3(0.0f, 1.0f, 0.0f) * deltaTime * camera.speed);
    }
    else {
        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.processKey(cameraMovement::FORWARD, deltaTime);
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.processKey(cameraMovement::BACKWARD, deltaTime);
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.processKey(cameraMovement::LEFT, deltaTime);
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.processKey(cameraMovement::RIGHT, deltaTime);
        if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            camera.processKey(cameraMovement::UP, deltaTime);
        if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            camera.processKey(cameraMovement::DOWN, deltaTime);
    }
}


