#include "../lib/parser.h"


class Bencodingparser
{
    private:
    std::string data;
    int index;

    public:
    Bencodingparser(const std::string& data){
        this->data=data;
        this->index=0;
    }
     Bencodingptr decoder()
     { if(data[index]=='i') return std::make_shared< Bencodingobject>(Decodeint());// returns shared pointer to the value that decodeint fxn returns 
       if(std::isdigit(data[index])) return std::make_shared< Bencodingobject>(decodestring());// returns shared pointer to the value that decodestring() fxn returns 
       if(data[index]=='l') return std::make_shared< Bencodingobject>(decodelist());// returns shared pointer to the value that decodelist() fxn returns 
       if(data[index]=='d') return std::make_shared< Bencodingobject>( DecodeDictionary());// returns shared pointer to the value that DecodeDictionary() fxn returns 
       throw std::runtime_error("Invalid Bencode input");
     }
      
      
      int Decodeint(){// return type int
        ++index;
        int end= data.find('e',index);// finds e and then stores that index in end
        int length = std::stoi(data.substr(index,end-index));//gets line from index index to end-index and then converts it to int
        index=end+1;
        return length;
      }
    
      std::string decodestring(){// return type  std::string
        int size=data.find(':',index);//finds : and then stores that index in end
        int slength=std::stoi(data.substr(index,size-index));;//gets line from index index to size-index and then converts it to int
        std::string realstring=data.substr(size+1,slength);;//gets line from index size+1 to slength
        index=size+slength+1;
        return realstring;
      }
       std::vector<Bencodingptr> decodelist(){// return type    std::vector<Bencodingptr>
        ++index;
         std::vector<Bencodingptr> list;
         while(data[index] !='e'){
            list.push_back(decoder());
         }
         index++;
         return list;
       }

         std::map<std::string, Bencodingptr> DecodeDictionary(){// return type std::map<std::string, Bencodingptr>
            ++index;
             std::map<std::string, Bencodingptr> dict;
             while(data[index]!='e'){
                std::string key=decodestring();
                dict[key]=decoder();
             }
             ++index;
             return dict;
         }

};
void dencodedfile(Bencodingptr obj) { 
    if (std::holds_alternative<int>(*obj)) { // check if *obj contains int
        std::cout << std::get<int>(*obj); // gets that int value
    } else if (std::holds_alternative<std::string>(*obj)) { // check if *obj contains string
        std::cout << "\"" << std::get<std::string>(*obj) << "\""; // gets that string
    } else if (std::holds_alternative<std::vector<Bencodingptr>>(*obj)) { // check if *obj contains std::vector<Bencodingptr>
        std::cout << "[\n";
        const auto& vec = std::get<std::vector<Bencodingptr>>(*obj); // get the vector from the variant
        for (size_t i = 0; i < vec.size(); ++i) {        
            dencodedfile(vec[i]); // recursively print each element in the list
            if (i != vec.size() - 1) std::cout << ","; // print comma between elements
            std::cout << "\n";
        }
        std::cout << "]"; // close the list
    } else if (std::holds_alternative<std::map<std::string, Bencodingptr>>(*obj)) { // check if *obj contains std::map<std::string, Bencodingptr>
        std::cout << "{\n";
        const auto& mp = std::get<std::map<std::string, Bencodingptr>>(*obj); // get the map from the variant
        size_t count = 0;
        for (const auto& pair : mp) {
            // print each key-value pair in the dictionary
            std::cout << "\"" << pair.first << "\": ";
            dencodedfile(pair.second/*, indent + 2*/); // recursively print the value
            if (++count != mp.size()) std::cout << ","; // print comma between pairs
            std::cout << "\n";
        }
        std::cout << "}"; // close the dictionary
    }
}

int main() {
    std::string bencoded = "d8:announce42:udp://tracker.opentrackr.org:1337/announce7:comment8:Politics10:created by14:uTorrent/3.4.813:creation datei1476989190e8:encoding5:UTF-84:infod6:lengthi6320796e4:name28:DJ Estaline - Bush-Texas.mp312:piece lengthi16384e6:pieces7720:" + std::string(7720, 'a') + "8:url-listl87:http://djestaline.wikidot.com/local--files/ficheiros/DJ%20Estaline%20-%20Bush-Texas.mp3e7:website29:http://djestaline.wikidot.comee";
    Bencodingparser parser(bencoded);
    auto result = parser.decoder();
    dencodedfile(result);
    return 0;
}
