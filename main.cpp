#include <iostream>
#include "PetriNet.h"
using namespace std;

void readAsPatient(vector<Place> &p,map<string,Transition> &t){
    p.push_back(Place{"wait"});
    p.push_back(Place{"inside"});
    p.push_back(Place{"done"});
    // # liet ke Arc in va out cho moi transition
    t.insert(pair<string, Transition>("start",Transition({In_Arc(p[0])},{Out_Arc(p[1])})));
    t.insert(pair<string, Transition>("change",Transition({In_Arc(p[1])},{Out_Arc(p[2])})));
};
void readAsSpecialist(vector<Place> &p,map<string,Transition> &t){
    p.push_back(Place{"free"});
    p.push_back(Place{"docu"});
    p.push_back(Place{"busy"});

    // # liet ke Arc in va out cho moi transition
    t.insert(pair<string, Transition>("start",Transition({In_Arc(p[0])},{Out_Arc(p[2])})));
    t.insert(pair<string, Transition>("end",Transition({In_Arc(p[1])},{Out_Arc(p[0])})));
    t.insert(pair<string, Transition>("change",Transition({In_Arc(p[2])},{Out_Arc(p[1])})));
};
void readAsMerge(vector<Place> &p,map<string,Transition> &t){
    p.push_back(Place{"free"});
    p.push_back(Place{"docu"});
    p.push_back(Place{"wait"});
    p.push_back(Place{"busy"});
    p.push_back(Place{"done"});
    p.push_back(Place{"inside"});

    // # liet ke Arc in va out cho moi transition
    t.insert(pair<string, Transition>("start",Transition({In_Arc(p[0]),In_Arc(p[2])},{Out_Arc(p[3]),Out_Arc(p[5])})));
    t.insert(pair<string, Transition>("end",Transition({In_Arc(p[1])},{Out_Arc(p[0])})));
    t.insert(pair<string, Transition>("change",Transition({In_Arc(p[5]),In_Arc(p[3])},{Out_Arc(p[1]),Out_Arc(p[4])})));
};
void readOptional(string Input_file,vector<Place> &p,map<string,Transition> &t){
    ifstream input;
    input.open(Input_file);
    string line;
    int x;
    getline(input, line);
    line.erase(0, (int)line.find(' ') + 1);
    int NumofPlaces = stoi(line);
    getline(input, line);
    line.erase(0, (int)line.find(' ') + 1);
    while (line.find(',') != string::npos)
    {
        x = line.find(',');
        p.push_back(Place{line.substr(0,x)});
        line.erase(0, x + 1);
    }
    p.push_back(Place{line});
    getline(input, line);
    line.erase(0, (int)line.find(' ') + 1);
    int NumofTransitions = stoi(line);
    while (input.eof())
    {
        getline(input, line);
        line.erase(0, (int)line.find(' ') + 1);
        string Name;
        Name = line.substr(0, (int)line.find('['));
        line.erase(0, (int)line.find('['));
        line.erase(line.begin());
        line.erase(line.end() - 1);
        string IN = line.substr(0,(int)line.find(';'));
        string OUT = line.substr((int)line.find(';') + 1, (int)line.size());
        map<string, int> in;
        map<string, int> out;
        while (IN.find(',') != string::npos)
        {
           x = IN.find(',');
           in[IN.substr(0,x)]++;
           IN.erase(0, x + 1);
        }
        in[IN]++;
        while (OUT.find(',') != string::npos)
        {
           x = OUT.find(',');
           out[OUT.substr(0,x)]++;
           OUT.erase(0, x + 1);
        }
        out[OUT]++;
        vector<In_Arc> in_arc;
        for(auto temp: in){
            in_arc.push_back(In_Arc(temp.first,temp.second));
        }
        vector<Out_Arc> out_arc;
        for(auto temp: in){
            out_arc.push_back(Out_Arc(temp.first,temp.second));
        }
        t.insert(pair<string, Transition>(Name,Transition(in_arc,out_arc)));
    }
    input.close();
};


int main(){
    //so luong place
    cout << "Option:" << endl;
    cout << "1.Patient" << endl << "2.Specialist" << endl << "3.Merge" << endl << "4.Optional" << endl;
    string option; cout << "Your Option: "; cin >> option;

    vector<Place> p;
    map<string,Transition> t;
    PetriNet *PN;

    bool validOPtion = false;
    while(!validOPtion) {
        if(option == "1" || option == "Patient" || option == "patient"){
            readAsPatient(p,t);
            validOPtion = true;
            cout << "---------------" << endl << 
            "Patient Petri Net" << endl;
        }
        else if(option == "2" || option == "Specialist" || option == "specialist"){
            readAsSpecialist(p,t);
            validOPtion = true;
            cout << "---------------" << endl << 
            "Specialist Petri Net" << endl;
        }
        else if(option == "3" || option == "Merge" || option == "merge"){
            readAsMerge(p,t);
            validOPtion = true;
            cout << "---------------" << endl << 
            "Merge Petri Net" << endl;
        }
        else if(option == "4" || option == "Optional" || option == "optional"){
            string Input;
            cout << "Enter Input file (Input file must be in the same directory):" << endl; cin >> Input;
            readOptional(Input,p,t);
            validOPtion = true;
        }
        if(!validOPtion){
            cout << "Please try again!" << endl; cin >> option;
        }
    }
    
    PN = new PetriNet(p,t);
    cout << "Please Enter Initial Marking: " << endl;
    PN->InitialMarking();

    //Save initial Marking
    ////////////////////////////////////////////////////////////////
    vector<int> initial_token;
    int size = PN->ps.size();
    for (int i = 0; i < PN->ps.size(); i++) {
        initial_token.push_back(PN->ps[i].getToken());
    }
    ////////////////////////////////////////////////////////////////
    
    bool runningPN = true;
    while(runningPN) {
        cout << "---------------" << endl;
        cout << "Choosing option:\n"
             << "1.Running a firing Sequence\n"
             << "2.Firing each transition you choice\n"
             << "3.Print all reachable Marking\n"
             << "4.Back into initial Marking\n"; cin >> option;
        if(option == "1"){
            //Print all transition and places
            //
            cout << "Enter Firing Sequence (Enter '.' for stop): ";
            vector<string> firing_seq;
            string temp = "aa";
            while(temp != ".") {
                cin >> temp;
                if(temp == ".") break;
                bool valid = false;
                for (auto i:PN->mp)
                {
                    if(i.first == temp) {
                        valid = true;
                    }
                }
                if(valid) firing_seq.push_back(temp);
                else cout << "Invalid: " << temp << endl <<"Try again!" << endl;
            }
            PN->run(firing_seq);
        }
        else if(option == "2") {
            vector<string> transition;
            string temp = "nonstop";
            while (temp != ".") {
                if (!transition.empty()) transition.pop_back();
                cout << "Enter transition (Enter '.' to stopping to run Petri Net): ";
                cin >> temp;
                if (temp == ".") break;
                bool valid = false;
                for (auto i:PN->mp)
                {
                    if(i.first == temp) {
                        valid = true;
                    }
                }
                if(valid) transition.push_back(temp);
                PN->run(transition);
            }
        }
        else if(option == "3") {
            PN->ReachableMarking();
        }
        else if(option == "4") {
            PN->Reset(initial_token);
        }
        string choice;
        cout << "Do you want to continue?(y/n): ";cin >> choice;
        if(choice != "y" && choice != "Y") {
            cout << "Done!";
            runningPN = false;
        }
    }
    delete PN;
    return 0;
}
