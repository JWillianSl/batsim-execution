#include <intervalset.hpp>

void usage_example()
{
    // Creation
    IntervalSet a; // empty
    IntervalSet b = IntervalSet::ClosedInterval(0,4); // [0,4] AKA {0,1,2,3,4}
    IntervalSet c = IntervalSet::from_string_hyphen("3-5,8"); // [3,5]∪[8] AKA {3,4,5,8}

    // Format
    c.to_string_brackets(); // "[3,5]∪[8]"
    c.to_string_hyphen(); // "3-5,8"
    c.to_string_elements(); // "3,4,5,8"

    // Set operations
    IntervalSet intersection_set = (a & c); // {}
    IntervalSet union_set = (b + c); // [0,5]∪[8]
    IntervalSet difference_set = (b - c); // [0,2]

    // In-place set operations
    a += IntervalSet::empty_interval_set(); // a remains empty
    // -= and &= are also defined.

    // Common operations
    int number_of_elements = c.size(); // 4
    int first_element = c.first_element(); // 3
    IntervalSet first_two_elements = c.left(2); // [3-4]
    IntervalSet two_random_elements = c.random_pick(2); // Two different random values from c
    c.contains(0); // false
    a.is_subset_of(c); // true
}
