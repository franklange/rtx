#include <rtx/message.hpp>

#include <gtest/gtest.h>

namespace rtx::test {

TEST(message_test, pack)
{
    EXPECT_EQ("foo.bar|baz", pack("foo.bar", "baz"));
}

TEST(message_test, unpack)
{
    const message exp{"foo.bar", "baz"};
    EXPECT_EQ(exp, unpack("foo.bar|baz"));
}

} // namespace Template::test
