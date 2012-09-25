#include "include/Octree.hpp"
#include "include/Vec.hpp"

#include <iostream>
#include <string>

// Random number in [0,1)
inline double drand() {
  return ::drand48();
}

template <typename Box>
void print_box(const Box& b, std::string padding = std::string()) {
  std::cout << padding << "Box " << b.index() << " (Level " << b.level() << "): " << b.morton_index() << "\n";

  padding.append(2,' ');
  if (b.is_leaf()) {
    for (auto ci = b.body_begin(); ci != b.body_end(); ++ci)
      std::cout << padding << "Point " << ci->index() << ": " << ci->morton_index() << "\t" << ci->point() << "\n";
  } else {
    for (auto ci = b.child_begin(); ci != b.child_end(); ++ci)
      print_box(*ci, padding);
  }
}



int main(int argc, char** argv)
{
  if (argc < 2) {
    std::cerr << "Usage: test_tree NUM_POINTS\n";
    exit(1);
  }

  int N = atoi(argv[1]);

  typedef Vec<double[3],3> point_type;

  Octree<point_type> otree(BoundingBox<point_type>(point_type(0), point_type(1)));

  std::vector<point_type> points;
  for (int k = 0; k < N; ++k) {
    point_type p;
    p[0] = drand();
    p[1] = drand();
    p[2] = drand();
    points.push_back(p);
  }

  otree.construct_tree(points.begin(), points.end());

  print_box(*otree.box_begin());

  return 0;
}
