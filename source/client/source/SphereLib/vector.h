#pragma once
#include <math.h>
#include <vector>

class Vector3d: public D3DXVECTOR3
{
	public:
		Vector3d (void) {};

		Vector3d (const Vector3d& a)
		{
			x = a.x;
			y = a.y;
			z = a.z;
		};

		Vector3d (float a, float b, float c)
		{
			x = a;
			y = b;
			z = c;
		};

		bool operator== (const Vector3d& a) const
		{
			if (a.x == x && a.y == y && a.z == z)
			{
				return true;
			}
			return false;
		};

		bool operator!= (const Vector3d& a) const
		{
			if (a.x != x || a.y != y || a.z != z)
			{
				return true;
			}

			return false;
		};


		Vector3d& operator = (const Vector3d& A)
		{
			x = A.x;
			y = A.y;
			z = A.z;
			return (*this);
		};

		Vector3d operator + (const Vector3d& A) const
		{
			Vector3d Sum (x + A.x, y + A.y, z + A.z);
			return (Sum);
		};

		Vector3d operator - (const Vector3d& A) const
		{
			Vector3d Diff (x - A.x, y - A.y, z - A.z);
			return (Diff);
		};

		Vector3d operator * (const float s) const
		{
			Vector3d Scaled (x * s, y * s, z * s);
			return (Scaled);
		};

		Vector3d operator / (const float s) const
		{
			float r = 1.0f / s;
			Vector3d Scaled (x * r, y * r, z * r);
			return (Scaled);
		};

		void operator += (const Vector3d A)
		{
			x += A.x;
			y += A.y;
			z += A.z;
		};
		void operator -= (const Vector3d A)
		{
			x -= A.x;
			y -= A.y;
			z -= A.z;
		};
		void operator *= (const float s)
		{
			x *= s;
			y *= s;
			z *= s;
		}

		Vector3d operator - (void) const
		{
			Vector3d Negated (-x, -y, -z);
			return (Negated);
		};

		float operator [] (const int i) const
		{
			return ((i == 0) ? x : ((i == 1) ? y : z));
		};
		float& operator [] (const int i)
		{
			return ((i == 0) ? x : ((i == 1) ? y : z));
		};

		float GetX (void) const
		{
			return x;
		};
		float GetY (void) const
		{
			return y;
		};
		float GetZ (void) const
		{
			return z;
		};

		void SetX (float t)
		{
			x   = t;
		};
		void SetY (float t)
		{
			y   = t;
		};
		void SetZ (float t)
		{
			z   = t;
		};

		void Set (float a, float b, float c)
		{
			x = a;
			y = b;
			z = c;
		};

		void Zero (void)
		{
			x = y = z = 0;
		};

		Vector3d negative (void) const
		{
			Vector3d result;
			result.x = -x;
			result.y = -y;
			result.z = -z;
			return result;
		}

		float Magnitude (void) const
		{
			return (sqrtf (x * x + y * y + z * z));
		};

		void Lerp (const Vector3d& from, const Vector3d& to, float slerp)
		{
			*this = to - from;
			*this *= slerp;
			*this += from;
		};

		float Length (void) const
		{
			return float (sqrtf (x * x + y * y + z * z));
		};

		float Length2 (void) const
		{
			float l2 = x * x + y * y + z * z;
			return l2;
		};

		inline float Distance (const Vector3d& a) const
		{
			return sqrtf (DistanceSq (a));
		}

		inline float Distance2d (const Vector3d& a) const
		{
			return sqrtf (DistanceSq2d (a));
		}

		float DistanceXY (const Vector3d& a) const
		{
			float dx = a.x - x;
			float dy = a.y - y;
			float dist = dx * dx + dy * dy;
			return dist;
		}

		float DistanceSq (const Vector3d& a) const
		{
			float dx = a.x - x;
			float dy = a.y - y;
			float dz = a.z - z;
			return dx * dx + dy * dy + dz * dz;
		};

		float DistanceSq2d (const Vector3d& a) const
		{
			float dx = a.x - x;
			float dy = a.y - y;
			return dx * dx + dy * dy;
		};

		float Normalize (void)
		{
			float l = Length();
			if (l != 0)
			{
				x /= l;
				y /= l;
				z /= l;
			}
			else
			{
				x = y = z = 0;
			}
			return l;
		};

		float Dot (const Vector3d& a) const
		{
			return (x * a.x + y * a.y + z * a.z);
		};

		bool IsInStaticRange() const;
		void Cross (const Vector3d& a, const Vector3d& b)
		{
			x = a.y * b.z - a.z * b.y;
			y = a.z * b.x - a.x * b.z;
			z = a.x * b.y - a.y * b.x;
		};
};


typedef std::vector<Vector3d> Vector3dVector;
inline Vector3d operator * (float s, const Vector3d& v)
{
	Vector3d Scaled (v.x * s, v.y * s, v.z * s);
	return (Scaled);
};