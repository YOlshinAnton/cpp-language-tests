//
//  main.cpp
//  Testing.cpp-language-tests
//
//  Created by Антон Ёлшин on 06.01.2024.
//

#include <gtest/gtest.h>

#include <cctype>
#include <limits>
#include <fstream>
#include <iostream>

bool IsNumber(std::istream &stream) {
  if (std::isdigit(stream.peek())) {
      return true;
  }

  bool isNumber = false;
  if (stream.peek() == '-') {
      int ch1 = stream.get();
      int ch2 = stream.get();
      int ch3 = stream.get();
      if (std::isdigit(ch2) || ch2 == '.' && std::isdigit(ch3))
      {
      isNumber = true;
      }
      stream.putback(ch3);
      stream.putback(ch2);
      stream.putback(ch1);
  } else if (stream.peek() == '.') {
      int ch1 = stream.get();
      int ch2 = stream.get();
      if (ch1 == '.' && std::isdigit(ch2))
      {
      isNumber = true;
      }
      stream.putback(ch2);
      stream.putback(ch1);
  }
  return isNumber;
}

int SkipSpace(std::istream &stream)
{
    if (std::isspace(stream.peek())) {
        char ch;
        while (std::isspace((ch = stream.get()))) {}
        stream.putback(ch);
    }
    return stream.peek();
}

bool isSeparator(int c) {
  return c == ',';
}
bool isNewLine(int c) {
  return c == '\n';
}

/// @brief 
/// @throw ios::failure "Bad character"
/// @throw ios::failure "Missed separator"
double ReadNextCSVNumber(std::istream &stream)
{
    double number;

    if (!IsNumber(stream)) {
        std::stringstream ss;
        ss << "Bad character " << "'" << (char)stream.peek() << "'(" << stream.peek() << ")" << " at " << stream.tellg() << ".";
        throw std::ios::failure(ss.str());
    }

    stream >> number;

    int separator = stream.get();
    if (!stream.eof() && (!isSeparator(separator) && !isNewLine(separator))) {
        std::stringstream ss;
        ss << "',' or new line expected. Gotten '" << (char)separator << "' at " << stream.tellg() << ".";
        throw std::ios::failure(ss.str());
    }
    if (isNewLine(separator)) {
        stream.putback(separator);
    }

    return number;
}

TEST(istream, ReadNumber)
{
    std::istringstream ss("., 10, .10\n 10.10, a, 0");
    std::istream& stream = ss;
    
    EXPECT_THROW(ReadNextCSVNumber(stream), std::ios::failure);

    char buff[256];
    stream.read(buff, 3);

    EXPECT_DOUBLE_EQ(10, ReadNextCSVNumber(stream));
    SkipSpace(stream);
    
    EXPECT_DOUBLE_EQ(0.1, ReadNextCSVNumber(stream));
    SkipSpace(stream);

    EXPECT_DOUBLE_EQ(10.1, ReadNextCSVNumber(stream));
    SkipSpace(stream);

    EXPECT_THROW(ReadNextCSVNumber(stream), std::ios::failure);
    stream.read(buff, 3);

    EXPECT_DOUBLE_EQ(0, ReadNextCSVNumber(stream));

    stream.get();
    EXPECT_TRUE(stream.eof());
}

int main(int argc, char **argv) {
    
    std::cout << "c++17 TESTS" << std::endl;
    
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
