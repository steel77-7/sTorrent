#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include <memory>
#include <variant>
#include <fstream>
#include <cctype>
#include <map>
#include <iterator>
#include <vector>

struct Bencodingobject;
using Bencodingptr = std::shared_ptr<Bencodingobject>;
struct Bencodingobject : public std::variant<
                             int,
                             std::string,
                             std::vector<Bencodingptr>,
                             std::map<std::string, Bencodingptr>>
{
    using variant ::variant;
};

class Bencodingparser
{
private:
    std::string data;
    int index;

public:
    Bencodingparser(const std::string &data) {}

    Bencodingptr decoder() {}

    int Decodeint() {}

    std::string decodestring() {}

    std::vector<Bencodingptr> decodelist() {}

    std::map<std::string, Bencodingptr> DecodeDictionary() {}
};

#endif;