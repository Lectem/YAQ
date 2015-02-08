#pragma once

#include <vector>
#include "AABB.h"

namespace YAQT
{
    template<class Object, class T = double, int MAX_OBJECTS = 10,int MAX_LEVELS = 5>
    class QuadTree {
        using _AABB = AABB<T>;
        class QuadTreeObject
        {
            friend class QuadTree;
            _AABB _aabb;
            Object* _object;
            explicit QuadTreeObject(AABB<T> aabb,Object* o):
                    _aabb(aabb),_object(o)
            {

            }
        };
        int _level;
        int _total_items;
        std::vector<QuadTreeObject> _objects;
        _AABB _bounds;
        QuadTree<Object,T>* _parent;
        QuadTree<Object,T>* _nodes[4]={};

        void createChildren()
        {
            _nodes[0]=new QuadTree(_bounds.getRegion(_AABB::Region::NW),this);
            _nodes[1]=new QuadTree(_bounds.getRegion(_AABB::Region::NE),this);
            _nodes[2]=new QuadTree(_bounds.getRegion(_AABB::Region::SW),this);
            _nodes[3]=new QuadTree(_bounds.getRegion(_AABB::Region::SE),this);
        }
        void innerAdd(QuadTreeObject o)
        {
            _total_items++;
            _objects.push_back(o);
        }


    public:
        QuadTree(_AABB dimensions,QuadTree<Object,T> * parent = nullptr):
                _bounds(dimensions),_parent(parent) {}

        ~QuadTree()
        {
            for(int i=0; i<4;++i)
            {
                if(_nodes[i])delete _nodes[i];
            }
        }

        bool push(QuadTreeObject o)
        {
            if(_bounds.contains(o._aabb))
            {
                if(!_nodes[0]) // No subdivision yet
                {
                    if(_objects.size() < MAX_OBJECTS)
                    {
                        innerAdd(o);
                        return true;
                    }
                    createChildren();

                }
            }
            //this object doesn't fit in this quadtree
            return false;
        }
        bool push(_AABB aabb,Object* o)
        {
            return push(QuadTreeObject(aabb,o));
        }
    };
}