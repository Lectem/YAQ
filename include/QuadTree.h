#pragma once

#include <vector>
#include "AABB.h"
#include <iostream>
using namespace std;
namespace YAQ
{
    template<class Object, class T = double, int MAX_OBJECTS = 5,int MAX_LEVELS = 5>
    class QuadTree {
    public:using _AABB = AABB<T>;
    private:
        class QuadTreeObject
        {
            friend class QuadTree;
            _AABB _aabb;
            Object _object;
            explicit QuadTreeObject(AABB<T> aabb,Object o):
                    _aabb(aabb),_object(o) {}
        };
        int _level=0;
        int _total_items=0;
        std::vector<QuadTreeObject> _objects;
        _AABB _bounds;
        QuadTree* _parent;
        QuadTree* _nodes[4]={};

        void createChildren();
        void moveNodesIntoChildren();
        void innerAdd(QuadTreeObject o);
        void getAll(vector<QuadTreeObject>& res);
        void getZone(_AABB zone,vector<QuadTreeObject>& res);

    public:
        QuadTree(_AABB dimensions,QuadTree * parent = nullptr):
                _bounds(dimensions),_parent(parent)
        {
            if(_parent)_level=_parent->_level+1;
            else _level=0;
        }

        ~QuadTree()
        {
            for(int i=0; i<4;++i)
            {
                if(_nodes[i])delete _nodes[i];
            }
        }

        bool push(QuadTreeObject o);
        bool push(_AABB aabb,Object o);

        vector<QuadTreeObject> queryAABB(_AABB zone);

        void display();
    };


    template<class Object, class T, int MAX_OBJECTS,int MAX_LEVELS>
    void QuadTree<Object,T,MAX_OBJECTS,MAX_LEVELS>::innerAdd(QuadTree::QuadTreeObject o)
    {
        _total_items++;
        _objects.push_back(o);
        cout << "object added at level " << _level << endl;
    }


    template<class Object, class T, int MAX_OBJECTS,int MAX_LEVELS>
    void QuadTree<Object,T,MAX_OBJECTS,MAX_LEVELS>::createChildren()
    {
        _nodes[0]=new QuadTree(_bounds.getRegion(_AABB::Region::NW),this);
        _nodes[1]=new QuadTree(_bounds.getRegion(_AABB::Region::NE),this);
        _nodes[2]=new QuadTree(_bounds.getRegion(_AABB::Region::SW),this);
        _nodes[3]=new QuadTree(_bounds.getRegion(_AABB::Region::SE),this);
        cout << "new level" << endl;
    }

    template<class Object, class T, int MAX_OBJECTS,int MAX_LEVELS>
    void QuadTree<Object,T,MAX_OBJECTS,MAX_LEVELS>::moveNodesIntoChildren()
    {
        for(auto it =_objects.rbegin();it != _objects.rend();++it)
        {
            for(int i=0;i<4;++i)
            {
                if(_nodes[i]->push(*it))
                {
                    _objects.erase(std::next(it).base());
                    i=4;
                }
            }

        }
    }

    template<class Object, class T, int MAX_OBJECTS,int MAX_LEVELS>
    bool QuadTree<Object,T,MAX_OBJECTS,MAX_LEVELS>::push(QuadTree::QuadTreeObject o)
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
                moveNodesIntoChildren();
                std::cout << "test" << std::endl;

            }

            for(int i=0;i<4;++i)
            {
                if(_nodes[i]->push(o))
                {
                    _total_items++;
                    return true;
                }
            }
            innerAdd(o);
            return true;
        }
        //this object doesn't fit in this quadtree
        return false;
    }

    template<class Object, class T, int MAX_OBJECTS,int MAX_LEVELS>
    void QuadTree<Object,T,MAX_OBJECTS,MAX_LEVELS>::getAll(vector<QuadTreeObject>& res)
    {
        res.insert(res.end(), _objects.begin(),_objects.end());
        if(_nodes[0])
        {
            for(int i=0;i<4;++i)
            {
                _nodes[i]->getAll(res);
            }
        }
    }

    template<class Object, class T, int MAX_OBJECTS,int MAX_LEVELS>
    void QuadTree<Object,T,MAX_OBJECTS,MAX_LEVELS>::getZone(QuadTree::_AABB zone, vector<QuadTree::QuadTreeObject> &res)
    {
        if(zone.contains(_bounds))
        {
            res.reserve(_total_items);
            getAll(res);
        }
        else
        {
            for(auto &o : _objects)
            {
                if(o._aabb.intersect(zone))
                {
                    res.push_back(o);
                }
            }
            if(_nodes[0])
            {
                for(int i=0;i<4;i++)
                {
                    //Break if we know that the zone is fully contained by a region
                    if(_nodes[i]->_bounds.intersect(zone))
                    {
                        _nodes[i]->getZone(zone,res);
                        if(_nodes[i]->_bounds.contains(zone))
                        {
                            break;
                        }
                    }
                }
            }
        }
    }


    template<class Object, class T, int MAX_OBJECTS,int MAX_LEVELS>
    bool QuadTree<Object,T,MAX_OBJECTS,MAX_LEVELS>::push(QuadTree::_AABB aabb, Object o)
    {
        QuadTreeObject obj(aabb,o);
        return push(obj);
    }

    template<class Object, class T, int MAX_OBJECTS,int MAX_LEVELS>
    vector<typename QuadTree<Object,T,MAX_OBJECTS,MAX_LEVELS>::QuadTreeObject>  QuadTree<Object,T,MAX_OBJECTS,MAX_LEVELS>::queryAABB(QuadTree::_AABB zone)
    {
        vector<QuadTreeObject> res;
        getZone(zone,res);
        return res;
    }

    template<class Object, class T, int MAX_OBJECTS,int MAX_LEVELS>
    void QuadTree<Object,T,MAX_OBJECTS,MAX_LEVELS>::display()
    {
        cout <<"level "<< _level <<"\t";
        for(int i=0;i<_level;++i)cout << '-';
        cout <<_objects.size() << endl;
        if(_nodes[0])
            for(int i=0;i<4;++i)
                _nodes[i]->display();
    }

}