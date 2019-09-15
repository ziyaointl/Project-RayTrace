#ifndef SPHERE_HPP
#define SPHERE_HPP
#include "vec3.hpp"
#include "hittable.hpp"

class Sphere: public Hittable {
	public:
		Sphere(const Vec3 &center, float radius): center(center), radius(radius) { };
		Vec3 center;
		float radius;

		Vec3 normalAt(const Vec3 &pt) const {
			return normalized(pt - center);
		}

		virtual bool hit(const Ray &r, float tMin, float tMax, HitRecord &rec) const {
			Vec3 oc = r.origin - center;
			float a = r.direction * r.direction;
			float b = r.direction * oc;
			float c = oc * oc - radius * radius;
			float discriminant = b*b - a*c;
			if (discriminant < 0) { // No solutions exist
				return false;
			}
			float t = (-b - sqrt(discriminant)) / a;
			if (t > tMax || t < tMin) { // The smaller t is out of range
                t = (-b + sqrt(discriminant)) / a;
            } 
            if (t < tMax && t > tMin) {
                rec.t = t;
                rec.p = r.pointAt(t);
                rec.normal = normalAt(rec.p);
                return true;
            }
            return false;
		}
};

#endif
