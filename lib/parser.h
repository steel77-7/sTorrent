#ifndef PARSER_H
#define PARSER_H

#include<iostream>// For input/output operations
#include<string>// For std::string
#include<memory>// For std::shared_ptr
#include<variant> // For std::variant
#include<cctype> // For std::isdigit
#include<map> // For std::map
#include<vector> // For std::vector
struct Bencodingobject;//creating a struct 
using Bencodingptr=std::shared_ptr<Bencodingobject>;//defining Bencodingptr equals to the shared pointer(smart pointer(automatically manages memory) that is pointing to words the sruct)
struct Bencodingobject:public std::variant<//making struct inherit  from class variant 
 int,
 std::string,
 std::vector<Bencodingptr>, //created vector of type Bencodingptr
 std::map<std::string,Bencodingptr>>//created map where key is a string and value is Bencodingptr
{
    using variant::variant;// so that we can use variant's constructors
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
