#ifndef HITTABLE
#define HITTABLE

#include"utility.h"
class Material;  // alert the compiler that the pointer is to a class,

class Hit_record {
	public:
		point3 int_p;	// intersection point
		Vec3 normal;	// normal at intersection point
		double t;		// parameter value of the ray at intersection point
		shared_ptr<Material> mat_ptr;	// pointer to the material of the object

		double u, v;	// texture coordinates

		bool front_face;	// true if the ray hits the front face of the object
	public:
		Hit_record() : int_p(0,0,0), normal(0,0,0), t(0.0f), mat_ptr(nullptr), u(0.0f), v(0.0f), front_face(false){}
		
		void set_face_normal(const Ray& r, const Vec3& outward_normal) {
			// set the normal of the object at the intersection point
			front_face = dot(r.direction(), outward_normal) < 0;
			normal = front_face ? outward_normal : -outward_normal;
		}
};

class Hittable {
	public:
		virtual ~Hittable() = default;
		// By this pure virtual member, function hittable is an abstract class 
		virtual bool TestIntersection(const Ray& r, Interval ray_t, Hit_record& rec) const = 0;
};

#endif 
