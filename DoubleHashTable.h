#ifndef DOUBLE_HASH_TABLE_H
#define DOUBLE_HASH_TABLE_H

/****************************************
 * UW User ID: b9cheng
 * Submitted for ECE 250
 * Semester of Submission: Winter 2017
 *
 * By submitting this file, I affirm that
 * I am the author of all modifications to
 * the provided code.
 ****************************************/

#include "Exception.h"
#include "ece250.h"

enum state { EMPTY, OCCUPIED, DELETED };

template<typename T>
class DoubleHashTable {
	private:
		int count;
		int power;
		int array_size;
		T *array;
		state *array_state;

		int h1( T const & ) const; // first hash function
		int h2( T const & ) const; // second hash function

	public:
		DoubleHashTable( int = 5 );
		~DoubleHashTable();
		int size() const;
		int capacity() const;
		bool empty() const;
		bool member( T const & ) const;
		T bin( int ) const;

		void print() const;

		void insert( T const & );
		bool remove( T const & );
		void clear();
};

template<typename T >
DoubleHashTable<T >::DoubleHashTable( int m ):
count( 0 ), power( m ),
array_size( 1 << power ),
array( new T [array_size] ),
array_state( new state[array_size] ) {

	for ( int i = 0; i < array_size; ++i ) {
		array_state[i] = EMPTY;
	}
}

template<typename T >
DoubleHashTable<T >::~DoubleHashTable() {
	delete [] array;
	delete [] array_state;
}

template<typename T >
int DoubleHashTable<T >::size() const {
	return count;
}

template<typename T >
int DoubleHashTable<T >::capacity() const {
	return array_size;
}

template<typename T >
bool DoubleHashTable<T >::empty() const {
    if(count == 0){
        return true;
    }
	else return false;
}

template<typename T >
int DoubleHashTable<T >::h1( T const &obj ) const {
    int objInInt = static_cast<int>(obj);
    objInInt %= array_size; //module the integer by array_size to get probe
    if(objInInt < 0){
        objInInt += array_size; //add array_size back when reaches negative
    }
	return objInInt;
}

template<typename T >
int DoubleHashTable<T >::h2( T const &obj ) const {
	int objInInt = static_cast<int>(obj);
    objInInt /= array_size;
    objInInt %= array_size;
	while(objInInt < 0){
        objInInt += array_size; //add array_size back when reaches negative
	}
	if(objInInt % 2 == 0){
        objInInt += 1; //add one for changing even number to odd
	}
	return objInInt;
}

template<typename T >
bool DoubleHashTable<T >::member( T const &obj ) const {
	int iniProbe = h1(obj);
	for(int i = 0; i < array_size; i++){ //check for array_size times
        if(array_state[iniProbe] == OCCUPIED && array[iniProbe] == obj){
            return true;
        }else{
            iniProbe += h2(obj); //apply h2 function for finding corresponding probe
            iniProbe %= array_size;
            while(iniProbe >= array_size){
                iniProbe -= array_size;
            }
        }
	}
	return false;
}

template<typename T >
T DoubleHashTable<T >::bin( int n ) const {
    if(array_state[n] == OCCUPIED){
        return array[n];
    }else{
        return T();
    }
}

template<typename T >
void DoubleHashTable<T >::insert( T const &obj ) {
    if(count >= array_size){
        overflow ex; //to check if the hash table is full
        throw ex;
        return;
    }
    int iniProbe = h1(obj);
    while(array_state[iniProbe] != EMPTY && array_state[iniProbe] != DELETED) //keeping executing h2 until getting an valid probe
    {
        iniProbe += h2(obj);
        iniProbe %= array_size;
        while(iniProbe >= array_size){ //apply h2 function for finding valid probe
            iniProbe -= array_size;
        }
    }
    array[iniProbe] = obj;
    array_state[iniProbe] = OCCUPIED;
    count += 1;
    return;
}

template<typename T >
bool DoubleHashTable<T >::remove( T const &obj ) {
    int iniProbe = h1(obj);
    for(int i = 0; i < array_size; i++){ //check for array_size times
        if(array_state[iniProbe] == OCCUPIED && array[iniProbe] == obj){
            array_state[iniProbe] = DELETED; //set state to empty to remove item
            count -= 1;
            return true;
        }else{
            iniProbe += h2(obj); //apply h2 function for finding corresponding probe
            iniProbe %= array_size;
            while(iniProbe >= array_size){
                iniProbe -= array_size;
            }
        }
	}
    return false;
}

template<typename T >
void DoubleHashTable<T >::clear() {
    delete [] array_state; //re-initialize arrays and counter to clear data
    array_state = new state[array_size];
    delete [] array;
    array = new T[array_size];
    count = 0;
    return ;
}

template<typename T >
void DoubleHashTable<T >::print() const {
    if(array_size > 0){
        for(int i = 0; i < array_size; i++){
            if(array_state[i] != EMPTY && array_state[i] != DELETED){
                std::cout << "item " << i << ": " << array[i] << std::endl;
            }
        }
    }
	return;
}

#endif
