#version 330 core
out vec4 FragColor;
in vec2 texCoord;
uniform sampler2D aTexture;    // 输入的纹理
uniform vec3 sunColor;         // 太阳的颜色
uniform vec3 glowColor;        // 光晕的颜色
uniform float time;            // 时间变量，用于动态效果
void main() {
    vec4 baseColor = texture(aTexture, texCoord);    // 从纹理中获取基础颜色
    float flicker = 0.8 + 0.2 * sin(time * 1.5);  // 使光效随时间波动，增加闪烁效果
    vec3 sunEffect = mix(sunColor, glowColor, flicker);  // 太阳核心与光晕的混合
    vec3 finalColor = mix(baseColor.rgb, sunEffect, flicker);  // 使用闪烁的光强进行混合
    FragColor = vec4(finalColor, baseColor.a);// 输出最终的颜色：保留原始纹理的透明度
}



