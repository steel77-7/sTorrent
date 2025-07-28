#include <iostream>
#include <random>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
using namespace std;

/* string peer_id_gen()
{

    std::string peer_id = "-UT2025-";
    const int suffix_length = 20 - peer_id.size();
    const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, sizeof(charset) - 2);

    for (int i = 0; i < suffix_length; i++)
    {
        peer_id += charset[dist(gen)]; //for good randomness
    }
    return peer_id;
} */


std::string peer_id_gen() {
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    return to_string(uuid);
}