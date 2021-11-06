#include <iostream>
#include "PetriNet.h"
using namespace std;


int main(){
    //so luong place
    int numOfPlaces = 4;
    //initial marking
    int initial_marking[] = {1,0,2,3};
    vector<Place> ps;
    for(int i=0;i<numOfPlaces;i++){
        string name = "P"+to_string(i+1);
        Place p = Place(name,initial_marking[i]);
        ps.push_back(p);
    }
    // hai transition t1 va t2
    map<string,Transition> ts;
    // # liet ke Arc in va out cho moi transition
    ts.insert(pair<string, Transition>("t1",Transition({In_Arc(ps[0])},{Out_Arc(ps[1]), Out_Arc(ps[2])})));
    ts.insert(pair<string, Transition>("t2",Transition({In_Arc(ps[1]),In_Arc(ps[2])},{Out_Arc(ps[3]), Out_Arc(ps[0])})));
    // firing sequence
    vector<string> firing_sequence = {"t1", "t2", "t1"};
    // setting petrinet voi dau vao la ts
    PetriNet petri_net = PetriNet(ts,ps);
    petri_net.run(firing_sequence);
}
