#include <iostream>
#include <string>
#include "b+tree.h"
using namespace BPlusTree;
using namespace std;

int main()
{
  Tree<double, string> t;
  Tree<double, string>::iterator it;
  Tree<double, string>::reverse_iterator rit;

 
  t.insert(4, "A");  // insert (4,A) into B+Tree
  t.insert(2, "B");  // insert (2,B) into B+Tree

  t[1] = "C";        // insert (1,C) into B+Tree
  t[0] = "D";      // insert (0,D) into B+Tree

  cout << t[2] << endl;
  try {
    cout << t.at(3) << endl;  // 3 is not in B+Tree. It will throw out_of_range exception.
  } catch (std::out_of_range e){
    cout << e.what() << endl;
  }
 
  cout << t.contains(2) << " " << t.contains(5) << endl;

  // ascending order
  for (it = t.begin(); it!= t.end(); ++it) {
    cout << it.get_key() << " -> " << it.get_val() << endl;
    it.set_val("J"); // set the value to J
  }

  // rescending order
  cout << endl;
  t.erase(t.begin());
  for (rit = t.rbegin(); rit!= t.rend(); rit++) {
    cout << rit.get_key() << " -> " << rit.get_val() << endl;
  }

}


