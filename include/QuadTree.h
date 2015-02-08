#pragma once

#include <vector>
#include "AABB.h"
#include <iostream>
using namespace std;
namespace YAQT
{
    template<class Object, class T = double, int MAX_OBJECTS = 5,int MAX_LEVELS = 5>
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
        int _level=0;
        int _total_items=0;
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
            cout << "new level" << endl;
        }

        void moveNodesIntoChildren()
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

        void innerAdd(QuadTreeObject o)
        {
            _total_items++;
            _objects.push_back(o);
            cout << "object added at level " << _level << endl;
        }

        void getAll(vector<QuadTreeObject>& res)
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


        void getZone(_AABB zone,vector<QuadTreeObject>& res)
        {
        //TODO can do faster with a litle more checking
            if(zone.contains(_bounds))
            {
                res.reserve(_total_items);
                getAll(res);
            }
            else
            {
                if(_nodes[0])
                {
                    for(int i=0;i<4;i++)
                    {
                        _nodes[i]->getZone(zone,res);
                    }
                }
            }
        }

    public:
        QuadTree(_AABB dimensions,QuadTree<Object,T> * parent = nullptr):
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
        bool push(_AABB aabb,Object* o)
        {
            QuadTreeObject obj(aabb,o);
            return push(obj);
        }

        vector<QuadTreeObject> queryAABB(_AABB zone)
        {
            vector<QuadTreeObject> res;
            getZone(zone,res);
            return res;
        }

        void display()
        {
            cout <<"level "<< _level <<"\t";
            for(int i=0;i<_level;++i)cout << '-';
            cout <<_objects.size() << endl;
            if(_nodes[0])
                for(int i=0;i<4;++i)
                    _nodes[i]->display();
        }
    };
}