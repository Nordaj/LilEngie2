#include "vec3.h"
#include "misc.h"

namespace LilEngie::Math
{
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
}
