#include "intervalset.hpp"

#include <random>
#include <vector>

#include <boost/algorithm/string.hpp>

using namespace std;

//! Create an empty IntervalSet.
IntervalSet::IntervalSet()
{
}

//! Create an IntervalSet made of a single interval.
IntervalSet::IntervalSet(const IntervalSet::ClosedInterval& interval)
{
    insert(interval);
}

//! Create an IntervalSet from another IntervalSet (copy constructor).
IntervalSet::IntervalSet(const IntervalSet& other)
{
    set = other.set;
}

//! Create an IntervalSet from a single integer.
IntervalSet::IntervalSet(int integer)
{
    insert(integer);
}

/**
 * @brief Iterator to beginning **element**.
 * @details **Note**: Iterating **intervals** is much more efficient
 *          (via intervals_begin() and intervals_end()).
 */
IntervalSet::element_const_iterator IntervalSet::elements_begin() const
{
    return boost::icl::elements_begin(set);
}

/**
 * @brief Iterator to ending **element**.
 * @details **Note**: Iterating **intervals** is much more efficient
 *          (via intervals_begin() and intervals_end()).
 */
IntervalSet::element_const_iterator IntervalSet::elements_end() const
{
    return boost::icl::elements_end(set);
}

//! Iterator to beginning **interval**.
IntervalSet::const_iterator IntervalSet::intervals_begin() const
{
    return set.begin();
}

//! Iterator to ending **interval**.
IntervalSet::const_iterator IntervalSet::intervals_end() const
{
    return set.end();
}

//! Remove all the elements in the IntervalSet. In other words, make it empty.
void IntervalSet::clear()
{
    set.clear();
}

//! Insert an IntervalSet in another. This is similar to operator+=(const IntervalSet &).
void IntervalSet::insert(const IntervalSet& interval_set)
{
    for (auto it = interval_set.intervals_begin(); it != interval_set.intervals_end(); ++it)
        set.insert(*it);
}

//! Insert a ClosedInterval in an IntervalSet.
void IntervalSet::insert(ClosedInterval interval)
{
    set.insert(interval);
}

//! Insert an integer in an IntervalSet.
void IntervalSet::insert(int integer)
{
    set.insert(integer);
}

//! Remove an IntervalSet from another. This is similar to operator-=(const Intervalset &).
void IntervalSet::remove(const IntervalSet& interval_set)
{
    set -= interval_set.set;
}

//! Remove a ClosedInterval from an IntervalSet.
void IntervalSet::remove(ClosedInterval interval)
{
    set -= interval;
}

//! Remove an integer from an IntervalSet.
void IntervalSet::remove(int integer)
{
    set -= integer;
}

/**
 * @brief Create a sub-IntervalSet made of the nb_integers leftmost elements of the source IntervalSet.
 * @pre The source IntervalSet must contains nb_integers or more elements.
 */
IntervalSet IntervalSet::left(unsigned int nb_integers) const
{
    if (set.size() < nb_integers) {
        throw std::out_of_range("Invalid IntervalSet::left call. "s + "Looking for leftmost "s + to_string(nb_integers) + "values in a set containing "s + to_string(set.size()) + "values."s);
    }

    // Find the value of the n-th element
    unsigned int nb_inserted = 0;
    IntervalSet res;

    for (auto it = intervals_begin(); it != intervals_end() && nb_inserted < nb_integers; ++it) {
        // The size of the current interval
        unsigned int interval_size = it->upper() - it->lower() + 1;
        unsigned int nb_to_add = std::min(interval_size,
            nb_integers - nb_inserted);

        res.insert(ClosedInterval(it->lower(), it->lower() + nb_to_add - 1));
        nb_inserted += nb_to_add;
    }

    return res;
}

/**
 * @brief Create a sub-IntervalSet made of nb_integers randomly-picked elements from the source IntervalSet.
 * @pre The source IntervalSet must contains nb_integers or more elements.
 */
IntervalSet IntervalSet::random_pick(unsigned int nb_integers) const
{
    if (set.size() < (size_t)nb_integers) {
        throw std::out_of_range("Invalid IntervalSet::random_pick call. "s + "Looking for random "s + to_string(nb_integers) + "values in a set containing "s + to_string(set.size()) + "values."s);
    }

    // Boost interval set -> vector of int
    vector<int> int_vector;
    for (auto value_it = elements_begin(); value_it != elements_end(); ++value_it) {
        int integer = *value_it;
        int_vector.push_back(integer);
    }

    // Shuffle the vector
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(int_vector.begin(), int_vector.end(), g);

    // Keep the first n elements
    IntervalSet res;
    for (unsigned int i = 0; i < nb_integers; ++i)
        res.insert(int_vector[i]);

    return res;
}

//! Returns a const iterator to the biggest ClosedInterval in an IntervalSet.
IntervalSet::const_iterator IntervalSet::biggest_interval() const
{
    if (size() == 0)
        return intervals_end();

    auto res = intervals_begin();
    int res_size = res->upper() - res->lower() + 1;

    for (auto interval_it = ++intervals_begin(); interval_it != intervals_end(); ++interval_it) {
        int interval_size = interval_it->upper() - interval_it->lower() + 1;

        if (interval_size > res_size) {
            res = interval_it;
            res_size = interval_size;
        }
    }

    return res;
}

/**
 * @brief Returns the value of the first element of an IntervalSet.
 * @pre The IntervalSet must **NOT** be empty.
 */
int IntervalSet::first_element() const
{
    if (set.size() < 1) {
        throw std::out_of_range(
            "Invalid IntervalSet::first_element call. The set is empty.");
    }
    return *elements_begin();
}

//! Returns the number of **elements** of an IntervalSet.
unsigned int IntervalSet::size() const
{
    return set.size();
}

//! Returns whether an IntervalSet is empty. An empty IntervalSet does not contain any element.
bool IntervalSet::is_empty() const
{
    return set.size() == 0;
}

//! Returns whether an IntervalSet contains an integer.
bool IntervalSet::contains(int integer) const
{
    return boost::icl::contains(set, integer);
}

//! Returns whether an IntervalSet fully contains a ClosedInterval.
bool IntervalSet::contains(const IntervalSet::ClosedInterval & interval) const
{
    for (auto it = intervals_begin(); it != intervals_end(); ++it)
    {
        if (boost::icl::contains(*it, interval))
        {
            return true;
        }
    }
    return false;
}

//! Returns whether an IntervalSet is a subset of another IntervalSet.
bool IntervalSet::is_subset_of(const IntervalSet & other) const
{
    // TODO: complexity...
    for (auto it = elements_begin(); it != elements_end(); ++it)
        if (!other.contains(*it))
            return false;

    return true;
}

/**
 * @brief Returns a string representation of an IntervalSet.
 * @details This is the classical representation used in mathematics.
 *          For example, {1,2,3,7} is represented as [1,3]∪[7].
 */
std::string IntervalSet::to_string_brackets(const std::string& union_str,
    const std::string& opening_bracket,
    const std::string& closing_bracket,
    const std::string& sep) const
{
    vector<string> integer_strings;

    if (size() == 0)
        integer_strings.push_back(opening_bracket + closing_bracket);
    else {
        for (auto it = intervals_begin(); it != intervals_end(); ++it)
            if (it->lower() == it->upper())
                integer_strings.push_back(opening_bracket + to_string(it->lower()) + closing_bracket);
            else
                integer_strings.push_back(opening_bracket + to_string(it->lower()) + sep + to_string(it->upper()) + closing_bracket);
    }

    return boost::algorithm::join(integer_strings, union_str);
}

/**
 * @brief Returns a string representation of an IntervalSet.
 * @details This is a compact representation where {1,2,3,7} is represented as 1-3,7.
 *          Use sep=' ' to get a Batsim-compatible representation
 *          (see <a href="https://batsim.readthedocs.io/en/latest/interval-set.html#interval-set-string-representation">Batsim documentation about Interval sets representation</a>).
 */
std::string IntervalSet::to_string_hyphen(const std::string& sep, const std::string& joiner) const
{
    vector<string> integer_strings;
    for (auto it = intervals_begin(); it != intervals_end(); ++it)
        if (it->lower() == it->upper())
            integer_strings.push_back(to_string(it->lower()));
        else
            integer_strings.push_back(to_string(it->lower()) + joiner + to_string(it->upper()));

    return boost::algorithm::join(integer_strings, sep);
}

/**
 * @brief Returns a string representation of an IntervalSet.
 * @details This is the set representation of an IntervalSet.
 *          For example, {1,2,3,7} is represented as 1,2,3,7
 */
string IntervalSet::to_string_elements(const string& sep) const
{
    vector<string> integer_strings;
    for (auto it = elements_begin(); it != elements_end(); ++it)
        integer_strings.push_back(to_string(*it));

    return boost::algorithm::join(integer_strings, sep);
}

//! Assignment operator. Reset an IntervalSet content to the one of another IntervalSet.
IntervalSet& IntervalSet::operator=(const IntervalSet& other)
{
    set = other.set;
    return *this;
}

//! Assignment operator. Reset an IntervalSet content to the one of a ClosedInterval.
IntervalSet& IntervalSet::operator=(const IntervalSet::ClosedInterval& interval)
{
    set.clear();
    set.insert(interval);
    return *this;
}

/**
 * @brief Returns whether two IntervalSet exactly contain the same elements.
 */
bool IntervalSet::operator==(const IntervalSet& other) const
{
    return set == other.set;
}

//! Returns whether the content of two IntervalSet is different.
bool IntervalSet::operator!=(const IntervalSet& other) const
{
    return set != other.set;
}

/**
 * @brief Intersection + assignment operator.
 * @details a ~= b; means "Set a's value to be the intersection of a and b".
 */
IntervalSet& IntervalSet::operator&=(const IntervalSet& other)
{
    set &= other.set;
    return *this;
}

/**
 * @brief Difference + assignment operator. This is similar to remove(const IntervalSet &).
 * @details a -= b; means "Set a's value to be a without the elements of b".
 */
IntervalSet& IntervalSet::operator-=(const IntervalSet& other)
{
    set -= other.set;
    return *this;
}

/**
 * @brief Union + assignment operator. This is similar to insert(const IntervalSet &).
 * @details a += b; means "Set a's value to be the union of a and b".
 */
IntervalSet& IntervalSet::operator+=(const IntervalSet& other)
{
    set += other.set;
    return *this;
}

//! Difference operator. a - b returns an IntervalSet of the elements that are in a but are not in b.
IntervalSet IntervalSet::operator-(const IntervalSet& other) const
{
    IntervalSet ret = *this;
    ret -= other;
    return ret;
}

//! Union operator. a + b returns an IntervalSet of the elements that are in a, in b, or both in a and b.
IntervalSet IntervalSet::operator+(const IntervalSet& other) const
{
    IntervalSet ret = *this;
    ret += other;
    return ret;
}

//! Intersection operator. a & b returns an IntervalSet of the elements that are both in a and b.
IntervalSet IntervalSet::operator&(const IntervalSet& other) const
{
    IntervalSet ret = *this;
    ret &= other;
    return ret;
}

//! Returns an empty IntervalSet.
IntervalSet IntervalSet::empty_interval_set()
{
    return IntervalSet();
}

/**
 * @brief Parse an IntervalSet string representation and return the corresponding IntervalSet.
 * @details See IntervalSet::to_string_hyphen for representation details.
 */
IntervalSet IntervalSet::from_string_hyphen(const string& str,
    const string& sep, const string& joiner)
{
    IntervalSet res;

    // Split by sep to get all parts
    vector<string> parts;
    boost::split(parts, str, boost::is_any_of(sep), boost::token_compress_on);

    for (const string& part : parts) {
        // Each part can either be a single value or a closed interval
        vector<string> interval_parts;
        boost::split(interval_parts, part, boost::is_any_of(joiner),
            boost::token_compress_on);

        if (interval_parts.size() != 1 && interval_parts.size() != 2) {
            throw std::invalid_argument(
                "Invalid IntervalSet::from_string_hyphen call. "s + "Part '"s + part + "' cannot be parsed. "s + "It should either be a single integer (e.g., '42') "s + "or a closed interval "s + "(e.g., '42-44' to represent {42,43,44})."s);
        }

        if (interval_parts.size() == 1) {
            int integer = std::stoi(interval_parts[0]);
            res.insert(integer);
        } else {
            int valueIDa = std::stoi(interval_parts[0]);
            int valueIDb = std::stoi(interval_parts[1]);

            if (valueIDa > valueIDb) {
                throw std::invalid_argument(
                    "Invalid IntervalSet::from_string_hyphen call. "s + "Part '"s + part + "' cannot be parsed. "s + "First value ("s + interval_parts[0] + ") should NOT "s + "be greater than second value ("s + interval_parts[1] + ")"s);
            }

            res.insert(IntervalSet::ClosedInterval(valueIDa, valueIDb));
        }
    }

    return res;
}

/**
 * @brief Subscript operator.
 * @details Returns the index-th element of the IntervalSet.
 * @pre index must be positive and strictly smaller than size()
 */
int IntervalSet::operator[](int index) const
{
    if (index < 0 || index >= (int)this->size()) {
        throw std::out_of_range("Invalid IntervalSet::operator[] call. "s + "Looking for value at index="s + to_string(index) + " in a set containing "s + to_string(set.size()) + "values."s);
    }

    // TODO: avoid O(n) retrieval
    auto value_it = this->elements_begin();
    for (int i = 0; i < index; ++i)
        ++value_it;

    return *value_it;
}
