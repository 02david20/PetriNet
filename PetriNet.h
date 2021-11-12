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
    friend bool compareMarking(vector<Place>, vector<Place>);
    int getToken() {return this->token;}
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
public:
    // Set of place
    vector<Place> ps;
    map<string,Transition> mp;
private:
    void printReachableMarkingRec(vector< vector<Place> >& M, vector<string> firing_seq) {
        M.push_back(ps);
        for (map<string,Transition>::iterator i = mp.begin(); i != mp.end();++i)
        {
            if((*i).second.fire()) {
                updateMarking((*i).first);
                firing_seq.push_back((*i).first);
                //Check if the Marking already exist
                bool alreadyPresent = false;
                for (auto i : M)
                {
                    if(compareMarking(i,this->ps)) {
                        this->ps = M.back();
                        alreadyPresent = true;
                        break;
                    }
                }
                //If not print the Marking
                if(!alreadyPresent) {
                    cout << get_str_seq(firing_seq) << endl;
                    printMarking(this->ps);
                }
                printReachableMarkingRec(M,firing_seq);
                firing_seq.pop_back();
            }
        }
        return;
    }
    string get_str_seq(vector<string>firing_seq){
        string s="";
        int len = firing_seq.size();
        s+=firing_seq[0];
        for(int i=1;i<len;i++){
            s+=","+firing_seq[i];
        }
        return s;
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
public:
    PetriNet(vector<Place>p, map<string,Transition> m):mp(m),ps(p){};
    
    void printMarking(vector<Place>ps){
        cout<<"[";
        int len = ps.size();
        int i;

        for(i=0;i<len;i++){
            if(ps[i].token != 0) {
                cout<<ps[i].name<<"."<<to_string(ps[i].token);
                break;
            }
        }
        
        for(i=i+1;i<len;i++){
            if(ps[i].token != 0) {
                cout<<","<<ps[i].name<<"."<<to_string(ps[i].token);
            }
        }
        cout<<"]\n";
    }


    
    void run(vector<string>firing_seq){
        if(firing_seq.empty()) {
            cout << "Empty Firing Sequence"<< endl;
            return;
        }
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

    void InitialMarking(){
        int size = ps.size();
        for (int i = 0; i < size;i++)
        {
            cout << ps[i].name << ": "; cin >> ps[i].token;
        }
         for (std::map<string,Transition>::iterator it=mp.begin(); it!=mp.end(); ++it){
            for(int j=0;j<size;j++){
                vector<In_Arc>::iterator pIn = find((*it).second.in_arcs.begin(),(*it).second.in_arcs.end(),ps[j]);
                if(pIn!=(*it).second.in_arcs.end()) pIn->p=ps[j];
                vector<Out_Arc>::iterator pOut = find((*it).second.out_arcs.begin(),(*it).second.out_arcs.end(),ps[j]);
                if(pOut!=(*it).second.out_arcs.end()) pOut->p=ps[j];
            }
            
        }
    };
    void ReachableMarking(){
        vector< vector<Place> > M;
        vector<string> fs;
        cout << "Initial Marking: "; 
        printMarking(this->ps);
        printReachableMarkingRec(M,fs);
    }
    
    void printPlace(){
        cout << "Places:\n";
        for(auto c : this->ps){
           cout << '[' << c.name <<','<<c.token << ']' << endl;
        }
    }
    void printTransition(){
        cout << "Transition:\n";
        for(auto c : this->mp){
            cout << c.first <<'[';
            cout << "In_Arc:";
            string s = "";
            for(auto d: c.second.in_arcs){
                s += '[';
                s +=  d.p.name;
                s +=  ',';
                s +=  to_string(d.weight);
                s += "],";
            }
            s.erase(s.end() - 1);
            cout << s;
            cout << ",";
            s = "";
            cout << "Out_Arc:";
            for(auto d: c.second.out_arcs){
                s += '[';
                s +=  d.p.name;
                s +=  ',';
                s +=  to_string(d.weight);
                s += "],";
            }
            s.erase(s.end() - 1);
            cout << s;
            cout << ']' << endl;        
        }
    }
    void Reset(vector<int> &initial_token) {
        int size = ps.size();
        for (int i = 0; i < size;i++)
        {
            ps[i].token = initial_token[i];
        }
        for (std::map<string,Transition>::iterator it=mp.begin(); it!=mp.end(); ++it){
            for(int j=0;j<size;j++){
                vector<In_Arc>::iterator pIn = find((*it).second.in_arcs.begin(),(*it).second.in_arcs.end(),ps[j]);
                if(pIn!=(*it).second.in_arcs.end()) pIn->p=ps[j];
                vector<Out_Arc>::iterator pOut = find((*it).second.out_arcs.begin(),(*it).second.out_arcs.end(),ps[j]);
                if(pOut!=(*it).second.out_arcs.end()) pOut->p=ps[j];
            }
        }
        cout << "Back into inital Marking: ";
        printMarking(this->ps);
    }
};

bool compareMarking(vector<Place> M1, vector<Place> M2) {
    int len = M1.size();
    for (int i = 0; i < len; i++)
    {
       if(M1[i].token != M2[i].token) return false;
    }
    return true;
}
#endif
