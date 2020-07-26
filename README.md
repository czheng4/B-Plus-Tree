# B-Plus-Tree
This B+Tree is implemented in c++ using template, which allows you to use different data types for key and value. Additionally, you can specify the maximum number of children using template. One of the examples could be `Tree<double, string, 5>`. It tells the template that the data types of key and value are `double` and `string` respectively, and the maximum number of children the internal node hold is 5. 

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
| at                | Access elements |
| operator[]        | Access elements. It has the same behavior of the map. If the key doesn't exist and mapped value is not assigned to the return reference value, the size of B+Tree still gets increased by one with a default value |
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




  

