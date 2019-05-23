#include <vector>
#include "object.h"
#include "time.h"
void test_rgb();
vec3 ObjectReflect(vec3 p0, vec3 u, int id, int layer);
vec3 RandomReflect(vec3 p0, vec3 p, vec3 n);
vec3 lightPos = vec3(0, 15, 5);
vector<Object> objects;
int Object::ID = 0;
int main(void) {
	srand((unsigned)time(NULL));
	Object planeRight = Object(vec3(15, 15, 0), vec3(-1, 0, 0), vec3(0, 0, 1), 30.0f, 20.0f, vec3(17, 194, 238));
	Object planeLeft = Object(vec3(-15, 15, 0), vec3(1, 0, 0), vec3(0, 0, 1), 30.0f, 20.0f, vec3(187, 109, 116));
	Object planeTop = Object(vec3(0, 15, 10), vec3(0, 0, -1), vec3(0, 1, 0), 30.0f, 30.0f, vec3(162, 153, 167));
	Object planeButtom = Object(vec3(0, 15, -10), vec3(0, 0, 1), vec3(0, 1, 0), 30.0f, 30.0f, vec3(162, 153, 167));
	Object planeForward = Object(vec3(0, 30, 0), vec3(0, -1, 0), vec3(0, 0, 1), 30.0f, 20.0f, vec3(162, 153, 167));
	Object sphere1 = Object(vec3(-7, 22, -6), 3.5, ObjectType::sphere);
	Object sphere2 = Object(vec3(7, 16, -6), 3.5, ObjectType::sphere);
	Object lamp1 = Object(lightPos, 1.0f, ObjectType::lamp);
	objects.push_back(lamp1);
	objects.push_back(sphere1);
	objects.push_back(sphere2);
	objects.push_back(planeTop);
	objects.push_back(planeButtom);
	objects.push_back(planeLeft);
	objects.push_back(planeRight);
	objects.push_back(planeForward);
	test_rgb();
	return 0;
}
void test_rgb(void) {
	static bool first=true;
	const int X = 800;
	const int Z = 600;
	unsigned char *rgb = (unsigned char *)malloc(sizeof(unsigned char)*X * Z * 3);
	unsigned char *p; 
	unsigned x, z;
	for (int i = 0; i < 500; i++) {
		p = rgb;
		if (first) {
			for (z = 0; z < Z; z++) {
				if (z % (Z / 20) == 0)printf("%d:%%%.0f\n",i, (float)z / Z * 100);
				for (x = 0; x < X; x++) {
					vec3 p0 = vec3(0.0f);
					vec3 u = vec3(-4.0f + ((float)x / X)*8.0f, 6.0f / sqrt(2), 3.0f - ((float)z / Z)*6.0f);
					vec3 color = ObjectReflect(p0, u, -1, 0);
					*p++ = color.r;    /* R */
					*p++ = color.g;    /* G */
					*p++ = color.b;    /* B */
				}
			}
			first = false;
		}
		else {
			for (z = 0; z < Z; z++) {
				if (z % (Z / 20) == 0)printf("%d:%%%.0f\n", i, (float)z / Z * 100);
				for (x = 0; x < X; x++) {
					vec3 p0 = vec3(0.0f);
					vec3 u = vec3(-4.0f + ((float)x / X)*8.0f, 6.0f / sqrt(2), 3.0f - ((float)z / Z)*6.0f);
					vec3 color = ObjectReflect(p0, u, -1, 0);
					*p++ = (*p + color.r) / 2.0;    /* R */
					*p++ = (*p + color.g) / 2.0;    /* G */
					*p++ = (*p + color.b) / 2.0;    /* B */
				}
			}
		}
		FILE *fp = fopen("rgb.png", "wb");
		svpng(fp, X, Z, rgb, 0);
		fclose(fp);
	}
	free(rgb);
}
vec3 ObjectReflect(vec3 p0, vec3 u, int id, int layer) {
	vec3 p;
	if (layer >= 5)return vec3(0.0f);
	for (int i = 0; i < objects.size(); i++) {
		if (objects[i].id == id)continue;//从当前物体发射时，跳过检测
		if ((p = objects[i].IsIntersect(p0, u)) != vec3(0.0f)) {
			if (objects[i].objectType == ObjectType::lamp) {
				return 255.0f * vec3(1.0f);
			}
			else if (objects[i].objectType == ObjectType::sphere) {
				return ObjectReflect(p,reflect(u,objects[i].Getn(p)), objects[i].id,layer);
			}
			else if (objects[i].objectType == ObjectType::plane) {
				return //ObjectReflect(p, RandomReflect(p0, p, objects[i].Getn(p)), objects[i].id, layer + 1) * objects[i].color * (1.0f / 255);
				max(dot(normalize(lightPos - p), objects[i].Getn(p)), 0.0f) * objects[i].color;
			}
		}
	}
	return vec3(0.0f);
}
vec3 RandomReflect(vec3 p0, vec3 p, vec3 n) {
	float u, v, x, y, z;
	u = (rand() % 100 / 99.0) * 2 * PI;
	v = (rand() % 100 / 99.0) * PI;
	x = sin(v)*cos(u);
	y = sin(v)*sin(u);
	z = cos(v);
	//printf("%f %f %f %f %f\n", x, y, z, u, v);
	vec3 s1 = p0 + vec3(x, y, z);
	vec3 s2 = p + n;
	vec3 s = s1 + s2;
	return s - p;
}