/*
Implementation of functions from header.
v2.2 Borubash
*/

#include "coldet.h"
#include <math.h>


bool ColDet::detector(Hitbox first, Hitbox second, float norm[4])
{
	switch (first.type)
	{
	case 1:
		switch (second.type)
		{
		case 2:
			return distance(first.hitbox.point, second.hitbox.sphere.Middle) <= second.hitbox.sphere.Radius;
			break;
		case 3:
			return false;
			break;
		case 4:
			return false;
			break;
		}
		break;
	case 2:
		switch (second.type)
		{
		case 1:
			return distance(first.hitbox.sphere.Middle, second.hitbox.point) <= first.hitbox.sphere.Radius;
			break;
		case 2:
			return distance(first.hitbox.sphere.Middle, second.hitbox.sphere.Middle) <= first.hitbox.sphere.Radius + second.hitbox.sphere.Radius;
			break;
		case 3:
		{bool collisions = false;
		bool if_collision = false;
		float polygons[12][3][3];
		triangles(second.hitbox.cuboid, polygons);
		//We check in loop coliding with every plane of cuboid
		for (int l = 0; l < 12; l++)
		{
			//Firstly, we need to check if the sphere collides with plane that our polygon lies on
			bool outsidePlane = false;
			bool outsideAllVerts = false;
			bool outsideAllEdges = false;
			bool fullyInsidePlane = false;
			outsidePlane = planeCollision(polygons[l], first.hitbox.sphere);
			if (outsidePlane) continue; //If we are not colliding with plane there is no point in checking collision with polygon
			//Now we can colide with vertex, edge or be completely inside plane
			//To check if we are completely inside a polygon we will project plane into 2D
			float a[3] = { polygons[l][1][0] - polygons[l][0][0] , polygons[l][1][1] - polygons[l][0][1] , polygons[l][1][2] - polygons[l][0][2] };
			float b[3] = { polygons[l][2][0] - polygons[l][1][0] , polygons[l][2][1] - polygons[l][1][1] , polygons[l][2][2] - polygons[l][1][2] };
			float c[3] = { polygons[l][0][0] - polygons[l][2][0] , polygons[l][0][1] - polygons[l][2][1] , polygons[l][0][2] - polygons[l][2][2] };
			//We only need one vector, but all of them can be useful later
			float alen = sqrt(pow(a[0], 2) + pow(a[1], 2) + pow(a[2], 2));
			float planeX[3] = { a[0] / alen, a[1] / alen, a[2] / alen }; //normalizing one of vectors to work as an axis
			float* N = normal(polygons[l][0], polygons[l][1], polygons[l][2]);
			float planeY[3];
			//Cross product between our new X axis and plane normal will create perpendicular vector that can work as an Y axis
			planeY[0] = N[1] * planeX[2] - N[2] * planeX[1];
			planeY[1] = N[2] * planeX[0] - N[0] * planeX[2];
			planeY[2] = N[0] * planeX[1] - N[1] * planeX[0];
			float len = sqrt(pow(planeY[0], 2) + pow(planeY[1], 2) + pow(planeY[2], 2));
			//Normalizing the vector
			planeY[0] /= len;
			planeY[1] /= len;
			planeY[2] /= len;
			//Projecting vertices and sphere center
			float sphere2D[2] = { first.hitbox.sphere.Middle.X * planeX[0] + first.hitbox.sphere.Middle.Y * planeX[1] + first.hitbox.sphere.Middle.Z * planeX[2], first.hitbox.sphere.Middle.X * planeY[0] + first.hitbox.sphere.Middle.Y * planeY[1] + first.hitbox.sphere.Middle.Z * planeY[2] };
			float triangle2D[3][2] = { {polygons[l][0][0] * planeX[0] + polygons[l][0][1] * planeX[1] + polygons[l][0][2] * planeX[2], polygons[l][0][0] * planeY[0] + polygons[l][0][1] * planeY[1] + polygons[l][0][2] * planeY[2]},
									   {polygons[l][1][0] * planeX[0] + polygons[l][1][1] * planeX[1] + polygons[l][1][2] * planeX[2], polygons[l][1][0] * planeY[0] + polygons[l][1][1] * planeY[1] + polygons[l][1][2] * planeY[2]},
									   {polygons[l][2][0] * planeX[0] + polygons[l][2][1] * planeX[1] + polygons[l][2][2] * planeX[2], polygons[l][2][0] * planeY[0] + polygons[l][2][1] * planeY[1] + polygons[l][2][2] * planeY[2]} };
			//Winding number algorithm used to detect if sphere is inside polygon http://geomalgorithms.com/a03-_inclusion.html
			int cn = 0;
			for (int j = 0; j < 3; j++)
			{
				int k = j + 1;
				if (k == 3) k = 0;
				if (((triangle2D[j][1] <= sphere2D[1]) && (triangle2D[k][1] > sphere2D[1])) ||
					((triangle2D[j][1] > sphere2D[1]) && (triangle2D[k][1] <= sphere2D[1])))
				{
					float vt = (sphere2D[1] - triangle2D[j][1]) / (triangle2D[k][1] - triangle2D[j][1]);
					if (sphere2D[0] < triangle2D[j][0] + vt * (triangle2D[k][0] - triangle2D[j][0])) ++cn;
				}
			}
			fullyInsidePlane = cn & 1;
			//The collision with vertex is just calculating distances between vertexes and sphere center and comparing it with radius
			bool outsideV1 = sqrt(pow(polygons[l][0][0] - first.hitbox.sphere.Middle.X, 2) + pow(polygons[l][0][1] - first.hitbox.sphere.Middle.Y, 2) + pow(polygons[l][0][2] - first.hitbox.sphere.Middle.Z, 2)) > first.hitbox.sphere.Radius;
			bool outsideV2 = sqrt(pow(polygons[l][1][0] - first.hitbox.sphere.Middle.X, 2) + pow(polygons[l][1][1] - first.hitbox.sphere.Middle.Y, 2) + pow(polygons[l][1][2] - first.hitbox.sphere.Middle.Z, 2)) > first.hitbox.sphere.Radius;
			bool outsideV3 = sqrt(pow(polygons[l][2][0] - first.hitbox.sphere.Middle.X, 2) + pow(polygons[l][2][1] - first.hitbox.sphere.Middle.Y, 2) + pow(polygons[l][2][2] - first.hitbox.sphere.Middle.Z, 2)) > first.hitbox.sphere.Radius;
			if (outsideV1 && outsideV2 && outsideV3) outsideAllVerts = true;

			//The last one thing is to check collision with edges
			//Here i will use function found here: https://gamedev.stackexchange.com/questions/96459/fast-ray-sphere-collision-code
			if (!intersectRaySegmentSphere(polygons[l][0], a, first.hitbox.sphere.Middle, pow(first.hitbox.sphere.Radius, 2)) &&
				!intersectRaySegmentSphere(polygons[l][1], b, first.hitbox.sphere.Middle, pow(first.hitbox.sphere.Radius, 2)) &&
				!intersectRaySegmentSphere(polygons[l][2], c, first.hitbox.sphere.Middle, pow(first.hitbox.sphere.Radius, 2)))
			{
				//sphere outside of all triangle edges
				outsideAllEdges = true;
			}
			if (outsideAllVerts && outsideAllEdges && !fullyInsidePlane)
			{
				collisions = false;
			}
			else collisions = true;
			if (collisions)
			{
				if_collision = true;
				if (norm != nullptr)
				{
					norm[0] += N[0];
					norm[1] += N[1];
					norm[2] += N[2];
					norm[3]++;
				}
			}
		}
		if (if_collision) return true;
		else return false;
		break;
		}
		break;
		case 4:
		{bool collisions = false;
		bool if_collision = false;
		float polygons[2][3][3];
		triangles(second.hitbox.plane, polygons);
		//We check in loop coliding with every plane of cuboid
		for (int l = 0; l < 2; l++)
		{
			//Firstly, we need to check if the sphere collides with plane that our polygon lies on
			bool outsidePlane = false;
			bool outsideAllVerts = false;
			bool outsideAllEdges = false;
			bool fullyInsidePlane = false;
			outsidePlane = planeCollision(polygons[l], first.hitbox.sphere);
			if (outsidePlane) continue; //If we are not colliding with plane there is no point in checking collision with polygon
			//Now we can colide with vertex, edge or be completely inside plane
			//To check if we are completely inside a polygon we will project plane into 2D
			float a[3] = { polygons[l][1][0] - polygons[l][0][0] , polygons[l][1][1] - polygons[l][0][1] , polygons[l][1][2] - polygons[l][0][2] };
			float b[3] = { polygons[l][2][0] - polygons[l][1][0] , polygons[l][2][1] - polygons[l][1][1] , polygons[l][2][2] - polygons[l][1][2] };
			float c[3] = { polygons[l][0][0] - polygons[l][2][0] , polygons[l][0][1] - polygons[l][2][1] , polygons[l][0][2] - polygons[l][2][2] };
			//We only need one vector, but all of them can be useful later
			float alen = sqrt(pow(a[0], 2) + pow(a[1], 2) + pow(a[2], 2));
			float planeX[3] = { a[0] / alen, a[1] / alen, a[2] / alen }; //normalizing one of vectors to work as an axis
			float* N = normal(polygons[l][0], polygons[l][1], polygons[l][2]);
			float planeY[3];
			//Cross product between our new X axis and plane normal will create perpendicular vector that can work as an Y axis
			planeY[0] = N[1] * planeX[2] - N[2] * planeX[1];
			planeY[1] = N[2] * planeX[0] - N[0] * planeX[2];
			planeY[2] = N[0] * planeX[1] - N[1] * planeX[0];
			float len = sqrt(pow(planeY[0], 2) + pow(planeY[1], 2) + pow(planeY[2], 2));
			//Normalizing the vector
			planeY[0] /= len;
			planeY[1] /= len;
			planeY[2] /= len;
			//Projecting vertices and sphere center
			float sphere2D[2] = { first.hitbox.sphere.Middle.X * planeX[0] + first.hitbox.sphere.Middle.Y * planeX[1] + first.hitbox.sphere.Middle.Z * planeX[2], first.hitbox.sphere.Middle.X * planeY[0] + first.hitbox.sphere.Middle.Y * planeY[1] + first.hitbox.sphere.Middle.Z * planeY[2] };
			float triangle2D[3][2] = { {polygons[l][0][0] * planeX[0] + polygons[l][0][1] * planeX[1] + polygons[l][0][2] * planeX[2], polygons[l][0][0] * planeY[0] + polygons[l][0][1] * planeY[1] + polygons[l][0][2] * planeY[2]},
									   {polygons[l][1][0] * planeX[0] + polygons[l][1][1] * planeX[1] + polygons[l][1][2] * planeX[2], polygons[l][1][0] * planeY[0] + polygons[l][1][1] * planeY[1] + polygons[l][1][2] * planeY[2]},
									   {polygons[l][2][0] * planeX[0] + polygons[l][2][1] * planeX[1] + polygons[l][2][2] * planeX[2], polygons[l][2][0] * planeY[0] + polygons[l][2][1] * planeY[1] + polygons[l][2][2] * planeY[2]} };
			//Winding number algorithm used to detect if sphere is inside polygon http://geomalgorithms.com/a03-_inclusion.html
			int cn = 0;
			for (int j = 0; j < 3; j++)
			{
				int k = j + 1;
				if (k == 3) k = 0;
				if (((triangle2D[j][1] <= sphere2D[1]) && (triangle2D[k][1] > sphere2D[1])) ||
					((triangle2D[j][1] > sphere2D[1]) && (triangle2D[k][1] <= sphere2D[1])))
				{
					float vt = (sphere2D[1] - triangle2D[j][1]) / (triangle2D[k][1] - triangle2D[j][1]);
					if (sphere2D[0] < triangle2D[j][0] + vt * (triangle2D[k][0] - triangle2D[j][0])) ++cn;
				}
			}
			fullyInsidePlane = cn & 1;
			//The collision with vertex is just calculating distances between vertexes and sphere center and comparing it with radius
			bool outsideV1 = sqrt(pow(polygons[l][0][0] - first.hitbox.sphere.Middle.X, 2) + pow(polygons[l][0][1] - first.hitbox.sphere.Middle.Y, 2) + pow(polygons[l][0][2] - first.hitbox.sphere.Middle.Z, 2)) > first.hitbox.sphere.Radius;
			bool outsideV2 = sqrt(pow(polygons[l][1][0] - first.hitbox.sphere.Middle.X, 2) + pow(polygons[l][1][1] - first.hitbox.sphere.Middle.Y, 2) + pow(polygons[l][1][2] - first.hitbox.sphere.Middle.Z, 2)) > first.hitbox.sphere.Radius;
			bool outsideV3 = sqrt(pow(polygons[l][2][0] - first.hitbox.sphere.Middle.X, 2) + pow(polygons[l][2][1] - first.hitbox.sphere.Middle.Y, 2) + pow(polygons[l][2][2] - first.hitbox.sphere.Middle.Z, 2)) > first.hitbox.sphere.Radius;
			if (outsideV1 && outsideV2 && outsideV3) outsideAllVerts = true;

			//The last one thing is to check collision with edges
			//Here i will use function found here: https://gamedev.stackexchange.com/questions/96459/fast-ray-sphere-collision-code
			if (!intersectRaySegmentSphere(polygons[l][0], a, first.hitbox.sphere.Middle, pow(first.hitbox.sphere.Radius, 2)) &&
				!intersectRaySegmentSphere(polygons[l][1], b, first.hitbox.sphere.Middle, pow(first.hitbox.sphere.Radius, 2)) &&
				!intersectRaySegmentSphere(polygons[l][2], c, first.hitbox.sphere.Middle, pow(first.hitbox.sphere.Radius, 2)))
			{
				//sphere outside of all triangle edges
				outsideAllEdges = true;
			}
			if (outsideAllVerts && outsideAllEdges && !fullyInsidePlane)
			{
				collisions = false;
			}
			else collisions = true;
			if (collisions)
			{
				if_collision = true;
				if (norm != nullptr)
				{
					norm[0] += N[0];
					norm[1] += N[1];
					norm[2] += N[2];
					norm[3]++;
				}
			}
		}
		if (if_collision) return true;
		else return false;
		break;
		}
		break;
		}
		break;
	case 3:
		switch (second.type)
		{
		case 1:
			return false;
			break;
		case 2:
		{bool collisions = false;
		bool if_collision = false;
		float polygons[12][3][3];
		triangles(first.hitbox.cuboid, polygons);
		//We check in loop coliding with every plane of cuboid
		for (int l = 0; l < 12; l++)
		{
			//Firstly, we need to check if the sphere collides with plane that our polygon lies on
			bool outsidePlane = false;
			bool outsideAllVerts = false;
			bool outsideAllEdges = false;
			bool fullyInsidePlane = false;
			outsidePlane = planeCollision(polygons[l], second.hitbox.sphere);
			if (outsidePlane) continue; //If we are not colliding with plane there is no point in checking collision with polygon
			//Now we can colide with vertex, edge or be completely inside plane
			//To check if we are completely inside a polygon we will project plane into 2D
			float a[3] = { polygons[l][1][0] - polygons[l][0][0] , polygons[l][1][1] - polygons[l][0][1] , polygons[l][1][2] - polygons[l][0][2] };
			float b[3] = { polygons[l][2][0] - polygons[l][1][0] , polygons[l][2][1] - polygons[l][1][1] , polygons[l][2][2] - polygons[l][1][2] };
			float c[3] = { polygons[l][0][0] - polygons[l][2][0] , polygons[l][0][1] - polygons[l][2][1] , polygons[l][0][2] - polygons[l][2][2] };
			//We only need one vector, but all of them can be useful later
			float alen = sqrt(pow(a[0], 2) + pow(a[1], 2) + pow(a[2], 2));
			float planeX[3] = { a[0] / alen, a[1] / alen, a[2] / alen }; //normalizing one of vectors to work as an axis
			float* N = normal(polygons[l][0], polygons[l][1], polygons[l][2]);
			float planeY[3];
			//Cross product between our new X axis and plane normal will create perpendicular vector that can work as an Y axis
			planeY[0] = N[1] * planeX[2] - N[2] * planeX[1];
			planeY[1] = N[2] * planeX[0] - N[0] * planeX[2];
			planeY[2] = N[0] * planeX[1] - N[1] * planeX[0];
			float len = sqrt(pow(planeY[0], 2) + pow(planeY[1], 2) + pow(planeY[2], 2));
			//Normalizing the vector
			planeY[0] /= len;
			planeY[1] /= len;
			planeY[2] /= len;
			//Projecting vertices and sphere center
			float sphere2D[2] = { second.hitbox.sphere.Middle.X * planeX[0] + second.hitbox.sphere.Middle.Y * planeX[1] + second.hitbox.sphere.Middle.Z * planeX[2], second.hitbox.sphere.Middle.X * planeY[0] + second.hitbox.sphere.Middle.Y * planeY[1] + second.hitbox.sphere.Middle.Z * planeY[2] };
			float triangle2D[3][2] = { {polygons[l][0][0] * planeX[0] + polygons[l][0][1] * planeX[1] + polygons[l][0][2] * planeX[2], polygons[l][0][0] * planeY[0] + polygons[l][0][1] * planeY[1] + polygons[l][0][2] * planeY[2]},
									   {polygons[l][1][0] * planeX[0] + polygons[l][1][1] * planeX[1] + polygons[l][1][2] * planeX[2], polygons[l][1][0] * planeY[0] + polygons[l][1][1] * planeY[1] + polygons[l][1][2] * planeY[2]},
									   {polygons[l][2][0] * planeX[0] + polygons[l][2][1] * planeX[1] + polygons[l][2][2] * planeX[2], polygons[l][2][0] * planeY[0] + polygons[l][2][1] * planeY[1] + polygons[l][2][2] * planeY[2]} };
			//Winding number algorithm used to detect if sphere is inside polygon http://geomalgorithms.com/a03-_inclusion.html
			int cn = 0;
			for (int j = 0; j < 3; j++)
			{
				int k = j + 1;
				if (k == 3) k = 0;
				if (((triangle2D[j][1] <= sphere2D[1]) && (triangle2D[k][1] > sphere2D[1])) ||
					((triangle2D[j][1] > sphere2D[1]) && (triangle2D[k][1] <= sphere2D[1])))
				{
					float vt = (sphere2D[1] - triangle2D[j][1]) / (triangle2D[k][1] - triangle2D[j][1]);
					if (sphere2D[0] < triangle2D[j][0] + vt * (triangle2D[k][0] - triangle2D[j][0])) ++cn;
				}
			}
			fullyInsidePlane = cn & 1;
			//The collision with vertex is just calculating distances between vertexes and sphere center and comparing it with radius
			bool outsideV1 = sqrt(pow(polygons[l][0][0] - second.hitbox.sphere.Middle.X, 2) + pow(polygons[l][0][1] - second.hitbox.sphere.Middle.Y, 2) + pow(polygons[l][0][2] - second.hitbox.sphere.Middle.Z, 2)) > second.hitbox.sphere.Radius;
			bool outsideV2 = sqrt(pow(polygons[l][1][0] - second.hitbox.sphere.Middle.X, 2) + pow(polygons[l][1][1] - second.hitbox.sphere.Middle.Y, 2) + pow(polygons[l][1][2] - second.hitbox.sphere.Middle.Z, 2)) > second.hitbox.sphere.Radius;
			bool outsideV3 = sqrt(pow(polygons[l][2][0] - second.hitbox.sphere.Middle.X, 2) + pow(polygons[l][2][1] - second.hitbox.sphere.Middle.Y, 2) + pow(polygons[l][2][2] - second.hitbox.sphere.Middle.Z, 2)) > second.hitbox.sphere.Radius;
			if (outsideV1 && outsideV2 && outsideV3) outsideAllVerts = true;

			//The last one thing is to check collision with edges
			//Here i will use function found here: https://gamedev.stackexchange.com/questions/96459/fast-ray-sphere-collision-code
			if (!intersectRaySegmentSphere(polygons[l][0], a, second.hitbox.sphere.Middle, pow(second.hitbox.sphere.Radius, 2)) &&
				!intersectRaySegmentSphere(polygons[l][1], b, second.hitbox.sphere.Middle, pow(second.hitbox.sphere.Radius, 2)) &&
				!intersectRaySegmentSphere(polygons[l][2], c, second.hitbox.sphere.Middle, pow(second.hitbox.sphere.Radius, 2)))
			{
				//sphere outside of all triangle edges
				outsideAllEdges = true;
			}
			if (outsideAllVerts && outsideAllEdges && !fullyInsidePlane)
			{
				collisions = false;
			}
			else collisions = true;
			if (collisions)
			{
				if_collision = true;
				if (norm != nullptr)
				{
					norm[0] += N[0];
					norm[1] += N[1];
					norm[2] += N[2];
					norm[3]++;
				}
			}
		}
		if (if_collision) return true;
		else return false;
		break;
		}
		break;
		case 4:
			return false;
		break;
		}
		break;
	case 4:
		switch (second.type)
		{
		case 1:
			return false;
			break;
		case 2:
			{bool collisions = false;
			bool if_collision = false;
			float polygons[2][3][3];
			triangles(first.hitbox.plane, polygons);
			//We check in loop coliding with every plane of cuboid
			for (int l = 0; l < 2; l++)
			{
				//Firstly, we need to check if the sphere collides with plane that our polygon lies on
				bool outsidePlane = false;
				bool outsideAllVerts = false;
				bool outsideAllEdges = false;
				bool fullyInsidePlane = false;
				outsidePlane = planeCollision(polygons[l], second.hitbox.sphere);
				if (outsidePlane) continue; //If we are not colliding with plane there is no point in checking collision with polygon
				//Now we can colide with vertex, edge or be completely inside plane
				//To check if we are completely inside a polygon we will project plane into 2D
				float a[3] = { polygons[l][1][0] - polygons[l][0][0] , polygons[l][1][1] - polygons[l][0][1] , polygons[l][1][2] - polygons[l][0][2] };
				float b[3] = { polygons[l][2][0] - polygons[l][1][0] , polygons[l][2][1] - polygons[l][1][1] , polygons[l][2][2] - polygons[l][1][2] };
				float c[3] = { polygons[l][0][0] - polygons[l][2][0] , polygons[l][0][1] - polygons[l][2][1] , polygons[l][0][2] - polygons[l][2][2] };
				//We only need one vector, but all of them can be useful later
				float alen = sqrt(pow(a[0], 2) + pow(a[1], 2) + pow(a[2], 2));
				float planeX[3] = { a[0] / alen, a[1] / alen, a[2] / alen }; //normalizing one of vectors to work as an axis
				float* N = normal(polygons[l][0], polygons[l][1], polygons[l][2]);
				float planeY[3];
				//Cross product between our new X axis and plane normal will create perpendicular vector that can work as an Y axis
				planeY[0] = N[1] * planeX[2] - N[2] * planeX[1];
				planeY[1] = N[2] * planeX[0] - N[0] * planeX[2];
				planeY[2] = N[0] * planeX[1] - N[1] * planeX[0];
				float len = sqrt(pow(planeY[0], 2) + pow(planeY[1], 2) + pow(planeY[2], 2));
				//Normalizing the vector
				planeY[0] /= len;
				planeY[1] /= len;
				planeY[2] /= len;
				//Projecting vertices and sphere center
				float sphere2D[2] = { second.hitbox.sphere.Middle.X * planeX[0] + second.hitbox.sphere.Middle.Y * planeX[1] + second.hitbox.sphere.Middle.Z * planeX[2], second.hitbox.sphere.Middle.X * planeY[0] + second.hitbox.sphere.Middle.Y * planeY[1] + second.hitbox.sphere.Middle.Z * planeY[2] };
				float triangle2D[3][2] = { {polygons[l][0][0] * planeX[0] + polygons[l][0][1] * planeX[1] + polygons[l][0][2] * planeX[2], polygons[l][0][0] * planeY[0] + polygons[l][0][1] * planeY[1] + polygons[l][0][2] * planeY[2]},
										   {polygons[l][1][0] * planeX[0] + polygons[l][1][1] * planeX[1] + polygons[l][1][2] * planeX[2], polygons[l][1][0] * planeY[0] + polygons[l][1][1] * planeY[1] + polygons[l][1][2] * planeY[2]},
										   {polygons[l][2][0] * planeX[0] + polygons[l][2][1] * planeX[1] + polygons[l][2][2] * planeX[2], polygons[l][2][0] * planeY[0] + polygons[l][2][1] * planeY[1] + polygons[l][2][2] * planeY[2]} };
				//Winding number algorithm used to detect if sphere is inside polygon http://geomalgorithms.com/a03-_inclusion.html
				int cn = 0;
				for (int j = 0; j < 3; j++)
				{
					int k = j + 1;
					if (k == 3) k = 0;
					if (((triangle2D[j][1] <= sphere2D[1]) && (triangle2D[k][1] > sphere2D[1])) ||
						((triangle2D[j][1] > sphere2D[1]) && (triangle2D[k][1] <= sphere2D[1])))
					{
						float vt = (sphere2D[1] - triangle2D[j][1]) / (triangle2D[k][1] - triangle2D[j][1]);
						if (sphere2D[0] < triangle2D[j][0] + vt * (triangle2D[k][0] - triangle2D[j][0])) ++cn;
					}
				}
				fullyInsidePlane = cn & 1;
				//The collision with vertex is just calculating distances between vertexes and sphere center and comparing it with radius
				bool outsideV1 = sqrt(pow(polygons[l][0][0] - second.hitbox.sphere.Middle.X, 2) + pow(polygons[l][0][1] - second.hitbox.sphere.Middle.Y, 2) + pow(polygons[l][0][2] - second.hitbox.sphere.Middle.Z, 2)) > second.hitbox.sphere.Radius;
				bool outsideV2 = sqrt(pow(polygons[l][1][0] - second.hitbox.sphere.Middle.X, 2) + pow(polygons[l][1][1] - second.hitbox.sphere.Middle.Y, 2) + pow(polygons[l][1][2] - second.hitbox.sphere.Middle.Z, 2)) > second.hitbox.sphere.Radius;
				bool outsideV3 = sqrt(pow(polygons[l][2][0] - second.hitbox.sphere.Middle.X, 2) + pow(polygons[l][2][1] - second.hitbox.sphere.Middle.Y, 2) + pow(polygons[l][2][2] - second.hitbox.sphere.Middle.Z, 2)) > second.hitbox.sphere.Radius;
				if (outsideV1 && outsideV2 && outsideV3) outsideAllVerts = true;

				//The last one thing is to check collision with edges
				//Here i will use function found here: https://gamedev.stackexchange.com/questions/96459/fast-ray-sphere-collision-code
				if (!intersectRaySegmentSphere(polygons[l][0], a, second.hitbox.sphere.Middle, pow(second.hitbox.sphere.Radius, 2)) &&
					!intersectRaySegmentSphere(polygons[l][1], b, second.hitbox.sphere.Middle, pow(second.hitbox.sphere.Radius, 2)) &&
					!intersectRaySegmentSphere(polygons[l][2], c, second.hitbox.sphere.Middle, pow(second.hitbox.sphere.Radius, 2)))
				{
					//sphere outside of all triangle edges
					outsideAllEdges = true;
				}
				if (outsideAllVerts && outsideAllEdges && !fullyInsidePlane)
				{
					collisions = false;
				}
				else collisions = true;
				if (collisions)
				{
					if_collision = true;
					if (norm != nullptr)
					{
						norm[0] += N[0];
						norm[1] += N[1];
						norm[2] += N[2];
						norm[3]++;
					}
				}
			}
			if (if_collision) return true;
			else return false;
			break;
			}
			break;
		case 3:
			return false;
			break;
		case 4:
			return false;
			break;
		}
		break;
	}
}

void ColDet::triangles(Plane model, float triangles[2][3][3])
{
	float vertices[4][3];
	if (model.Z_shift == 0.f)
	{
		for (int i = 0; i < 4; i++)
		{
			vertices[i][0] = model.one_vertex.X + (i % 2) * model.X_shift;
			vertices[i][1] = model.one_vertex.Y + ((i / 2) % 2) * model.Y_shift;
			vertices[i][2] = model.one_vertex.Z;
		}
	}
	else if (model.X_shift == 0.f)
	{
		for (int i = 0; i < 4; i++)
		{
			vertices[i][0] = model.one_vertex.X;
			vertices[i][1] = model.one_vertex.Y + ((i / 2) % 2) * model.Y_shift;
			vertices[i][2] = model.one_vertex.Z + (i % 2) * model.Z_shift;
		}
	}
	else if (model.Y_shift == 0.f)
	{
		for (int i = 0; i < 4; i++)
		{
			vertices[i][0] = model.one_vertex.X + (i % 2) * model.X_shift;
			vertices[i][1] = model.one_vertex.Y;
			vertices[i][2] = model.one_vertex.Z + ((i / 2) % 2) * model.Z_shift;
		}
	}
	
	
	triangles[0][0][0] = vertices[0][0];
	triangles[0][0][1] = vertices[0][1];
	triangles[0][0][2] = vertices[0][2];
	
	triangles[0][1][0] = vertices[1][0];
	triangles[0][1][1] = vertices[1][1];
	triangles[0][1][2] = vertices[1][2];

	triangles[0][2][0] = vertices[2][0];
	triangles[0][2][1] = vertices[2][1];
	triangles[0][2][2] = vertices[2][2];

	triangles[1][0][0] = vertices[3][0];
	triangles[1][0][1] = vertices[3][1];
	triangles[1][0][2] = vertices[3][2];

	triangles[1][1][0] = vertices[2][0];
	triangles[1][1][1] = vertices[2][1];
	triangles[1][1][2] = vertices[2][2];

	triangles[1][2][0] = vertices[1][0];
	triangles[1][2][1] = vertices[1][1];
	triangles[1][2][2] = vertices[1][2];

}

void ColDet::triangles(Cuboid model, float triangles[12][3][3])
{
	float vertices[8][3];
	for (int i = 0; i < 8; i++)
	{
		vertices[i][0] = model.one_vertex.X + (i % 2) * model.X_shift;
		vertices[i][1] = model.one_vertex.Y + ((i / 2) % 2) * model.Y_shift;
		vertices[i][2] = model.one_vertex.Z + (i / 4) * model.Z_shift;
	}
	//X min
	triangles[0][0][0] = vertices[0][0];
	triangles[0][0][1] = vertices[0][1];
	triangles[0][0][2] = vertices[0][2];
	triangles[0][1][0] = vertices[4][0];
	triangles[0][1][1] = vertices[4][1];
	triangles[0][1][2] = vertices[4][2];
	triangles[0][2][0] = vertices[6][0];
	triangles[0][2][1] = vertices[6][1];
	triangles[0][2][2] = vertices[6][2];
	
	triangles[1][0][0] = vertices[0][0];
	triangles[1][0][1] = vertices[0][1];
	triangles[1][0][2] = vertices[0][2];
	triangles[1][1][0] = vertices[4][0];
	triangles[1][1][1] = vertices[4][1];
	triangles[1][1][2] = vertices[4][2];
	triangles[1][2][0] = vertices[2][0];
	triangles[1][2][1] = vertices[2][1];
	triangles[1][2][2] = vertices[2][2];

	//X max
	triangles[2][0][0] = vertices[5][0];
	triangles[2][0][1] = vertices[5][1];
	triangles[2][0][2] = vertices[5][2];
	triangles[2][1][0] = vertices[1][0];
	triangles[2][1][1] = vertices[1][1];
	triangles[2][1][2] = vertices[1][2];
	triangles[2][2][0] = vertices[7][0];
	triangles[2][2][1] = vertices[7][1];
	triangles[2][2][2] = vertices[7][2];

	triangles[3][0][0] = vertices[5][0];
	triangles[3][0][1] = vertices[5][1];
	triangles[3][0][2] = vertices[5][2];
	triangles[3][1][0] = vertices[1][0];
	triangles[3][1][1] = vertices[1][1];
	triangles[3][1][2] = vertices[1][2];
	triangles[3][2][0] = vertices[3][0];
	triangles[3][2][1] = vertices[3][1];
	triangles[3][2][2] = vertices[3][2];

	//Y min
	triangles[4][0][0] = vertices[0][0];
	triangles[4][0][1] = vertices[0][1];
	triangles[4][0][2] = vertices[0][2];
	triangles[4][1][0] = vertices[1][0];
	triangles[4][1][1] = vertices[1][1];
	triangles[4][1][2] = vertices[1][2];
	triangles[4][2][0] = vertices[5][0];
	triangles[4][2][1] = vertices[5][1];
	triangles[4][2][2] = vertices[5][2];

	triangles[5][0][0] = vertices[0][0];
	triangles[5][0][1] = vertices[0][1];
	triangles[5][0][2] = vertices[0][2];
	triangles[5][1][0] = vertices[1][0];
	triangles[5][1][1] = vertices[1][1];
	triangles[5][1][2] = vertices[1][2];
	triangles[5][2][0] = vertices[4][0];
	triangles[5][2][1] = vertices[4][1];
	triangles[5][2][2] = vertices[4][2];

	//Y max
	triangles[6][0][0] = vertices[6][0];
	triangles[6][0][1] = vertices[6][1];
	triangles[6][0][2] = vertices[6][2];
	triangles[6][1][0] = vertices[7][0];
	triangles[6][1][1] = vertices[7][1];
	triangles[6][1][2] = vertices[7][2];
	triangles[6][2][0] = vertices[3][0];
	triangles[6][2][1] = vertices[3][1];
	triangles[6][2][2] = vertices[3][2];

	triangles[7][0][0] = vertices[6][0];
	triangles[7][0][1] = vertices[6][1];
	triangles[7][0][2] = vertices[6][2];
	triangles[7][1][0] = vertices[7][0];
	triangles[7][1][1] = vertices[7][1];
	triangles[7][1][2] = vertices[7][2];
	triangles[7][2][0] = vertices[2][0];
	triangles[7][2][1] = vertices[2][1];
	triangles[7][2][2] = vertices[2][2];

	//Z min
	triangles[8][0][0] = vertices[1][0];
	triangles[8][0][1] = vertices[1][1];
	triangles[8][0][2] = vertices[1][2];
	triangles[8][1][0] = vertices[0][0];
	triangles[8][1][1] = vertices[0][1];
	triangles[8][1][2] = vertices[0][2];
	triangles[8][2][0] = vertices[3][0];
	triangles[8][2][1] = vertices[3][1];
	triangles[8][2][2] = vertices[3][2];

	triangles[9][0][0] = vertices[1][0];
	triangles[9][0][1] = vertices[1][1];
	triangles[9][0][2] = vertices[1][2];
	triangles[9][1][0] = vertices[0][0];
	triangles[9][1][1] = vertices[0][1];
	triangles[9][1][2] = vertices[0][2];
	triangles[9][2][0] = vertices[2][0];
	triangles[9][2][1] = vertices[2][1];
	triangles[9][2][2] = vertices[2][2];

	//Z max
	triangles[10][0][0] = vertices[4][0];
	triangles[10][0][1] = vertices[4][1];
	triangles[10][0][2] = vertices[4][2];
	triangles[10][1][0] = vertices[5][0];
	triangles[10][1][1] = vertices[5][1];
	triangles[10][1][2] = vertices[5][2];
	triangles[10][2][0] = vertices[7][0];
	triangles[10][2][1] = vertices[7][1];
	triangles[10][2][2] = vertices[7][2];

	triangles[11][0][0] = vertices[4][0];
	triangles[11][0][1] = vertices[4][1];
	triangles[11][0][2] = vertices[4][2];
	triangles[11][1][0] = vertices[5][0];
	triangles[11][1][1] = vertices[5][1];
	triangles[11][1][2] = vertices[5][2];
	triangles[11][2][0] = vertices[6][0];
	triangles[11][2][1] = vertices[6][1];
	triangles[11][2][2] = vertices[6][2];
}

float* ColDet::normal(float* A, float* B, float* C)
{
	float AB[3];
	float AC[3];
	float normal[3];
	AB[0] = B[0] - A[0];
	AB[1] = B[1] - A[1];
	AB[2] = B[2] - A[2];

	AC[0] = C[0] - A[0];
	AC[1] = C[1] - A[1];
	AC[2] = C[2] - A[2];

	normal[0] = AB[1] * AC[2] - AB[2] * AC[1];
	normal[1] = AB[2] * AC[0] - AB[0] * AC[2];
	normal[2] = AB[0] * AC[1] - AB[1] * AC[0];
	float len = sqrt(pow(normal[0], 2) + pow(normal[1], 2) + pow(normal[2], 2));
	normal[0] /= len;
	normal[1] /= len;
	normal[2] /= len;
	return normal;
}

float ColDet::distance(Point A, Point B)
{
	float X = pow(A.X - B.X, 2);
	float Y = pow(A.Y - B.Y, 2);
	float Z = pow(A.Z - B.Z, 2);
	return sqrt(X + Y + Z);
}

bool ColDet::planeCollision(float triangles[3][3], Sphere sphere)
{
	//Ax + By + Cz + D = 0 - plane equation, D is equal to -P * N (dot product) where P is the average of vertices (center of gravity) and A B C are just normal vector coordinates
	float* N = normal(triangles[0], triangles[1], triangles[2]);
	float P[3] = { triangles[0][0] + triangles[1][0] + triangles[2][0], triangles[0][1] + triangles[1][1] + triangles[2][1], triangles[0][2] + triangles[1][2] + triangles[2][2] };
	float D = -P[0] * N[0] + -P[1] * N[1] + -P[2] * N[2];
	//Using equation we can calculate the distance between sphere origin and plane
	float Dist = sphere.Middle.X * N[0] + sphere.Middle.Y * N[1] + sphere.Middle.Z * N[2] + D;
	if (abs(Dist) > sphere.Radius) return true;
	else return false;
}

bool ColDet::intersectRaySegmentSphere(float o[3], float d[3], Point so, float radius2)
{
	//we pass in d non-normalized to keep it's length
	//then we use that length later to compare the intersection point to make sure
	//we're within the actual ray segment
	float l = sqrt(pow(d[0], 2) + pow(d[1], 2) + pow(d[2], 2));
	d[0] /= l;
	d[1] /= l;
	d[2] /= l;

	float m[3] = { o[0] - so.X, o[1] - so.Y, o[2] - so.Z };
	float b = m[0] * d[0] + m[1] * d[1] + m[2] * d[2]; //m.dot(d);
	float c = m[0] * m[0] + m[1] * m[1] + m[2] * m[2] - radius2; //m.dot(m) - radius2;

	// Exit if r’s origin outside s (c > 0) and r pointing away from s (b > 0)
	if (c > 0.0f && b > 0.0f)
		return false;
	float discr = b * b - c;

	// A negative discriminant corresponds to ray missing sphere
	if (discr < 0.0f)
		return false;

	// Ray now found to intersect sphere, compute smallest t value of intersection
	float t = -b - sqrtf(discr);

	// If t is negative, ray started inside sphere so clamp t to zero
	if (t < 0.0f)
		t = 0.0f;
	//ip = o + (d * t);

	//here's that last segment check I was talking about
	if (t > l)
		return false;

	return true;
}