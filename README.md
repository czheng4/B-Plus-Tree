# B-Plus-Tree
This B+Tree is implemented in c++ using template, which allows you to use different data types for key and value. Additionally, you can specify the maximum number of children using template. One of the examples could be `Tree<double, string, 5>`. It tells the template that the data types of key and value are `double` and `string` respectively, and the maximum number of children the internal node hold is 5. If the third template parameter is not specified, 5 will be used.

# B+Tree Member functions

| Function Name     | Explanation   |
| -------------     | ------------- |
| insert(key, val)  | Insert a record into B+Tree. It the key exists in the tree, the value will be overwritten by current value |
| find(key)         | Return an iterator to the record equal to the given key. If the key doesn't exist, it returns end() |
| erase(key)        | Remove the record equal to the given key from B+Tree. Nothing happens if key doesn't exist |
| erase(it)         | Remove the record from B+Tree given an iterator. |
| erase(rit)        | Remove the record from B+Tree given a reverse iterator |
| contains(key)     | Return true if key exists in B+Tree | 
| size()            | Return the size of B+Tree |
| empty()           | Return true if the B+Tree is empty |
| clear()           | Clear the entire B+Tree |
| lower_bound(key)  | Return an iterator pointing the record whose key is greater than or equal to a given key. If there's no such a record, it returns end() |
| upper_bound(key)  | Return an iterator pointing the record whose key is greater than a given key. If there's no such a record, it returns end() |
| get_keys()        | Return a vector of all keys in B+Tree |
| get_vals()        | Return a vector of all values in B+Tree |
| at                | Access elements. It has the same behavior of `map` |
| operator[]        | Access elements. It has the same behavior of `map` If the key doesn't exist and mapped value is not assigned to the return reference value, the size of B+Tree still gets increased by one with a default value |
| begin()           | Return iterator to beginning |
| end()             | Return iterator to end (one pass the last record) |
| rbegin()          | Return reverse iterator to reverse beginning |
| rend()            | Return reverse iterator to reverse end (one before the first record) |

# iterator/reverse_iterator Member functions

| Function Name     | Explanation   |
|-------------------|----------------|
| get_key()         | Return the key |
| get_val()         | Retrun the value |
| set_val(val)      | Set the value |
| advance(distance) | Move the iterator by distance. More precisely, if distance is greater than 0, operator++ get called for "distance" times. If the distance is less than 0, operator-- get called for "distance" times |

# Example

```
#include <iostream>
#include <string>
#include "b+tree.h"
using namespace BPlusTree;
using namespace std;

int main(int argc, char **argv)
{
  Tree<double, string> t;
  Tree<double, string>::iterator it;
  Tree<double, string>::reverse_iterator rit;

 
  t.insert(4, "A");  // insert (4,A) into B+Tree
  t.insert(2, "B");  // insert (2,B) into B+Tree

  t[1] = "C";        // insert (1,C) into B+Tree
  t[0] = "D"; 		 // insert (0,D) into B+Tree

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
```
The result is as follows.
```
B
B+Tree: iterator is out of range
1 0
0 -> D
1 -> C
2 -> B
4 -> A

4 -> J
2 -> J
1 -> J
```

# A tool program
A tool program is written for you to let you to insert and delete records, and print the B+Tree info. We will use `double` and `string` as key and value data types, respectively. It has the following commands.

  

