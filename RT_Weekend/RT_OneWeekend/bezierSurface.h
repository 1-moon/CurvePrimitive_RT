
#ifndef BEZIERSURFACE_H 
#define BEZIERSURFACE_H

#include "bezierCurve.h"
#include "boundingBox.h"

class BezierSurface : public Hittable {
public:
    BezierSurface() : ctrl_points_(nullptr) {}
    BezierSurface(point3* controlPoints, shared_ptr<Material> mat)
        : ctrl_points_(controlPoints), material_(mat) {}

    
    point3 cubicPolynomial(const point3* P, const double& t) const;  

    point3 BezierSurfacePoint(const point3* controlPoints, const double& u, const double& v)const;


    virtual bool TestIntersection(const Ray& castRay, Interval ray_t, Hit_record& rec) const ;  

public:
    point3* ctrl_points_;
    shared_ptr<Material> material_;
    double width = 1.0f;
};

point3 BezierSurface::cubicPolynomial(const point3* ctrl_p, const double& t)const {
    double b0 = bernsteinTerm(0, 3, t);
    double b1 = bernsteinTerm(1, 3, t);
    double b2 = bernsteinTerm(2, 3, t);
    double b3 = bernsteinTerm(3, 3, t);

    return ctrl_p[0] * b0 + ctrl_p[1] * b1 + ctrl_p[2] * b2 + ctrl_p[3] * b3;
}

point3 BezierSurface::BezierSurfacePoint(const point3* controlPoints, const double& u, const double& v)const{
    point3 uCurve[4], vCurve[4];
    // u �������� cubic polynomial ���
    for (int i = 0; i < 4; ++i) {
        point3 ctrl_p[4] = { controlPoints[i], controlPoints[4 + i], controlPoints[8 + i], controlPoints[12 + i] };
        uCurve[i] = cubicPolynomial(ctrl_p, u);
    }
    // ���� ������ ����Ͽ� v �������� cubic polynomial ���
    for (int i = 0; i < 4; ++i) {
        vCurve[i] = cubicPolynomial(uCurve, v);
    }
    return vCurve[0]; // �̴� surface ���� ���� ��Ÿ���ϴ�.
}


bool BezierSurface::TestIntersection(const Ray& castRay, Interval ray_t, Hit_record& rec) const {
    bool hit_anything = false;
    double closest_so_far = ray_t.max;

    // �ٻ����� ���� �˻縦 ���� ǥ���� �̻�ȭ
    const int divisions = 10; // ǥ���� ������ ���� ��
    for (int u = 0; u < divisions; ++u) {
        for (int v = 0; v < divisions; ++v) {
            double u_param = double(u) / (divisions - 1);
            double v_param = double(v) / (divisions - 1);
            
            auto surface_point = BezierSurfacePoint(ctrl_points_, u_param, v_param);
            // �� ���� ����Ʈ �ֺ��� ������ ������ ���� ������ ���� �˻�
            Vec3 oc = castRay.origin() - surface_point;
            auto a = castRay.direction().length_squared();
            auto half_b = dot(oc, castRay.direction());
            auto c = oc.length_squared() - 0.1; // ���� ���� ������ ����

            auto discriminant = half_b * half_b - a * c;
            if (discriminant > 0) {
                auto sqrtd = sqrt(discriminant);

                // �� ������ �� �� ����� �� ã��
                auto root = (-half_b - sqrtd) / a;
                if (root < ray_t.min || root > closest_so_far) {
                    root = (-half_b + sqrtd) / a;
                }
                if (root >= ray_t.min && root <= closest_so_far) {
                    // ���� ���� ������Ʈ
                    rec.t = root;
                    rec.int_p = castRay.at(rec.t);
                    rec.normal = Normalize(rec.int_p - surface_point); // ������������ ����
                    rec.mat_ptr = material_;

                    hit_anything = true;
                    closest_so_far = rec.t;
                }
            }
        }
    }

    return hit_anything;
}

#endif  ;
