#pragma once

#include "wyrdpch.h"
#include "core/Log.h"
#include "core/maths/Ray.h"
#include "core/maths/BoundingBox.h"

#include <glm/glm.hpp>

namespace Wyrd
{
	class PhysicsUtils
	{
	public:
		inline static void ScreenPosToWorldRay(const glm::vec2& mousePos, const glm::vec2& screenSize, const glm::mat4& viewMatrix, const glm::mat4& projection, Ray& ray)
		{
			// The ray Start and End positions, in Normalized Device Coordinates (Have you read Tutorial 4 ?)
			glm::vec4 lRayStart_NDC(
				((float)mousePos.x / (float)screenSize.x - 0.5f) * 2.0f, // [0,1024] -> [-1,1]
				((float)(screenSize.y - mousePos.y) / (float)screenSize.y - 0.5f) * 2.0f, // [0, 768] -> [-1,1]
				-1.0, // The near plane maps to Z=-1 in Normalized Device Coordinates
				1.0f
			);
			glm::vec4 lRayEnd_NDC(
				((float)mousePos.x / (float)screenSize.x - 0.5f) * 2.0f,
				((float)(screenSize.y - mousePos.y) / (float)screenSize.y - 0.5f) * 2.0f,
				1.0,
				1.0f
			);


			// The Projection matrix goes from Camera Space to NDC.
			// So inverse(ProjectionMatrix) goes from NDC to Camera Space.
			glm::mat4 InverseProjectionMatrix = glm::inverse(projection);

			// The View Matrix goes from World Space to Camera Space.
			// So inverse(ViewMatrix) goes from Camera Space to World Space.
			glm::mat4 InverseViewMatrix = glm::inverse(viewMatrix);

			//glm::vec4 lRayStart_camera = InverseProjectionMatrix * lRayStart_NDC;    lRayStart_camera /= lRayStart_camera.w;
			//glm::vec4 lRayStart_world = InverseViewMatrix * lRayStart_camera; lRayStart_world /= lRayStart_world.w;
			//glm::vec4 lRayEnd_camera = InverseProjectionMatrix * lRayEnd_NDC;      lRayEnd_camera /= lRayEnd_camera.w;
			//glm::vec4 lRayEnd_world = InverseViewMatrix * lRayEnd_camera;   lRayEnd_world /= lRayEnd_world.w;


			// Faster way (just one inverse)
			glm::mat4 M = glm::inverse(projection * viewMatrix);
			glm::vec4 lRayStart_world = M * lRayStart_NDC; lRayStart_world/=lRayStart_world.w;
			glm::vec4 lRayEnd_world   = M * lRayEnd_NDC  ; lRayEnd_world  /=lRayEnd_world.w;


			glm::vec3 lRayDir_world(lRayEnd_world - lRayStart_world);
			lRayDir_world = glm::normalize(lRayDir_world);


			ray.origin = glm::vec3(lRayStart_world);
			ray.dir = glm::normalize(lRayDir_world);
		}

		inline static bool IntersectRayBoundingBox(const glm::mat4& modelMatrix, const Ray& ray, const BoundingBox& boundingBox)
		{
			float tMin = 0.0f;
			float tMax = 100000.0f;

			glm::vec3 OBBposition_worldspace(modelMatrix[3].x, modelMatrix[3].y, modelMatrix[3].z);

			glm::vec3 delta = OBBposition_worldspace - ray.origin;
			// Test intersection with the 2 planes perpendicular to the OBB's X axis
			{
				glm::vec3 xaxis(modelMatrix[0].x, modelMatrix[0].y, modelMatrix[0].z);
				float e = glm::dot(xaxis, delta);
				float f = glm::dot(ray.dir, xaxis);

				if (fabs(f) > 0.001f) { // Standard case

					float t1 = (e + boundingBox._minExtent.x) / f; // Intersection with the "left" plane
					float t2 = (e + boundingBox._maxExtent.x) / f; // Intersection with the "right" plane
					// t1 and t2 now contain distances betwen ray origin and ray-plane intersections

					// We want t1 to represent the nearest intersection, 
					// so if it's not the case, invert t1 and t2
					if (t1 > t2) {
						float w = t1; t1 = t2; t2 = w; // swap t1 and t2
					}

					// tMax is the nearest "far" intersection (amongst the X,Y and Z planes pairs)
					if (t2 < tMax)
						tMax = t2;
					// tMin is the farthest "near" intersection (amongst the X,Y and Z planes pairs)
					if (t1 > tMin)
						tMin = t1;

					// And here's the trick :
					// If "far" is closer than "near", then there is NO intersection.
					// See the images in the tutorials for the visual explanation.
					if (tMax < tMin)
						return false;

				}
				else { // Rare case : the ray is almost parallel to the planes, so they don't have any "intersection"
					if (-e + boundingBox._minExtent.x > 0.0f || -e + boundingBox._maxExtent.x < 0.0f)
						return false;
				}
			}


			// Test intersection with the 2 planes perpendicular to the OBB's Y axis
			// Exactly the same thing than above.
			{
				glm::vec3 yaxis(modelMatrix[1].x, modelMatrix[1].y, modelMatrix[1].z);
				float e = glm::dot(yaxis, delta);
				float f = glm::dot(ray.dir, yaxis);

				if (fabs(f) > 0.001f) {

					float t1 = (e + boundingBox._minExtent.y) / f;
					float t2 = (e + boundingBox._maxExtent.y) / f;

					if (t1 > t2) { float w = t1; t1 = t2; t2 = w; }

					if (t2 < tMax)
						tMax = t2;
					if (t1 > tMin)
						tMin = t1;
					if (tMin > tMax)
						return false;

				}
				else {
					if (-e + boundingBox._minExtent.y > 0.0f || -e + boundingBox._maxExtent.y < 0.0f)
						return false;
				}
			}


			// Test intersection with the 2 planes perpendicular to the OBB's Z axis
			// Exactly the same thing than above.
			{
				glm::vec3 zaxis(modelMatrix[2].x, modelMatrix[2].y, modelMatrix[2].z);
				float e = glm::dot(zaxis, delta);
				float f = glm::dot(ray.dir, zaxis);

				if (fabs(f) > 0.001f) {

					float t1 = (e + boundingBox._minExtent.z) / f;
					float t2 = (e + boundingBox._maxExtent.z) / f;

					if (t1 > t2) { float w = t1; t1 = t2; t2 = w; }

					if (t2 < tMax)
						tMax = t2;
					if (t1 > tMin)
						tMin = t1;
					if (tMin > tMax)
						return false;

				}
				else {
					if (-e + boundingBox._minExtent.z > 0.0f || -e + boundingBox._maxExtent.z < 0.0f)
						return false;
				}
			}

			//intersection_distance = tMin;

			return true;
		}
	};
}