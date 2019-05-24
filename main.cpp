#include <vector>
#include "object.h"
#include "time.h"
void test_rgb();
vec3 ObjectReflect(vec3 p0, vec3 u, int id, int layer,vec3 color);
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
	const int X = 800;
	const int Z = 600;
	const int N = 1;
	unsigned int *total = (unsigned int*)malloc(sizeof(unsigned int)*X * Z * 3);
	unsigned char *rgb = (unsigned char *)malloc(sizeof(unsigned char)*X * Z * 3);
	unsigned char *p;
	unsigned int *pt=total;
	unsigned x, z;
	for (int i = 0; i < X * Z * 3; i++) {
		*pt++ = 0;
	}
	for (int i = 0; i < N; i++) {
		pt = total;
		for (z = 0; z < Z; z++) {
			if (z % (Z / 20) == 0)printf("%d:%%%.0f\n", i, (float)z / Z * 100);
			for (x = 0; x < X; x++) {
				vec3 p0 = vec3(0.0f);
				vec3 u = vec3(-4.0f + ((float)x / X)*8.0f, 6.0f / sqrt(2), 3.0f - ((float)z / Z)*6.0f);
				vec3 color = ObjectReflect(p0, u, -1, 0 ,vec3(0.0f));
				*pt += color.r;
				pt++;
				*pt += color.g;
				pt++;
				*pt += color.b;
				pt++;
			}
		}
	}
	p = rgb;
	pt = total;
	for (z = 0; z < Z; z++) {
		for (x = 0; x < X; x++) {
			for (int i = 0; i < 3; i++) {
				*p = *pt / N;
				p++;
				pt++;
			}
		}
	}
	FILE *fp = fopen("rgb.png", "wb");
	svpng(fp, X, Z, rgb, 0);
	fclose(fp);
	free(rgb);
	free(total);
}
vec3 ObjectReflect(vec3 p0, vec3 u, int id, int layer,vec3 color) {
	vec3 p;
	if (layer >= 2)return vec3(0.0f);
	for (int i = 0; i < objects.size(); i++) {
		if (objects[i].id == id)continue;//从当前物体发射时，跳过检测
		if ((p = objects[i].IsIntersect(p0, u)) != vec3(0.0f)) {
			if (objects[i].objectType == ObjectType::lamp) {
				return 255.0f * vec3(1.0f);
			}
			else if (objects[i].objectType == ObjectType::sphere) {
				return ObjectReflect(p,reflect(u,objects[i].Getn(p)), objects[i].id,layer,color);
			}
			else if (objects[i].objectType == ObjectType::plane) {
				//vec3 diffuse = max(dot(normalize(lightPos - p), objects[i].Getn(p)), 0.0f) * objects[i].color;
				vec3 random = ObjectReflect(p, RandomReflect(p0, p, objects[i].Getn(p)), objects[i].id, layer + 1, objects[i].color);
				if (layer != 0) {
					float ratio;
					float l = sqrt((p - p0).x * (p - p0).x + (p - p0).y * (p - p0).y + (p - p0).z * (p - p0).z);
					ratio = max((10.0f - l) / 10.0f, 0.2f);
					return ratio * random * color * (1.0f/255.0f) + (1 - ratio) * color;
				}
				return random;
				//max(dot(normalize(lightPos - p), objects[i].Getn(p)), 0.0f) * objects[i].color;
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
	return n + vec3(x, y, z);
}