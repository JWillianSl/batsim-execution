#include <intervalset.hpp>

void set_operations_example()
{
    IntervalSet s1 = IntervalSet::from_string_hyphen("3,4-7,10-20,22,24-28");
    IntervalSet s2 = IntervalSet::from_string_hyphen("4,19-21,23");

    // Classical set operations
    IntervalSet s_union = (s1 + s2);
    IntervalSet s_intersection = (s1 & s2);
    IntervalSet s_difference = (s1 - s2);

    // In-place operations
    s1 += s2; // s1 = s1 ∪ s2
    s1 &= s2; // s1 = s1 ∩ s2
    s1 -= s2; // s1 = s1 \ s2
}
