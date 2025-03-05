#version 330 core

// 光源结构体
struct Light {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec3 position;
};

// 材质结构体
struct Material {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};

// 输入变量
in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexture;

// 输出变量
out vec3 position;
out vec3 normal;
out vec2 texCoord;

// Uniform变量
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    // 模型变换
    vec4 worldPos = model * vec4(vPosition, 1.0);
    // 投影变换
    gl_Position = projection * view * worldPos;
    // 传递给片段着色器
    position = vec3(worldPos.xyz);
    normal = normalize(mat3(transpose(inverse(model))) * vNormal); // 变换法向量到世界坐标
    texCoord = vTexture;
}