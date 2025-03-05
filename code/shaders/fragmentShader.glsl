#version 330 core

out vec4 FragColor;

in vec2 texCoord;
in vec3 normal;
in vec3 worldPos;

uniform sampler2D aTexture; // 漫反射纹理
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform float time;
uniform float roughness, fresnel;
uniform float spotlightInnerAngle; // 聚光灯的内角
uniform float spotlightOuterAngle; // 聚光灯的外角

// Cook-Torrance BRDF模型
float cookTorranceSpec(vec3 lightDir, vec3 viewDir, vec3 surfaceNormal, float r, float f);
float beckmannDistribution(float NdotH, float f);

void main() {
    float globalAlpha = 0.1;
    vec3 global = lightColor * globalAlpha;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - worldPos);

    // 计算光照的张角
    float theta = dot(lightDir, normalize(lightPos - worldPos));
    float epsilon = spotlightInnerAngle - spotlightOuterAngle;
    float intensity = clamp((theta - spotlightOuterAngle) / epsilon, 0.0, 1.0);

    float diffuse = max(dot(normal, lightDir), 0.0) * intensity;

    vec3 viewDir = normalize(cameraPos - worldPos);
    float speculat = cookTorranceSpec(
        lightDir,
        viewDir,
        norm,
        roughness,
        fresnel);

    vec3 result = (global + diffuse + speculat);

    vec4 color = texture(aTexture, texCoord);  // 纹理采样

    float flicker = 0.8 + 0.2 * sin(time * 1.5);

    FragColor = vec4(result, 1.0) * color * flicker;
}

// Cook-Torrance BRDF
float cookTorranceSpec(vec3 lightDir, vec3 viewDir, vec3 surfaceNormal, float r, float f){
    float VdotN = max(dot(viewDir, surfaceNormal), .0);
    float LdotN = max(dot(lightDir, surfaceNormal), .0);

    vec3 H = normalize(lightDir + viewDir);

    float NdotH = max(dot(surfaceNormal, H), .0);
    float VdotH = max(dot(viewDir, H), 0.000001);
    float x = 2.0 * NdotH / VdotH;
    float G = min(1.0, min(x * VdotN, x * LdotN));

    float D = beckmannDistribution(NdotH, r);
    float F = pow(1.0 - VdotN, f);

    return G * F * D / max(3.14159265 * VdotN * LdotN, 0.000001);
}

float beckmannDistribution(float NdotH, float f){
    float NdotH2 = NdotH * NdotH;
    return exp((NdotH2 - 1) / (f * f * NdotH2)) / (3.14159265 * f * f * NdotH2 * NdotH2);
}
