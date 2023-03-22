#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{
}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{
}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
		void insertFix(AVLNode<Key, Value>* parent, AVLNode<Key,Value>* new_node);
		void removeFix(AVLNode<Key,Value>* parent, int8_t diff);
		void rotateLeft(AVLNode<Key,Value>* current);
		void rotateRight(AVLNode<Key,Value>* current);



};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
	
	if(this->root_ == nullptr){
		this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
	}
	else{
    AVLNode<Key,Value> *current = static_cast<AVLNode<Key,Value>*>(this->root_);
    // TODO
		bool finished = false;
		while(finished == false){
			if(current->getKey() == new_item.first){
				current->setValue(new_item.second);
          finished = true;
			}

			else if(current->getKey() < new_item.first){
        if(current->getRight()!=NULL){
        	current = current->getRight();
        }
        else{
          current->setRight(new AVLNode<Key, Value>(new_item.first, new_item.second, current));
					if(current->getBalance()==-1){
						current->setBalance(0);	
						finished = true;
					}
					else if(current->getBalance()==0){
						current->setBalance(1);
						insertFix(current,current->getRight());
						finished = true;
					}     
        }
			}

			else if(current->getKey() > new_item.first){
      	if(current->getLeft()!= NULL){
          current = current->getLeft();
        }
        else{
        	current->setLeft(new AVLNode<Key, Value>(new_item.first, new_item.second, current));
					if(current->getBalance()==1){
							current->setBalance(0);
							finished = true;		
							
					}
					else if(current->getBalance()==0){
						current->setBalance(-1);		
						insertFix(current,current->getLeft());
						finished = true;
					}
		    }  
      }
		}
	}	
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key,Value>* parent, AVLNode<Key,Value>* new_node)
	{
	
		if(parent == NULL||parent->getParent() == NULL){
			return;
		}
		AVLNode<Key,Value>* grandparent = parent->getParent();
		
		if(grandparent->getRight() == parent){
			grandparent->updateBalance(1);
			if(grandparent->getBalance()==0){
				
			}
			else if(grandparent->getBalance()==1){
				insertFix(grandparent,parent);
			}
			else{
				if(parent->getRight()== new_node){
					grandparent->setBalance(0);
					parent->setBalance(0);
					rotateLeft(grandparent);
				}
				else{
					if(new_node->getBalance()==-1){
						parent->setBalance(1);
						grandparent->setBalance(0);
						new_node->setBalance(0);
					}
					else if(new_node->getBalance()==0){
						parent->setBalance(0);
						grandparent->setBalance(0);
						new_node->setBalance(0);
					}
					else if(new_node->getBalance()==+1){
						parent->setBalance(0);
						grandparent->setBalance(-1);
						new_node->setBalance(0);
					}
					rotateRight(parent);
					rotateLeft(grandparent);
				}
			}
		}
		else if(parent==grandparent->getLeft()){
			grandparent->updateBalance(-1);
			if(grandparent->getBalance()==0){
			}
			else if(grandparent->getBalance()==-1){
				insertFix(grandparent,parent);
			}
			else{
				if(parent->getLeft()==new_node){
					parent->setBalance(0);
					grandparent->setBalance(0);
					rotateRight(grandparent);
				}
				else{
					
					if(new_node->getBalance()==-1){
						parent->setBalance(0);
						grandparent->setBalance(1);
						new_node->setBalance(0);
					}
					else if(new_node->getBalance()==0){
						parent->setBalance(0);
						grandparent->setBalance(0);
						new_node->setBalance(0);
					}
					else if(new_node->getBalance()==1){
						parent->setBalance(-1);
						grandparent->setBalance(0);
						new_node->setBalance(0);
					}
					rotateLeft(parent);
					rotateRight(grandparent);
				}
			}
		}


		//insert fix  precondition p/n are balanced
		//postfix = g, p, and n are balanced
		//if p is null or parent return
		//let g =parent(p)
		//assume
		/*Assume p is left child of g [For right child swap left/right, +/-]
– b(g) += -1 // Update g's balance to new accurate value for now
– Case 1: b(g) == 0, return
– Case 2: b(g) == -1, insertFix(g, p) // recurse
– Case 3: b(g) == -2
• If zig-zig then rotateRight(g); b(p) = b(g) = 0
• If zig-zag then rotateLeft(p); rotateRight(g);
– Case 3a: b(n) == -1 then b(p) = 0; b(g) = +1; b(n) = 0;
– Case 3b: b(n) == 0 then b(p) = 0; b(g) = 0; b(n) = 0;
– Case 3c: b(n) == +1 then b(p)= -1; b(g) = 0; b(n) = 0;

*/

}
	

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
	
	if(this->root_ == nullptr){
			return;
		}
	AVLNode<Key,Value>* current = static_cast<AVLNode<Key,Value>*>(this->internalFind(key));
	if(current==NULL){
		return;
	}
	int diff;
	
	if(current->getLeft()!=NULL && current->getRight()!=NULL){
		AVLNode<Key,Value> *pred = static_cast<AVLNode<Key,Value>*>(this->predecessor(current));
		nodeSwap(current, pred);	
	}
	AVLNode<Key,Value>* parent = current->getParent();
	if(current->getParent()!=NULL){
		if(current->getParent()->getRight()==current){
		diff = -1;

		}
		else if(current->getParent()->getLeft()==current){
			diff = 1;
		}	
	}
	if(current->getLeft()==nullptr && current->getRight()==nullptr){
		if(current == this->root_){
			delete current;
			this->root_ = nullptr;
			removeFix(parent,diff);		
		}
		else if(current == current ->getParent()->getRight()){
			current -> getParent()->setRight(nullptr);
			delete current;
			removeFix(parent,diff);			
		}
		else if(current == current->getParent()->getLeft()){
			current -> getParent()->setLeft(nullptr);
			delete current;
			removeFix(parent,diff);			
		}	
	}
	else if(current->getLeft()==nullptr&&current->getRight()!=nullptr){
		if(current == this->root_){
			current->getRight()->setParent(nullptr);
			this->root_ = current->getRight();
			delete current;
			removeFix(parent,diff);		
		}
		else if(current->getParent()->getRight() == current){
			current ->getParent()->setRight(current->getRight());
			current->getRight()->setParent(current->getParent());
			delete current;	
			removeFix(parent,diff);
		}	
		else if(current->getParent()->getLeft() == current){
			current ->getParent()->setLeft(current->getRight());
			current->getRight()->setParent(current->getParent());
			delete current;				
			removeFix(parent,diff);	
		}
	}
	else if(current->getLeft()!=nullptr&&current->getRight()==nullptr){
		if(current == this->root_){
			current->getLeft()->setParent(nullptr);
			this->root_ = current->getLeft();	
			delete current;		
			removeFix(parent,diff);				
		}
		else if(current->getParent()->getRight() == current){
			current ->getParent()->setRight(current->getLeft());
			current->getLeft()->setParent(current->getParent());
			delete current;			
			removeFix(parent,diff);	
		}		
		else if(current->getParent()->getLeft() == current){
			current ->getParent()->setLeft(current->getLeft());
			current->getLeft()->setParent(current->getParent());
			delete current;		
			removeFix(parent,diff);
		}			
	}
}


template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key,Value>* parent,int8_t diff)
{
	if(parent == nullptr){
		return;
	}
	AVLNode<Key,Value>* grandparent = parent->getParent();
	int8_t difff;
	if(grandparent != nullptr){
		if(parent==grandparent->getLeft()){
			difff = 1;
		}
		else if(parent==grandparent->getRight()){
			difff = -1;
		}
	}
		if(parent->getBalance()+diff == -2){
			if(parent->getLeft()->getBalance() == -1){
				parent->setBalance(0);
				parent->getLeft()->setBalance(0);
				rotateRight(parent);
				removeFix(grandparent,difff);
			}
			else if(parent->getLeft()->getBalance() == 0){
				parent->setBalance(-1);
				parent->getLeft()->setBalance(1);
				rotateRight(parent);
			}
			else if(parent->getLeft()->getBalance() == 1){
				if(parent->getLeft()->getRight()->getBalance()==1){
					parent->setBalance(0);
					parent->getLeft()->setBalance(-1);
					parent->getLeft()->getRight()->setBalance(0);
				}
				else if(parent->getLeft()->getRight()->getBalance() == 0){
					parent->setBalance(0);
					parent->getLeft()->setBalance(0);
					parent->getLeft()->getRight()->setBalance(0);
				}
				else if(parent->getLeft()->getRight()->getBalance()== -1){
					parent->setBalance(1);
					parent->getLeft()->setBalance(0);
					parent->getLeft()->getRight()->setBalance(0);
				}
				rotateLeft(parent->getLeft());
				rotateRight(parent);
				removeFix(grandparent, difff);
				}
		}
	else if (parent->getBalance()+diff == 2){
		if(parent->getRight()->getBalance() == 1){
			parent->setBalance(0);
			parent->getRight()->setBalance(0);
			rotateLeft(parent);
			removeFix(grandparent,difff);
		}
		else if(parent->getRight()->getBalance() == 0){
			parent->setBalance(1);
			parent->getRight()->setBalance(-1);
			rotateLeft(parent);
		}
		else if(parent->getRight()->getBalance() == -1){
			if(parent->getRight()->getLeft()->getBalance()==-1){
				parent->setBalance(0);
				parent->getRight()->setBalance(1);
				parent->getRight()->getLeft()->setBalance(0);
			}
			else if(parent->getRight()->getLeft()->getBalance() == 0){
				parent->setBalance(0);
				parent->getRight()->setBalance(0);
				parent->getRight()->getLeft()->setBalance(0);
			}
			else if(parent->getRight()->getLeft()->getBalance()== 1){
				parent->setBalance(-1);
				parent->getRight()->setBalance(0);
				parent->getRight()->getLeft()->setBalance(0);
			}
			rotateRight(parent->getRight());
			rotateLeft(parent);
			removeFix(grandparent, difff);
		}
	}
	else if(parent->getBalance()+diff == 1){
		parent->setBalance(1);
	}
	else if(parent->getBalance()+diff== -1){
		parent->setBalance(-1);
	}
	else if(parent->getBalance()+diff==0){
		parent->setBalance(0);
		removeFix(grandparent,difff);
	}
}
template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key,Value>* current)
{
	
	AVLNode<Key,Value>* temp = current->getLeft();
	if(current->getParent()!=nullptr){
		if(current->getParent()->getRight()==current){
			current->getParent()->setRight(temp);
		}
		else{
			current->getParent()->setLeft(temp);
		}
	}
	else{
		this->root_ = temp;
	}
	temp->setParent(current->getParent());
	current->setParent(temp);
	current->setLeft(temp->getRight());
	if(temp->getRight()!= NULL){
		temp->getRight()->setParent(current);
	}
	temp->setRight(current); 
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key,Value>* current)
{
	AVLNode<Key,Value>* temp = current->getRight();
	if(current->getParent()!=nullptr){
		if(current->getParent()->getRight()==current){
			current->getParent()->setRight(temp);
		}
		else{
			current->getParent()->setLeft(temp);
		}
	}
	else{
		this->root_ = temp;
	}
	temp->setParent(current->getParent());
	current->setParent(temp);
	current->setRight(temp->getLeft());
	if(temp->getLeft()!= NULL){
		temp->getLeft()->setParent(current);
	}
	temp->setLeft(current);
	
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
