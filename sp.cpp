#include "./sp.h"
#include <glm/glm.hpp>
#include <src/util/log.h>
using namespace glm;
using namespace std;

#define INFINTE 32767



vector<pair<double,double> >GetShortestPath(
    vector<pair<double,double> > const& points,
    vector<pair<int,int> > const& edges,
    vector<tuple<int,int,int> > const& faces,
    pair<double,double> from,
    pair<double,double> to) 
{

    int vnum = points.size();
    int prev[vnum];         //记录前一个节点
    double disToStart[vnum];          //记录每个节点到起点的距离
    double disSheet[vnum][vnum];
    bool haveBeenMarked[vnum];

    const long start = find(points.begin(),points.end(),from) - points.begin();
    const long final = find(points.begin(),points.end(),to)-points.begin();
    bool singlestart = true,singlefinal = true;
    for(auto edge : edges)
    {
        if(edge.first == start||edge.second ==start)
        {
            singlestart = false;
            break;
        }
    }
    for(auto edge : edges)
    {
        if(edge.first == final||edge.second ==final)
        {
            singlefinal = false;
            break;
        }
    }

    if(singlefinal||singlestart)
    {
        filelog<<"singlestart or singlefinal";
        return vector<pair<double,double>>();
    }

    //初始化邻接表
    for(int i = 0;i<vnum;i++)
        for(int j = 0;j<vnum;j++)
            disSheet[i][j] = INFINTE;

    for(auto edge : edges) {
        disSheet[edge.first][edge.second] = sqrt(pow((points[edge.first].first-points[edge.second].first),2)+pow((points[edge.first].second-points[edge.second].second),2)) ;//(vec2(points[edge.first].first,points[edge.first].second) - vec2(points[edge.second].first,points[edge.second].second)).length();
        disSheet[edge.second][edge.first] = disSheet[edge.first][edge.second];
    }

    for(int i = 0;i<vnum;i++)
    {
        disToStart[i] = disSheet[start][i];
        haveBeenMarked[i] = false;
        if(disToStart[i] == INFINTE)
            prev[i] = -1;
        else prev[i] = start;
    }

    disToStart[start] = 0;
    haveBeenMarked[start] = true;

    for(int i = 0;i<vnum;i++)
    {
        int mindist = INFINTE;
        int cloestVer = start;
        for(int j = 0;j<vnum;j++)
        {
            if((!haveBeenMarked[j])&&disToStart[j]<mindist)
            {
                if(j == start)
                    continue;
                cloestVer = j;
                mindist = disToStart[j];
            }
        }

        haveBeenMarked[cloestVer] = true;

        for(int j = 0;j<vnum;j++)
        {
            if(haveBeenMarked[j])
                continue;
            if(disSheet[cloestVer][j] + disToStart[cloestVer]<disToStart[j])
            {
                disToStart[j] = disSheet[cloestVer][j] + disToStart[cloestVer];
                prev[j] = cloestVer;
            }
        }
    }

    auto path = vector<pair<double,double>>();
    path.push_back(pair<double,double>(to.first,to.second));
    for(int current = final;(current != start) && (current != -1);current = prev[current])
    {
        path.push_back(pair<double,double>(points[prev[current]].first,points[prev[current]].second));
//        console<<"back\t";
    }

//    char output[20];
//
//    for(int i = 0;i<vnum;i++)
//    {
//        sprintf(output,"prev[%d] is %d",i,prev[i]);
//        filelog<<output;
//    };
//
//    sprintf(output,"%d\n",path.size());
//
//    console<<output;
//    for(auto a : path)
//    {
//        filelog << a.first<<"\t"<<a.second<<"\n";
//    }

    return path;
}
