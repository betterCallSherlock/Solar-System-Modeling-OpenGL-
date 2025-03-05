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
in vec3 position;
in vec3 normal;
in vec2 texCoord;

// Uniform变量
uniform vec3 eye_position;   // 相机位置
uniform Light light;         // 光源
uniform Material material;   // 物体材质
uniform vec2 shadowRangeMin;
uniform vec2 shadowRangeMax;
uniform mat4 view;
uniform int isShadow;        // 阴影开关
uniform sampler2D texture1;  // 纹理
uniform float time;
out vec4 fColor;

// 光照计算
vec4 calculateLighting(vec3 pos, vec3 norm, vec3 eyePos, Light light, Material material) {
    vec3 N = normalize(norm);
    vec3 L = normalize(light.position - pos);       // 光源方向
    vec3 V = normalize(eyePos - pos);               // 视角方向
    vec3 R = reflect(-L, N);                        // 反射向量
    vec4 I_a = light.ambient * material.ambient;    // 环境光
    float diffuseIntensity = max(dot(L, N), 0.0);   // 漫反射
    vec4 I_d = diffuseIntensity * light.diffuse * material.diffuse;
    float specularIntensity = pow(max(dot(R, V), 0.0), material.shininess);// 镜面反射
    vec4 I_s = specularIntensity * light.specular * material.specular;
    if (dot(L, N) < 0.0) // 如果光源在背面，移除镜面分量
        I_s = vec4(0.0);
    return I_a + I_d + I_s;// 合并光照
}

void main() {
    if (isShadow == 1) {
        // 检查裁剪平面
        if ( position.x < shadowRangeMin.x || position.x > shadowRangeMax.x
         || position.z < shadowRangeMin.y  || position.z > shadowRangeMax.y) {
                discard; // 丢弃片段
         }
        fColor = vec4(0.0, 0.0, 0.0, 1.0); // 阴影部分全黑
    } else {
        vec4 lighting = calculateLighting(position, normal, eye_position, light, material);
        vec4 texColor = texture(texture1, texCoord);
        float flicker = 0.8 + 0.2 * sin(time * 1.5);
        fColor =  lighting * texColor * flicker; // 结合纹理和光照
    }
}