/*
Copyright (C) 2011 by Rio Yokota, Simon Layton, Lorena Barba

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include <FMM_plan.hpp>
#include <Dataset.hpp>
#include <SphericalLaplaceKernel.hpp>
#include <EvaluatorBase.hpp>

template <typename Box>
void print_box(const Box& b, std::string padding = std::string()) {
  std::cout << padding << "Box " << b.index()
            << " (Level " << b.level() << ", Parent " << b.parent().index() << "): "
            << b.morton_index() << "    " << b.center() << "\n";

  padding.append(2,' ');
  if (!b.is_leaf()) {
    for (auto ci = b.child_begin(); ci != b.child_end(); ++ci)
      print_box(*ci, padding);
  } else {
    for (auto ci = b.body_begin(); ci != b.body_end(); ++ci)
      std::cout << padding << "Point " << ci->index() << ": " << ci->morton_index() << "\t" << ci->point() << "\n";
  }
}

// Random number in [0,1)
inline double drand() {
  return ::drand48();
}

int main(int argc, char **argv)
{
  int numBodies = 100;
  int P = 5;
  THETA = 1 / sqrtf(4);                                         // Multipole acceptance criteria
  FMM_options opts;

  // parse command line args
  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i],"-N") == 0) {
      i++;
      numBodies = atoi(argv[i]);
    } else if (strcmp(argv[i],"-P") == 0) {
      i++;
      P = atoi(argv[i]);
    } else if (strcmp(argv[i],"-theta") == 0) {
      i++;
      THETA = (double)atof(argv[i]);
    } else if (strcmp(argv[i],"-bottomup") == 0) {
      opts.tree = BOTTOMUP;
    } else {
      printf("[W]: Unknown command line arg: \"%s\"\n",argv[i]);
    }
  }

  // Init the FMM Kernel
  SphericalLaplaceKernel K(P);
  typedef SphericalLaplaceKernel::point_type point_type;
  typedef SphericalLaplaceKernel::charge_type charge_type;
  typedef SphericalLaplaceKernel::result_type result_type;


  // Init points and charges
  std::vector<point_type> points(numBodies);
  for (int k = 0; k < numBodies; ++k)
    points[k] = point_type(drand(), drand(), drand());
  std::vector<point_type> jpoints = points;

  std::vector<charge_type> charges(numBodies);
  for (int k = 0; k < numBodies; ++k)
    charges[k] = drand();
  std::vector<charge_type> charges_copy = charges;

  BoundingBox<point_type> bbox(points.begin(), points.end());
  Octree<point_type> otree(bbox);

  // create evaluator
  auto *eval = EvaluatorBase<Octree<point_type>,SphericalLaplaceKernel>::createEvaluator(otree, K, opts);
  std::cout << eval->name() << std::endl;

}