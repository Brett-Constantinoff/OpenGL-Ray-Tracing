#pragma once
#include "Vec3.h"
#include "Ray.h"

struct HitRecord
{
    Vec3<float> m_point{};
    Vec3<float> m_normal{};
    float m_t{};
    bool m_frontFace{};

    // calculates which direction the normal should face
    void setFaceNormal(const Ray& ray, const Vec3<float>& normal)
    {
        m_frontFace = dot<float>(ray.m_direction, normal) < 0;
        m_normal = m_frontFace ? normal : -normal;
    }
};

struct Sphere
{
    Sphere(Vec3<float> center, float radius) :
        m_center{center}, m_radius{radius}
        {

        }

    bool hit(Ray& ray, float tMin, float tMax, HitRecord& record) const;

    Vec3<float> m_center;
    float m_radius;
};