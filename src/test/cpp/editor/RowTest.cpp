#include "editor/Row.hpp"
#include <gtest/gtest.h>

TEST(RowTest, DefaultConstructor)
{
    Row row;
    EXPECT_EQ(row.getSize(), 0);
    EXPECT_EQ(row.getString(), "");
}

TEST(RowTest, SetAndGetString)
{
    Row row;
    row.setString("hello");
    EXPECT_EQ(row.getString(), "hello");
    EXPECT_EQ(row.getSize(), 5);
}

TEST(RowTest, InsertChar)
{
    Row row;
    row.setString("test");
    row.insertChar(1, 'X');
    EXPECT_EQ(row.getString(), "tXest");
}

TEST(RowTest, InsertCharIntoEmpty)
{
    Row row;
    row.insertChar(0, 'A');
    EXPECT_EQ(row.getString(), "A");
}

TEST(RowTest, InsertString)
{
    Row row;
    row.setString("test");
    row.insertString(2, "XX");
    EXPECT_EQ(row.getString(), "teXXst");
}

TEST(RowTest, InsertStringIntoEmpty)
{
    Row row;
    row.insertString(0, "hello");
    EXPECT_EQ(row.getString(), "hello");
}

