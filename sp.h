#ifndef SHORTEST_PATH
#define SHORTEST_PATH

#include <bits/stdc++.h>

std::vector<std::pair<double,double> > GetShortestPath(
    std::vector<std::pair<double,double> > const& points,
    std::vector<std::pair<int,int> > const& edges,
    std::vector<std::tuple<int,int,int> > const& faces,
    std::pair<double,double> from,
    std::pair<double,double> to
    );

#endif // SHORTEST PATH