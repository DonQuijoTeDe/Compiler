#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <queue>
#include <algorithm>
using namespace std;

vector<string> NFA[12];
vector<string> DFA;
queue<string> dictionary;
string acceptingstate="";
int counter=0;
int category=0;

void storeinput(string s, int type){
    if(s == "*" && type == 0) {
        acceptingstate = to_string(counter);
    }
    NFA[counter].push_back(s);
    cout << NFA[counter].at(type) << " ";
}

void storeoutput(string output){
    DFA.push_back(output);
}

bool checkrepeated(string s, queue<string> tempreference){
    while(!tempreference.empty()){
        if(tempreference.front() == s) {
            return true;
        } else {
            tempreference.pop();
        }
    }
    return false;
}

void checkcutting(string s, queue<string> &q_reference){
    while(true){
        size_t found = s.find(',');
        if(found != string::npos) {
            string temp = s.substr(0, 1);
            if(!checkrepeated(temp, q_reference)) {
                q_reference.push(temp);
            }
            s = s.substr(found+1, s.length()-found+1);
        } else {
            if(!checkrepeated(s, q_reference)) {
                q_reference.push(s);
            }
            break;
        }
    }
}

void cutting(string str){
    int type=0;
    while(true){
        size_t found = str.find(' ');
        if(counter==0) {
            checkcutting(str, dictionary);
            dictionary.pop();
            category = stoi(dictionary.back(), nullptr, 10);
            cout << category;

            break;
        } else if(counter != 0 && found != string::npos) {
            string temp = str.substr(0, found);
            storeinput(temp, type);
            str = str.substr(found+1, str.length()-found+1);
        } else {
            storeinput(str, type);

            break;
        }
        type++;
    }
    cout << "\n";
}

vector<int> increasingorder (queue<string> q){
    vector<int> vec;
    while(!q.empty()){
        int dec = stoi(q.front(), nullptr, 10);
        vec.push_back(dec);
        q.pop();
    }
    if(vec.size() > 1) {
        sort(vec.begin(),vec.end());
    }
    return vec;
}

void checklambda(queue<string> &q, queue<string> &state_reference){
    queue<string> temp;
    do {
        if(!q.empty()) {
            checkcutting(q.front(), temp);
            q.pop();
        }
        int dec = stoi(temp.front(), nullptr, 10);
        string target = NFA[dec].at(0);
        if(target != "0" && target != "*" && !checkrepeated(target, state_reference)) {
            q.push(target);
        }
        if(!checkrepeated(temp.front(), state_reference)) {
            state_reference.push(temp.front());
        }
        temp.pop();
    } while(!q.empty() || !temp.empty());
}

string concatenating(vector<int> vec){
    string s = "";
    if(vec.size() == 1) {
        s = to_string(vec.front());
    } else {
        for(vector<int>::iterator it=vec.begin();it!=vec.end();it++){
            s = s + to_string(*it) + ",";
        }
        s.erase(s.length()-1);
    }
    return s;
}

string checkacceptingstate(string output){
    string temp = output;
    size_t found = temp.find(acceptingstate);
    if(found != string::npos) {
        return "*" + temp;
    } else {
        return temp;
    }
}

void transformation(){
    /* the algorithm of NFA to DFA
     * first step : preprocessing the starting state
     * second step : according to the NFA table to
     * check if one state could go to another state
     */
    /*------------------------------------------------*/
    queue<string> visited; /* the states that are already visited */
    queue<string> processing; /* the states that we still need to process */
    queue<string> stateresult; /* the states that have been processed by < processing > */
    /*---First step : preprocessing--------------------*/
    queue<string> t0;
    t0.push("1");
    checklambda(t0, stateresult);
    vector<int> v0 = increasingorder(stateresult);
    string s0 = concatenating(v0);
    processing.push(s0);
    /*-------------Second step-------------------------*/
    do {
        storeoutput(processing.front());
        for(int i=1;i<=category;i++){
            queue<string> q;
            checkcutting(processing.front(), q);
            queue<string> temp;
            stateresult = temp;
            do {
                int dec = stoi(q.front(), nullptr, 10);
                if(NFA[dec].at(i) != "0" && NFA[dec].at(i) != "*" ) {
                    temp.push(NFA[dec].at(i));
                    checklambda(temp, stateresult);
                } else if(NFA[dec].at(i) == "0") {
                    //cout << "dec: " << dec << " NFA[" << dec << "].at[" << i << "]: " <<  NFA[dec].at(i) << "\n";
                } else if(NFA[dec].at(i) == "*") {
                    //cout << "dec: " << dec << " NFA[" << dec << "].at[" << i << "]: " <<  NFA[dec].at(i) << "\n";
                }
                q.pop();
            } while(!q.empty());
            vector<int> vec = increasingorder(stateresult);
            if(vec.size() != 0) {
                string str = concatenating(vec);
                if(!checkrepeated(str, visited)) {
                    visited.push(str);
                    processing.push(str);
                }
                storeoutput(str);
            } else {
                storeoutput("0");
            }
        }
        processing.pop();
    }
     while(!processing.empty());
    /*-------------------------------------------------*/
}

int main(){
    /* here is the input operation*/

    string input;
    fstream loadfile("NFA.txt", ios::in);
    if(loadfile.is_open()) {
        while(getline(loadfile, input)){
            cutting(input);
            counter++;
        }
        loadfile.close();
    }
    else
        cout << "Unable to open the file\n";

    /*here is the NFA to DFA operation*/

    transformation();

    /*here is the output operation*/

    fstream storefile("DFA.txt", ios::out);
    string alphabet = dictionary.front();
    dictionary.pop();
    while(dictionary.size() != 1) {
        alphabet = alphabet + "," + dictionary.front();
        dictionary.pop();
    }
    storefile << alphabet;
    int t1 = 0;
    for(vector<string>::iterator it=DFA.begin();it!=DFA.end();it++) {
        if((t1)%3 == 0) {
            storefile << "\n";
        }
        storefile << checkacceptingstate(*it) << " ";
        t1++;
    }
    storefile.close();

    return 0;
}
