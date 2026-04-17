#include <iostream>
#include <list>
#include <map>
#include <string>
using namespace std;

class package
{
    public:
    // methos




    // __init__ method
    package(){};

};

class condition
{
    public:
    condition();
};

class player : public package, public condition
{
    private:
    map<string, float> state;


    public:
    

    // property
    
    map<string, float> get_state();
    float get_ATK();
    float get_DEF();
    float get_HP();
    float get_EXP();
    float get_Money();

    // method
    void change_state();
    void change_ATK();
    void change_DEF();
    void change_HP();
    void change_EXP();
    void change_Money();
    void save();
    void load_save();

    // __init__ method
    player() : package(), condition()
    {
        state = {
            {"ATK", 100}, 
            {"DEF", 100},
            {"HP", 1000},
            {"EXP",0},
            {"Money",100}
        };
    }

};

