#include <gtest/gtest.h>

#include <intervalset.hpp>

TEST(IntervalSet, constructor)
{
    IntervalSet s1;
    EXPECT_EQ(s1.size(), 0);
    EXPECT_EQ(s1.is_empty(), true);
    EXPECT_THROW(s1.first_element(), std::out_of_range);

    IntervalSet s2 = s1;
    EXPECT_EQ(s2.size(), 0);
    EXPECT_EQ(s2.is_empty(), true);
    EXPECT_EQ(s2, s1);

    IntervalSet s3 = IntervalSet::ClosedInterval(0,1);
    EXPECT_EQ(s3.size(), 2);
    EXPECT_EQ(s3.is_empty(), false);
    EXPECT_EQ(s3.first_element(), 0);
    EXPECT_TRUE(s3.contains(1));

    IntervalSet s4 = 2;
    EXPECT_EQ(s4.size(), 1);
    EXPECT_EQ(s4.is_empty(), false);
    EXPECT_EQ(s4.first_element(), 2);
    EXPECT_TRUE(s4.contains(2));
}

TEST(IntervalSet, clear)
{
    IntervalSet s1;
    IntervalSet s2 = 4;
    IntervalSet s3 = IntervalSet::ClosedInterval(0,10);
    IntervalSet s4 = IntervalSet::from_string_hyphen("0,3-5,13-17");

    EXPECT_NE(s2, s1);
    s2.clear();
    EXPECT_EQ(s2, s1);

    EXPECT_NE(s3, s1);
    s3.clear();
    EXPECT_EQ(s3, s1);

    EXPECT_NE(s4, s1);
    s4.clear();
    EXPECT_EQ(s4, s1);
}

TEST(IntervalSet, insert)
{
    IntervalSet s1;
    IntervalSet s2 = s1;
    EXPECT_EQ(s1, IntervalSet::empty_interval_set());

    s1.insert(4);
    s2 += 4;
    EXPECT_EQ(s1, IntervalSet::from_string_hyphen("4"));
    EXPECT_EQ(s2, s1);

    s1.insert(IntervalSet::ClosedInterval(2,7));
    s2 += IntervalSet::ClosedInterval(2,7);
    EXPECT_EQ(s1, IntervalSet::from_string_hyphen("2-7"));
    EXPECT_EQ(s2, s1);

    s1.insert(IntervalSet::from_string_hyphen("0-1,4-8,10"));
    s2 += IntervalSet::from_string_hyphen("0-1,4-8,10");
    EXPECT_EQ(s1, IntervalSet::from_string_hyphen("0-8,10"));
    EXPECT_EQ(s2, s1);
}

TEST(IntervalSet, remove)
{
    IntervalSet s1 = IntervalSet::from_string_hyphen("4-7,10,17-23");
    IntervalSet s2 = s1;

    s1.remove(5);
    s2 -= 5;
    EXPECT_EQ(s1, IntervalSet::from_string_hyphen("4,6-7,10,17-23"));
    EXPECT_EQ(s2, s1);

    s1.remove(IntervalSet::ClosedInterval(7, 13));
    s2 -= IntervalSet::ClosedInterval(7, 13);
    EXPECT_EQ(s1, IntervalSet::from_string_hyphen("4,6,17-23"));
    EXPECT_EQ(s2, s1);

    s1.remove(IntervalSet::from_string_hyphen("6,17-18,22"));
    s2 -= IntervalSet::from_string_hyphen("6,17-18,22");
    EXPECT_EQ(s1, IntervalSet::from_string_hyphen("4,19-21,23"));
    EXPECT_EQ(s2, s1);
}

TEST(IntervalSet, left)
{
    IntervalSet s1;
    EXPECT_THROW(s1.left(1), std::out_of_range);

    s1 = IntervalSet::ClosedInterval(5,10);
    EXPECT_THROW(s1.left(7), std::out_of_range);
    EXPECT_EQ(s1.left(3), IntervalSet::from_string_hyphen("5-7"));

    s1 = IntervalSet::from_string_hyphen("5,7,11-13");
    EXPECT_THROW(s1.left(6), std::out_of_range);
    EXPECT_EQ(s1.left(4), IntervalSet::from_string_hyphen("5,7,11-12"));
}

TEST(IntervalSet, random_pick)
{
    IntervalSet s1;
    EXPECT_THROW(s1.random_pick(1), std::out_of_range);

    s1 = IntervalSet::ClosedInterval(5,10);
    EXPECT_THROW(s1.random_pick(7), std::out_of_range);
    for (int i = 0; i < 6; i++)
        EXPECT_TRUE(s1.random_pick(i).is_subset_of(s1));

    s1 = IntervalSet::from_string_hyphen("5,7,11-13");
    EXPECT_THROW(s1.random_pick(6), std::out_of_range);
    for (int i = 0; i < 5; i++)
        EXPECT_TRUE(s1.random_pick(i).is_subset_of(s1));
}

TEST(IntervalSet, biggest_interval)
{
    IntervalSet s1;
    EXPECT_EQ(s1.biggest_interval(), s1.intervals_end());

    s1 = IntervalSet::from_string_hyphen("5-10");
    EXPECT_NE(s1.biggest_interval(), s1.intervals_end());
    EXPECT_EQ(IntervalSet(*s1.biggest_interval()),
        IntervalSet(IntervalSet::ClosedInterval(5,10)));

    s1 = IntervalSet::from_string_hyphen("3,4-7,10-20,22,24-28");
    EXPECT_NE(s1.biggest_interval(), s1.intervals_end());
    EXPECT_EQ(IntervalSet(*s1.biggest_interval()),
        IntervalSet(IntervalSet::ClosedInterval(10,20)));
}

TEST(IntervalSet, contains)
{
    IntervalSet s1 = IntervalSet::from_string_hyphen("3,4-7,10-20,22,24-28");

    for (int i = 0; i < 50; i++)
    {
        bool expected_contains = (i == 3) ||
                                 (i >= 4 && i <= 7) ||
                                 (i >= 10 && i <= 20) ||
                                 (i == 22) ||
                                 (i >= 24 && i <= 28);
        EXPECT_EQ(s1.contains(i), expected_contains);
    }

    // Exact intervals
    EXPECT_TRUE(s1.contains(IntervalSet::ClosedInterval(3)));
    EXPECT_TRUE(s1.contains(IntervalSet::ClosedInterval(4,7)));
    EXPECT_TRUE(s1.contains(IntervalSet::ClosedInterval(10,20)));
    EXPECT_TRUE(s1.contains(IntervalSet::ClosedInterval(22)));
    EXPECT_TRUE(s1.contains(IntervalSet::ClosedInterval(24,28)));

    // Sub intervals
    EXPECT_TRUE(s1.contains(IntervalSet::ClosedInterval(4,5)));
    EXPECT_TRUE(s1.contains(IntervalSet::ClosedInterval(5,7)));
    EXPECT_TRUE(s1.contains(IntervalSet::ClosedInterval(5,6)));

    // Larger intervals
    EXPECT_FALSE(s1.contains(IntervalSet::ClosedInterval(9,10)));
    EXPECT_FALSE(s1.contains(IntervalSet::ClosedInterval(10,22)));
    EXPECT_FALSE(s1.contains(IntervalSet::ClosedInterval(3,28)));
}

TEST(IntervalSet, is_subset_of)
{
    IntervalSet s1 = IntervalSet::ClosedInterval(4,7);
    IntervalSet s2 = IntervalSet::ClosedInterval(0,10);
    const auto empty = IntervalSet::empty_interval_set();

    EXPECT_TRUE(empty.is_subset_of(empty));
    EXPECT_TRUE(empty.is_subset_of(s1));
    EXPECT_TRUE(s1.is_subset_of(s1));
    EXPECT_FALSE(s1.is_subset_of(empty));

    EXPECT_TRUE(s1.is_subset_of(s1));
    EXPECT_TRUE(s1.is_subset_of(s2));
    EXPECT_TRUE(s2.is_subset_of(s2));
    EXPECT_FALSE(s2.is_subset_of(s1));
}

TEST(IntervalSet, to_string)
{
    IntervalSet s1;
    EXPECT_EQ(s1.to_string_brackets(), "[]");
    EXPECT_EQ(s1.to_string_hyphen(), "");
    EXPECT_EQ(s1.to_string_elements(), "");

    s1 = IntervalSet::from_string_hyphen("5-10");
    EXPECT_EQ(s1.to_string_brackets(), "[5,10]");
    EXPECT_EQ(s1.to_string_hyphen(), "5-10");
    EXPECT_EQ(s1.to_string_elements(), "5,6,7,8,9,10");

    s1 = IntervalSet::from_string_hyphen("3,4-7,10-20,22,24-28");
    EXPECT_EQ(s1.to_string_brackets(), "[3,7]∪[10,20]∪[22]∪[24,28]");
    EXPECT_EQ(s1.to_string_hyphen(), "3-7,10-20,22,24-28");
    EXPECT_EQ(s1.to_string_elements(),
        "3,4,5,6,7,10,11,12,13,14,15,16,17,18,19,20,22,24,25,26,27,28");
}


TEST(IntervalSet, set_intersection)
{
    IntervalSet s1, s2;
    EXPECT_EQ((s1 & s2), IntervalSet::empty_interval_set());
    s1 &= s2;
    EXPECT_EQ(s1, IntervalSet::empty_interval_set());

    s1 = IntervalSet::ClosedInterval(3,5);
    s2 = IntervalSet::empty_interval_set();
    EXPECT_EQ((s1 & s2), IntervalSet::empty_interval_set());
    s1 &= s2;
    EXPECT_EQ(s1, IntervalSet::empty_interval_set());

    s1 = IntervalSet::from_string_hyphen("3,4-7,10-20,22,24-28");
    s2 = IntervalSet::from_string_hyphen("4,19-21,23");
    EXPECT_EQ((s1 & s2), IntervalSet::from_string_hyphen("4,19-20"));
    s1 &= s2;
    EXPECT_EQ(s1, IntervalSet::from_string_hyphen("4,19-20"));
}


TEST(IntervalSet, set_union)
{
    IntervalSet s1, s2;
    EXPECT_EQ((s1 + s2), IntervalSet::empty_interval_set());
    s1 += s2;
    EXPECT_EQ(s1, IntervalSet::empty_interval_set());

    s1 = IntervalSet::ClosedInterval(3,5);
    s2 = IntervalSet::empty_interval_set();
    EXPECT_EQ((s1 + s2), IntervalSet(IntervalSet::ClosedInterval(3,5)));
    s1 += s2;
    EXPECT_EQ(s1, IntervalSet(IntervalSet::ClosedInterval(3,5)));

    s1 = IntervalSet::from_string_hyphen("3,4-7,10-20,22,24-28");
    s2 = IntervalSet::from_string_hyphen("4,19-21,23");
    EXPECT_EQ((s1 + s2), IntervalSet::from_string_hyphen("3-7,10-28"));
    s1 += s2;
    EXPECT_EQ(s1, IntervalSet::from_string_hyphen("3-7,10-28"));
}


TEST(IntervalSet, set_difference)
{
    IntervalSet s1, s2;
    EXPECT_EQ((s1 - s2), IntervalSet::empty_interval_set());
    s1 -= s2;
    EXPECT_EQ(s1, IntervalSet::empty_interval_set());

    s1 = IntervalSet::ClosedInterval(3,5);
    s2 = IntervalSet::empty_interval_set();
    EXPECT_EQ((s1 - s2), IntervalSet(IntervalSet::ClosedInterval(3,5)));
    s1 -= s2;
    EXPECT_EQ(s1, IntervalSet(IntervalSet::ClosedInterval(3,5)));

    s1 = IntervalSet::from_string_hyphen("3,4-7,10-20,22,24-28");
    s2 = IntervalSet::from_string_hyphen("4,19-21,23");
    EXPECT_EQ((s1 - s2),
        IntervalSet::from_string_hyphen("3, 5-7,10-18,22,24-28"));
    s1 -= s2;
    EXPECT_EQ(s1,
        IntervalSet::from_string_hyphen("3, 5-7,10-18,22,24-28"));
}

TEST(IntervalSet, element_traversal)
{
    IntervalSet s = IntervalSet::from_string_hyphen("4,19-21,23");
    static const int expected_values[] = {4,19,20,21,23};
    int i = 0;
    for (auto it = s.elements_begin(); it != s.elements_end(); it++, i++)
    {
        EXPECT_EQ(*it, expected_values[i]);
    }
}

TEST(IntervalSet, interval_traversal)
{
    IntervalSet s = IntervalSet::from_string_hyphen("4,19-21,23");
    static const IntervalSet::ClosedInterval expected_values[] = {
        IntervalSet::ClosedInterval(4),
        IntervalSet::ClosedInterval(19,21),
        IntervalSet::ClosedInterval(23)
    };
    int i = 0;
    bool failed_comparison = false;
    for (auto it = s.intervals_begin(); it != s.intervals_end(); it++, i++)
    {
        if (*it != expected_values[i])
        {
            FAIL();
        }
    }
}

TEST(IntervalSet, from_string_hyphen)
{
    IntervalSet s = IntervalSet::from_string_hyphen("4,19-21,23");
    IntervalSet expected = 4;
    expected += IntervalSet::ClosedInterval(19,21);
    expected += 23;
    EXPECT_EQ(s, expected);

    EXPECT_THROW(IntervalSet::from_string_hyphen("meh"), std::invalid_argument);
    EXPECT_THROW(IntervalSet::from_string_hyphen(",4"), std::invalid_argument);
    EXPECT_THROW(IntervalSet::from_string_hyphen("1-3-5"), std::invalid_argument);
    EXPECT_THROW(IntervalSet::from_string_hyphen("4-1"), std::invalid_argument);
}

TEST(IntervalSet, equality)
{
    IntervalSet s1 = IntervalSet::from_string_hyphen("4,19-21,23");
    IntervalSet s2 = s1;

    // They are equal.
    EXPECT_EQ(s1, s2);
    EXPECT_TRUE(s1 == s2);
    EXPECT_FALSE(s1 != s2);
    for (auto it1 = s1.intervals_begin(), it2 = s2.intervals_begin();
         it1 != s1.intervals_end() && it2 != s2.intervals_end();
         ++it1, ++it2)
    {
        EXPECT_TRUE(*it1 == *it2);
        EXPECT_FALSE(*it1 != *it2);
    }

    // They are different.
    s2 -= 4;
    EXPECT_NE(s1, s2);
    EXPECT_FALSE(s1 == s2);
    EXPECT_TRUE(s1 != s2);
    for (auto it1 = s1.intervals_begin(), it2 = s2.intervals_begin();
         it1 != s1.intervals_end() && it2 != s2.intervals_end();
         ++it1, ++it2)
    {
        EXPECT_FALSE(*it1 == *it2);
        EXPECT_TRUE(*it1 != *it2);
    }
}

TEST(IntervalSet, random_access_operator)
{
    EXPECT_THROW(IntervalSet::empty_interval_set()[0], std::out_of_range);

    IntervalSet s1 = IntervalSet::from_string_hyphen("4,19-21,23");
    EXPECT_THROW(s1[-1], std::out_of_range);
    EXPECT_EQ(s1[0], 4);
    EXPECT_EQ(s1[1], 19);
    EXPECT_EQ(s1[2], 20);
    EXPECT_EQ(s1[3], 21);
    EXPECT_EQ(s1[4], 23);
    EXPECT_THROW(s1[5], std::out_of_range);
}


void usage_example();
TEST(IntervalSet, docs_usage)
{
    usage_example();
}

void constructors_example();
TEST(IntervalSet, docs_constructors)
{
    constructors_example();
}

void set_operations_example();
TEST(IntervalSet, docs_set_operations)
{
    set_operations_example();
}

void membership_example();
TEST(IntervalSet, docs_membership)
{
    membership_example();
}

void access_example();
TEST(IntervalSet, docs_access)
{
    access_example();
}

void traversal_example();
TEST(IntervalSet, docs_traversal)
{
    traversal_example();
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
