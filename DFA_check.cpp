#include <iostream>
#include <fstream>
#include <string>
#include <queue>
using namespace std;

vector<string> DFA[15];
queue<string> category;
int counter=0;

queue<string> cutting(string s) {
    queue<string> q;
    string temp = s;
    for(int i=0;i<s.length();i++) {
        q.push(temp.substr(0,1));
        //cout << "q.push: " << temp.substr(0,1) << "\n";
        temp = temp.substr(1,temp.length()-1);
        //cout << "after cut s: " << temp << "\n";
    }
    return q;
}

queue<int> str_map_int(queue<string> q) {
    queue<int> result;
    while(!q.empty()) {
        if(q.front() == category.front()) {
            result.push(1);
            //cout << "1";
        }
        else {
            result.push(2);
            //cout << "2";
        }
        q.pop();
    }
    //cout << "\n";
    return result;
}

bool checkacceptingstate(string state) {
    size_t found = state.find('*');
    if(found != string::npos) {
        return true;
    } else {
        return false;
    }
}

int searching(string target) {
    for(int i=1;i<counter;i++) {
        if(DFA[i][0] == target) {
            return i;
        }
    }
}

void operating(string input) {
    queue<string> temp = cutting(input);
    queue<int> q = str_map_int(temp);
    int index = 1;
    string state;
    do {
        if(DFA[index].at(q.front()) != "0") {
            state = DFA[index].at(q.front());
            index = searching(state);
            //cout << state << "\n";
        } else {
            cout << "error";
            break;
        }
        q.pop();
    } while(!q.empty());
    if(q.empty() && checkacceptingstate(state)) {
        cout << "valid";
    } else if(q.empty() && !checkacceptingstate(state)) {
        cout << "error";
    }
    cout << "\n";
}

void printDFA() {
    for(int i=1;i<counter;i++) {
        for(int j=0;j<=category.size();j++) {
            cout << DFA[i].at(j) << " ";
        }
        cout << "\n";
    }
    cout << "\n\n";
}

void storeinput(string input) {
    while(true) {
        int type=0;
        if(counter == 0) {
            string t0 = input.substr(0,1);
            category.push(t0);
            cout << t0 << " ";
            size_t f0 = input.find(',');
            if(f0 == string::npos) {
                cout << "\n";
                break;
            }
            input = input.substr(f0+1, input.length()-f0+1);
        } else {
            size_t f1 = input.find(' ');
            if(f1 == string::npos) {
                DFA[counter].push_back(input);
                break;
            }
            string t1 = input.substr(0, f1);
            DFA[counter].push_back(t1);
            input = input.substr(f1+1, input.length()-f1+1);
        }
        type++;
    }
}

int main() {
    /*here is the input operation*/
    string input;
    fstream loadfile("DFA.txt", ios::in);
    if(loadfile.is_open()) {
        while(getline(loadfile, input)) {
            storeinput(input);
            counter++;
        }
    } else {
        cout << "Can't not open the file!\n";
    }

    printDFA();

    /*here is the output operation*/
    while(cin) {
        string input;
        getline(cin, input);
        operating(input);
    }

    return 0;
}
