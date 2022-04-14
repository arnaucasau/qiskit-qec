#include "properties.h"


/*
 * Compute the minimum distance of a stabilizer code.
 * The stabilizer corresponds to the space W spanned by a
 * set of symplectic vectors. The weight of a symplectic vector
 * is defined as the weight of the corresponding Pauli operator.
 * This algorithm enumerates all low weight vectors x in F_2^{2n}
 * to discover a lowest-weight vector in W^\perp - W.
 *
 * Returns the minimum distance of the corresponding code, or 0 if
 * the minimum distance exceeds max_weight.
 */
int minimum_distance(std::vector<std::vector<int> > &symplectic_vectors,
                     int max_weight)
{
  if(!is_isotropic(symplectic_vectors))
    throw std::logic_error("symplectic vectors must span isotropic space");
  int n = symplectic_vectors[0].size() / 2;
  int rk = rank(symplectic_vectors);
  int k = n - rk;
  for(int weight=1; weight <= (max_weight < n ? max_weight : n); weight++) {
    std::vector<int> state;
    Combinations c(n, weight);
    std::vector<int> num_paulis(weight, 3);
    while(c.next_combination(state)) {
      std::vector<int> pstate;
      ProductIterator p(num_paulis);
      while(p.next_element(pstate)) {
        std::vector<int> error(2*n, 0);
        for(int i=0; i<weight; i++) {
          if(pstate[i] == 0 || pstate[i] == 2) error[state[i]] = 1;
          if(pstate[i] == 1 || pstate[i] == 2) error[state[i] + n] = 1;
        }
        // Test if error is in W^\perp
        bool inWperp = is_orthogonal(error, symplectic_vectors);
        // Test if error is in W
        std::vector<std::vector<int> > extended = symplectic_vectors;
        extended.push_back(error);
        bool inW = rank(extended) == rk;
        if(inWperp)
          if((k > 0 && !inW) || (k == 0 && inW))
            return weight;
      } 
    }
  }
  return 0;
}