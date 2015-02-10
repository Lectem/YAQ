#include <string>
#include "QuadTree.h"

using namespace YAQ;
int main(int argc, char * argv[])
{
    YAQ::QuadTree<int> test( AABB<double>(0,0,500,500)  );
    int i;
    test.push(AABB<double>(20,50,1,1),&i);
    test.push(AABB<double>(0,10,1,1),&i);
    test.push(AABB<double>(0,10,1,1),&i);
    test.push(AABB<double>(0,30,1,1),&i);
    test.push(AABB<double>(0,30,1,1),&i);
    test.push(AABB<double>(0,10,1,1),&i);
    test.push(AABB<double>(10,10,1,1),&i);
    test.push(AABB<double>(350,350,125,125),&i);
    test.push(AABB<double>(350,350,125,125),&i);
    test.push(AABB<double>(350,350,125,125),&i);
    test.push(AABB<double>(350,350,1,1),&i);
    test.push(AABB<double>(350,355,125,125),&i);
    test.push(AABB<double>(350,350,125,125),&i);
    test.push(AABB<double>(350,350,125,125),&i);
    test.push(AABB<double>(350,350,2,2),&i);
    test.display();
    auto res=test.queryAABB(AABB<double>(0,0,100,100));
    cout << res.size() << endl;

    return 0;
}