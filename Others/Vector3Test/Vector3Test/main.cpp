#include <stdio.h>
#include "vector.h"

int main()
{
	float incX = 1;
	float incY = 5;
	float incZ = 6;

	for(int i=0; i < 10; ++i)
	{
        printf("== %d ==\n", i);

		Vector3 v1(incX, 2, 3);
		Vector3 v2(4, incY, incZ);

        Vector3 add = v1 + v2;
        printf("v1 + v2(%f, %f, %f)\n", add.x, add.y, add.z);
        Vector3 sub = v1 - v2;
        printf("v1 - v2(%f, %f, %f)\n", sub.x, sub.y, sub.z);

        Vector3 scale = v1 * i;
        printf("v1 * i(%f, %f, %f)\n", scale.x, scale.y, scale.z);

        printf("V1 Length:%f\n", v1.Length());
        printf("V2 Length:%f\n", v2.Length());

        Vector3 n = v2.Normalize();
        printf("V2 Normalize:(%f, %f, %f)\n", n.x, n.y, n.z);

		float dot = v1 * v2;
		Vector3 cross = v1 % v2;
		printf("v1 dot v2:%f\n", dot);
		printf("V1 cross v2:(%f, %f, %f)\n", cross.x, cross.y, cross.z);
		
		Vector3 dist = v2 - v1;
		printf("point V1 to point V2 Dist:(%f)\n", dist.Length());

        incX += 1;
        incY += 2;
        incZ += 3;
	}

	return 0;
}
