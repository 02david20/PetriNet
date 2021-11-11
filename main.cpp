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
    cout << "Not yet";
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
        else if(option == "4" || option == "Optional" || option == "Optional"){
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

    bool runningPN = true;
    while(runningPN) {
        cout << "---------------" << endl;
        cout << "Choosing option:\n"
             << "1.Running a firing Sequence\n"
             << "2.Print all reachable Marking\n"; cin >> option;
        if(option == "1"){
            //Print all transition and places
            //
            cout << "Enter Firing Sequence (Enter '.' for stop): ";
            vector<string> firing_seq;
            string temp = "aa";
            while(temp != ".") {
                cin >> temp;
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
            PN->ReachableMarking();
        }
        string choice;
        cout << "Do you want to continue?(y/n): ";cin >> choice;
        if(choice != "y" && choice != "Y") {
            cout << "done";
            runningPN = false;
        }
    }

    

    delete PN;
    return 0;
}
