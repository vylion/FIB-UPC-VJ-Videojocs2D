
#include "Collider.h"

float Collider::orientation2D(const vec2& p, const vec2& q, const vec2& r)
{
    vec2 pq = q - p;
    vec2 pr = r - p;

    return pq.x * pr.y - pr.x * pq.y;
}

float Collider::orientation25D(const vec2& p, const vec2& q, const vec2& r, const vec2& t)
{
    float pos = (q.x-p.x)*(r.y-p.y)*((t.x-p.x)*(t.x+p.x)+(t.y-p.y)*(t.y+p.y));
    pos +=    (r.x-p.x)*(t.y-p.y)*((q.x-p.x)*(q.x+p.x)+(q.y-p.y)*(q.y+p.y));
    pos +=    (t.x-p.x)*(q.y-p.y)*((r.x-p.x)*(r.x+p.x)+(r.y-p.y)*(r.y+p.y));

    float neg = ((q.x-p.x)*(q.x+p.x)+(q.y-p.y)*(q.y+p.y))*(r.y-p.y)*(t.x-p.x);
    neg +=    ((r.x-p.x)*(r.x+p.x)+(r.y-p.y)*(r.y+p.y))*(t.y-p.y)*(q.x-p.x);
    neg +=    ((t.x-p.x)*(t.x+p.x)+(t.y-p.y)*(t.y+p.y))*(q.y-p.y)*(r.x-p.x);

    return (pos - neg);
}

bool Collider::checkInsideRectangle(const Collider::lineSegment& t, const Collider::lineSegment& s) {

    num x_min;
    num y_min;
    num x_max;
    num y_max;

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
    return (((s.first.x >= x_min and s.first.x <= x_max) and
        (s.first.y >= y_min and s.first.y <= y_max)) or
        (s.second.x >= x_min and s.second.x <= x_max) and
        (s.second.y >= y_min and s.second.y <= y_max));
        */

    //Since we only call this function when lines are aligned, just checking in
    // one axis already works
    return ((s.first.x >= x_min and s.first.x <= x_max) or
        (s.second.x >= x_min and s.second.x <= x_max));
}

bool Collider::lineCross(Collider::lineSegment& s, Collider::lineSegment& t)
{
    num t1 = Collider::orientation2D(s.first, s.second, t.first);
    num t2 = Collider::orientation2D(s.first, s.second, t.second);
    num s1 = Collider::orientation2D(t.first, t.second, s.first);
    num s2 = Collider::orientation2D(t.first, t.second, s.second);

    //Segment t has endpoints on either side of s
    if((t1 < 0) != (t2 < 0)) {
        //Segment s has endpoints on either side of t
        if((s1 < 0) != (s2 < 0)) {
            // Intersection in a middle point
            return true;
        }
        //Segment s has just one endpoint on the line over segment t
        else if((s1 > 0) != (s2 > 0)) {
            // Endpoint on the intersection
            return true;
        }
    }

    /*
    //Segment t has just one endpoint on the line over segment s
    else if((t1 > 0) != (t2 > 0)) {
        //Segment t and segment s share an endpoint


        if(s[0] == t[0] or s[1] == t[0] or s[0] == t[1] or s[1] == t[1]) {
            desc = "Intersection at a shared endpoint (orange)";
            colour = orange;
        }
        //Segment s has endpoints on either side of t
        else if((s1 < 0) != (s2 < 0)) {
            desc = "Endpoint on the intersection (yellow)";
            colour = yellow;
        }
    }
    //Segment t has both endpoints on the line over segment s
    else if((t1 == 0) and (t2 == 0)) {
        desc = " on a shared line";

        //Segment s and segment t share one endpoint
        if(s[0] == t[0] or s[1] == t[0] or s[0] == t[1] or s[1] == t[1]) {
            //Segment s and t share both endpoints
            if((s[0] == t[0] and s[1] == t[1]) or (s[0] == t[1] and s[1] == t[0])) {
                desc = "Full overlap (white)";
                colour = white;
            }
            else {
                desc = "Intersection at a shared endpoint (purple)";
                colour = purple;
            }
        }
        //Segment t has one endpoint inside the rectangle covering both endpoints of s
        //Since t's endpoints are already on the line over s, this means having it on s
        else if(checkInsideRectangle(s, t)) {
            desc = "Overlap (intersection)" + desc + " (dark blue)";
            colour = darkblue;
        }
        //Segment t has both endpoints on the line over s, but none inside s
        else {
            desc = "No intersection, but" + desc + " (blue)";
            colour = blue;
        }
    }
    */
    
    return false;
}
