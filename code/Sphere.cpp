#include "Sphere.h"

Sphere::Sphere(float radius, int xFragment, int yFragment) {
    const double PI = acos(-1);
    for(int y = 0; y <= yFragment; ++ y){
        for(int x = 0; x <= xFragment; ++ x){
            float yFrac = (float ) y / yFragment;
            float xFrac = (float ) x / xFragment;
            float xPos = radius * std::cos(xFrac * 2.f * PI) * std::sin(yFrac * PI);
            float yPos = radius * std::cos(yFrac * PI);
            float zPos = radius * std::sin(xFrac * 2.f * PI) * std::sin(yFrac * PI);
            vertex.push_back(xPos);
            vertex.push_back(yPos);
            vertex.push_back(zPos);
            vertex.push_back( xFrac);
            vertex.push_back( yFrac);
        }
    }

    for(int y = 0; y < yFragment; ++ y){
        for(int x = 0;x < xFragment; ++ x){
            indices.push_back(y*(xFragment + 1) + x);
            indices.push_back((y+1)*(xFragment + 1) + x);
            indices.push_back((y+1)*(xFragment + 1) + x + 1);

            indices.push_back(y*(xFragment + 1) + x);
            indices.push_back(y*(xFragment + 1) + x + 1);
            indices.push_back((y+1)*(xFragment + 1) + x + 1);
        }
    }

}

unsigned int Sphere::getIndicesSize() {
    return indices.size();
}

unsigned int Sphere::getVertexSize() {
    return vertex.size();
}

int* Sphere::getIndicesAddress() {
    return &indices[0];
}

float* Sphere::getVertexAddress() {
    return &vertex[0];
}
void Sphere::setRadius(float radius) {
    // 更新半径
    RADIUS = radius;

    // 清空原有的顶点数据
    vertex.clear();

    const double PI = acos(-1);

    // 重新计算球体的顶点位置
    for (int y = 0; y <= Y_FRAGMENT; ++y) {
        for (int x = 0; x <= X_FRAGMENT; ++x) {
            float yFrac = (float)y / Y_FRAGMENT;
            float xFrac = (float)x / X_FRAGMENT;

            // 更新每个顶点的坐标（使用新的半径）
            float xPos = radius * std::cos(xFrac * 2.f * PI) * std::sin(yFrac * PI);
            float yPos = radius * std::cos(yFrac * PI);
            float zPos = radius * std::sin(xFrac * 2.f * PI) * std::sin(yFrac * PI);

            // 将顶点坐标和纹理坐标添加到顶点数组中
            vertex.push_back(xPos);
            vertex.push_back(yPos);
            vertex.push_back(zPos);
            vertex.push_back(xFrac); // 添加纹理坐标 x
            vertex.push_back(yFrac); // 添加纹理坐标 y
        }
    }
}
