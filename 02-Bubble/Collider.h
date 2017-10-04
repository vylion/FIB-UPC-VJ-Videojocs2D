#ifndef _COLLIDER_INCLUDE
#define _COLLIDER_INCLUDE

#include <glm/glm.hpp>

class Collider
{
    typedef std::pair<vec2, vec2> lineSegment;

    public:
        bool lineCross(const lineSegment& s, const lineSegment& t);

    private:
        float orientation2D(const vec2& p, const vec2& q, const vec2& r);
        float orientation25D(const vec2& p, const vec2& q, const vec2& r, const vec2& t);
        bool Collider::checkInsideRectangle(const Collider::lineSegment& t, const Collider::lineSegment& s);
};

#endif // _COLLIDER_INCLUDE
