//
// Created by Steve Wheeler on 23/08/2023.
//

#pragma once
#include "slib.hpp"
#include <vector>


namespace smath
{
    float distance(const slib::vec3& vec);
    slib::vec3 normalize(slib::vec3 vec);
    slib::vec3 centroid(const std::vector<slib::vec3>& points);
    float dot(const slib::vec3& v1, const slib::vec3& v2);
    slib::vec3 cross(const slib::vec3& v1, const slib::vec3& v2);
    slib::mat4 perspective(float zFar, float zNear, float aspect, float fov);
    slib::mat4 view(const slib::vec3& eye, const slib::vec3& target, const slib::vec3& up);
    slib::mat4 rotation(const slib::vec3& eulerAngles);
    slib::mat4 scale(const slib::vec3& scale);
    slib::mat4 translation(const slib::vec3& translation);
    slib::mat4 identity();
    slib::mat4 fpsview(const slib::vec3& eye, float pitch, float yaw);
    void sampleNearest(const slib::texture& tex, float u, float v, int& r, int& g, int& b);
    void sampleBilinear(const slib::texture& tex, float u, float v, float& r, float& g, float& b);
}; // namespace smath
