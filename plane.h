#include <svpng/svpng.inc>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

enum ReflectType {
	diffuse,
	specular
};
class Plane {
public:
	glm::vec3 p1;
	glm::vec3 n;
	glm::vec3 up;
	float width;
	float height;
	glm::vec3 color;
	ReflectType reflectType = diffuse;
	Plane(glm::vec3 P1, glm::vec3 N, glm::vec3 Up,float Width,float Height,glm::vec3 Color) {
		p1 = P1;
		n = N;
		up = Up;
		width = Width;
		height = Height;
		color = Color;
	}
	bool IsIntersect(glm::vec3 p0, glm::vec3 u) {
		bool i = 0;
		float t;
		float ndotu = glm::dot(n, u);
		if (abs(ndotu) < 0.002) {
			t = 0;
		}
		else {
			t = glm::dot(n, p1 - p0) / ndotu;
		}
		if (t > 0) {
			glm::vec3 p = p0 + u * t;
			glm::vec3 r = glm::normalize(glm::cross(n, up));
			float hor = abs(glm::dot((p - p1), r));
			float ver = abs(glm::dot((p - p1), up));
			//printf("%.2f %.2f %.2f,%.2f %.2f %.2f\n", p.x, p.y, p.z,u.x,u.y,u.z);
			i = (hor <= width / 2) && (ver <= height / 2);
		}
		if (t > 0 && i) {
			return true;
		}
		else {
			return false;
		}
	}
};