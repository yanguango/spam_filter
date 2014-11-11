#include <string>
#include <fstream>
#include <iostream>
#include <dirent.h>
#include <map>
#include <vector>
#include <util.h>
#include <sstream>
#include "single_process.h"

using namespace std;

const string SingleProcess::FEATURE_CHARS = ";([!$#";

class ClassProcess {
  private:
    string ham_path;
    string spam_path;
    map<string, float> class_word_map;
    int num_rows;
    vector<string> features;

  public:
    ClassProcess(string hp, string sp){
      ham_path = hp;
      spam_path = sp;
      num_rows = 0;
    }

    vector<string> ListEmailInDir(string file_path) {
      DIR* dir;
      dirent* pdir;
      vector<string> files;

      dir = opendir(file_path.c_str());

      while ((pdir = readdir(dir))) {
        files.push_back(pdir->d_name);
      }

      return files;
    }

    vector<string> SplitTrainTest(vector<string> &train) {
      std::size_t const half_size = train.size() / 2;
      vector<string> test(train.begin(), train.begin() + half_size);
      train.erase(train.begin() + half_size, train.end());
      return test;
    }

    map<string, float> process() {
      vector<string> ham = ListEmailInDir(ham_path);
      vector<string> spam = ListEmailInDir(spam_path);
      
      // std::random_shuffle (train.begin(), train.end());
      // vector<string> test = SplitTrainTest(train);

      for(int i = 0; i < ham.size(); i++) {
        SingleProcess sp(ham_path + ham[i]);
        map<string, double> map = sp.WordCount();
        class_word_map.insert(map.begin(), map.end());
        num_rows++;
      }

      for(int i = 0; i < spam.size(); i++) {
        SingleProcess sp(spam_path + spam[i]);
        map<string, double> map = sp.WordCount();
        class_word_map.insert(map.begin(), map.end());
        num_rows++;
      }

      vector<PAIR> wordvec = SortWordPair(class_word_map);

      vector<PAIR> pair_features(wordvec.begin(), wordvec.begin() + 100);
      ofstream myfile;
      myfile.open("features.txt");
      for (int i = 0; i != pair_features.size(); ++i) {  
        myfile << pair_features[i].first << " " << pair_features[i].second << endl;
        features.push_back(pair_features[i].first);
      }
      myfile.close();
      

      myfile.open("data.txt", std::ofstream::out | std::ofstream::app);
      for(int i = 0; i < ham.size(); i++) {
        SingleProcess sp(ham_path + ham[i]);
        sp.WordCount();
        string s = sp.output(features);
        myfile << s << endl;
      }

      for(int i = 0; i < spam.size(); i++) {
        SingleProcess sp(spam_path + spam[i]);
        sp.WordCount();
        string s = sp.output(features);
        myfile << s << endl;
      }

      myfile.close();

      return class_word_map;
 
    }

};



