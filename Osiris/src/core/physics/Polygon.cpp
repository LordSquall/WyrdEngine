#include "osrpch.h"
#include "Polygon.h"

#include "MathHelper.h"

namespace Osiris
{
	Polygon::Polygon()
	{

	}

	Polygon::~Polygon()
	{

	}

	void Polygon::BuildFromRectangle(const glm::vec4 rect)
	{
		/* clear previous points */
		pts.clear();

		/* add the 4 corners of the rectangle */
		pts.push_back({ rect.x, rect.y });
		pts.push_back({ rect.x + rect.z, rect.y });
		pts.push_back({ rect.x + rect.z, rect.y + rect.w });
		pts.push_back({ rect.x, rect.y + rect.w });

		/* set center point */
		center.x = rect.x + (rect.z / 2.0f);
		center.y = rect.y + (rect.w / 2.0f);

		/* calculate the aabb */
		aabb = rect;
	}

	Polygon::Direction Polygon::CollisionTest_AABB(Polygon* polygonA, Polygon* polygonB)
	{
		if (Rect::Contains(polygonA->aabb, polygonB->aabb) == true)
		{
			glm::vec2 compass[] = {
				glm::vec2(0.0f, 1.0f),	// up
				glm::vec2(1.0f, 0.0f),	// right
				glm::vec2(0.0f, -1.0f),	// down
				glm::vec2(-1.0f, 0.0f)	// left
			};

			float max = 0.0f;
			unsigned int best_match = -1;
			for (unsigned int i = 0; i < 4; i++)
			{
				float dot_product = glm::dot(glm::normalize(polygonB->center), compass[i]);
				if (dot_product > max)
				{
					max = dot_product;
					best_match = i;
				}
			}

			return (Polygon::Direction)(best_match);
		}
		else
		{
			return Polygon::Direction::NONE;
		}
	}

	bool Polygon::CollisionTest_SAT(Polygon* polygonA, Polygon* polygonB, glm::vec2* correctionVector, float* correctionDepth)
	{
		Polygon* poly1 = polygonA;
		Polygon* poly2 = polygonB;

		float overlap = INFINITY;

		for (int shape = 0; shape < 2; shape++)
		{
			if (shape == 1)
			{
				poly1 = polygonB;
				poly2 = polygonA;
			}

			for (int a = 0; a < poly1->pts.size(); a++)
			{
				int b = (a + 1) % poly1->pts.size();
				glm::vec2 axisProj = { -(poly1->pts[b].y - poly1->pts[a].y), poly1->pts[b].x - poly1->pts[a].x };
				float d = sqrtf(axisProj.x * axisProj.x + axisProj.y * axisProj.y);
				axisProj = { axisProj.x / d, axisProj.y / d };

				// Work out min and max 1D points for r1
				float min_r1 = INFINITY, max_r1 = -INFINITY;
				for (int p = 0; p < poly1->pts.size(); p++)
				{
					float q = (poly1->pts[p].x * axisProj.x + poly1->pts[p].y * axisProj.y);
					min_r1 = std::min(min_r1, q);
					max_r1 = std::max(max_r1, q);
				}

				// Work out min and max 1D points for r2
				float min_r2 = INFINITY, max_r2 = -INFINITY;
				for (int p = 0; p < poly2->pts.size(); p++)
				{
					float q = (poly2->pts[p].x * axisProj.x + poly2->pts[p].y * axisProj.y);
					min_r2 = std::min(min_r2, q);
					max_r2 = std::max(max_r2, q);
				}

				// Calculate actual overlap along projected axis, and store the minimum
				overlap = std::min(std::min(max_r1, max_r2) - std::max(min_r1, min_r2), overlap);

				if (!(max_r2 >= min_r1 && max_r1 >= min_r2))
					return false;
			}
		}

		/* we are given a delta position then we want to return the feedback */
		if (correctionVector != nullptr)
		{
			glm::vec2 d = { polygonB->center.x - polygonA->center.x, polygonB->center.y - polygonA->center.y };
			float s = sqrtf(d.x * d.x + d.y * d.y);
			correctionVector->x = -(overlap * d.x / s);
			correctionVector->y = -(overlap * d.y / s);
		}

		return true;
	}

	bool Polygon::CollisionTest_DIAG(Polygon* polygonA, Polygon* polygonB, glm::vec2* correctionVector, float* correctionDepth)
	{
		Polygon* poly1 = polygonA;
		Polygon* poly2 = polygonB;
	
		for (int shape = 0; shape < 2; shape++)
		{
			if (shape == 1)
			{
				poly1 = polygonB;
				poly2 = polygonA;
			}

			for (int p = 0; p < poly1->pts.size(); p++)
			{

				glm::vec2 line_r1s = poly1->center;
				glm::vec2 line_r1e = poly1->pts[p];

				glm::vec2 displacement = { 0,0 };

				for (int q = 0; q < poly2->pts.size(); q++)
				{
					glm::vec2 line_r2s = poly2->pts[q];
					glm::vec2 line_r2e = poly2->pts[(q + 1) % poly2->pts.size()];

					// Standard "off the shelf" line segment intersection
					float h = (line_r2e.x - line_r2s.x) * (line_r1s.y - line_r1e.y) - (line_r1s.x - line_r1e.x) * (line_r2e.y - line_r2s.y);
					float t1 = ((line_r2s.y - line_r2e.y) * (line_r1s.x - line_r2s.x) + (line_r2e.x - line_r2s.x) * (line_r1s.y - line_r2s.y)) / h;
					float t2 = ((line_r1s.y - line_r1e.y) * (line_r1s.x - line_r2s.x) + (line_r1e.x - line_r1s.x) * (line_r1s.y - line_r2s.y)) / h;

					if (t1 >= 0.0f && t1 < 1.0f && t2 >= 0.0f && t2 < 1.0f)
					{
						displacement.x += (1.0f - t1) * (line_r1e.x - line_r1s.x);
						displacement.y += (1.0f - t1) * (line_r1e.y - line_r1s.y);
					}
				}


				/* we are given a delta position then we want to return the feedback */
				if (correctionVector != nullptr)
				{
					correctionVector->x += displacement.x * (shape == 0 ? -1 : +1);
					correctionVector->y += displacement.y * (shape == 0 ? -1 : +1);
				}
			}
		}

		if (correctionVector != nullptr)
		{
			return false;
		}
		return true;
	}
}
