
#include "Collider.h"

float Collider::orientation2D(const glm::vec2& p, const glm::vec2& q, const glm::vec2& r)
{
    // Indica la orientacion del vector pq respecto al vector pr
    // O, dicho de otra forma, si r está "por encima" o "por debajo" de la recta
    // formada por pq
    glm::vec2 pq = q - p;
    glm::vec2 pr = r - p;

    return pq.x * pr.y - pr.x * pq.y;
}

float Collider::orientation25D(const glm::vec2& p, const glm::vec2& q, const glm::vec2& r, const glm::vec2& t)
{
    // Indica la orientacion de un punto respecto un círculo
    // Dentro, fuera o en la circumferencia
    float pos = (q.x-p.x)*(r.y-p.y)*((t.x-p.x)*(t.x+p.x)+(t.y-p.y)*(t.y+p.y));
    pos +=    (r.x-p.x)*(t.y-p.y)*((q.x-p.x)*(q.x+p.x)+(q.y-p.y)*(q.y+p.y));
    pos +=    (t.x-p.x)*(q.y-p.y)*((r.x-p.x)*(r.x+p.x)+(r.y-p.y)*(r.y+p.y));

    float neg = ((q.x-p.x)*(q.x+p.x)+(q.y-p.y)*(q.y+p.y))*(r.y-p.y)*(t.x-p.x);
    neg +=    ((r.x-p.x)*(r.x+p.x)+(r.y-p.y)*(r.y+p.y))*(t.y-p.y)*(q.x-p.x);
    neg +=    ((t.x-p.x)*(t.x+p.x)+(t.y-p.y)*(t.y+p.y))*(q.y-p.y)*(r.x-p.x);

    return (pos - neg);
}

bool Collider::checkInsideRectangle(const Collider::lineSegment& t, const Collider::lineSegment& s) {
    // Comprueba si al menos una parte de un segmento está dentro del rectángulo
    // que forman los extremos del otro segmento
    float x_min;
    float y_min;
    float x_max;
    float y_max;

    if(t.first.x > t.second.x) {
        x_min = t.second.x;
        x_max = t.first.x;
    }
    else {
        x_min = t.first.x;
        x_max = t.second.x;
    }

    if(t.first.y > t.second.y) {
        y_min = t.second.y;
        y_max = t.first.y;
    }
    else {
        y_min = t.first.y;
        y_max = t.second.y;
    }
    /*
    return (((s.first.x >= x_min && s.first.x <= x_max) and
        (s.first.y >= y_min && s.first.y <= y_max)) or
        (s.second.x >= x_min && s.second.x <= x_max) and
        (s.second.y >= y_min && s.second.y <= y_max));
        */

    //Since we only call this function when lines are aligned, just checking in
    // one axis already works
    return ((s.first.x >= x_min && s.first.x <= x_max) ||
        (s.second.x >= x_min && s.second.x <= x_max));
}

int Collider::lineCrossTest(const Collider::lineSegment& s, const Collider::lineSegment& t)
{
    //Indica si una línea cruza o no, y un montón de casos límite

    float t1 = Collider::orientation2D(s.first, s.second, t.first);
    float t2 = Collider::orientation2D(s.first, s.second, t.second);
    float s1 = Collider::orientation2D(t.first, t.second, s.first);
    float s2 = Collider::orientation2D(t.first, t.second, s.second);

    //Segment t has endpoints on either side of s
    if((t1 < 0) != (t2 < 0)) {
        //Segment s has endpoints on either side of t
        if((s1 < 0) != (s2 < 0)) {
            // Intersection in a middle point
            return FULL_CROSS;
        }
        //Segment s has just one endpoint on the line over segment t
        else if((s1 > 0) != (s2 > 0)) {
            // Endpoint on the intersection
            return T_CROSS;
        }
    }
    //Segment t has just one endpoint on the line over segment s
    else if((t1 > 0) != (t2 > 0)) {
        //Segment t && segment s share an endpoint
        if(s.first == t.first || s.second == t.first || s.first == t.second || s.second == t.second) {
            return ANGLE_CROSS;
        }
        //Segment s has endpoints on either side of t
        else if((s1 < 0) != (s2 < 0)) {
            return T_CROSS;
        }
    }
    //Segment t has both endpoints on the line over segment s
    else if((t1 == 0) && (t2 == 0)) {

        //Segment s && segment t share one endpoint
        if(s.first == t.first || s.second == t.first || s.first == t.second || s.second == t.second) {
            //Segment s && t share both endpoints
            if((s.first == t.first && s.second == t.second) || (s.first == t.second && s.second == t.first)) {
                return FULL_OVERLAP;
            }
            else {
                return CLOCKHAND_OVERLAP;
            }
        }
        //Segment t has one endpoint inside the rectangle covering both endpoints of s
        //Since t's endpoints are already on the line over s, this means having it on s
        else if(checkInsideRectangle(s, t)) {
            return PARTIAL_OVERLAP;
        }
        //Segment t has both endpoints on the line over s, but none inside s
        else {
            return NOT_CROSS;
        }
    }

    return NOT_CROSS;
}

// Auxiliar function to pick the line cross cases we want to consider a
// collision
bool Collider::lineCollision(const Collider::lineSegment& s, const Collider::lineSegment& t)
{
    int test = lineCrossTest(s, t);

    return test > 0;
}
