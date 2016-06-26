#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
using namespace std;

/* the structure of each Grammer rule */
struct Grammer {
	char LHS;
	string RHS;
	bool visited = false;
};

vector<Grammer> gram;
vector<char> result;

void ReadInput();
void StoreOutput();
void Derivation();
bool CheckVisited(char t);
bool Cmp(char a, char b);
void StringCutting(int k, string &ref);

int main() {
	ReadInput();
	Derivation();
	StoreOutput();

	system("pause");
	return 0;
}

void ReadInput() {
	int Case;
	fstream loadfile("input.txt", ios::in);
	if (loadfile.is_open()) {
		loadfile >> Case;
		for (int i = 0; i<Case; i++) {
			gram.push_back(Grammer());
			loadfile >> gram[i].LHS
				     >> gram[i].RHS;
		}
		loadfile.close();
	}
	else
		cout << "Unable to open the file\n";
}

void StringCutting(int j, string &ref) {
	if (j == 0) {
		ref = ref.substr(1, ref.length() - 1);
	}
	else if (j == ref.length() - 1) {
		ref = ref.substr(0, ref.length() - 1);
	}
	else {
		string head = ref.substr(0, j);
		string tail = ref.substr(j + 1, ref.length() - j - 1);
		ref = head + tail;
	}
}

bool Cmp(char t0, char t1) {
	return t0 - 'A' < t1 - 'A';
}

bool CheckVisited(char t) {
	for (int i = 0; i < result.size(); i++) {
		if (result[i] == t)
			return true;
	}
	return false;
}

void Derivation() {
	queue<char> lambda;
	vector<int> nonterminal;
	string str;

	for (int i = 0; i < gram.size(); i++) {
		str = gram[i].RHS;
		if (str.at(0) == 'l') {
			/* it can derive Lambda , we need to record it */
			lambda.push(gram[i].LHS);
			result.push_back(gram[i].LHS);
			gram[i].visited = true;
		}
		else if (str.at(0) - 'A' >= 0 && str.at(0) - 'A' <=25) {
			/* it is a nonterminal , we need to record it and deal with it later */
			nonterminal.push_back(i);
		}
		else {
			/* it is a terminal and we don't need to deal with it */
			gram[i].visited = true;
		}
	}
	
	int index;
	char front;
	while (!lambda.empty()) {
		front = lambda.front();
		for (int i = 0; i < nonterminal.size(); i++) {
			index = nonterminal[i];
			str = gram[index].RHS;
			if (!gram[index].visited) {
				string temp;
				temp = str;
				for (int j = 0; j < temp.length(); j++) {
					if (str.length() == 1 && str.at(0) == front) {
						str = "";
						gram[index].RHS = str;
						break;
					}
					if (temp.at(j) == front) {
						StringCutting(j, str);
						gram[index].RHS = str;
					}
				}
				if (str.empty() && !CheckVisited(gram[index].LHS)) {
					result.push_back(gram[index].LHS);
					lambda.push(gram[index].LHS);
					gram[index].visited = true;
				}
			}
		}
		lambda.pop();
	}
}

void StoreOutput() {
	/* we need to store results in increasing order */
	sort(result.begin(), result.end(), Cmp);

	fstream storefile("output.txt", ios::out);
	for (int i = 0; i < result.size(); i++)
		storefile << result[i] << " ";
	storefile.close();
}
