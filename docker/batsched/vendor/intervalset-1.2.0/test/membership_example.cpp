#include <intervalset.hpp>

void membership_example()
{
    IntervalSet s1 = IntervalSet::from_string_hyphen("3,4-7,10-20,22,24-28");
    IntervalSet s2 = IntervalSet::from_string_hyphen("5-6,15,19,28");

    s1.contains(4); // Does s1 contains 4? Yup.
    s1.contains(IntervalSet::ClosedInterval(8,25)); // Nope.
    s2.is_subset_of(s1); // Yup, s2 ⊆ s1
}
