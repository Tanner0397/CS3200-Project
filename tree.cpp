#include "node.h"

//----NODE----

Node::Node()//Constructor
{
  numKeys = 0;//First its empty
  for(unsigned int i = 0; i < TREE_ORDER; i++)
  {
    childNodes[i] = nullptr;
    parent = nullptr;
  }//end for loop
}//end node constructor

Node::Node(const Node &node)
{
  keys = node.keys;
  for(unsigned int i = 0; i < TREE_ORDER; i++)
  {
    childNodes[i] = node.childNodes[i];
  }//end for
  parent = node.parent;
}//end copy constructor

unsigned int Node::getNumKeys()
{
  return keys.size();
}//end getNumKeys method

unsigned int Node::getKey(int index)
{
  return keys[index];
}//end getKeys

unsigned int Node::getLastKey()
{
  if(!keys.empty())
    return keys.back();
}//end get last key meothod

bool Node::isLeafNode()
{
  if(dynamic_cast<leafNode*>(this))
  {
    return true;
  }
  return false;
}//end isLeafNode

Node* Node::getChild(int index)
{
  return childNodes[index];
}//end getChild

Node* Node::getParent()
{
  return parent;
}

bool Node::isFull()
{
  if(keys.size() == TREE_ORDER-1)
    return true;
  return false;
}//end isFull method

void Node::insertKey(unsigned int key)
{
  keys.push_back(key);
  std::sort(keys.begin(), keys.end());
}//end insertionKey

std::vector<unsigned int> Node::getKeys()
{
  return keys;
}//end getKeys method

void Node::deleteKeyIndex(int index)
{
  if(/*index < keys.size()*/true)
  {
    numKeys--;
    keys.erase(keys.begin()+index);
  }//end if

}//end deleteKey method

void Node::setChild(Node* node, int index)
{
  childNodes[index] = node;
}//end setChild moethod

void Node::setParent(Node* node)
{
  parent = node;
}

void Node::insertChild(Node* node)
{
  unsigned int last = node->getLastKey();
  //if we're using the arrange children method the node will have 2 or 3 keys, not one
  if(keys.size() == 2)//this node has 2 keys
  {
    //Node* temp;//used to swap
    if(last <= keys[0])
    {/*Since the last ket of node is less than or equal to the last node of the first
      key, the first key must be the new element that was inserted into the node*/

      //shift children
      childNodes[2] = childNodes[1];
      childNodes[1] = childNodes[0];
      childNodes[0] = node;
    }//end if last key of node is less thatn or equal to the first key of the node
    else if(keys[0] < last && last <= keys[1])
    {/*here the second key is the new key, so the last child for 2 nodes is new*/
      childNodes[2] = childNodes[1];
      childNodes[1] = node;
    }//if the last key of node is greater than keys[0] but less than or equal to keys[1]
    else
    {
      childNodes[2] = node;
    }
  }//end if
  else if(keys.size() == 3)//numKeys == 3
  {
    if(last <= keys[0])
    {
      childNodes[3] = childNodes[2];
      childNodes[2] = childNodes[1];
      childNodes[1] = childNodes[0];
      childNodes[0] = node;
    }//end f
    else if(keys[0] < last && last <= keys[1])
    {
      childNodes[3] = childNodes[2];
      childNodes[2] = childNodes[1];
      childNodes[1] = node;
    }//end else if
    else if(keys[1] < last && last <= keys[2])
    {
      childNodes[3] = childNodes[2];
      childNodes[2] = node;
    }
    else
    {
      childNodes[3] = node;
    }
  }//end else
}//end arrangeChildren Method

std::string Node::print(int num)
{
  std::string buffer;
  buffer += ("node" + std::to_string(num) + " [label = \"");
  for(unsigned int i = 0; i < TREE_ORDER-1; i++)
  {
    if(i < keys.size())
      buffer += "<f" + std::to_string(i) + "> |" + std::to_string(keys[i]) + "|";
    else
      buffer += "<f" + std::to_string(i) + "> | |";
  }//end for
  buffer += "<f3> \"]\n";
  return buffer;
}//end print method

//----END NODE----
//----LEAF NODE----

leafNode::leafNode()
{
  Node();
  siblings[0] = nullptr;
  siblings[1] = nullptr;
}//end leafNode constructor

leafNode* leafNode::getLeftSibling()
{
  return siblings[0];
}//end left get

leafNode* leafNode::getRightSibling()
{
  return siblings[1];
}//end Right get

void leafNode::setLeftSibling(leafNode* node)
{
  siblings[0] = node;
}//end left get

void leafNode::setRightSibling(leafNode* node)
{
  siblings[1] = node;
}//end Right get

//----END LEAF NODE----
//----B PLUS TREE----

BPlusTree::BPlusTree()
{
  root = new leafNode();//Starting Node is a leaf node!
}//end BPlusTree constructor

Node* BPlusTree::searchKey(unsigned int const key)
{
  return search(root, key);
}//end searchKey method

Node* BPlusTree::search(Node* node ,unsigned int const key)
{
  if(node->isLeafNode())
  {
    return node;
  }//this is the node that contains the key
  else
  {
    if(node->getNumKeys() == 1)//There is only 1 key in this node
    {
      if(key <= node->getKey(0))
        return BPlusTree::search(node->getChild(0), key);//The left most child Node
      else
        BPlusTree::search(node->getChild(1), key);
    }//end if numKeys == 1
    else if(node->getNumKeys() == 2)//There are 2 keys in the node
    {
      if(key <= node->getKey(0))
        return BPlusTree::search(node->getChild(0), key);//goto left most child
      else if(key > node->getKey(0) && key <= node->getKey(1))//keys[0] < key <= keys[1]
        return BPlusTree::search(node->getChild(1), key);//goto middle child
      else//The key is greater than all the keys in the node
        return BPlusTree::search(node->getChild(2), key);//goto right most child
    }//end if numKeys == 2
    else if(node->getNumKeys() == 3)
    {
      if(key <= node->getKey(0))
        return BPlusTree::search(node->getChild(0), key);//goto left most child
      else if(key > node->getKey(0) && key <= node->getKey(1))//keys[0] < key <= keys[1]
        return BPlusTree::search(node->getChild(1), key);//goto second child
      else if(key > node->getKey(1) && key <= node->getKey(2))//keys[1] < key <= keys[2]
        return BPlusTree::search(node->getChild(2), key);//goto third child
      else//The key is greater than all the keys in the node
        return BPlusTree::search(node->getChild(3), key);//goto right most child
    }//end else numKeys == 3
  }//end else
}//end search method

void BPlusTree::split(Node* node, unsigned int const key)
{
  std::vector<unsigned int> copy = node->getKeys();//get copy of keys
  copy.push_back(key);
  std::sort(copy.begin(), copy.end());//Sort the keys
  /*copy has all the keys values + 1 extra node. The left two nodes will
  be put in the left child and the 2 right most nodes will be put in the right child
  The second child from the lest will be copied into the parent node*/

  //Create a new leaf node and move half of the keys to it
  if(node->isLeafNode())
  {
    Node* parent = node->getParent();
    leafNode* newLeaf = new leafNode();
    newLeaf->setLeftSibling(dynamic_cast<leafNode*>(node));//this node is the left sibling of the node
    newLeaf->setRightSibling(dynamic_cast<leafNode*>(node)->getRightSibling());//right sibling of the new leaf is old right sibling of the node
    dynamic_cast<leafNode*>(node)->setRightSibling(newLeaf);//right sibling of the node is the new leaf
    newLeaf->insertKey(copy[2]);
    newLeaf->insertKey(copy[3]);//Insert the last two elements of the keys
    newLeaf->setParent(parent);//set parent of new sibling
    node->deleteKeyIndex(LAST_KEY);//Delete the last element
    node->deleteKeyIndex(LAST_KEY-1);//Delete the last element
    node->insertKey(copy[1]);//insert the key being split
    if(node != root)//If the parent of the node isn't full
    {
      if(!parent->isFull())//SEG FAULT - PARENT IS NULL!
      {
        parent->insertKey(copy[1]);//insert the second key of the copy vector
        parent->insertChild(newLeaf);
      }
    }//end if
    else if(node != root && parent->isFull())
    {
      //The parent node is full, so that also needs to be split
      split(parent, copy[2]);//This will be an internal node
      //Do Stuff------------------------
    }
    else if(node == root)
    {
      Node* newRoot = new Node();
      newRoot->insertKey(copy[1]);//insert second key
      newRoot->setChild(node, 0);//node is leftChild
      newRoot->setChild(newLeaf, 1);//newLeaf is right child
      node->setParent(newRoot);
      newLeaf->setParent(newRoot);
      root = newRoot;//Set the new root
    }
  }//end ifLeaf
  else//This is an internal node
  {
    Node* parent = node->getParent();
    Node* newNode = new Node();
    newNode->insertKey(copy[2]);
    newNode->insertKey(copy[3]);//insert last 2 keys in the last node
    newNode->setParent(parent);
    node->deleteKeyIndex(LAST_KEY);//
    node->deleteKeyIndex(LAST_KEY-1);//internel nodes don't need a redundant entry
    if(node != root && !parent->isFull())
    {
      parent->insertKey(copy[1]);//the second key
      parent->insertChild(newNode);
    }//end if
    else if(node != root && parent->isFull())
    {
      //The parent node is full, so that also needs to be split
      split(parent, copy[1]);//This will be an internal node
      //Do Stuff------------------------
    }//end else parent is full
    else if(node == root)
    {
      Node* newRoot = new Node();
      newRoot->insertKey(copy[1]);//insert second key
      newRoot->setChild(node, 0);//node is leftChild
      newRoot->setChild(newNode, 1);//newLeaf is right child
      node->setParent(newRoot);//new level new parent
      newNode->setParent(newRoot);//set parent because new level
      root = newRoot;//Set the new root;
    }//ense else node is parent
  }//end else internal node

}//end split method

void BPlusTree::insert(Node* node, unsigned int const key)
{
  Node* result = BPlusTree::searchKey(key);
  if(!result->isFull())
  {
    result->Node::insertKey(key);
  }//end if node is full
  else//the node is full
  {
    BPlusTree::split(result, key);
  }//end else
}//end insert method

void BPlusTree::insertKey(unsigned int const key)
{
  BPlusTree::insert(root, key);
}//end insert key method

void BPlusTree::iteratePrint(Node* node, std::ofstream &buffer, int &index)
{
  int counter = 0;
  //buffer += node->print(index);
  if(node->getChild(0) != nullptr)
  {
    buffer << node->getChild(0)->print(index+1);
    buffer << "node" << std::to_string(index) << ":f0->node" << std::to_string(index+1) << "\n";
    counter++;
  }
  if(node->getChild(1) != nullptr)
  {
    buffer << node->getChild(1)->print(index+2);
    buffer << "node" << std::to_string(index) << ":f1->node" << std::to_string(index+2) << "\n";
    counter++;
  }
  if(node->getChild(2) != nullptr)
  {
    buffer << node->getChild(2)->print(index+3);
    buffer << "node" << std::to_string(index) << ":f2->node" << std::to_string(index+3) << "\n";
    counter++;
  }
  if(node->getChild(3) != nullptr)
  {
    buffer << node->getChild(3)->print(index+4);
    buffer << "node" << std::to_string(index) << ":f2->node" << std::to_string(index+4) << "\n";
    counter++;
  }//Done printing off children
  //--------------------------------------------------------
  index += counter;
  if(node->getChild(0) != nullptr)
    iteratePrint(node->getChild(0), buffer, index);
  if(node->getChild(1) != nullptr)
    iteratePrint(node->getChild(1), buffer, index);
  if(node->getChild(2) != nullptr)
    iteratePrint(node->getChild(2), buffer, index);
  if(node->getChild(3) != nullptr)
    iteratePrint(node->getChild(3), buffer, index);
}//end print method

void BPlusTree::print()
{
  int index = 0;
  std::ofstream buffer;
  buffer.open("output.gv");
  buffer << "digraph {\n node [ shape = record]\n";
  buffer << root->print(index) << std::endl;
  iteratePrint(root, buffer, index);
  buffer << "}";
}//end print method

//----END B PLUS TREE----