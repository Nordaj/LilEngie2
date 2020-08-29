#include "vec3.h"
#include "mat4.h"
#include "misc.h"

namespace LilEngie::Math
{
    vec3 RayRayNearest(vec3 po, vec3 pd, vec3 qo, vec3 qd)
    {
        //http://geomalgorithms.com/a07-_distance.html
        vec3 wo = po - qo;

        float a = dot(pd, pd);
        float b = dot(pd, qd);
        float c = dot(qd, qd);
        float d = dot(pd, wo);
        float e = dot(qd, wo);

        float sc = (b * e - c * d) / (a * c - b * b);
        return po + pd * sc;
    }

    float RayPointDistance(vec3 ro, vec3 rd, vec3 p)
    {
        //Find closest point on line
        vec3 rp = p - ro;
        vec3 c = proj(rd, rp);

        //Clamp point to ray
        if (dot(c, rd) < 0)
            c = vec3(0, 0, 0);

        //Find distance from line pt to p
        return len(ro + c - p);
    }

    float LinePointDistance(vec3 a, vec3 b, vec3 pt)
    {
        vec3 rp = pt - a;
        vec3 c = proj(b - a, rp);
        return len(a + c - pt);
    }

    vec3 LineClosestPoint(vec3 a, vec3 b, vec3 p)
    {
        return a + proj(b - a, p - a);
    }

    float RayPlaneInt(vec3 ro, vec3 rd, vec3 po, vec3 pn)
    {
        //https://www.cs.princeton.edu/courses/archive/fall00/cs426/lectures/raycast/sld017.htm
        float d = -dot(po, pn);
        float t = -(dot(ro, pn) + d) / dot(rd, pn);
        return t;
    }

    void ClipLinePlane(vec3& a, vec3& b, vec3 po, vec3 pn)
    {
        //Check for intersection/get t value
        float t = RayPlaneInt(a, b - a, po, pn);
        if (t < 0 || t > 1)
            return;

        //Move the right side based side of the plane
        if (dot(po - a, pn) > 0)
            a += (b - a) * t;
        else
            b += (a - b) * (1 - t);
    }

    void FrustrumRaycast(const mat4& vp, vec3 pt, vec3& ro, vec3& rd)
    {
        //Use inverse of view projection frustrum matrix
        mat4 ivp = inverse(vp);

        //Setup points to be transformed to world space
        vec4 a = vec4(pt.x, pt.y, 0, 1); //Might need to be -1 on z idk
        vec4 b = vec4(pt.x, pt.y, 1, 1);

        //Transformation of the points
        a = ivp * a;
        a /= a.w;
        b = ivp * b;
        b /= b.w;

        //Parameterize into rays
        ro = a.xyz();
        rd = normalized((b - a).xyz());
    }
}
