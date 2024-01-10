// / This is my attempt at finally learning C++!
// / This is a CLI game that is supposed to imitate the game Little Alchemy.
// / Little Alchemy is a Game where you combine different objects to discover 
// / new items that will be made.
// / I do have plans to turn this into a GUI game as i did in python and will do in java, javascript and C#.
// / I learned alot about different C++ types and how they interact and work with eachother.
// / The basic concept of creating the CLI game is very similar across languages and thats why I find
// / it so fun to create the same project acrsoo different languages.
// / I will be using this experience to create more CLI games until i feel comfortable enough to move to 
// / creating GUI games.
// / I used the json header provided by Niels Lohmann [github link --- https://github.com/nlohmann/json]
// / I have alot of experience using json and this header made is SUPER easy to get data from json.
// / Niels also made it very similar to other languages when it comes to indexing information using bracket notation!.
// / Please check out his github the dude is a mad legend imo.
// / I have to fins a way to get my current work directory path and someone from 2009 provided this greate example on
// / stack overflow [link --- https://stackoverflow.com/questions/875249/how-to-get-current-directory]. it was edited recently (2001) by
// / Jimmy T [stack overflow link --- https://stackoverflow.com/users/1659790/jimmy-t]. Big Big shout out to these 2 madlads. They made my job
// / super easy and i learned quite a bit about how to navigate files in C++ as well as learning quite a bit about strings too!
// / Thanks for checking out my project and feel free to check out more on my github --- https://github.com/Marcusgb710
// / ~~ByteSized0001~~



#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <cctype>
#include <windows.h>
#include <stdlib.h>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

enum INPUT_TYPE {str, num};

string lowwer(string str){
    string _str = "";
    for(char i : str){
        _str += tolower(i);
        
    }
    return _str;
}

string ExePath(string fName) {
    TCHAR buffer[MAX_PATH] = { 0 };
    GetModuleFileName( NULL, buffer, MAX_PATH );
    string m = (string)buffer;
    std::wstring::size_type pos = m.find_last_of("\\/");
    return m.substr(0, pos) + "\\" + fName;
}

json getData(){
    std::ifstream in(ExePath("e.json"));
    if(!in.is_open()){
        return 1;
    }

    json data = json::parse(in);
    return data["items"];
}

class Item{
    private:
        json data{getData()};
        string name{};
        string png{};
        bool unlocked{};
        
        std::vector<json> recepies{};

    public:
        Item(){}
        Item(string name, bool unlocked = false):
            name{name},
            unlocked{unlocked},
            recepies{data[0][name]["recepies"]},
            png{data[0][name]["png"]}
            {
                
            };
        string getName() const {return name;};
        std::vector<json> getRecepies() const {return recepies[0];};
};

bool checkRecipies(Item s1, Item s2){
    for(json i: s1.getRecepies()){
        if(lowwer(i["name"]) == lowwer(s2.getName()))
        return true;
    }
    return false;
}

string getRecepie(Item s1, Item s2){
    for(json i: s1.getRecepies()){
        if(lowwer(i["name"]) == lowwer(s2.getName())){return i["result"];}  
    }
    return "";
}

std::vector<Item> gameInit(){
    std::vector<Item> items;
    items.push_back(Item("air", true));
    items.push_back(Item("rock", true));
    items.push_back(Item("water", true));
    items.push_back(Item("fire", true));
    return items;
}

bool checkIfItemExists(string name, std::vector<Item> items){
    for(Item i : items){
        if(lowwer(i.getName()) == lowwer(name)){
            return true;
        }
    }
    return false;
}

Item getItemByName(string name, std::vector<Item> items){
    if(checkIfItemExists(name, items)){
        for(Item i : items){
            if(lowwer(i.getName()) == lowwer(name)){
                return i;
            }
        }
    }
    Item _i;
    return _i;

}

std::vector<Item> createNewItem(string name, std::vector<Item> items){
    const string _name = lowwer(name);
    for(Item i : items){
        if(i.getName() == _name){
            return items;
        }
    }
    items.push_back(Item(_name, true));
    return items;
}

INPUT_TYPE checkInput(string _str){
    for (char letter : _str){
        if(_str.find_first_not_of("0123456789")){
            continue;
        }
        return str;
    }
    return num;
}

int main(){
    
    TCHAR buffer[MAX_PATH] = { 0 };
    GetModuleFileName( NULL, buffer, MAX_PATH );
    string userInput;
    string inputLowered = "";
    Item result;
    Item selection1;
    Item selection2;
    std::vector<Item> items = gameInit();

    while(true){
        for(int i =0; i < items.size(); i++){
            cout << "[" << i <<"] "<< items[i].getName() << endl;
        }

        cout << "Enter an Item From The Database: ";
        inputLowered = "";
        cin >> userInput;
        if(userInput == "exit"){return 1;};
        

        switch(checkInput(userInput)){
            case str:
                for(int i=0; i<userInput.size(); i++)
                {
                    char letter = userInput[i];
                    char lowerLetter = tolower(letter);
                    inputLowered += lowerLetter;
                }
                result = getItemByName(inputLowered, items);
                if(result.getName() == ""){cout << "Item not found" << endl; continue;}
                break;
            case num:
            const int userNum = stoi(userInput);
                if(userNum > items.size()-1){cout << "Input is too large" << endl; continue;}
                result = items[userNum];
                break;
        }

        if(selection1.getName() == ""){
            selection1 = result;
        }
        else if(selection1.getName() != "" && selection2.getName() == "")
        {
            selection2 = result;
            if(checkRecipies(selection1, selection2)){
                string recepieName = getRecepie(selection1, selection2);
                items = createNewItem(recepieName, items);
                cout << "Item1: " << selection1.getName() << endl;
                cout << "Item2: " << selection2.getName() << endl;
                cout << "You Made: " << recepieName << endl;
            }
            else if(checkRecipies(selection2, selection1)){
                string recepieName = getRecepie(selection2, selection1);
                items = createNewItem(recepieName, items);
                cout << "Item1: " << selection2.getName() << endl;
                cout << "Item2: " << selection1.getName() << endl;
                cout << "You Made: " << recepieName << endl;
            }
            
            selection1 = Item();
            selection2 = Item();
            
            
        }
        if(selection1.getName() != ""){cout << selection1.getName() << endl;}
        }     

    return 0;
}

