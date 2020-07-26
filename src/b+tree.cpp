#include "b+tree.h"
#include <iostream>
Node::Node() {
  next_leaf = nullptr;
  prev_leaf = nullptr;
}

Tree::Tree(size_t max): max_degree(max) {
  root = new Node();
}

void Tree::erase(double key) {

  Node *n = root; 
  size_t i;
  int delete_index = -1;
  int min_keys = (max_degree - 1) / 2;
  int size;
  Node *left, *right;
  Node *parent;
  size_t traverse_index;
  vector <size_t> traverse_indices;
  vector <Node*> parents;
  bool records = true;

  /* remember an internal node along with index, whose key is euqal to param "key" 
     when we delete the leftmost key in the subtree, we will update the internal node's key,
     which has the same value as param "key". The new key will be the new replaced element.
  */
  Node *same_value_node = nullptr;
  int same_value_index = -1;

  /* find the leaf node first */
  while (n->nodes.size() != 0) {
    for (i = 0; i < n->keys.size(); i++) {
      if (key == n->keys[i]) {
        same_value_node = n;
        same_value_index = i;
      }
      if (key < n->keys[i]) break;
    }

    if (n->nodes.size() != 0) {
      traverse_indices.push_back(i);
      parents.push_back(n);
      n = n->nodes[i];
    }
  }

  /* find the index */
  for (i = 0; i < n->keys.size(); i++) {
    if (key == n->keys[i]) {
      delete_index = i;
      break;
    }
  }

  /* key is not found in tree */
  if(delete_index == -1) return;
  
  
  /* delete the record */
  n->keys.erase(n->keys.begin() + delete_index);
  n->vals.erase(n->vals.begin() + delete_index);
  if (n == root) return;
  

  left = n->prev_leaf;
  right = n->next_leaf;

  /* case1: the bucket is big enough after deletion operation */
  if ((int)n->keys.size() >= min_keys) {
    
    /* the leftmost key is deleted */
    if (same_value_node != nullptr) {
        same_value_node->keys[same_value_index] = n->keys[0];
    }
    return;
  }
  
  /* merge or borrow the node from neighbors 
     until the size of current bucket is >= min_keys or get to root node 
  */
  while((int)n->keys.size() < min_keys && n != root) {


    left = n->prev_leaf;
    right = n->next_leaf;
    parent = parents[parents.size() - 1];
    parents.pop_back();
    traverse_index = traverse_indices[traverse_indices.size() - 1];
    traverse_indices.pop_back();
      
    /* case:2 borrow from left node 
       when it's not the leftmost node in the substree and the left node's size is big enought.
       traverse_index minus 1 is because the index of key is one less than the index of nodes.
    */
    if (left != nullptr && traverse_index != 0) {
      size = left->keys.size();

      if (size > min_keys) {
      
        traverse_index--;
        /* if it's leaf nodes, we steal the rightmost key and val in the left node 
           and update the parent's key with its key.
           Otherwise we bring down the parent key to the current node and 
           bring up the rightmost key in the left node.
        */
        if(records) {
          n->keys.insert(n->keys.begin(), left->keys[size - 1]);
          parent->keys[traverse_index] = n->keys[0];

          n->vals.insert(n->vals.begin(), left->vals[size - 1]);
          left->vals.pop_back();
        } else {
          n->keys.insert(n->keys.begin(), parent->keys[traverse_index]);
          parent->keys[traverse_index] = left->keys[size - 1];
          n->nodes.insert(n->nodes.begin(), left->nodes[left->nodes.size() - 1]);
          left->nodes.pop_back();
        }
       
        left->keys.pop_back();
      
        return;
      }

    /* case3: borrow from right node */
    } else if (right != nullptr && traverse_index != parent->nodes.size() - 1) {

     
      size = right->keys.size();

      if (size > min_keys){
         /* the leftmost key in the subtree could be deleted */
         if (same_value_node != nullptr) {
          same_value_node->keys[same_value_index] = n->keys[0];
         }
        
        /* if it's leaf nodes, we steal the leftmost key and val in the right node 
           and update the parent's key with its key.
           Otherwise we bring down the parent key to the current node and 
           bring up the leftmost key in the right node.
        */

        if(records) {
          n->keys.push_back(right->keys[0]);
          // I haven't delete it, so we use index 1.
          parent->keys[traverse_index] = right->keys[1]; 
          
          n->vals.push_back(right->vals[0]);
          right->vals.erase(right->vals.begin());       

        } else {

          n->keys.push_back(parent->keys[traverse_index]);
          parent->keys[traverse_index] = right->keys[0];
          
          n->nodes.push_back(right->nodes[0]);
          right->nodes.erase(right->nodes.begin());
        }
         

        right->keys.erase(right->keys.begin());
        
      
        return;

      }
    }

    /* case:4 merge the node to left node */
    if (left != nullptr && traverse_index != 0) {
      
      
      /* reset the next and prev leaf */
      left->next_leaf = n->next_leaf;
      if (n->next_leaf != nullptr) n->next_leaf->prev_leaf = left;
      

      /* when it's not leaf nodes, bring down the parent key and merge nodes as well */
      if(!records) {
        left->keys.push_back(parent->keys[traverse_index - 1]);
        for (i = 0; i < n->nodes.size(); i++){
          left->nodes.push_back(n->nodes[i]);
        }
      }

      /* merge keys */
      for (i = 0; i < n->keys.size(); i++) {
        left->keys.push_back(n->keys[i]);
        if(records) left->vals.push_back(n->vals[i]);
      }


      // erase the parent key and node
      parent->keys.erase(parent->keys.begin() + traverse_index - 1);
      parent->nodes.erase(parent->nodes.begin() + traverse_index);

     
      /* merge into a root node */
      if(parent->keys.size() == 0 && parent == root) {
      
        delete n;
        delete root;
        root = left;         
        return;
      }

      delete n;
      n = parent;
      

    /* case5: merge the right node to n node */
    } else if (right != nullptr && traverse_index != parent->nodes.size() - 1) {

      /* we may delete the leftmost key in the subtree */
      if (same_value_node != nullptr) {
        same_value_node->keys[same_value_index] = n->keys[0];
      }
     
      n->next_leaf = right->next_leaf;
      if (right->next_leaf != nullptr) right->next_leaf->prev_leaf = n;


      /* when it's not leaf nodes, bring down the parent key and merge nodes as well */
      if(!records) {
        n->keys.push_back(parent->keys[traverse_index]);
        for (i = 0; i < right->nodes.size(); i++){
          n->nodes.push_back(right->nodes[i]);
        }
      }

      /* get keys and vals */
      for(i = 0; i < right->keys.size(); i++) {
        n->keys.push_back(right->keys[i]);
        if(records) n->vals.push_back(right->vals[i]);
      }

      /* update parent nodes and keys */
      parent->nodes[traverse_index + 1] = n;
      parent->keys.erase(parent->keys.begin() + traverse_index);
      parent->nodes.erase(parent->nodes.begin() + traverse_index);

      
      if(parent->keys.size() == 0 && parent == root) {
        delete right;
        delete root;
        root = n;
        return;
      }

      delete right;
      n = parent;
 
    }

    records = false;
    same_value_node =nullptr;
  }



}

bool Tree::insert(double key, void *data) {
  
  if (find(key) != nullptr) {
    cout << "exists" << endl;
    return false;
  }
  size_t i, j, size, median_key;
  size_t traverse_index;
  vector <size_t> traverse_indices;
  vector <Node*> parents;

  Node *right;
  Node *n = root; 
  Node *parent;
  bool records = true;

  
  /* find the leaf node */
  while (1) {

    for (i = 0; i < n->keys.size(); i++) {
      if (key < n->keys[i]) break;
    }
    if (n->nodes.size() != 0) {
      traverse_indices.push_back(i);
      parents.push_back(n);
      n = n->nodes[i];
    } else break;

  }

 
  /* put the val and key in the proper postion */
  n->keys.insert(n->keys.begin() + i, key);
  n->vals.insert(n->vals.begin() + i, data);
 

  /* split the node until the bucket(key) is not full any more */
  while (n->keys.size() == max_degree) {
    median_key = n->keys[max_degree / 2];

    
    /* no matter weather we split the internal node or root node 
       We need the "right" node. When we split the nodes that contain records, the median was kept. 
       Otherwise, the median was deleted. 

    */
    right = new Node();
    if (records) j = max_degree / 2;
    else j = max_degree / 2 + 1;
    
    for (i = j; i < max_degree; i++) {
      if (records) right->vals.push_back(n->vals[i]);
      right->keys.push_back(n->keys[i]);
    }
     

    for (i = (n->nodes.size() + 1) / 2; i < n->nodes.size(); i++) {
      right->nodes.push_back(n->nodes[i]);
    }

    /* when we split the root node, create the new parent node.
       The original node became the "left" node.
    */
    if (traverse_indices.size() == 0) {


      parent = new Node();

      parent->nodes.push_back(n);
      parent->nodes.push_back(right);
      parent->keys.push_back(median_key);
      
      /* connect the leaf nodes */
    
      right->next_leaf = n->next_leaf;
      if (n->next_leaf != nullptr) n->next_leaf->prev_leaf = right;
      n->next_leaf = right;
      right->prev_leaf = n;

      

     
      root = parent;
      n->keys.resize(max_degree / 2);
      if (records) n->vals.resize(max_degree / 2);

      if (n->nodes.size() != 0) n->nodes.resize((n->nodes.size() + 1)/ 2);
      // cout << "size: " << parent->nodes.size() << " " << n->nodes.size() << " " << right->nodes.size() << endl;


    } else {

      /* when we split the internal node, the original node keeps the half capacity as the left node.
         Also the median key was added to it's parent.
       */
      
      if(records) n->vals.resize(max_degree/2);
      n->keys.resize(max_degree/2);

      if (n->nodes.size() != 0) n->nodes.resize((n->nodes.size() + 1) / 2); // inrernal node

      /* connect the leaf nodes */
      right->next_leaf = n->next_leaf;
      if (n->next_leaf != nullptr) n->next_leaf->prev_leaf = right;
      n->next_leaf = right;
      right->prev_leaf = n;
      
      parent = parents[parents.size() - 1];
      parents.pop_back();

      traverse_index = traverse_indices[traverse_indices.size() - 1];
      traverse_indices.pop_back();

      parent->keys.insert(parent->keys.begin() + traverse_index, median_key);
      parent->nodes.insert(parent->nodes.begin() + traverse_index + 1, right);
   
      n = parent; 
      records = false;
     
    }

   
  }
 
}

void *Tree::find(double key) {
  Node *n = root; 
  size_t i;

  /* find the leaf node first */
  while (n->nodes.size() != 0) {
    for (i = 0; i < n->keys.size(); i++) {
      if (key < n->keys[i]) break;
    }

    if (n->nodes.size() != 0) n = n->nodes[i];
  }

  /* check to see if we find the key */
  for (i = 0; i < n->keys.size(); i++) {
    if (key == n->keys[i]) return n->vals[i];
  }
  return nullptr;
}
void print(Node *n, int j = 0) {
  if (n == nullptr) return;
  size_t i;
  cout << j << " : ";
  for (i = 0; i < n->keys.size(); i++) {
    cout << n->keys[i] << " ";
  }
  cout << endl;

  for (i = 0; i < n->nodes.size(); i++) {
    print(n->nodes[i], j + 1);
  }
  
}
void Tree::print_keys() {
 

  print(root);

  // cou t << "keys: ";
  // while (n->nodes.size() != 0) {
  //   for (i = 0; i < n->keys.size(); i++) cout << n->keys[i] << " ";
  //   cout << endl;
  //   n = n->nodes[0];
  // }
  // cout << endl;

  // do {
  //   for (i = 0; i < n->keys.size(); i++) {
  //     cout << n->keys[i] << " ";
  //   }
  //   cout << "      ";

  //   n = n->next_leaf;
  // } while (n != nullptr);

  // cout << endl;
}

vector <double> Tree::get_keys() {
  Node *n = root;
  size_t i;
  vector <double> rv;
  while (n->nodes.size() != 0) {
    n = n->nodes[0];
  }
  do {
    for (i = 0; i < n->keys.size(); i++) {
      if (n->keys[i] != ((Person*)(n->vals[i]))->d ) printf("%lf != %lf\n", n->keys[i],  ((Person*)(n->vals[i]))->d);
      rv.push_back(n->keys[i]);
    }
    n = n->next_leaf;
  } while (n != nullptr);

  return rv;
}






