#include <gtest/gtest.h>

#include "similarity/bf_hash.h"

using namespace grgl;

TEST(BloomFilter, BFHashEq) {
    BFHash bf1(1024);
    BFHash bf2(1024);

    size_t item = 101;
    bf1.addItem(item);
    bf2.addItem(item);
    item = 199;
    bf1.addItem(item);
    bf2.addItem(item);
    item = 0;
    bf1.addItem(item);
    bf2.addItem(item);

    ASSERT_EQ(bf1.vector(), bf2.vector());
    item = 66;
    bf2.addItem(item);
    ASSERT_NE(bf1.vector(), bf2.vector());
}
