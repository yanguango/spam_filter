#include "util.h"
#include <string>
#include <fstream>
#include <iostream>
#include <dirent.h>
#include <map>
#include <vector>
#include "feature_extractor.h"
#include <sstream>


using namespace std;

class Preprocessor{
  private:
    vector<string> features;
  public:
    vector<string> ListEmailInDir(std::string path) {
      DIR*    dir;
      dirent* pdir;
      std::vector<std::string> files;

      dir = opendir(path.c_str());

      while (pdir = readdir(dir)) {
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


    vector<string> ExtractFeatures() {
      string ham_path = "data/enron1/ham/";
      string spam_path = "data/enron1/spam/";

      vector<string> ham_files = ListEmailInDir(ham_path);
      vector<string> spam_files = ListEmailInDir(spam_path);
      map<string, int> total_map;

      for(int i = 0; i < ham_files.size(); i++) {
        FeatureExtractor fx(ham_path + ham_files[i]);
        map<string, int> map = fx.WordCount();
        total_map.insert(map.begin(), map.end());
      }

      for(int i = 0; i < spam_files.size(); i++) {
        FeatureExtractor fx(spam_path + spam_files[i]);
        map<string, int> map = fx.WordCount();
        total_map.insert(map.begin(), map.end());
      }


      vector<PAIR> wordvec = SortWordPair(total_map);

      vector<PAIR> pair_features(wordvec.begin(), wordvec.begin() + 100);

      ofstream myfile;
      myfile.open ("features.txt");
      for (int i = 0; i != pair_features.size(); ++i) {  
        myfile << pair_features[i].first << " " << pair_features[i].second << endl;
        features.push_back(pair_features[i].first);
      }
      myfile.close();


      myfile.open ("train.txt");
      for(int i = 0; i < ham_files.size(); i++) {
        FeatureExtractor fx(ham_path + ham_files[i]);
        fx.WordCount();
        string s = fx.output(features);
        cout << s << endl;
        myfile << s << endl;
      }
      myfile.close();

    }
};

int main() {
  Preprocessor *p = new Preprocessor();
  p->ExtractFeatures();
  // string file = "data/enron1/ham/0002.1999-12-13.farmer.ham.txt";
  // int count;
  // ifstream ifs(file);
  //   if(!ifs) {
  //     exit(-1);
  //   }
  //   std::string word;
  //   while(ifs >> word) {
  //     count++;
  //   }
  //   cout << count;

  // Preprocessor *p = new Preprocessor(file);
  // map<string, int> word_map = p->WordCount();

  // string ham_path = "data/enron1/ham";
  // vector<string> train = ListEmailInDir(ham_path);
  // cout << train.size() << endl;
  // std::random_shuffle (train.begin(), train.end());
  // vector<string> test = SplitTrainTest(train);
  //
  //
  // cout << train.size() << " " << test.size() << endl;
  // for(int i=0; i<test.size();i++)
  //   // cout << test[i] << endl;


  return 0;
}
