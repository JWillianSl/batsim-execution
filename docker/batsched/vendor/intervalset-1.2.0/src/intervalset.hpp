#pragma once

#include <string>

#include <boost/icl/closed_interval.hpp>
#include <boost/icl/interval_set.hpp>

struct IntervalSet {
public:
    /**
     * @brief A closed interval of integers.
     */
    typedef boost::icl::closed_interval<int, std::less> ClosedInterval;
    typedef boost::icl::interval_set<int,
        ICL_COMPARE_INSTANCE(ICL_COMPARE_DEFAULT,
            int),
        ClosedInterval>
        Set;
    typedef Set::element_const_iterator element_const_iterator;
    typedef Set::const_iterator const_iterator;

public:
    IntervalSet();
    IntervalSet(const ClosedInterval& interval);
    IntervalSet(const IntervalSet& other);
    IntervalSet(int integer);

    // Traverse elements of the set. This is slow. Prefer interval traversal.
    element_const_iterator elements_begin() const;
    element_const_iterator elements_end() const;

    // Traverse the intervals of the set.
    const_iterator intervals_begin() const;
    const_iterator intervals_end() const;

    void clear();
    void insert(const IntervalSet& interval_set);
    void insert(ClosedInterval interval);
    void insert(int integer);

    void remove(const IntervalSet& interval_set);
    void remove(ClosedInterval interval);
    void remove(int integer);

    IntervalSet left(unsigned int nb_integers) const;
    IntervalSet random_pick(unsigned int nb_integers) const;

    const_iterator biggest_interval() const;

    int first_element() const;
    unsigned int size() const;
    bool is_empty() const;
    bool contains(int integer) const;
    bool contains(const ClosedInterval & interval) const;
    bool is_subset_of(const IntervalSet & other) const;

    std::string to_string_brackets(const std::string& union_str = "∪",
        const std::string& opening_bracket = "[",
        const std::string& closing_bracket = "]",
        const std::string& sep = ",") const;
    std::string to_string_hyphen(const std::string& sep = ",",
        const std::string& joiner = "-") const;
    std::string to_string_elements(const std::string& sep = ",") const;

    IntervalSet& operator=(const IntervalSet& other);
    IntervalSet& operator=(const IntervalSet::ClosedInterval& interval);

    bool operator==(const IntervalSet& other) const;
    bool operator!=(const IntervalSet& other) const;

    IntervalSet& operator&=(
        const IntervalSet& other); // a &= b  <==> a = intersection(a, b)
    IntervalSet& operator-=(
        const IntervalSet& other); // a -= b  <==> a = a \ b (set difference)
    IntervalSet& operator+=(
        const IntervalSet& other); // a += b  <==> a = union(a,b)

    IntervalSet operator-(
        const IntervalSet& other) const; // a - b <==> a \ b (set difference)
    IntervalSet operator+(
        const IntervalSet& other) const; // a + b <==> union(a,b)
    IntervalSet operator&(
        const IntervalSet& other) const; // a & b <==> intersection(a,b)

    int operator[](int index) const;

public:
    static IntervalSet from_string_hyphen(
        const std::string& str,
        const std::string& sep = ",",
        const std::string& joiner = "-");
    static IntervalSet empty_interval_set();

private:
    Set set;
};
