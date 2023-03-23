#include "Sphere.h"

bool Sphere::hit(Ray& ray, float tMin, float tMax, HitRecord& record) const
{
    Vec3<float> originToCenter{ray.m_origin - m_center};

    // solve equation t^2 * dot(b, b) + 2t * dot(b, ray direction) + dot(ray direction, ray direction) - r^2 = 0
    // -> t^2 * a + bt + c = 0

    // equal to dot(direction, direction) (a)
    float a{ray.m_direction.lengthSquared()};

    //simplify b in quadratic equation (b)
    float halfB{dot<float>(originToCenter, ray.m_direction)};

    // (c)
    float c{originToCenter.lengthSquared() - m_radius * m_radius};

    // 0 roots no hit
    float discr{halfB * halfB - a * c};
    if (discr < 0.0f)
        return false;

    // one root
    float discrSq{std::sqrt(discr)};
    float root{(-halfB - discrSq) / a};

    // two roots
    if (root < tMin || tMax < root)
    {
        root = (-halfB + discrSq) / a;
        if (root < tMin || tMax < root)
            return false;
    }

    // record the hit information
    record.m_t = root;
    record.m_point = ray.pointAt(record.m_t);
    record.m_normal = (record.m_point - m_center) / m_radius;
    record.setFaceNormal(ray, record.m_normal);

    return true;
}