#include <vector>
#include "object.h"

void test_rgb();
vec3 ObjectReflect(vec3 p0, vec3 u,bool inSphere);
vec3 lightPos = vec3(0, 18, 0);
vector<Object> objects;

int main(void) {
	Object planeRight = Object(vec3(15, 15, 0), vec3(-1, 0, 0), vec3(0, 0, 1), 30.0f, 20.0f, vec3(17, 194, 238));
	Object planeLeft = Object(vec3(-15, 15, 0), vec3(1, 0, 0), vec3(0, 0, 1), 30.0f, 20.0f, vec3(187, 109, 116));
	Object planeTop = Object(vec3(0, 15, 10), vec3(0, 0, -1), vec3(0, 1, 0), 30.0f, 30.0f, vec3(128, 128, 128));
	Object planeButtom = Object(vec3(0, 15, -10), vec3(0, 0, 1), vec3(0, 1, 0), 30.0f, 30.0f, vec3(128, 128, 128));
	Object planeForward = Object(vec3(0, 30, 0), vec3(0, -1, 0), vec3(0, 0, 1), 30.0f, 20.0f, vec3(128, 128, 128));
	Object sphere1 = Object(vec3(-7, 20, -6), 4);
	objects.push_back(sphere1);
	objects.push_back(planeTop);
	objects.push_back(planeButtom);
	objects.push_back(planeLeft);
	objects.push_back(planeRight);
	objects.push_back(planeForward);
	test_rgb();
	return 0;
}
void test_rgb(void) {
	const int X = 1920;
	const int Z = 1080;
	unsigned char *rgb = (unsigned char *)malloc(sizeof(unsigned char)*X * Z * 3);
	unsigned char *p = rgb;
	unsigned x, z;
	FILE *fp = fopen("rgb.png", "wb");
	for (z = 0; z < Z; z++)
		for (x = 0; x < X; x++) {
			vec3 p0 = vec3(0.0f);
			vec3 u = vec3(-8.0f + ((float)x / X)*16.0f, 9.0f / sqrt(2), 4.5f - ((float)z / Z)*9.0f);
			vec3 color = ObjectReflect(p0, u ,0);
			*p++ = color.r;    /* R */
			*p++ = color.g;    /* G */
			*p++ = color.b;    /* B */
		}
	svpng(fp, X, Z, rgb, 0);
	fclose(fp);
	free(rgb);
}
vec3 ObjectReflect(vec3 p0, vec3 u ,bool inSphere) {
	vec3 p;
	for (int i = 0; i < objects.size(); i++) {
		if (objects[i].objectType == sphere && inSphere)continue;
		if ((p = objects[i].IsIntersect(p0, u)) != vec3(0.0f)) {
			if (objects[i].objectType == sphere) {
				return ObjectReflect(p,reflect(u,objects[i].Getn(p)),true);
			}
			return (max(dot(normalize(lightPos - p), objects[i].Getn(p)), 0.0f)) * objects[i].color;
		}
	}
	return vec3(0.0f);
}