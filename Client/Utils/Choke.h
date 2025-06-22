#ifndef CHOKE_H
#define CHOKE_h

#include <iostream> 
#include <unistd.h> 
#include "JSONSerializer.cpp"
#include "../../lib/Peerjoin.h"


class Choke{ 
    private : 
        PeerManager *p  ; 
    public : 
    Choke(PeerManager* p ); 
    void choking(); 

};

#endif