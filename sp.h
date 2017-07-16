#ifndef SHORTEST_PATH
#define SHORTEST_PATH

#include <bits/stdc++.h>
#include "./util/math.h"
#include "./util/log.h"


#define INFINITE 32767
#define K 0.2


struct surPoint {
    int vertex;
    double cost = 0;
    surPoint* next = NULL;
};


std::vector<std::pair<double,double> > GetShortestPath(
    std::vector<std::pair<double,double> > const& points,
    std::vector<std::pair<int,int> > const& edges,
    std::vector<std::tuple<int,int,int> > const& faces,
    std::pair<double,double> from,
    std::pair<double,double> to
    );

#endif // SHORTEST PATH