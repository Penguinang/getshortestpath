#include "./sp.h"
#include <GLFW/glfw3.h>

using namespace std;

bool isPointCloseEnoughToBoundary(vector<pair<double,double>> const& points,
                                  int pointIndex,
                                  vector<pair<int,int> > const& edges,
                                  bool* const& isEdgeBoundary,
                                  int edgeNum);

vector<pair<double,double> >GetShortestPath(
        vector<pair<double,double> > const& points,
        vector<pair<int,int> > const& edges,
        vector<tuple<int,int,int> > const& faces,
        pair<double,double> from,
        pair<double,double> to)
{

    int vnum = points.size();
    int edgenum = edges.size();

    double starttime = glfwGetTime();
    double deltatime,lasttime = starttime;

    int* prev = new int[vnum];
    double* disToStart = new double[vnum];
    double* disMatrix = new double[vnum*vnum];
    bool* haveBeenMarked = new bool[vnum];
    bool* onEdges = new bool[vnum];
    bool* isEdgeBoundary = new bool[edges.size()];
    bool* isPointNearBoundary = new bool[vnum];


    //判断点是否在某个三角边上,来确定点是否有可能出现在路径上
    for(auto face : faces)
    {
        int a,b,c;
        std::tie(a,b,c) = face;
        onEdges[edges[a].first] = true;
        onEdges[edges[a].second] = true;
        onEdges[edges[b].first] = true;
        onEdges[edges[b].second] = true;
        onEdges[edges[c].first] = true;
        onEdges[edges[c].second] = true;

        if(!isEdgeBoundary[a])
            isEdgeBoundary[a] = true;
        else isEdgeBoundary[a] = false;
        if(!isEdgeBoundary[b])
            isEdgeBoundary[b] = true;
        else isEdgeBoundary[b] = false;
        if(!isEdgeBoundary[c])
            isEdgeBoundary[c] = true;
        else isEdgeBoundary[c] = false;
    }

    for(int i = 0;i<vnum;i++)
    {
        {
            if(isPointCloseEnoughToBoundary(points,i,edges,isEdgeBoundary,edgenum))
                isPointNearBoundary[i] = true;
        }
    }

    const int start = find(points.begin(),points.end(),from) - points.begin();
    const int final = find(points.begin(),points.end(),to) - points.begin();
    if(!onEdges[start]||!onEdges[final])
    {
        filelog<<"\nstart or final is single\n";
        console<<"\nstart or final is single\n";
        return vector<pair<double,double>>();
    }

    //初始化邻接矩阵
    for(int i = 0;i<vnum;i++)
        for(int j = 0;j<vnum;j++)
            disMatrix[i*vnum+j] = INFINITE;

    for(auto edge : edges) {
        disMatrix[edge.first*vnum+edge.second] = (Vector2(points[edge.first].first,points[edge.first].second) - Vector2(points[edge.second].first,points[edge.second].second)).len();
        disMatrix[edge.second*vnum+edge.first] = disMatrix[edge.first*vnum+edge.second];
    }


    for(int i = 0;i<vnum;i++)
    {
        disToStart[i] = disMatrix[start*vnum+i];
        haveBeenMarked[i] = false;
        if(disToStart[i] == INFINITE)
            prev[i] = -1;
        else prev[i] = start;
    }

    disToStart[start] = 0;
    haveBeenMarked[start] = true;


    for(int i = 0;i<vnum;i++)
    {
        int mindist = INFINITE;
        int closestVer = start;
        for(int j = 0;j<vnum;j++)
        {
            if(!onEdges[j]||!isPointNearBoundary[j])
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
            if(haveBeenMarked[j]||!onEdges[j]||!isPointNearBoundary[j])
                continue;
            j;
            if(disMatrix[closestVer*vnum+j] + disToStart[closestVer]<disToStart[j])
            {
                disToStart[j] = disMatrix[closestVer*vnum+j] + disToStart[closestVer];
                prev[j] = closestVer;
            }

        }
    }

    auto path = vector<pair<double,double>>();
    for(int current = final;current != start;current = prev[current])
    {
        if(current == -1)
        {
            filelog<<"\nno way between start and final\n";
            console<<"\nno way between start and final\n";
            return vector<pair<double,double>>();
        }
        path.push_back(pair<double,double>(points[current].first,points[current].second));
    }
    path.push_back(pair<double,double>(points[start].first,points[start].second));

    delete prev;
    delete disMatrix;
    delete disToStart;
    delete haveBeenMarked;
    delete onEdges;
    delete isEdgeBoundary;
    delete isPointNearBoundary;

    return path;
}

bool isPointCloseEnoughToBoundary(vector<pair<double,double>> const& points,
                                  int pointIndex,
                                  vector<pair<int,int> > const& edges,
                                  bool* const& isEdgeBoundary,
                                  int edgeNum)
{
    for(int i = 0;i<edgeNum;i++)
    {
        if(!isEdgeBoundary[i])
            continue;
        Vector2 ab = Vector2(points[edges[i].second].first,points[edges[i].second].second) - Vector2(points[edges[i].first].first,points[edges[i].first].second);
        Vector2 ac = Vector2(points[pointIndex].first,points[pointIndex].second) - Vector2(points[edges[i].first].first,points[edges[i].first].second);
        Vector2 bc = Vector2(points[pointIndex].first,points[pointIndex].second) - Vector2(points[edges[i].second].first,points[edges[i].second].second);
        float ab_ac = ab&ac;
        if(ab_ac<0)
            if(ac.len()< K )
                return true;
        float ab_ab = ab&ab;
        if(ab_ac>ab_ab)
            if(bc.len()<K)
                return true;
        ab_ac /= ab_ab;
        
        Vector2 D = ab_ac*ab - ac;
        if(D.len()<K)
            return true;
    }
    return false;

}
