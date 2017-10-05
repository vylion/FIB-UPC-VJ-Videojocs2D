#ifndef _COLLIDER_INCLUDE
#define _COLLIDER_INCLUDE

#include <glm/glm.hpp>

class Collider
{
    typedef std::pair<vec2, vec2> lineSegment;

    public:
        bool lineCrollision(const lineSegment& s, const lineSegment& t);

    private:
        float orientation2D(const vec2& p, const vec2& q, const vec2& r);
        float orientation25D(const vec2& p, const vec2& q, const vec2& r, const vec2& t);
        bool Collider::checkInsideRectangle(const Collider::lineSegment& t, const Collider::lineSegment& s);
        int lineCrossTest(const lineSegment& s, const lineSegment& t);

        // Line cross cases:
        const int NOT_CROSS = 0;        //Lines do not cross
        const int FULL_CROSS = 1;       //Lines cross fully at middle point
        const int T_CROSS = 2;          //One end is in the middle of the other line
        const int ANGLE_CROSS = 3;      //Lines share one end
        const int PARTIAL_OVERLAP = 4;  //Lines overlap
        const int CLOCKHAND_OVERLAP = 5;//Lines share endpoint and partially overlap
        const int FULL_OVERLAP = 6;     //Lines fully overlap
};

#endif // _COLLIDER_INCLUDE
