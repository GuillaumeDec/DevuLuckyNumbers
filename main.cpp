#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

/*
 * ERL.cpp
 *
 *  Created on: 16 jan. 2016
 *      Author: decerprit
 */

//! some pre-processor options for potential further integration
//#ifndef ERL_HPP_
//#define ERL_HPP_
//#include "ERL.hpp"

/*!
 * @name class Devu
 * @brief Devu is the main class that stores input values and the modulo value,
 * and computes the sum of all necessary combinations or a given input set (computeSum is overloaded for these purposes)
 */
class Devu{
public:
    Devu();
    Devu(unsigned int, unsigned int, unsigned int);
    //! Compute the final sum of the DEVU challenge
    unsigned long long int computeSum();
    //! Compute the sum for a given set of 4,5,6 numbers
    unsigned long long int computeSum(unsigned int, unsigned int, unsigned int);
    //! Compute the number of possible combinations for a given set of 4,5,6 numbers
    unsigned long long int computeMultiplicity(unsigned int, unsigned int, unsigned int);
    unsigned long long int computeMultiplicity2(unsigned int, unsigned int, unsigned int);
    //! standard factorial computation
    unsigned long long int computeFactorial(unsigned int);
    //! standard combinatorial computation
    unsigned long long int NchooseK(unsigned int, unsigned int);
    unsigned long long int NchooseK2(unsigned int, unsigned int);
    unsigned long long int getModulo(){return modulo;}
    //! getters that return the values of input integers
    unsigned int getN4(){return in_n4;}
    unsigned int getN5(){return in_n5;}
    unsigned int getN6(){return in_n6;}
    long long int getMultiplicity(int i, int j, int k){return vec_multiplicity[i][j][k];}
    long long int getSumElem(int i, int j, int k){return vec_sum[i][j][k];}
    
private:
    //! modulo as defined by the challenge
    static const unsigned long long int modulo = 1e9 + 7;
    //! three input integers x (n4), y (n5), z (n6) as defined by the challenge
    unsigned int in_n4, in_n5, in_n6;
    // vector to store already computed multiplicities & sums
    vector<vector<vector< long long int> > > vec_multiplicity;
    vector<vector<vector< long long int> > > vec_sum;
};

//#endif /* ERL_HPP_ */

Devu::Devu(unsigned int n4, unsigned int n5, unsigned int n6){
	in_n4 = n4;
	in_n5 = n5;
	in_n6 = n6;
	//! allocation of vector space as per input values n4, n5 and n6
	vec_multiplicity.resize(n4+1);
	vec_sum.resize(n4+1);
	for (unsigned int i = 0; i < n4+1; ++i)
    {
        vec_multiplicity[i].resize(n5+1);
        vec_sum[i].resize(n5+1);
        for (unsigned int j = 0; j < n5+1; ++j)
        {
            vec_multiplicity[i][j].resize(n6+1, -99);
            vec_sum[i][j].resize(n6+1, -99);
        }
    }
}

/*{
 unsigned long long int Devu::computeFactorial(unsigned int n){
 @DEPRECATED
 // works but intractable for large values
 if(n == 1 || n == 0)
 return 1;
 unsigned long long int tmp = n * this->computeFactorial(n-1);
 return tmp;
 }
 
 unsigned long long int Devu::NchooseK(unsigned int N, unsigned int k){
    if(k < 1 || N <1)
        return 1;
    return this->computeFactorial(N) / this->computeFactorial(N-k) / this->computeFactorial(k);
 }
 }*/

unsigned long long int Devu::computeMultiplicity(unsigned int n4,unsigned int n5, unsigned int n6){
	/*!
	 * returns the number of possible arrangements from a set of subsets each containing n4, n5 and n6 identical digits.
	 * from a set containing 3 groups of n4,n5 and n6 identical numbers, with a total of N = n4+n5+n6 digits,
	 * there are (N,n4) ways to arrange the n4 digits, then, for the N-n4 remaining positions, there are (N-n4, n5) ways to
	 * arrange the n5 digits, and for the N-n4-n5 = n6 remaining positions, there are (n6,n6) = 1 way to arrange the last n6 digits.
	 * Here, (N,k) = Devu::NchooseK(N,k)
	 * @retval int, default is 1 if N is <= 0
	 * The new way, updated here, makes use of the recursive expression: M(n4,n5,n6) = M(n4-1,n5,n6)+M(n4,n5-1,n6)+M(n4,n5,n6-1)
	 * also, intermediate values are stored to speed up the execution (otherwise intractable for nX>=10 or so)
	 */
	unsigned int N = n4 + n5 + n6;
	if(N <= 0)
        return 1;
    long long int & curr_multiplicity = vec_multiplicity[n4][n5][n6];
    if(curr_multiplicity != -99)
    	return curr_multiplicity;
    curr_multiplicity = 0;
    if(n4 > 0){
    	curr_multiplicity += computeMultiplicity(n4-1,n5,n6) % this->getModulo();
    }
    if(n5 > 0){
    	curr_multiplicity += computeMultiplicity(n4,n5-1,n6) % this->getModulo();
    }
    if(n6 > 0){
    	curr_multiplicity += computeMultiplicity(n4,n5,n6-1) % this->getModulo();
    }
    return curr_multiplicity;
    
}

unsigned long long int Devu::computeSum(unsigned int n4, unsigned int n5, unsigned int n6){
    /*!
     * compute the sum of combinations for a given set of n4,n5,n6 using:
     * s(x,y,z) = 10*(s(x-1,y,z)+s(x,y-1,z)+s(x,y,z-1)) + 4*m(x-1,y,z) + 5*m(x,y-1,z) + 6*m(x,y,z-1)
     * where m is the multiplicity as defined in Devu::computeMultiplicity
     * note that here infinite recursion is avoided thanks to the if statements
     * also, intermediate values are stored to speed up the execution (otherwise intractable for nX>=10 or so)
     */
	unsigned int N = n4 + n5 + n6;
	if(N <= 0)
		return 0;
	long long int & tmp = vec_sum[n4][n5][n6];
	if(tmp != -99)
		return tmp;
	tmp = 0;
	if(n4 > 0){
		tmp += ( 10 * this->computeSum(n4-1, n5, n6) + 4 * this->computeMultiplicity(n4-1, n5, n6) ) % this->getModulo();
	}
	if(n5 > 0){
		tmp += ( 10 * this->computeSum(n4, n5-1, n6) + 5 * this->computeMultiplicity(n4, n5-1, n6) ) % this->getModulo();
	}
	if(n6 > 0){
		tmp += ( 10 * this->computeSum(n4, n5, n6-1) + 6 * this->computeMultiplicity(n4, n5, n6-1) ) % this->getModulo();
	}
	return tmp;
}

unsigned long long int Devu::computeSum(){
	/*!
	 * execute repetitive calls of sum computation for all x,y,z combinations
	 * where x,y,z are the challenge's input variables (stored as private vars in class Devu)
     * @todo since all the_sum terms are independent to each other, we can easily MPI this function
	 */
	unsigned long long int the_sum = 0;
	for(int i=this->getN4(); i>=0; i--)
		for(int j=this->getN5(); j>=0; j--)
			for(int k=this->getN6(); k>=0; k--)
				the_sum += this->Devu::computeSum(i, j, k);
	return the_sum % this->getModulo();
}


int main(int argc, char * argv[]) {
	/*!
	 * main function to be executed. Challenge input is retrieved using cin (catches stdin).
	 */
	unsigned int n4, n5, n6;
	cin >> n4 >> n5 >> n6;
    //	n4 = atof(argv[1]);
    //	n5 = atof(argv[2]);
    //	n6 = atof(argv[3]);
    
	//! create a Devu object that is initialized using input variables
	Devu* aDevu = new Devu(n4, n5, n6);
	//! execute the full challenge (sum) computation
    unsigned long long int total_sum = aDevu->computeSum();
    //! flush the sum output value to stdout
	cout << total_sum << endl;
	return 0;
}
