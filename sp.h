#ifndef SHORTEST_PATH
#define SHORTEST_PATH

#include <bits/stdc++.h>
#include "./util/math.h"
#include "./util/log.h"


#define INFINITE 32767
#define K 0.1


std::vector<std::pair<double,double> > GetShortestPath(
    std::vector<std::pair<double,double> > const& points,
    std::vector<std::pair<int,int> > const& edges,
    std::vector<std::tuple<int,int,int> > const& faces,
    std::pair<double,double> from,
    std::pair<double,double> to
    );

//int pointInFace(Vector3 A, Vector3 B, Vector3 C, Vector3 P);


#endif // SHORTEST PATH