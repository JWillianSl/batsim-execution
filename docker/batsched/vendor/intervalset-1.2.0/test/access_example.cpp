#include <intervalset.hpp>

void access_example()
{
    IntervalSet s = IntervalSet::from_string_hyphen("3,10-16");

    s.first_element(); // 3
    s.left(1); // 3
    s.left(2); // {3,10}
    s.left(4); // {3,10,11,12}
    s.random_pick(2); // Two different random elements from s

    // Access can be done via operator[]
    // WARNING: This is very slow! Use iterators if performance matters.
    s[0]; // 3
    s[4]; // 13
}
