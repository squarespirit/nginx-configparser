#include <sstream>
#include <string>

#include "gtest/gtest.h"
#include "config_parser.h"

class NginxStringConfigTest : public ::testing::Test {
protected:
    bool ParseString(const std::string& config_string) {
        std::stringstream config_stream(config_string);
        return parser_.Parse(&config_stream, &config_);
    }
    NginxConfigParser parser_;
    NginxConfig config_;
};

TEST_F(NginxStringConfigTest, SimpleConfig) {
    EXPECT_TRUE(ParseString("foo bar;")) << 
            "Simple config parsed incorrectly.";
    EXPECT_EQ(1, config_.statements_.size()) << "Didn't get one statement.";
    EXPECT_EQ("foo", config_.statements_.at(0)->tokens_.at(0));
}

TEST_F(NginxStringConfigTest, EmptyConfig) {
    EXPECT_FALSE(ParseString("")) << "Got empty string wrong";
}

TEST_F(NginxStringConfigTest, BadEndingConfig) {
    EXPECT_FALSE(ParseString("foo bar")) << 
            "Config with no ending semicolon was actually parsed successfully";
    EXPECT_FALSE(ParseString("foo } ")) <<
            "Config with no opening brace was actually parsed successfully";
}

TEST_F(NginxStringConfigTest, SingleNestedConfig) {
    EXPECT_TRUE(ParseString("foo { bar baz wiggle; }")) << 
            "One-statement single nested config parsed incorrectly";
    EXPECT_TRUE(ParseString("foo { bar; baz wiggle; }")) << 
            "Two-statement single nested config parsed incorrectly";
}

TEST_F(NginxStringConfigTest, DoubleNestedConfig) {
    EXPECT_TRUE(ParseString("foo { bar { baz qux; } }")) <<
            "Double-nested config parsed incorrectly.";
}
