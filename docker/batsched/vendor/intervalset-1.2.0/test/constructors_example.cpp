#include <intervalset.hpp>

void constructors_example()
{
    // Create an empty IntervalSet
    IntervalSet s1;

    // Copy an existing IntervalSet
    IntervalSet s2 = s1;

    // Create an IntervalSet from one interval
    IntervalSet s3 = IntervalSet::ClosedInterval(0,1);

    // Create an IntervalSet from one integer
    IntervalSet s4 = 2;
}
