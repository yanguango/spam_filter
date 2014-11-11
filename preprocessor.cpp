#include "util.h"
#include "class_process.h"

using namespace std;

int main() {
  vector<string> features;

  string ham_path = "data/enron1/ham/";
  string spam_path = "data/enron1/spam/";

  ClassProcess ham_process(ham_path, spam_path);
  map<string, float> ham_word_map;
  ham_word_map = ham_process.process();

  return 0;
}
