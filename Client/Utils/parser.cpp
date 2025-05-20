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
     { if(data[index]=='i') return std::make_shared< Bencodingobject>(Decodeint());
       if(std::isdigit(data[index])) return std::make_shared< Bencodingobject>(decodestring());
       if(data[index]=='l') return std::make_shared< Bencodingobject>(decodelist());
       if(data[index]=='d') return std::make_shared< Bencodingobject>( DecodeDictionary());
       throw std::runtime_error("Invalid Bencode input");
     }
      int Decodeint(){
        ++index;
        int end= data.find('e',index);
        int length = std::stoi(data.substr(index,end-index));
        index=end+1;
        return length;
      }
      std::string decodestring(){
        int size=data.find(':',index);
        int slength=std::stoi(data.substr(index,size-index));
        std::string realstring=data.substr(size+1,slength);
        index=size+slength+1;
        return realstring;
      } std::string decodestring(){
        int size=data.find(':',index);
        int slength=std::stoi(data.substr(index,size-index));
        std::string realstring=data.substr(size+1,slength);
        index=size+slength+1;
        return realstring;
      }
       std::vector<Bencodingptr> decodelist(){
        ++index;
         std::vector<Bencodingptr> list;
         while(data[index] !='e'){
            list.push_back(decoder());
         }
         index++;
         return list;
       }
         std::map<std::string, Bencodingptr> DecodeDictionary(){
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
void dencodedfile(Bencodingptr obj){
     if(std::holds_alternative<int>(*obj)){
        std::cout<<std::get<int>(*obj)<<"\n";
     }else if(std::holds_alternative<std::string>(*obj)){
        std::cout<<std::get<std::string>(*obj)<<"\n";
     } else if (std::holds_alternative<std::vector<Bencodingptr>>(*obj)) {
        std::cout << "[\n";
        for (auto& item : std::get<std::vector<Bencodingptr>>(*obj)) {
            dencodedfile(item);
        }
        std::cout << "]\n";
    } else if (std::holds_alternative<std::map<std::string, Bencodingptr>>(*obj)) {
        std::cout << "{\n";
        for (auto& pair : std::get<std::map<std::string, Bencodingptr>>(*obj)) {
            std::cout << "  \"" << pair.first << "\": ";
           dencodedfile(pair.second);
        }
        std::cout <<"}\n";
    }
    }int main() {
    std::string bencoded = "d8:announce42:udp://tracker.opentrackr.org:1337/announce7:comment8:Politics10:created by14:uTorrent/3.4.813:creation datei1476989190e8:encoding5:UTF-84:infod6:lengthi6320796e4:name28:DJ Estaline - Bush-Texas.mp312:piece lengthi16384e6:pieces7720:" + std::string(7720, 'a') + "8:url-listl87:http://djestaline.wikidot.com/local--files/ficheiros/DJ%20Estaline%20-%20Bush-Texas.mp3e7:website29:http://djestaline.wikidot.comee";
    Bencodingparser parser(bencoded);
    auto result = parser.decoder();
    dencodedfile(result);
    return 0;
}


