#ifndef ABUTL_HPP
#define ABUTL_HPP

#include <cmath>
#include <vector>
#include <sstream>
#include <iostream>

using namespace std;

const double pi = (atan(1) * 4);

template <typename temp>

string tostring(const temp& input)
{
    stringstream convert;
    convert << input;
    return convert.str();
}

struct point2
{
    double x, y;
    point2(double set_x, double set_y)
    {
        x = set_x;
        y = set_y;
    }
    point2()
    {
        x = 0;
        y = 0;
    };
    double magnitude()
    {
        return sqrt(pow(x, 2) + pow(y, 2));
    }
    point2 operator + (const point2& p)
    {
        return point2(x + p.x, y + p.y);
    }
    point2 operator - (const point2& p)
    {
        return point2(p.x - x, p.y - x);
    }
    point2 operator * (const double& d)
    {
        return point2(x * d, y * d);
    }
    point2 operator / (const double& d)
    {
        return point2(x / d, y / d);
    }
    bool operator == (const point2& p)
    {
        return (x == p.x && y == p.y);
    }
    bool operator != (const point2& p)
    {
        return !(x == p.x || y == p.y);
    }
};

struct point3
{
    double x, y, z;
    point3(double set_x, double set_y, double set_z)
    {
        x = set_x;
        y = set_y;
        z = set_z;
    }
    point3()
    {
        x = 0;
        y = 0;
        z = 0;
    };
    double magnitude()
    {
        return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
    }
    point3 operator + (const point3& p)
    {
        return point3(x + p.x, y + p.y, z + p.z);
    }
    point3 operator - (const point3& p)
    {
        return point3(x - p.x, y - p.y, z - p.z);
    }
    point3 operator * (const double& d)
    {
        return point3(x * d, y * d, z * d);
    }
    point3 operator / (const double& d)
    {
        return point3(x / d, y / d, z / d);
    }
    bool operator == (const point3& p)
    {
        return (x == p.x && y == p.y && z == p.z);
    }
    bool operator != (const point3& p)
    {
        return !(x == p.x || y == p.y || z == p.z);
    }
};

struct interval
{
	double imin, imax;
	bool is_closed;
	interval(double set_min, double set_max, bool set_closed = true)
	{
		imin = min(set_min, set_max);
		imax = max(set_min, set_max);
		is_closed = set_closed;
	}
	interval()
	{
	    imin = 0;
		imax = -1;
	}
	bool is_null()
	{
	    return (imin == 0 && imax == -1);
	}
	bool in_interval(double input)
	{
		if(is_null())
		{
			return true;
		}
		if(is_closed)
		{
			return (input >= imin && input <= imax);
		}
		else
		{
			return (input > imin && input < imax);
		}
	}
};

double deg_to_rad(double deg)
{
    return ((deg * pi) / 180);
}

double rad_to_deg(double rad)
{
    return ((rad * 180) / pi);
}

double min3(double x, double y, double z)
{
    return min(min(x, y), z);
}

double max3(double x, double y, double z)
{
    return max(max(x, y), z);
}

double snap(double input, int res)
{
    return (round(input / (double)res) * res);
}

double limit(double input, double lim_min, double lim_max)
{
    if(input < lim_min)
    {
        return lim_min;
    }
    if(input > lim_max)
    {
        return lim_max;
    }
    return input;
}

double angular_limit(double input)
{
    double pi2 = (pi * 2);
    if(input >= pi2)
    {
        return (input - pi2);
    }
    if(input < 0)
    {
        return (input + pi2);
    }
    return input;
}

double lerp(double value0, double value1, double delta)
{
    double diff = (value1 - value0);
    if(diff > delta)
    {
        return (value0 + delta);
    }
    if(diff < delta)
    {
        return (value0 - delta);
    }
    return value1;
}

#endif // ABUTL_HPP
