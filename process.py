from string import punctuation
from collections import Counter
import os

FEATURE_CHARS=';([!$#'
STOPLIST = []
CAP_FEATUES = ["capital_run_length_average", "capital_run_length_longest", "capital_run_length_total"]
CONSIDER_CAP_FEATURES = False

def process_class(file_path):
    class_words_counter = Counter()
    
    paths = []
    for root, dirs, files in os.walk(file_path):
        paths.extend([os.path.join(root, name) for name in files])
    
    email_size = len(paths)
    
    words_freq_counter_data = []
    
    for file_path in paths:
        words_counter  = Counter()
        words_freq_counter = Counter()
        for line in open(file_path):
            for word in line.split():
                if ((len(word) > 1 and (not word.isdigit())) or (word in FEATURE_CHARS)):
                    words_counter[word] += 1
        
        cap_num = 0
        cap_seq_num = 0
        longest_cap_seq = 0
        current_cap_seq_len = 0
        for line in open(file_path):
            for letter in line:
                if letter.isupper():
                    cap_num += 1
                    current_cap_seq_len += 1
                elif current_cap_seq_len > 0:
                    longest_cap_seq = max(current_cap_seq_len, longest_cap_seq)
                    current_cap_seq_len = 0
                    cap_seq_num += 1

        words_freq_counter["capital_run_length_average"] = cap_num * 1.0 / cap_seq_num
        words_freq_counter["capital_run_length_longest"] = longest_cap_seq
        words_freq_counter["capital_run_length_total"] = cap_num

        once_words = dict.fromkeys(words_counter.keys(), 1)
        class_words_counter.update(once_words)
    
        keys = words_counter.keys()
        word_keys = [k for k in keys if len(k) > 1]
        punc_keys = [k for k in keys if len(k) == 1]
    
        word_stats =  { k: words_counter[k] for k in word_keys }
        punc_stats =  { k: dict(words_counter)[k] for k in punc_keys }
    
        word_total = float(sum(word_stats.values()))
        punc_total = float(sum(punc_stats.values()))
        
        for word in word_stats:
            words_freq_counter[word] = word_stats[word] * 100.0 / word_total
    
        for punc in punc_stats:
            words_freq_counter[punc] = punc_stats[punc] * 100.0 / punc_total
    
        words_freq_counter_data.append(words_freq_counter)

    class_words_counter = { k: class_words_counter[k] for k in class_words_counter.keys() if k not in STOPLIST }
    for k,v in class_words_counter.items():
        class_words_counter[k] = v * 1.0 /  email_size
    return Counter(class_words_counter), words_freq_counter_data

def extract_features():
    stoplist_path = "stoplist.txt"
    for line in open(stoplist_path):
        for stop in line.split():
            STOPLIST.append(stop)
    
    ham_words_counter, ham_freq_counter_data = process_class("data/enron1/ham")
    spam_words_counter, spam_freq_counter_data = process_class("data/enron1/spam")
    
    total_counter = ham_words_counter + spam_words_counter
    
    diff_counter = Counter()
    for k in total_counter.keys():
        diff_counter[k] = abs(ham_words_counter[k] - spam_words_counter[k])
    
    features = [k for k,v in diff_counter.most_common(200)]
    
    if CONSIDER_CAP_FEATURES:
        features += CAP_FEATUES

    features_data = open('features.txt', 'wt')
    features_data.write(",".join(features))
    features_data.close()
    
    tf_data = open('tf_data.txt', 'wt')
    bool_data = open('bool_data.txt', 'wt')
    tf_csv_data = open('tf_csv_data.csv', 'wt')
    bool_csv_data = open('bool_csv_data.csv', 'wt')
    tf_csv_data.write(",".join([str(i) for i in xrange(1,202)]) + "\n")
    bool_csv_data.write(",".join([str(i) for i in xrange(1,202)]) + "\n")
    for c in ['ham', 'spam']:
        for m in eval(c + '_freq_counter_data'):
            tf_output = []
            bool_output = []
            
                
            for feature in features:
                tf_output.append(str(m[feature]))
                bool_output.append("0" if m[feature] == 0 else "1")
            
            label = '0\n' if c == 'ham' else '1\n'
            tf_output.append(label)
            bool_output.append(label)
            tf_data.write(" ".join(tf_output))
            bool_data.write(" ".join(bool_output))
            tf_csv_data.write(",".join(tf_output))
            bool_csv_data.write(",".join(bool_output))
    
    tf_data.close()
    bool_data.close()
    tf_csv_data.close()

extract_features()
