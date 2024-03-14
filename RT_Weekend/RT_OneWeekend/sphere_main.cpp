
#include <iostream>
#include "rtweekend.h"
#include "color.h"
#include "hittable_list.h"
#include "hittable.h"
#include "sphere.h"


color ray_color(const Ray& r, const Hittable_list world) {
	Hit_record rec;

	if (world.TestIntersection(r, interval(0, infinity), rec)) {	// kick things off by setting the initial range to 0~infinity
		return 0.5 * (rec.normal + color(1, 1, 1));	// hit shaded based on the normal of the closest sphere, passeed back from the output parametre
	}

	Vec3 unit_direction = Normalize(r.direction());
	auto t = 0.5 * (unit_direction.y() + 1.0);	 // add 1 to ensure it's above zero, multiply by 0.5 to get a num between 0~1
	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}
double hit_sphere(const point3& centre, double radius, point3& o, const Vec3& dirVec) {
	Vec3 oc = o - centre; // camera origin - centre of the sphere
	auto a = dot(dirVec, dirVec);  // v = A + tB
	auto b = 2.0 * dot(oc, dirVec);
	auto c = dot(oc, oc) - radius * radius;
	auto discriminant = b * b - 4 * a * c; // discriminant
	// returns true if t value is real 
	if (discriminant < 0) {
		return -1.0;	// return negative number 
	}
	else {		
		return (-b - sqrt(discriminant)) / (2.0 * a);	// only care about smaller t value 
	}
}

int main()
{
	// Image data 
	const auto aspect_ratio = 16.0 / 9.0;
	const int image_width = 400;	
	const int image_height = static_cast<int>(image_width / aspect_ratio);	// 225px 

	// World 
	Hittable_list world;
	world.add(make_shared<Sphere>(point3(0, 0, -1), 0.5)); // smaller sphere
	world.add(make_shared<Sphere>(point3(0, -100.5, -1), 100)); // larger sphere


	// ========== Camera part ===============
	// virtual viewport
	auto viewport_height = 2.0;
	auto viewport_width = aspect_ratio * viewport_height;	//3.555
	auto focal_length = 1.0;

	auto origin = point3(0, 0, 0);
	auto lower_left_corner = origin - Vec3(viewport_width / 2, viewport_height / 2, focal_length);
	auto horizontal = Vec3(viewport_width, 0.0, 0.0);
	auto vertical = Vec3(0.0, viewport_height, 0.0);

	// ========== Render part ===============  
	std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

	for (int j = image_height - 1; j >= 0; j--) {
		std::cerr << "Scanlines remaining: " << j << '\n';
		for (int i = 0; i < image_width; i++) {
			auto u = double(i) / (image_width - 1);	// 0~1
			auto v = double(j) / (image_height - 1);	// 0~1
			Ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);

			color pixel_color = ray_color(r, world);

			write_color(std::cout, pixel_color);
		}
	}
	std::cerr << "\nDone.\n";
}