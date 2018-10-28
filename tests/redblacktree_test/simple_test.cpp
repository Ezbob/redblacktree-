#include <redblacktree.hpp>

int main()
{
    RedBlackTree::Tree<int, std::string> t;
    std::string v{"hello"};
    std::string v2{"hello23"};
    std::string v3{"Plah"};

    auto a = {2, 3, 42, 1, 23, 0, -1};

    for ( auto elm : a ) {
        if ( elm == 42 ) {
            t.insert(elm, &v3);
        }  else if ( elm == -1 ) {
            t.insert(elm, &v2);
        } else {
            t.insert(elm, &v);  
        }
    }

    std::cout << t << std::endl;

    std::cout << "min: " << *t.minimum() << " max: " << *t.maximum() << std::endl; 

    std::cout << "--->" << *t.get(-1) << std::endl;

    std::cout << *t.maximum() << std::endl;

    for ( auto elm : a ) {
        std::cout << "removing element: " << elm << std::endl;
        if ( t.remove(elm) ) {
            std::cout << "Huzzarh!" << std::endl;
            std::cout << t << std::endl;
        } else {
            std::cout << "Buhuh" << std::endl;
        }
    }

    t.del();
    return 0;
}
