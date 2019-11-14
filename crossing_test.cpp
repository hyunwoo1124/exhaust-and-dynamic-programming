///////////////////////////////////////////////////////////////////////////////
// crossing_test.cpp
//
// Unit tests for crossing_algs.hpp
//
///////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include <random>

#include "rubrictest.hpp"

#include "crossing_types.hpp"
#include "crossing_algs.hpp"

int main() {

  Rubric rubric;

  crossing::grid empty2(2, 2), empty4(4, 4);
  unsigned int empty2_solution = 2, empty4_solution = 20;

  crossing::grid horizontal(4, 4);
  horizontal.set(0, 3, crossing::CELL_THICKET);
  unsigned int horizontal_solution = 19;

  crossing::grid vertical(4, 4);
  vertical.set(1, 0, crossing::CELL_THICKET);
  vertical.set(2, 0, crossing::CELL_THICKET);
  vertical.set(3, 0, crossing::CELL_THICKET);
  unsigned int vertical_solution = 10;

  crossing::grid all_thicket(4, 4);
  for (crossing::coordinate r = 0; r < 4; ++r) {
    for (crossing::coordinate c = 0; c < 4; ++c) {
      if (!(r == 0 && c == 0)) {
        all_thicket.set(r, c, crossing::CELL_THICKET);
      }
    }
  }
  unsigned int all_thicket_solution = 0;

  crossing::grid maze(4, 4);
  /*    ..XX
        X..X
        XX..
        XXX.   */
  maze.set(0, 2, crossing::CELL_THICKET);
  maze.set(0, 3, crossing::CELL_THICKET);
  maze.set(1, 0, crossing::CELL_THICKET);
  maze.set(1, 3, crossing::CELL_THICKET);
  maze.set(2, 0, crossing::CELL_THICKET);
  maze.set(2, 1, crossing::CELL_THICKET);
  maze.set(3, 0, crossing::CELL_THICKET);
  maze.set(3, 1, crossing::CELL_THICKET);
  maze.set(3, 2, crossing::CELL_THICKET);

  unsigned int maze_solution = 1;

  std::mt19937 gen;
  crossing::grid small_random =  crossing::grid::random(4, 5, 4, gen),
               medium_random = crossing::grid::random(12, 25, 50, gen),
               large_random =  crossing::grid::random(20, 79, 211, gen);
      
  rubric.criterion("exhaustive search - simple cases", 4, [&]() {
      TEST_EQUAL("empty2", empty2_solution, marsh_crossing_exhaustive(empty2));
      TEST_EQUAL("empty4", empty4_solution, marsh_crossing_exhaustive(empty4));
      TEST_EQUAL("horizontal", horizontal_solution, marsh_crossing_exhaustive(horizontal));
      TEST_EQUAL("vertical", vertical_solution, marsh_crossing_exhaustive(vertical));
      TEST_EQUAL("all_thicket no. of paths", all_thicket_solution, marsh_crossing_exhaustive(all_thicket));
    });

 
  rubric.criterion("exhaustive search - maze", 1, [&]() {
      TEST_EQUAL("correct", maze_solution, marsh_crossing_exhaustive(maze));
    });
  
  rubric.criterion("dynamic programming - simple cases", 4, [&]() {
      TEST_EQUAL("empty2", empty2_solution, marsh_crossing_dyn_prog(empty2));
      TEST_EQUAL("empty4", empty4_solution, marsh_crossing_dyn_prog(empty4));
      TEST_EQUAL("horizontal", horizontal_solution, marsh_crossing_dyn_prog(horizontal));
      TEST_EQUAL("vertical", vertical_solution, marsh_crossing_dyn_prog(vertical));
      auto output = marsh_crossing_dyn_prog(all_thicket);
      TEST_EQUAL("all_thicket no. of paths", 0, output);
    });
   
  rubric.criterion("dynamic programming - maze", 1, [&]() {
      TEST_EQUAL("correct", maze_solution, marsh_crossing_dyn_prog(maze));
    });

  /*
  std::cout << std:: endl << std:: endl;
  std::cout << "medium random=" << marsh_crossing_dyn_prog(medium_random) << std::endl;
  medium_random.print();

  std::cout << std:: endl << std:: endl;
  std::cout << "Large random=" << marsh_crossing_dyn_prog(large_random) << 
    std::endl;
  large_random.print();
  */

  rubric.criterion("dynamic programming - random instances", 1, [&]() {
      std::cout << std::endl;
      
      auto small_output = marsh_crossing_dyn_prog(small_random);
      TEST_EQUAL("small", 8, small_output);
      
      auto medium_output = marsh_crossing_dyn_prog(medium_random);
      TEST_EQUAL("medium", 17625, medium_output);
      
      auto large_output = marsh_crossing_dyn_prog(large_random);
      TEST_EQUAL("large", 2830718912, large_output);
    });

  rubric.criterion("stress test", 2,[&]() {
      const crossing::coordinate ROWS = 5,
	MAX_COLUMNS = 15;
      const unsigned SEED = 20181130;
      
      std::mt19937 gen(SEED);
      
      for (crossing::coordinate columns = 1; columns <= MAX_COLUMNS; ++columns) {
	auto area = ROWS * columns,
	  thick = area / 10;
	crossing::grid setting = crossing::grid::random(ROWS, columns, 
							thick, gen);
	TEST_EQUAL("random grid with " + std::to_string(columns) + " columns",
		   crossing::marsh_crossing_exhaustive(setting),
		   crossing::marsh_crossing_dyn_prog(setting));
      }
    });
  
  return rubric.run();
}
