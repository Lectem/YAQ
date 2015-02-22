#include <string>
#include "QuadTree.h"

using namespace YAQ;
int main(int argc, char * argv[])
{
    YAQ::QuadTree<int> test( AABB<double>(0,0,500,500)  );
    int i=0;
    test.push(AABB<double>(20,50,1,1),++i);
    test.push(AABB<double>(0,10,1,1),++i);
    test.push(AABB<double>(0,10,1,1),++i);
    test.push(AABB<double>(0,30,1,1),++i);
    test.push(AABB<double>(0,30,1,1),++i);
    test.push(AABB<double>(0,10,1,1),++i);
    test.push(AABB<double>(10,10,1,1),++i);
    test.push(AABB<double>(350,350,125,125),++i);
    test.push(AABB<double>(350,350,125,125),++i);
    test.push(AABB<double>(350,350,125,125),++i);
    test.push(AABB<double>(350,350,1,1),++i);
    test.push(AABB<double>(350,355,125,125),++i);
    test.push(AABB<double>(350,350,125,125),++i);
    test.push(AABB<double>(350,350,125,125),++i);
    test.push(AABB<double>(350,350,2,2),++i);
    test.push(AABB<double>(-50000,350,100000000,2),++i);

    test.cout_display();
    test.queryAABB(AABB<double>(0,0,100,100));
    vector<QuadTreeNode<int>::QuadTreeObject > queryAABB = test.queryAABB(AABB<double>(0,0,100,100));
    cout << "found "<< queryAABB.size() << " objects" << endl;
    vector<QuadTreeNode<int>::QuadTreeObject > queryAABBbis = test.queryAABB(AABB<double>(-555555,295,10000000,100));
    cout << "found "<< queryAABBbis.size() << " objects" << endl;
    for(QuadTreeNode<int>::QuadTreeObject & obj:queryAABBbis)
    {
        cout << obj._object << endl;
    }

    /*if(queryAABB.size()>0)
    {
        queryAABB[0].update(AABB<double>(350,350,2,2));
    }*/


    return 0;
}