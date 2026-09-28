#include <intervalset.hpp>

void traversal_example()
{
    IntervalSet s = IntervalSet::from_string_hyphen("4,19-21,23");

    // The intervals can be traversed
    for (auto it = s.intervals_begin(); it != s.intervals_end(); ++it)
    {
        // Use operator* to retrieve the interval
        const IntervalSet::ClosedInterval & itv = *it;

        // The two bounding elements can be retrieved this way
        int interval_minimum_element = lower(itv);
        int interval_maximum_element = upper(itv);
    }

    // The individual values can also be traversed
    // Please DO note that this may be way slower than iterating over intervals
    for (auto it = s.elements_begin(); it != s.elements_end(); ++it)
    {
        // Use operator* to retrieve the element value
        int element = *it;
    }
}
