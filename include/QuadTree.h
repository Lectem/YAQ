#pragma once

#include "QuadTreeNode.h"

namespace YAQ {
    /**
    * \class QuadTree
    * \brief A basic Quadtree implementation
    *
    * Quadtrees are useful for 2D space partitioning and quick lookup based on a region of the space.
    * Do note that this tree can not hold objects that are not intersecting its bounds
    *
    * \tparam Object The type of objects stored in the tree
    * \tparam Type of number used by the coordinates system
    * \tparam MAX_OBJECTS Maximum number of objects per node
    * \tparam MAX_LEVELS Would limit the size of the tree ? Not implemented.
    */
    template<class Object, class T = double, int MAX_OBJECTS = 5,int MAX_LEVELS = 5>
    class QuadTree : public QuadTreeNode<Object,T,MAX_OBJECTS,MAX_LEVELS>
    {
    public:
        using Node = QuadTreeNode<Object,T,MAX_OBJECTS,MAX_LEVELS>;
        using typename Node::_AABB ;
        using typename Node::QuadTreeObject;
        QuadTree(_AABB dimensions):Node(dimensions, nullptr) {}


        /**
        * \brief Adds a QuadTreeObject to the tree
        *
        * For a normal QuadTreeNode, if the item is not contained, we just return false. But for a QuadTree,
        * we need to store it somewhere if it actually intersects the QuadTree.
        * Objects that do not intersect the QuadTree bounds will NOT be added.
        *
        * \param o The QuadTreeObject to be added
        * \return true on success
        * \return false on failure
        */
        bool push(QuadTreeObject &o);

        /**
        * \brief Adds an object to the tree
        *
        * For a normal QuadTreeNode, if the item is not contained, we just return false. But for a QuadTree,
        * we need to store it somewhere if it actually intersects the QuadTree.
        * Objects that do not intersect the QuadTree bounds will NOT be added.
        *
        * \param o The Object to be added
        * \param aabb the rectangle associated to the object
        * \return true on success
        * \return false on failure
        */
        bool push(_AABB aabb,Object &o);


        /**
        * \brief Lookup method
        *
        * This method will create a list of the objects that are intersecting a given zone of the space
        * \param zone The rectangle that defines the area of the query.
        * \return a vector containing a COPY of the QuadTreeObjects which _aabb intersects \p zone
        */
        vector<QuadTreeObject> queryAABB(_AABB zone);

        void cout_display() const;

    private:
        vector<QuadTreeObject> _crossingObjects;
    };


    template<class Object, class T, int MAX_OBJECTS,int MAX_LEVELS>
    bool QuadTree<Object,T,MAX_OBJECTS,MAX_LEVELS>::push(QuadTree::QuadTreeObject &o)
    {
        if(this->_bounds.contains(o._aabb))
        {
            return static_cast<Node*>(this)->push(o);
        }
        else if(this->_bounds.intersect(o._aabb))
        {
            _crossingObjects.push_back(o);
            return true;
        }
        return false;
    }

    template<class Object, class T, int MAX_OBJECTS,int MAX_LEVELS>
    bool QuadTree<Object,T,MAX_OBJECTS,MAX_LEVELS>::push(QuadTree::_AABB aabb, Object &o)
    {
        QuadTreeObject obj(aabb,o);
        return push(obj);;
    }

    template<class Object, class T, int MAX_OBJECTS,int MAX_LEVELS>
    vector<typename QuadTree<Object,T,MAX_OBJECTS,MAX_LEVELS>::Node::QuadTreeObject >  QuadTree<Object,T,MAX_OBJECTS,MAX_LEVELS>::queryAABB(QuadTree::_AABB zone)
    {
        vector<QuadTreeObject> res;
        for(QuadTreeObject & o : _crossingObjects)
        {
            if(o._aabb.intersect(zone))
                res.push_back(o);
        }
        this->getZone(zone,res);
        return res;
    }


    template<class Object, class T, int MAX_OBJECTS,int MAX_LEVELS>
    void QuadTree<Object,T,MAX_OBJECTS,MAX_LEVELS>::cout_display() const
    {
        cout <<"level x\t(-INF,-INF)\t(+INF,+INF)\t";
        cout <<"\t"<< _crossingObjects.size() << "\t[";
        for(auto &o:_crossingObjects)
        {
            cout << " "<< o._object ;
        }
        cout <<" ]"<< endl;
        static_cast<Node const *>(this)->cout_display();
    }

}
