#include <string>
#include "QuadTree.h"

using namespace YAQT;
int main(int argc, char * argv[])
{
    YAQT::QuadTree<int> test( AABB<double>(0,0,500,500)  );
    int i;
    test.push(AABB<double>(10,10,1,1),&i);

    return 0;
}