#ifndef PMATH_H
#define PMATH_H
#include "Plane.h"
#include <cmath>
namespace MATH {
	class PMath {
	public:
		static Plane normalize(const Plane &p){
			float mag = sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
			return Plane(p.x / mag, p.y / mag, p.z / mag, p.d / mag);
		}

		/// Get the distance form a point (Vec3) to a plane
		static float distance(const Vec3 &v, const Plane &p){
			Vec3 n = p;
			float mag = VMath::mag(n);
			return (p.x*v.x + p.y*v.y + p.z*v.z + p.d) / mag;
		}
		static Vec3 reflect(const Vec3 &v, const Plane &p){
			float lamda = 2.0f * VMath::dot(p, v);
			return v + lamda * p;
		}
	};
}
#endif
