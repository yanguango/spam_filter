#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>

using namespace std;

typedef pair<string, float> PAIR; 

struct CmpByValue {  
  bool operator()(const PAIR& lhs, const PAIR& rhs) {  
    return lhs.second > rhs.second;  
  }  
};

vector<PAIR> SortWordPair(map<string, float> &wordmap) {
  vector<PAIR> wordvec(wordmap.begin(), wordmap.end());  
  sort(wordvec.begin(), wordvec.end(), CmpByValue());  
  
  return wordvec;
}

