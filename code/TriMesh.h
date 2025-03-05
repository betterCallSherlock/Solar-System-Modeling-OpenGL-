#ifndef _TRI_MESH_H_
#define _TRI_MESH_H_

#include "Angel.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <float.h>
// 材质
struct Material {
    std::string name;           // 材质名称
    glm::vec3 ambient;          // 环境光反射
    glm::vec3 diffuse;          // 漫反射
    glm::vec3 specular;         // 镜面反射
    float shininess;            // 光泽度
    std::string diffuse_map;    // 漫反射纹理
};


// 三角面片中的顶点序列
typedef struct vIndex {
    unsigned int x, y, z;
    vIndex(int ix, int iy, int iz) : x(ix), y(iy), z(iz) {}
} vec3i;

class TriMesh
{
public:
    TriMesh();
    ~TriMesh();

    std::vector<glm::vec3> getVertexPositions();
    std::vector<glm::vec3> getVertexColors();
    std::vector<glm::vec3> getVertexNormals();
    std::vector<glm::vec2> getVertexTextures();

    std::vector<vec3i> getFaces();
    std::vector<glm::vec3> getPoints();
    std::vector<glm::vec3> getColors();
    std::vector<glm::vec3> getNormals();
    std::vector<glm::vec2> getTextures();

    void computeTriangleNormals();
    void computeVertexNormals();

    // 获取和设置物体的旋转平移变化
    glm::vec3 getTranslation();
    glm::vec3 getRotation();
    glm::vec3 getScale();
    glm::mat4 getModelMatrix();
    glm::vec3 getPosition();

    void setTranslation(glm::vec3 translation);
    void setRotation(glm::vec3 rotation);
    void setScale(glm::vec3 scale);

    void setNormalize(bool do_norm);
    bool getNormalize();
    float getDiagonalLength();

    // 设置物体旋转位移动画的参数
    void setTranslateTheta(float x, float y, float z);
    void setRotateTheta(float x, float y, float z);
    void setScaleTheta(float x, float y, float z);

    // 根据theta变量更新空间变化数值
    void updateScale();
    void updateRotate();
    void updateTranslate();
    void updateTransform();

    // 投影曲面信息
    glm::vec3 sphereCenters[4];
    float sphereRadius[4];

    // 设置物体材质
    glm::vec4 getAmbient();
    glm::vec4 getDiffuse();
    glm::vec4 getSpecular();
    glm::mat4 getSelfRotation();
    float getShininess();

    void setAmbient(glm::vec4 _ambient );
    void setDiffuse(glm::vec4 _diffuse);
    void setSpecular(glm::vec4 _specular);
    void setShininess(float _shininess);
    // 设置投影曲面信息
    void setShadowSphere(glm::vec3 _sphereCenter[],float _radius[]);
    // 这里添加各种形状的、模型的读取顶点的函数
    void generateCube();
    void generateTriangle(glm::vec3 color);
    void generateSquare(glm::vec3 color);

    void generateCylinder(int num_division, float radius, float height);
    void generateDisk(int num_division, float radius);
    void generateCone(int num_division, float radius, float height);

    void readOff(const std::string& filename);
    void readMtl(const std::string& mtlPath);
    void readObj(const std::string& objPath,int Ftype);

    std::vector<Material*> faceMaterials;  // 存储每个面所使用的材质
    std::vector<Material> materials; // 存储所有材质
    Material* currentMaterial = nullptr; // 当前材质
    Material* findMaterial(const std::string& name);

    // 将读取的顶点根据三角面片上的顶点下标逐个加入
    // 要传递给GPU的points等容器内
    void storeFacesPoints();

    // 清除数据
    void cleanData();

    //设置运动
    void processKey(int dir, float deltaTime);

protected:
    std::vector<glm::vec3> vertex_positions;	// 顶点坐标
    std::vector<glm::vec3> vertex_colors;	// 顶点颜色
    std::vector<glm::vec3> vertex_normals;	// 顶点法向量
    std::vector<glm::vec2> vertex_textures;	// 顶点纹理坐标，注意是vec2
    glm::vec3 Position;         // 位置
    std::vector<vec3i> faces;	// 三角面片上每个顶点对应的顶点坐标的下标
    std::vector<vec3i> normal_index;	// 每个三角面片的顶点对应的顶点法向量的下标
    std::vector<vec3i> color_index;		// 每个三角面片的顶点对应的顶点颜色的下标
    std::vector<vec3i> texture_index;	// 每个三角面片的顶点对应纹理坐标的下标

    std::vector<glm::vec3> face_normals;	// 每个三角面片的法向量

    std::vector<glm::vec3> points;	// 传入着色器的绘制点
    std::vector<glm::vec3> colors;	// 传入着色器的颜色
    std::vector<glm::vec3> normals;	// 传入着色器的法向量
    std::vector<glm::vec2> textures;	// 传入着色器的纹理坐标，注意是vec2

    bool do_normalize_size;        // 是否将物体大小归一化
    float diagonal_length;      // 物体包围盒对角线长度，作为物体归一化系数
    glm::vec3 up_corner;				// 物体包围盒的上对角顶点
    glm::vec3 down_corner;           // 物体包围盒的下对角顶点
    glm::vec3 center;                // 物体的包围盒中心

    glm::vec3 translation;			// 物体的平移参数
    glm::vec3 rotation;				// 物体的旋转参数
    glm::vec3 scale;					// 物体的缩放参数

    glm::mat4 selfRotation;         // 自旋
    glm::vec4 ambient;				// 环境光
    glm::vec4 diffuse;				// 漫反射
    glm::vec4 specular;				// 镜面反射
    float shininess;			// 高光系数

    std::vector<glm::vec4> Ka;	//环境光系数
    std::vector<glm::vec4> Kd;	//漫反射系数
    std::vector<glm::vec4> Ks;	//镜面反射系数
    std::vector<glm::vec4> Em;	//自发光系数
    std::vector<float> Ns;	//高光系数


};


class Light: public TriMesh {
public:
    // 把实验3.2的阴影投影矩阵也加到Light类中
    glm::mat4 getShadowProjectionMatrix();

    // 设置衰减系数的参数
    // https://learnopengl-cn.readthedocs.io/zh/latest/02%20Lighting/05%20Light%20casters/

    void setAttenuation(float _constant, float _linear, float _quadratic);

    float getConstant();
    float getLinear();
    float getQuadratic();

protected:
    // 光源衰减系数的三个参数
    float constant; // 常数项
    float linear;	// 一次项
    float quadratic;// 二次项
};

#endif