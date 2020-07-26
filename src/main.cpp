#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "b+tree.h"
using namespace BPlusTree;
using namespace std;

void print_commands(FILE *f)
{
  fprintf(f, "A tool program for creating the B+tree,\n");
  fprintf(f, "\n");

  fprintf(f, "INSERT/I key val ...   - Insert a (key, val)\n");
  fprintf(f, "ERASE/E key ...        - Erase the record given a key\n");
  fprintf(f, "FIND/F key ...         - Print the pair given a key\n");
  fprintf(f, "SIZE/S                 - Print the size of tree\n");
  fprintf(f, "KEYS/K                 - Print keys\n");
  fprintf(f, "VALS/V                 - Print vals\n");
  fprintf(f, "TRAVERSE/T A|D         - Traverse the tree and print the pair. A|D is to in ascending or descending order\n");
  fprintf(f, "LB key ...             - Print the pair whose key >= the given key\n");
  fprintf(f, "UB key ...             - Print the pair whose key >  the given key\n");
  fprintf(f, "CLEAR/C                - Clear the tree\n");

 
}

void to_uppercase(string &s) 
{
  size_t i;
  for (i = 0; i < s.size(); i++) {
    if (s[i] >= 'a' && s[i] <= 'z') {
      s[i] = s[i] + 'A' -'a';
    }
  }
}


int main(int argc, char **argv)
{
  Tree<double, string> t;
  Tree<double, string>::iterator it;
  Tree<double, string>::reverse_iterator rit;
  vector <string> sv;
  istringstream ss;
  string prompt, s, l;
  string val;
  double key;
  vector <string> vals;
  vector <double> keys;

  size_t i,size;

  if (argc > 2 || (argc == 2 && strcmp(argv[1], "--help") == 0)) {
    fprintf(stderr, "usage: B+Tree [prompt]\n\n");
    print_commands(stderr);
    exit(1);
    }

  if (argc == 2) {
    prompt = argv[1];
    prompt += " ";
  }


  
  while (1) {
    if (prompt != "") printf("%s", prompt.c_str());
    if (!getline(cin, l)) return 0;
    sv.clear();
    ss.clear();
    ss.str(l);
    while (ss >> s) sv.push_back(s);

    
    size = sv.size();
    if(size != 0) to_uppercase(sv[0]); 

    if (size == 0) {
    } else if (sv[0] == "?") {
      print_commands(stdout);
    } else if (sv[0] == "Q") {
      exit(0);

    } else if (sv[0] == "INSERT" || sv[0] == "I") {
      if (size < 3 || (size - 1) % 2 != 0) {
        printf("usage: INSERT/I key1 val1 key2 val2 ....\n");
      } else {
        for (i = 0; i < (size - 1) / 2; i++) {
          val = sv[2+i*2];

          if(sscanf(sv[1+i*2].c_str(), "%lf", &key) != 1) {
            printf("(%s, %s) is not a valid record\n", sv[1+i*2].c_str(), sv[2+i*2].c_str());
          } else {
            t[key] = val;
          }
        } 
      }

    } else if (sv[0] == "ERASE" || sv[0] == "E") {
      if (size < 2) {
        printf("usage: ERASE/E key1 key2 ...\n");
      } else {
        for (i = 1; i < size; i++) {
          if (sscanf(sv[i].c_str(), "%lf", &key) != 1) {
            printf("%s is not a valid key\n", sv[i].c_str());
          } else if (!t.contains(key)) {
            printf("key %s doesn't exist\n", sv[i].c_str());
          } else {
            t.erase(key);
          }
        }
      } 

    } else if (sv[0] == "FIND" || sv[0] == "F") {
      if (size < 2) {
        printf("usage: FIND/F key1 key2 ...\n");
      } else {

        for (i = 1; i < size; i++) {
          if (sscanf(sv[i].c_str(), "%lf", &key) != 1) {
            printf("%s is not a valid key\n", sv[i].c_str());
          } else {
            it = t.find(key);
            if (it == t.end()) {
              printf("key %s doesn't exist\n", sv[i].c_str());
            } else {
              printf("%s -> %s\n", sv[i].c_str(), it.get_val().c_str());
            }
          }
        }
      } 

    } else if (sv[0] == "SIZE" || sv[0] == "S") {
      printf("size: %zu\n",t.size());

    } else if (sv[0] == "KEYS" || sv[0] == "K") {
      keys = t.get_keys();
      for (i = 0; i < keys.size(); i++) printf("%.2lf ",keys[i]);
      printf("\n");

    } else if (sv[0] == "VALS" || sv[0] == "V") { 
      vals = t.get_vals();
      for (i = 0; i < vals.size(); i++) printf("%s ",vals[i].c_str());
      printf("\n");

    } else if (sv[0] == "TRAVERSE" || sv[0] == "T") {
      if (size != 2 || (sv[1] != "A" && sv[1] != "D")) {
        printf("usage: TRAVERSE/T A|D\n");
      } else {
        if (sv[1] == "A") {
          for (it = t.begin(); it != t.end(); it++) {
            printf("%.2lf -> %s\n", it.get_key(), it.get_val().c_str());
          }
        } else {
          for (rit = t.rbegin(); rit != t.rend(); rit++) {
            printf("%.2lf -> %s\n", rit.get_key(), rit.get_val().c_str());
          }
        }
      }

    } else if (sv[0] == "LB") {
      if (size < 2) {
        printf("usage: LB key1 key2 ...\n");
      } else {
        for (i = 1; i < size; i++) {
          if (sscanf(sv[i].c_str(), "%lf", &key) != 1) {
            printf("%s is not a valid key\n", sv[i].c_str());
          } else {
            it = t.lower_bound(key);
            if (it == t.end()) {
              printf("key %s doesn't have a lower_bound\n", sv[i].c_str());
            } else {
              printf("%s lower_bound: %.2lf -> %s\n", sv[i].c_str(), it.get_key(), it.get_val().c_str());
            }
          }
        }
      }
    } else if (sv[0] == "UP") {
      if (size < 2) {
        printf("usage: UP key1 key2 ...\n");
      } else {
        for (i = 1; i < size; i++) {
          if (sscanf(sv[i].c_str(), "%lf", &key) != 1) {
            printf("%s is not a valid key\n", sv[i].c_str());
          } else {
            it = t.upper_bound(key);
            if (it == t.end()) {
              printf("key %s doesn't have a upper_bound\n", sv[i].c_str());
            } else {
              printf("%s upper_bound: %.2lf -> %s\n", sv[i].c_str(), it.get_key(), it.get_val().c_str());
            }
          }
        }
      }
    } else if (sv[0] == "CLEAR" || sv[0] == "C") {
      t.clear();
    }

  } // end of while 

}





