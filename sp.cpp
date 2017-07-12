#include "./sp.h"

using namespace std;

vector<pair<double,double> >GetShortestPath(
        vector<pair<double,double> > const& points,
        vector<pair<int,int> > const& edges,
        vector<tuple<int,int,int> > const& faces,
        pair<double,double> from,
        pair<double,double> to)
{

    int vnum = points.size();


    int* prev = new int[vnum];
    double* disToStart = new double[vnum];
    double* disMatrix = new double[vnum*vnum];
    bool* haveBeenMarked = new bool[vnum];
    bool* onFaceEdge = new bool[vnum];


    for(auto face : faces)
    {
        int a,b,c;
        std::tie(a,b,c) = face;
        onFaceEdge[edges[a].first] = true;
        onFaceEdge[edges[a].second] = true;
        onFaceEdge[edges[b].first] = true;
        onFaceEdge[edges[b].second] = true;
        onFaceEdge[edges[c].first] = true;
        onFaceEdge[edges[c].second] = true;
    }

    const long start = find(points.begin(),points.end(),from) - points.begin();
    const long final = find(points.begin(),points.end(),to) - points.begin();
    if(!onFaceEdge[start]||!onFaceEdge[final])
    {
        filelog<<"singlestart or singlefinal";
        return vector<pair<double,double>>();
    }

    //初始化邻接矩阵
    for(int i = 0;i<vnum;i++)
        for(int j = 0;j<vnum;j++)
            disMatrix[i*vnum+j] = INFINTE;

    for(auto edge : edges) {
        disMatrix[edge.first*vnum+edge.second] = (Vector2(points[edge.first].first,points[edge.first].second) - Vector2(points[edge.second].first,points[edge.second].second)).len();
        disMatrix[edge.second*vnum+edge.first] = disMatrix[edge.first*vnum+edge.second];
    }

    for(int i = 0;i<vnum;i++)
    {
        disToStart[i] = disMatrix[start*vnum+i];
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
        int closestVer = start;
        for(int j = 0;j<vnum;j++)
        {
            if(!onFaceEdge[j])
                continue;
            if((!haveBeenMarked[j])&&disToStart[j]<mindist)
            {
                if(j == start)
                    continue;
                closestVer = j;
                mindist = disToStart[j];
            }
        }

        haveBeenMarked[closestVer] = true;

        for(int j = 0;j<vnum;j++)
        {
            if(haveBeenMarked[j]||!onFaceEdge[j])
                continue;
            if(disMatrix[closestVer*vnum+j] + disToStart[closestVer]<disToStart[j])
            {
                disToStart[j] = disMatrix[closestVer*vnum+j] + disToStart[closestVer];
                prev[j] = closestVer;
            }
        }
    }

    auto path = vector<pair<double,double>>();
    path.push_back(pair<double,double>(to.first,to.second));
    for(int current = final;(current != start) && (current != -1);current = prev[current])
    {
        path.push_back(pair<double,double>(points[prev[current]].first,points[prev[current]].second));
    }
    if(path.size()<=1)
        return vector<pair<double,double >>();

    delete prev;
    delete disMatrix;
    delete disToStart;
    delete haveBeenMarked;

//    console<<"points number is " <<vnum ;

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
