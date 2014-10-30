#include <string>
#include <fstream>
#include <iostream>
#include <dirent.h>
#include <map>
#include <vector>
#include <util.h>
#include <sstream>

using namespace std;

class SingleProcess {
private:
  static const string FEATURE_CHARS;
  string file;
  map<string, float> wordmap;
  int total_num;
  bool is_spam;
  
public:
  SingleProcess(string f){
    
    file = f;
    cout << file << endl;
    total_num = 0;
    is_spam = (file.find("spam") != string::npos);
  }
  
  void PutWord(std::string word) {
    if(word.length() > 1 || 
      (word.length() == 1 && FEATURE_CHARS.find(word) != string::npos)) {
      total_num++;
      wordmap[word]++;
    }
  }

  map<string, float> WordCount() {
    ifstream ifs(file);
    if(!ifs) {
      exit(-1);
    }
    std::string word;
    while(ifs >> word) {
      PutWord(word); 
    }

    for (auto it = wordmap.begin(); it != wordmap.end(); it++) {
      it->second = it->second / total_num * 100.0;
    }
    return wordmap;
  }

  string output(vector<string> features) {
    stringstream ss;
    for(size_t i = 0; i < features.size(); ++i) {
      if(i != 0)
        ss << " ";
        ss << wordmap[features[i]] / total_num * 100.0;
    }
    ss << (is_spam ? " 1" : " 0"); 
    return ss.str();
  }


};


