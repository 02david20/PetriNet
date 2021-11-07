#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <chrono>
#include <thread>
using namespace std;

class Place{
    public:
    string name;
    int token;
    Place(string n, int tok=0): name(n),token(tok){};
    friend bool operator==(Place p1,Place p2){
        return p1.name==p2.name;
    }
};
class Out_Arc{
    public:
    Place p;
    int weight;
    Out_Arc(Place pl, int w=1): p(pl),weight(w){};
    void trigger(){
        p.token += weight;
    }
    friend bool operator==(Out_Arc p1,Out_Arc p2){
        return p1.p==p2.p;
    }
};
class In_Arc{
    public:
    Place p;
    int weight;
    In_Arc(Place pl, int w=1): p(pl),weight(w){};
    void trigger(){
        p.token -= weight;
    }
    bool checkUnblocked(){
        return p.token>=weight;
    }
    friend bool operator==(In_Arc p1,In_Arc p2){
        return p1.p==p2.p;
    }
};
class Transition{
    public:
    vector<In_Arc>in_arcs;
    vector<Out_Arc>out_arcs;
    Transition(vector<In_Arc>i,vector<Out_Arc>o){
        in_arcs = i;
        out_arcs = o;
    }
    bool fire(){
        bool enabled = true;
        vector<In_Arc>::iterator it = in_arcs.begin();
        while(it!=in_arcs.end()){
            if(!(*it).checkUnblocked()){
                enabled = false;
                break;
            }
            it++;
        }
        if(enabled){
            vector<In_Arc>::iterator it1 = in_arcs.begin();
            while(it1!=in_arcs.end()) {
                (*it1).trigger();
                it1++;
            }
            vector<Out_Arc>::iterator it2 = out_arcs.begin();
            while(it2!=out_arcs.end()) {
                (*it2).trigger();
                it2++;
            }
        }
        return enabled;
    }
};
class PetriNet{
    public:
    vector<Place> ps;
    map<string,Transition> mp;
    PetriNet(map<string,Transition> m,vector<Place>p):mp(m),ps(p){};
    string get_str_seq(vector<string>firing_seq){
        string s="";
        int len = firing_seq.size();
        s+=firing_seq[0];
        for(int i=1;i<len;i++){
            s+=","+firing_seq[i];
        }
        return s;
    }
    void printMarking(vector<Place>ps){
        cout<<"[";
        int len = ps.size();
        cout<<to_string(ps[0].token);
        for(int i=1;i<len;i++){
            cout<<","<<to_string(ps[i].token);
        }
        cout<<"]\n";
    }
    void updateMarking(string transition){
        int len = ps.size();
        Transition t = mp.find(transition)->second;
        //update vector Place
        for(int j=0;j<len;j++){
            vector<In_Arc>::iterator pIn = find(t.in_arcs.begin(),t.in_arcs.end(),ps[j]);
            if(pIn!=t.in_arcs.end()) ps[j]=pIn->p;
            vector<Out_Arc>::iterator pOut = find(t.out_arcs.begin(),t.out_arcs.end(),ps[j]);
            if(pOut!=t.out_arcs.end()) ps[j]=pOut->p;
        }
        //update other transitions
        for (std::map<string,Transition>::iterator it=mp.begin(); it!=mp.end(); ++it){
            if((*it).first!=transition){
                for(int j=0;j<len;j++){
                    vector<In_Arc>::iterator pIn = find((*it).second.in_arcs.begin(),(*it).second.in_arcs.end(),ps[j]);
                    if(pIn!=(*it).second.in_arcs.end()) pIn->p=ps[j];
                    vector<Out_Arc>::iterator pOut = find((*it).second.out_arcs.begin(),(*it).second.out_arcs.end(),ps[j]);
                    if(pOut!=(*it).second.out_arcs.end()) pOut->p=ps[j];
                }
            }
        }
    }
    void run(vector<string>firing_seq, bool auto_fire){
        if(!auto_fire){
            string str_seq = get_str_seq(firing_seq);
            int len = firing_seq.size();
            cout<<"Using firing sequence:" << " => "<<str_seq<<'\n';
            cout<<"start: ";
            printMarking(ps);
            for(int i=0;i<len;i++){
                Transition t = mp.find(firing_seq[i])->second;
                bool fire = t.fire();
                mp.find(firing_seq[i])->second = t;
                if(fire){
                    cout<<firing_seq[i]<<" fire!!"<<endl;
                    updateMarking(firing_seq[i]);
                    printMarking(ps);
                }
                else cout<<firing_seq[i]<<" fizzle!!"<<endl;
            }
            cout<<"end: ";
            printMarking(ps);
        }
        else{
            cout<<"start: ";
            printMarking(ps);
            int map_size = mp.size();
            bool all_failed = false;
            while(!all_failed){
                all_failed=true;
                for(int i=0;i<map_size;i++){
                    string name = "t"+to_string(i+1);
                    Transition t = mp.find(name)->second;
                    bool fire = t.fire();
                    mp.find(firing_seq[i])->second = t;
                    if(fire){
                        cout<<firing_seq[i]<<" fire!!"<<endl;
                        updateMarking(firing_seq[i]);
                        printMarking(ps);
                        all_failed=false;
                    }
                }
                std::this_thread::sleep_for(std::chrono::milliseconds {200});
            }
            cout<<"end: ";
            printMarking(ps);
        }
    }
};
int main(){
    //so luong place
    int numOfPlaces = 3;
    //initial marking
    int initial_marking[] = {5,0,1};
    vector<Place> ps;
    for(int i=0;i<numOfPlaces;i++){
        string name = "P"+to_string(i+1);
        Place p = Place(name,initial_marking[i]);
        ps.push_back(p);
    }
    // hai transition t1 va t2
    map<string,Transition> ts;
    // # liet ke Arc in va out cho moi transition
    ts.insert(pair<string, Transition>("t1",Transition({In_Arc(ps[0])},{Out_Arc(ps[1])})));
    ts.insert(pair<string, Transition>("t2",Transition({In_Arc(ps[1])},{Out_Arc(ps[2])})));
    // firing sequence
    vector<string> firing_sequence = {"t1", "t2", "t1"};
    // setting petrinet voi dau vao la ts
    PetriNet petri_net = PetriNet(ts,ps);
    bool auto_fire=true;
    petri_net.run(firing_sequence,auto_fire);
}
