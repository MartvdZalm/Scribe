#include "editor/Config.hpp"
#include <gtest/gtest.h>

#include <iostream>

TEST(ConfigTest, DefaultConstructor)
{
	Config config;
	EXPECT_EQ(config.x, 0);
	EXPECT_EQ(config.y, 0);
	EXPECT_EQ(config.rowOff, 0);
	EXPECT_EQ(config.colOff, 0);
	EXPECT_EQ(config.screenRows, 0);
	EXPECT_EQ(config.screenCols, 0);
}

TEST(ConfigTest, GetCursorPosition)
{
	Config config;

	std::cout << config.x << std::endl;
}