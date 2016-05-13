// sudo apt-get install libboost-all-dev
// 
#ifndef _CONCURRENT_MAP
#define _CONCURRENT_MAP

#include <map>
#include <mutex>
#include <iostream>
#include "Key.h"
#include "Vector.h"
using namespace std;


// link: http://codereview.stackexchange.com/questions/8715/thread-safe-stdmap-accessor
class ConcurrentMap {
    private:
        std::map<Key, Vector> _map;
        std::mutex _m;

    public:

        ~ConcurrentMap() {
            _map.clear();
            std::map<Key, Vector>().swap(this->_map);
        }

    	// thread-safe version
        void set(Key key, Vector value) {
            std::lock_guard<std::mutex> lk(this->_m);
            this->_map.insert(std::pair<Key, Vector>(key, value));
            // this->_map[key] = value;
        }


        // this version is not thread-safe
        void set_not_threadsafe(Key key, Vector value) {
            this->_map[key] = value;
        }

        // thread-safe version
        size_t erase(Key key) {
            std::lock_guard<std::mutex> lk(this->_m);
            return (this->_map.erase(key));
        }


        Vector & get(Key key) {
            std::lock_guard<std::mutex> lk(this->_m);
            return this->_map[key];
        }

        Vector & get_not_threadsafe(Key key) {
            return this->_map[key];
        }        

        bool empty() {
            std::lock_guard<std::mutex> lk(this->_m);
            return this->_map.empty();
        }

        std::map<Key, Vector>::iterator begin(){
            std::lock_guard<std::mutex> lk(this->_m);
        	return this->_map.begin();
        }

        std::map<Key, Vector>::iterator end() {
            std::lock_guard<std::mutex> lk(this->_m);
        	return this->_map.end();
        }

        std::map<Key, Vector>::iterator find(const Key &key) {
            std::lock_guard<std::mutex> lk(this->_m);
            return this->_map.find(key);
        }

        std::map<Key, Vector>::const_iterator begin_not_threadsafe() const{
            return this->_map.begin();
        }

        std::map<Key, Vector>::const_iterator end_not_threadsafe() const{
            return this->_map.end();
        }

        std::map<Key, Vector>::const_iterator find_not_threadsafe(const Key &key) {
            return this->_map.find(key);
        }

        void clear() {
        	this->_map.clear();
        }
};


/*usage:
	
	int main(int argc, char ** argv) {
	    guarded_map<int, int> m;
	    m.set(1, 10);
	    m.set(2, 20);
	    m.set(4, 30);
	    std::cout<<"m[2]="<<m.get(2)<<std::endl;
	    return 0;
	}

*/
#endif