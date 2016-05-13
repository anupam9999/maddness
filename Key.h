/*
 * Key.h
 *
 *  Created on: Aug 29, 2015
 *      Author: Mohammad Mahdi Javanmard
 */

#ifndef _KEY
#define _KEY


/*
 * The class Key, contains the information (level and node) about the position of each node in the tree.
 * It has two data members: level and number.
 * So, for example, for the following tree,
 *
 *
                 ___________________0__________________   ----------------- Level is 0, there is no node with (useful) data in this level
                /                                      \
       _________0________                      _________0________ -------------- Level is 1, there is no node with (useful) data in this level
      /                  \                    /                  \
      X                   X                   X               ____0___    ---------------- Level is 2, there are 3 nodes with (useful) data in this level, numbering: [2, 0], [2, 1], [2, 2]
                                                             /        \
                                                             X         X   ---------------------- Level is 3, there are 2 nodes with (useful) data in this level, numbering [3, 6], [3, 7]

 * We have Keys as follows:
 * [2, 0]
 * [2, 1]
 * [2, 2]
 * [3, 6]
 * [3, 7]
 *
 * */
 
class Key {
public:
	int l;
	int n;

	Key(int nInput, int lInput) {
		l = lInput;
		n = nInput;
	}

	Key(const Key &k) {
		l = k.l;
		n = k.n;
	}

	bool operator< (const Key &k) const {
		if (this -> n != k.n)
			return (this->n < k.n);
		return (this->l < k.l);
	}
};

struct MyHashCompare {
    static size_t hash( const Key& x ) {
        size_t prime = 31;
		size_t result = 1;
		result = prime * result + x.l;
		result = prime * result + x.n;
		return result;
    }
    //! True if strings are equal
    static bool equal( const Key& x, const Key& y ) {
        return ((x.n == y.n) && (x.l == y.l));
    }
};

#endif /* _KEY */
