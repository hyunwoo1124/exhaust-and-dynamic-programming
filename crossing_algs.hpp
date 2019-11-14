///////////////////////////////////////////////////////////////////////////////
// crossing_algs.hpp
//
// Algorithms that solve the marsh crossing problem.
//
// All of the TODO sections for this project reside in this file.
//
// This file builds on crossing_types.hpp, so you should familiarize yourself
// with that file before working on this file.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cassert>
#include <iostream>

#include "crossing_types.hpp"

#include <cmath>

namespace crossing {
// Solve the marsh crossing problem for the given grid, using an exhaustive
// optimization algorithm.
//
// This algorithm is expected to run in exponential time, so the grid's
// width+height must be small enough to fit in a 64-bit int; this is enforced
// with an assertion.
//
// The grid must be non-empty.
unsigned int marsh_crossing_exhaustive(const grid& setting) {

  // grid must be non-empty.
  assert(setting.rows() > 0);
  assert(setting.columns() > 0);

  // Compute the path length, and check that it is legal.
  const size_t steps = setting.rows() + setting.columns() - 2;
  assert(steps < 64);

  // TODO: implement the exhaustive optimization algorithm, then delete this
  // comment.
  unsigned int count_paths = 0;   // used unsigned to match with other data types


for(unsigned int bits = 0; bits <(pow(2,steps)); bits++){
  path candidate(setting);  // candidate [start]
  for(unsigned int k =0; k < steps; k++){
    unsigned int bit = (bits >> k) & 1;
    if(bit == 1){
      if(candidate.is_step_valid(STEP_DIRECTION_RIGHT)){ // if the condition is met for right add right
        candidate.add_step(STEP_DIRECTION_RIGHT);
      }
    }
    else{
      if(candidate.is_step_valid(STEP_DIRECTION_DOWN)){ // if the condition is met for for down add down
        candidate.add_step(STEP_DIRECTION_DOWN);
      }
    }
  }
  if((candidate.final_row() == setting.rows() -1) && (candidate.final_column() == setting.columns()-1)){
    // if candidate stayes inside the grid and never crosses an X cell increment count_paths++
    count_paths++;
  }
}


  return count_paths;
}

// Solve the marsh crossing problem for the given grid, using a dynamic
// programming algorithm.
//
// The grid must be non-empty.
unsigned int marsh_crossing_dyn_prog(const grid& setting) {

  // grid must be non-empty.
  assert(setting.rows() > 0);
  assert(setting.columns() > 0);

  unsigned int A[100][100];
  A[0][0] = 1;

  // TODO: implement the dynamic programming algorithm, then delete this
  // comment.
for (unsigned int i = 0; i < setting.rows(); i++){    // for i form 0 to r-1 inclusive
  for (unsigned int j = 0; j < setting.columns();j++){  // for j fro 0 to c-1 inclusive
  if (i == 0 && j == 0) {
    continue;
  }
    if (setting.get(i, j) == CELL_THICKET){

      A[i][j] = 0;
      continue;
    }
    int from_above = 0;
    int from_left = 0;

    if (i > 0 && A[i-1][j] != 0 ) { // if i>0 and A[i-1][j] is not None:

      from_left = A[i-1][j];
    }
    if (j > 0 && A[i][j-1] != 0){ // if j>0 and A[i][j-1] is not None:

      from_above = A[i][j-1];
    }

    A[i][j] = from_left + from_above; // A[i][j] = sum of from_above and from_left; or None if both from_above and from_left are None


  }
}

  return A[setting.rows()-1][setting.columns()-1];
}

}
