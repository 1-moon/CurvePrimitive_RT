#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "utility.h"
#include "hittable.h"
constexpr double kEpsilon = 1e-8;
class Triangle : public Hittable {
public:
    Triangle(const point3& v0, const point3& v1, const point3& v2, shared_ptr<Material> m)
        : vertex0(v0), vertex1(v1), vertex2(v2), material(m) {}

    virtual bool TestIntersection(const Ray& r, Interval ray_t, Hit_record& rec) const override;

public:
    point3 vertex0, vertex1, vertex2; // �ﰢ���� ������
    shared_ptr<Material> material; // ����
};

bool Triangle::TestIntersection(const Ray& r, Interval ray_t, Hit_record& rec) const {
    // ����-�ﰢ�� ���� �˰��� ����
    Vec3 v0v1 = vertex1 - vertex0;
    Vec3 v0v2 = vertex2 - vertex0;
    Vec3 pvec = cross(r.direction(), v0v2);
    double det = dot(v0v1, pvec);

    if (fabs(det) < kEpsilon) return false;

    double invDet = 1 / det;
    Vec3 tvec = r.origin() - vertex0;
    double u = dot(tvec, pvec) * invDet;
    if (u < 0 || u > 1) return false;

    Vec3 qvec = cross(tvec, v0v1);
    double v = dot(r.direction(), qvec) * invDet;
    if (v < 0 || u + v > 1) return false;

    double t = dot(v0v2, qvec) * invDet;
    if (!ray_t.surrounds(t)) return false;

    // ���� ���� ������Ʈ
    rec.t = t;
    rec.int_p = r.at(t);
    rec.normal = Normalize(cross(v0v1, v0v2)); // �ﰢ���� �� ���� ���
    rec.mat_ptr = material;

    return true;
}

#endif // TRIANGLE_H
