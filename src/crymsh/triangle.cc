#include <fstream>
#include "triangle.h"

void Triangle::write(std::ofstream& fo) {
  fo << "facet normal 0e0 0e0 0e0\n\touter loop\n";
  for (char i = 0; i < 3; ++i)
      fo << "\t\tvertex " << std::scientific << verts[i][0] << ' '
                          << verts[i][1] << ' '
                          << verts[i][2]
                          << std::endl;

  fo << "\tendloop\nendfacet\n";
}
