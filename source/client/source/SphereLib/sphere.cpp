#include "Stdafx.h"
#include "sphere.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

bool Vector3d::IsInStaticRange() const
{
	const float LIMIT = 3276700.0f;
	if (x < LIMIT && x > -LIMIT)
	{
		if (y < LIMIT && y > -LIMIT)
		{
			if (z < LIMIT && z > -LIMIT)
			{
				return true;
			}
		}
	}

	return false;
}

void Sphere::Set (const Vector3d& center, float radius)
{
	#ifdef __STATIC_RANGE__
	assert (center.IsInStaticRange());
	#endif
	mCenter = center;
	mRadius = radius;
	mRadius2 = radius * radius;
}


bool Sphere::RayIntersection (const Vector3d& rayOrigin,
							  const Vector3d& dir,
							  Vector3d* intersect)
{
	Vector3d EO = mCenter - rayOrigin;
	Vector3d V = dir;
	float dist2 = EO.x * EO.x + EO.y * EO.y + EO.z * EO.z;

	if (dist2 < mRadius2)
	{
		V *= -1;
	}

	float v = EO.Dot (V);
	float disc = mRadius2 - (EO.Length2() - v * v);

	if (disc > 0.0f)
	{
		if (intersect)
		{
			float d = sqrtf (disc);
			*intersect = rayOrigin + V * (v - d);

		}

		return true;
	}
	return false;
}

bool Sphere::RayIntersection (const Vector3d& rayOrigin,
							  const Vector3d& V,
							  float distance,
							  Vector3d* intersect)
{
	Vector3d sect;
	bool hit = RayIntersectionInFront (rayOrigin, V, &sect);

	if (hit)
	{
		float d = rayOrigin.DistanceSq (sect);
		if (d > (distance * distance))
		{
			return false;
		}
		if (intersect)
		{
			*intersect = sect;
		}
		return true;
	}
	return false;
}


bool Sphere::RayIntersectionInFront (const Vector3d& rayOrigin,
									 const Vector3d& V,
									 Vector3d* intersect)
{
	Vector3d sect;
	bool hit = RayIntersection (rayOrigin, V, &sect);

	if (hit)
	{

		Vector3d dir = sect - rayOrigin;

		float dot = dir.Dot (V);

		if (dot >= 0)
		{
			if (intersect)
			{
				*intersect = sect;
			}
			return true;
		}
	}
	return false;
}

void Sphere::Report (void)
{

}

#define BIGNUMBER 100000000.0

void Sphere::Compute (const SphereInterface& source)
{

	Vector3d xmin, xmax, ymin, ymax, zmin, zmax, dia1, dia2;

	xmin.Set (BIGNUMBER, BIGNUMBER, BIGNUMBER);
	xmax.Set (-BIGNUMBER, -BIGNUMBER, -BIGNUMBER);
	ymin.Set (BIGNUMBER, BIGNUMBER, BIGNUMBER);
	ymax.Set (-BIGNUMBER, -BIGNUMBER, -BIGNUMBER);
	zmin.Set (BIGNUMBER, BIGNUMBER, BIGNUMBER);
	zmax.Set (-BIGNUMBER, -BIGNUMBER, -BIGNUMBER);

	int count = source.GetVertexCount();

	for (int i = 0; i < count; i++)
	{
		Vector3d caller_p;
		source.GetVertex (i, caller_p);

		if (caller_p.GetX() < xmin.GetX())
		{
			xmin = caller_p;
		}
		if (caller_p.GetX() > xmax.GetX())
		{
			xmax = caller_p;
		}
		if (caller_p.GetY() < ymin.GetY())
		{
			ymin = caller_p;
		}
		if (caller_p.GetY() > ymax.GetY())
		{
			ymax = caller_p;
		}
		if (caller_p.GetZ() < zmin.GetZ())
		{
			zmin = caller_p;
		}
		if (caller_p.GetZ() > zmax.GetZ())
		{
			zmax = caller_p;
		}
	}

	float dx = xmax.GetX() - xmin.GetX();
	float dy = xmax.GetY() - xmin.GetY();
	float dz = xmax.GetZ() - xmin.GetZ();
	float xspan = dx * dx + dy * dy + dz * dz;

	dx = ymax.GetX() - ymin.GetX();
	dy = ymax.GetY() - ymin.GetY();
	dz = ymax.GetZ() - ymin.GetZ();
	float yspan = dx * dx + dy * dy + dz * dz;

	dx = zmax.GetX() - zmin.GetX();
	dy = zmax.GetY() - zmin.GetY();
	dz = zmax.GetZ() - zmin.GetZ();
	float zspan = dx * dx + dy * dy + dz * dz;

	dia1 = xmin;
	dia2 = xmax;
	float maxspan = xspan;

	if (yspan > maxspan)
	{
		maxspan = yspan;
		dia1 = ymin;
		dia2 = ymax;
	}

	if (zspan > maxspan)
	{
		dia1 = zmin;
		dia2 = zmax;
	}

	mCenter.SetX ((dia1.GetX() + dia2.GetX()) * 0.5f);
	mCenter.SetY ((dia1.GetY() + dia2.GetY()) * 0.5f);
	mCenter.SetZ ((dia1.GetZ() + dia2.GetZ()) * 0.5f);

	dx = dia2.GetX() - mCenter.GetX();
	dy = dia2.GetY() - mCenter.GetY();
	dz = dia2.GetZ() - mCenter.GetZ();
	mRadius2 = dx * dx + dy * dy + dz * dz;
	mRadius = float (sqrt (mRadius2));

	for (int j = 0; j < count; j++)
	{
		Vector3d caller_p;
		source.GetVertex (j, caller_p);
		dx = caller_p.GetX() - mCenter.GetX();
		dy = caller_p.GetY() - mCenter.GetY();
		dz = caller_p.GetZ() - mCenter.GetZ();
		float old_to_p_sq = dx * dx + dy * dy + dz * dz;
		if (old_to_p_sq > mRadius2)
		{
			float old_to_p = float (sqrt (old_to_p_sq));
			mRadius = (mRadius + old_to_p) * 0.5f;
			mRadius2 = mRadius * mRadius;
			float old_to_new = old_to_p - mRadius;

			float recip = 1.0f / old_to_p;

			float cx = (mRadius * mCenter.GetX() + old_to_new * caller_p.GetX()) * recip;
			float cy = (mRadius * mCenter.GetY() + old_to_new * caller_p.GetY()) * recip;
			float cz = (mRadius * mCenter.GetZ() + old_to_new * caller_p.GetZ()) * recip;

			mCenter.Set (cx, cy, cz);
		}
	}
}
