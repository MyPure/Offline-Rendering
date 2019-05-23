#include <svpng/svpng.inc>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#define PI 3.1415926
using namespace glm;
using namespace std;
enum ObjectType {
	plane,
	sphere,
	lamp
};
class Object{
public:
	vec3 color = vec3(0.0f);
	vec3 p1 = vec3(0.0f);
	vec3 n = vec3(0.0f);
	vec3 up = vec3(0.0f);
	float width = 0;
	float height = 0;
	int id;
	static int ID;
	vec3 p2 = vec3(0.0f);
	float r = 0;
	ObjectType objectType;
	Object(vec3 P1, vec3 N, vec3 Up,float Width,float Height,vec3 Color){
		p1 = P1;
		n = N;
		up = Up;
		width = Width;
		height = Height;
		color = Color;
		objectType = plane;
		id = ID++;
	}
	Object(vec3 P2, float R,ObjectType Type) {
		p2 = P2;
		r = R;
		objectType = Type;
		id = ID++;
	}
	vec3 IsIntersect(vec3 p0, vec3 u) {
		if (objectType == plane) {
			bool i = 0;
			float t;
			float ndotu = dot(n, u);
			vec3 p;
			if (abs(ndotu) < 0.002) {
				t = 0;
			}
			else {
				t = dot(n, p1 - p0) / ndotu;
			}
			if (t > 0) {
				p = p0 + u * t;
				vec3 r = normalize(cross(n, up));
				float hor = abs(dot((p - p1), r));
				float ver = abs(dot((p - p1), up));
				//printf("%.2f %.2f %.2f,%.2f %.2f %.2f\n", p.x, p.y, p.z,u.x,u.y,u.z);
				i = (hor <= width / 2) && (ver <= height / 2);
			}
			if (t > 0 && i) {
				return p;
			}
			else {
				return vec3(0.0f);
			}
		}
		else if (objectType == sphere|| objectType == lamp) {
			float A = dot(u, u), B = 2 * dot((p0 - p2), u), C = dot((p0 - p2), (p0 - p2)) - r * r;
			float delta = B * B - 4 * A*C;
			float t = (-1 * B - sqrt(delta)) / (2 * A);
			//printf("%.2f", delta);
			if (delta < 0) {
				return vec3(0.0f);
			}
			else {
				if (t < 0) {
					return vec3(0.0f);
				}
				else {
					return p0 + u * t;
				}
			}
		}
	}
	vec3 Getn(vec3 p) {
		if (objectType == plane) { 
			return n; 
		}
		else if (objectType == sphere) {
			return normalize(p - p2);
		}
	}
};