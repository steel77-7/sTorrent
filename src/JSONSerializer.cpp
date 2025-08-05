#include "JSONSerializer.h"
#pragma once
using namespace std;
using json = nlohmann ::json;

void from_json(const json &j, peerInfo &p)
{
    j.at("ip").get_to(p.ip);
    j.at("peer_id").get_to(p.peer_id);
    j.at("info_hash").get_to(p.info_hash);
    j.at("port").get_to(p.port);
}

void to_json(json &j, const peerInfo p)
{
    j = json{{"info_hash", p.info_hash},
             {"ip", p.ip},
             {"peer_id", p.peer_id},
             {"port", p.port}};
}

void from_json(const json &j, Message &m)
{
    j.at("success").get_to(m.success);
    j.at("type").get_to(m.type);
    j.at("message").get_to(m.message);
}
void to_json(json &j,const  Message &m)
{
    j = json{
        {"success", m.success},
        {"type", m.type},
        {"message", m.message},
    };
}

void from_json(const json &j, block &b)
{
    j.at("piece_id").get_to(b.piece_id);
    j.at("offset").get_to(b.offset);
    j.at("size").get_to(b.size);
    j.at("status").get_to(b.status);
}

void to_json(json &j,const  block &b)
{
    j = json{
        {"piece_id", b.piece_id},
        {"offset", b.offset},
        {"size", b.size},
        {"status", b.status}

    };
}

void from_json(const json &j,  send_file &b)
{
    j.at("block_info").get_to(b.block_info);
    j.at("data").get_to(b.data);
}

void to_json(json &j, const send_file &b)
{
    j = json{
        {"block_info", b.block_info},
        {"data", b.data}

    };
}
