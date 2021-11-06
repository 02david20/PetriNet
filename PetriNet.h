#ifndef PETRI_NET
#define PETRI_NET
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

class Place{
private:
    string name;
    int token;
    friend class PetriNet;
    friend class In_Arc;
    friend class Out_Arc;
public:
    Place(string n, int tok=0): name(n),token(tok){};
    friend bool operator==(Place p1,Place p2){
        return p1.name==p2.name;
    }
};

class Out_Arc{
private:
    Place p;
    int weight;
    friend class PetriNet;
public:
    Out_Arc(Place pl, int w=1): p(pl),weight(w){};
    void trigger(){
        p.token += weight;
    }
    friend bool operator==(Out_Arc p1,Out_Arc p2){
        return p1.p==p2.p;
    }
};

class In_Arc{
private:
    Place p;
    int weight;
    friend class PetriNet;
public:
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
private:
    vector<In_Arc>in_arcs;
    vector<Out_Arc>out_arcs;
    friend class PetriNet;
public:
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
private:
    map<string,int> places;
    map<string,Transition> mp;
public:
    PetriNet(map<string,Transition> m):mp(m){};
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
    void updateMarking(vector<Place>&ps,string transition){
        int len = ps.size();
        Transition t = mp.find(transition)->second;
        for(int j=0;j<len;j++){
            vector<In_Arc>::iterator pIn = find(t.in_arcs.begin(),t.in_arcs.end(),ps[j]);
            if(pIn!=t.in_arcs.end()) ps[j]=pIn->p;
            vector<Out_Arc>::iterator pOut = find(t.out_arcs.begin(),t.out_arcs.end(),ps[j]);
            if(pOut!=t.out_arcs.end()) ps[j]=pOut->p;
        }
    }
    void run(vector<string>firing_seq, vector<Place>&ps){
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
                updateMarking(ps,firing_seq[i]);
                printMarking(ps);
            }
            else cout<<firing_seq[i]<<" fizzle!!"<<endl;
        }
        cout<<"end: ";
        printMarking(ps);
    }
};
#endif