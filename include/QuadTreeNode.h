/**
* \file QuadTree.h
* \author Lectem
* \brief A simple Quadtree implementation
*/

#pragma once

#include <vector>
#include <list>
#include "AABB.h"
#include <iostream>
#include <stdexcept>


using namespace std;


namespace YAQ
{

    /**
    * \class QuadTreeNode
    * \brief A basic Quadtree implementation that accepts only objects contained in its bounds
    *
    * Quadtrees are useful for 2D space partitioning and quick lookup based on a region of the space.
    * \tparam Object The type of objects stored in the tree
    * \tparam Type of number used by the coordinates system
    * \tparam MAX_OBJECTS Maximum number of objects per node
    * \tparam MAX_LEVELS Would limit the size of the tree ? Not implemented.
    */
    template<class Object, class T, int MAX_OBJECTS ,int MAX_LEVELS>
    class QuadTree;

    template<class Object, class T = double, int MAX_OBJECTS = 5,int MAX_LEVELS = 5>
    class QuadTreeNode{
    public:
        using _AABB = AABB<T>;//< Prevents confusion with other AABB types

        friend class QuadTree<Object,T,MAX_OBJECTS,MAX_LEVELS>;
        /**
        * \struct QuadTreeObject
        * A structure holding the object and its boundaries (its AABB rectangle)
        */
        struct QuadTreeObject
        {
            friend class QuadTreeNode;
            _AABB _aabb;
            Object _object;
            QuadTreeNode *node= nullptr;
            explicit QuadTreeObject(AABB<T> const &aabb,Object const &o):
                    _aabb(aabb),_object(o) {}
        public:
            void update(_AABB const &newAABB);
        };
        
        /**
        * \param dimensions All objects must be strictly within the bounds of the tree, or will not be added to it.
        * \param parent A pointer to the parent node of this tree.
        */
        QuadTreeNode(_AABB dimensions, QuadTreeNode * parent = nullptr);

        ~QuadTreeNode();

        /**
        * \brief Adds a QuadTreeObject to the tree
        * \param o The QuadTreeObject to be added
        * \return true on success
        * \return false on failure
        */
        bool push(QuadTreeObject const &o);
        /**
        * \brief Adds an object to the tree
        * \param o The Object to be added
        * \param aabb the rectangle associated to the object
        * \return true on success
        * \return false on failure
        */
        bool push(_AABB aabb,Object const &o);


        /**
        * \brief Lookup method
        *
        * This method will create a list of the objects that are intersecting a given zone of the space
        * \param zone The rectangle that defines the area of the query.
        * \return a vector containing a COPY of the QuadTreeObjects which _aabb intersects \p zone
        */
        vector<QuadTreeObject> queryAABB(_AABB zone);

        /**
        * \brief A simple text display of the quadtree. Only works if object can be streamed to cout.
        */
        void cout_display() const;

    protected:
        int _level=0;
        int _total_items=0;
        std::list<QuadTreeObject> _objects;
        _AABB _bounds;
        QuadTreeNode * _parent;
        QuadTreeNode * _nodes[4]={};

        void createChildren();
        void moveNodesIntoChildren();
        void innerAdd(QuadTreeObject const &o);
        void getAll(vector<QuadTreeObject>& res);
        void getZone(_AABB zone,vector<QuadTreeObject>& res);
    };

    template<class Object, class T, int MAX_OBJECTS,int MAX_LEVELS>
    QuadTreeNode<Object,T,MAX_OBJECTS,MAX_LEVELS>::QuadTreeNode(_AABB dimensions, QuadTreeNode * parent):
    _bounds(dimensions),_parent(parent)
    {
        if(_parent)_level=_parent->_level+1;
        else _level=0;
    }

    template<class Object, class T, int MAX_OBJECTS,int MAX_LEVELS>
    QuadTreeNode<Object,T,MAX_OBJECTS,MAX_LEVELS>::~QuadTreeNode()
    {
        for(int i=0; i<4;++i)
        {
            if(_nodes[i])delete _nodes[i];
        }
    }

    template<class Object, class T, int MAX_OBJECTS,int MAX_LEVELS>
    void QuadTreeNode<Object,T,MAX_OBJECTS,MAX_LEVELS>::innerAdd(QuadTreeNode::QuadTreeObject const &o)
    {
        _total_items++;
        _objects.push_back(o);
        _objects.back().node=this;
    }


    template<class Object, class T, int MAX_OBJECTS,int MAX_LEVELS>
    void QuadTreeNode<Object,T,MAX_OBJECTS,MAX_LEVELS>::createChildren()
    {
        _nodes[0]=new QuadTreeNode(_bounds.getRegion(_AABB::Region::NW),this);
        _nodes[1]=new QuadTreeNode(_bounds.getRegion(_AABB::Region::NE),this);
        _nodes[2]=new QuadTreeNode(_bounds.getRegion(_AABB::Region::SW),this);
        _nodes[3]=new QuadTreeNode(_bounds.getRegion(_AABB::Region::SE),this);
    }

    template<class Object, class T, int MAX_OBJECTS,int MAX_LEVELS>
    void QuadTreeNode<Object,T,MAX_OBJECTS,MAX_LEVELS>::moveNodesIntoChildren()
    {
        for(auto it =_objects.rbegin();it != _objects.rend();++it)
        {
            for(int i=0;i<4;++i)
            {
                if(_nodes[i]->push(*it))
                {
                    //std::next needed for reverse iterator when deleting
                    _objects.erase(std::next(it).base());
                    i=4;
                }
            }

        }
    }

    template<class Object, class T, int MAX_OBJECTS,int MAX_LEVELS>
    bool QuadTreeNode<Object,T,MAX_OBJECTS,MAX_LEVELS>::push(QuadTreeNode::QuadTreeObject const &o)
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
    bool QuadTreeNode<Object,T,MAX_OBJECTS,MAX_LEVELS>::push(QuadTreeNode::_AABB aabb, Object const &o)
    {
        QuadTreeObject obj(aabb,o);
        return push(obj);
    }

    template<class Object, class T, int MAX_OBJECTS,int MAX_LEVELS>
    void QuadTreeNode<Object,T,MAX_OBJECTS,MAX_LEVELS>::getAll(vector<QuadTreeObject>& res)
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
    void QuadTreeNode<Object,T,MAX_OBJECTS,MAX_LEVELS>::getZone(QuadTreeNode::_AABB zone, vector<QuadTreeNode::QuadTreeObject> &res)
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
    vector<typename QuadTreeNode<Object,T,MAX_OBJECTS,MAX_LEVELS>::QuadTreeObject>  QuadTreeNode<Object,T,MAX_OBJECTS,MAX_LEVELS>::queryAABB(QuadTreeNode::_AABB zone)
    {
        vector<QuadTreeObject> res;
        getZone(zone,res);
        return res;
    }

    template<class Object, class T, int MAX_OBJECTS,int MAX_LEVELS>
    void QuadTreeNode<Object,T,MAX_OBJECTS,MAX_LEVELS>::cout_display() const
    {
        cout <<"level "<< _level <<"\t";
        cout << "(" << _bounds.x <<","<< _bounds.y <<")\t";
        cout << "(" << _bounds.x+ _bounds.w <<","<< _bounds.y+ _bounds.h <<")\t";
        for(int i=0;i<_level;++i)cout << '-';
        cout <<"\t"<<_objects.size() << "\t[";
        for(auto &o:_objects)
        {
            cout << " "<< o._object ;
        }
        cout <<" ]"<< endl;
        if(_nodes[0])
            for(int i=0;i<4;++i)
                _nodes[i]->cout_display();
    }

    template<class Object, class T, int MAX_OBJECTS,int MAX_LEVELS>
    void QuadTreeNode<Object,T,MAX_OBJECTS,MAX_LEVELS>::QuadTreeObject::update(_AABB const &newAABB) {
        /*if(node != nullptr)
        {
            node->_objects.remove(*this);
        }*/
        throw new runtime_error("Not implemented");
    }

}