#pragma once


namespace YAQ
{
    template<class T = double>
    struct AABB {

        enum Region{NW,NE,SW,SE};

        T x, y, w, h;

        AABB(T x=0,T y=0,T w=0,T h=0):
                x(x),y(y),w(w),h(h) {}

        bool intersect(AABB const &other);
        bool contains(AABB const &other);
        AABB getRegion(Region r)
        {
            switch (r)
            {
                case NW:
                    return AABB(x,y,w/2,h/2);
                case NE:
                    return AABB(x+w/2,y,w/2,h/2);
                case SW:
                    return AABB(x,y+h/2,w/2,h/2);
                case SE:
                    return AABB(x+w/2,y+h/2,w/2,h/2);
                default:
                    return *this;
            }
        }
    };


    template <class T>
    bool AABB<T>::intersect(AABB const &other)
    {
        return !((x >= other.x+other.w || other.x >= x+w) ||
                (y >= other.y+other.h|| other.y >= y+h));


    }



    template <class T>
    bool AABB<T>::contains(AABB const &other)
    {
        return x <= other.x && y <= other.y &&
                x+w >= other.x+other.w && y+h>= other.y+other.h;
    }

}