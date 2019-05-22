#include <svpng/svpng.inc>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <C:\Users\PureSurface\Documents\Program\ÀëÏßäÖÈ¾\ÀëÏßäÖÈ¾\plane.h>
void test_rgb();
const int planeSize = 5;
Plane planeRight = Plane(glm::vec3(15, 15, 0), glm::vec3(-1, 0, 0), glm::vec3(0, 0, 1), 30.0f, 20.0f, glm::vec3(17, 194, 238));
Plane planeLeft = Plane(glm::vec3(-15, 15, 0), glm::vec3(1, 0, 0), glm::vec3(0, 0, 1), 30.0f, 20.0f, glm::vec3(247, 9, 104));
Plane planeTop = Plane(glm::vec3(0, 15, 10), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0), 30.0f, 30.0f, glm::vec3(128, 128, 128));
Plane planeButtom = Plane(glm::vec3(0, 15, -10), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0), 30.0f, 30.0f, glm::vec3(128, 128, 128));
Plane planeForward = Plane(glm::vec3(0, 30, 0), glm::vec3(0, -1, 0), glm::vec3(0, 0, 1), 30.0f, 20.0f, glm::vec3(128, 128, 128));
Plane *planes;
int main(void) {
	planes = (Plane*)malloc(sizeof(Plane) * planeSize);
	*planes = planeRight;
	*(planes + 1) = planeLeft;
	*(planes + 2) = planeTop;
	*(planes + 3) = planeButtom;
	*(planes + 4) = planeForward;
	test_rgb();
	return 0;
}
void test_rgb(void) {
	const int X = 800;
	const int Z = 600;
	unsigned char *rgb = (unsigned char *)malloc(sizeof(unsigned char)*X * Z * 3);
	unsigned char *p = rgb;
	unsigned x, z;
	bool hit;
	FILE *fp = fopen("rgb.png", "wb");
	for (z = 0; z < Z; z++)
		for (x = 0; x < X; x++) {
			glm::vec3 p0 = glm::vec3(0.0f);
			glm::vec3 u = glm::vec3(4.0f - ((float)x / X)*8.0f, 6.0f / sqrt(2), 3.0f - ((float)z / Z)*6.0f);
			hit = false;
			for (int i = 0; i < planeSize; i++) {
				if ((*(planes + i)).IsIntersect(p0, u)) {
					*p++ = (*(planes + i)).color.r;    /* R */
					*p++ = (*(planes + i)).color.g;    /* G */
					*p++ = (*(planes + i)).color.b;    /* B */
					hit = true;
					break;
				}
			}
			if (!hit) {
				*p++ = 0;    /* R */
				*p++ = 0;    /* G */
				*p++ = 0;    /* B */
			}
		}
	svpng(fp, X, Z, rgb, 0);
	fclose(fp);
	free(rgb);
}
glm::vec3 reflect(glm::vec3 p0, glm::vec3 u,int number) {
	for (int i = 0; i < planeSize; i++) {
		if ((*(planes + i)).IsIntersect(p0, u)) {
			
		}
	}
}