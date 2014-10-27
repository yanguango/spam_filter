#include <string>
#include <fstream>
#include <iostream>
#include <dirent.h>
#include <map>
#include <vector>
#include <util.h>
#include <sstream>

using namespace std;

class FeatureExtractor {
private:
  string file;
  map<string, int> wordmap;
  int total_word;
  
public:
  FeatureExtractor(string f){
    file = f;
    total_word = 0;
  }
  ~FeatureExtractor() = default; 
  void PutWord(std::string word) {
    total_word++;
    wordmap[word]++;
  }

  map<string, int>& WordCount() {
    ifstream ifs(file);
    if(!ifs) {
      exit(-1);
    }
    std::string word;
    while(ifs >> word) {
      cout << word << endl;
      PutWord(word); 
    }
    return wordmap;
  }

  string output(vector<string> features) {
    stringstream ss;
    for(size_t i = 0; i < features.size(); ++i) {
      if(i != 0)
        ss << " ";
      // ss << wordmap[features[i]] / (total_word * 1.0);
      ss << total_word;
    }
    return ss.str();
  }


};


