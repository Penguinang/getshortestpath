#include "./sp.h"
#include <GLFW/glfw3.h>

using namespace std;
bool Intersection(Vector2 a,Vector2 b, Vector2 c,Vector2 d);
int pointInFace(Vector3 A, Vector3 B, Vector3 C, Vector3 P);

vector<pair<double,double> >GetShortestPath(
        vector<pair<double,double> > const& points,
        vector<pair<int,int> > const& edges,
        vector<tuple<int,int,int> > const& faces,
        pair<double,double> from,
        pair<double,double> to)
{

    int vnum = points.size();
    int edgeNum = edges.size();
    int verLimit = vnum/2;
    double* disToStart = new double[vnum];
    double* pdisToStart = new double[vnum];
    int prev[verLimit-1][vnum];

    double disMatrix[vnum][vnum] = {};
    bool* isEdgeBoundary = new bool[edgeNum]();

    const int start = find(points.begin(),points.end(),from) - points.begin();
    const int final = find(points.begin(),points.end(),to) - points.begin();

    for(auto face : faces)
    {
        int a,b,c;
        std::tie(a,b,c) = face;

        isEdgeBoundary[a] = !isEdgeBoundary[a];
        isEdgeBoundary[b] = !isEdgeBoundary[b];
        isEdgeBoundary[c] = !isEdgeBoundary[c];
    }

    for(auto edge : edges) {
        disMatrix[edge.first][edge.second] = (Vector2(points[edge.first].first,points[edge.first].second) - Vector2(points[edge.second].first,points[edge.second].second)).len();//sqrt(pow((points[edge.first].first-points[edge.second].first),2)+pow((points[edge.first].second-points[edge.second].second),2)) ;
        disMatrix[edge.second][edge.first] = disMatrix[edge.first][edge.second];
    }

    //无向图所以只需要填充一半
    for(int i = 0;i<vnum;i++)
    {
        Vector2 a(points[i].first,points[i].second);
        for(int j = i;j<vnum;j++)
        {
            if(i == j)
            {
                disMatrix[i][j] = 0;
                continue;
            }
            if(disMatrix[i][j])
                continue;

            Vector2 b(points[j].first,points[j].second);
            int crossMask = 0;
            for(int k = 0;k<edgeNum;k++)
            {
                if(isEdgeBoundary[k])
                {
                    Vector2 c(points[edges[k].first].first,points[edges[k].first].second);
                    Vector2 d(points[edges[k].second].first,points[edges[k].second].second);
                    if(Intersection(a,b,c,d)){
                        disMatrix[i][j] = INFINITE;
                        disMatrix[j][i] = INFINITE;
                        crossMask = 1;
                        break;
                    }
                }
            }
            if(crossMask)
                continue;

            int infacemask = 0;
            Vector2 mid = (a+b)/2;
            for(auto face : faces)
            {
                int a,b,c;
                std::tie(a,b,c) = face;
                int A,B,C;
                A = edges[a].first;
                B = edges[a].second;
                if(edges[b].first == A||edges[b].first == B)
                    C = edges[b].second;
                else C = edges[b].first;
                if(pointInFace(Vector2(points[A].first,points[A].second) ,Vector2(points[B].first,points[B].second),Vector2(points[C].first,points[C].second),mid))
                {
                    infacemask = 1;
                    break;
                }
            }
            if(!infacemask)
            {
                disMatrix[i][j] = INFINITE;
                disMatrix[j][i] = INFINITE;
            }
            else{
                disMatrix[i][j] = (a-b).len();
                disMatrix[j][i] = disMatrix[i][j];
            }
        }
    }

    for(int j = 0;j < vnum;j++)
    {
        for(int i = 0;i < verLimit - 1;i++)
            prev[i][j] = -1;

        disToStart[j] = disMatrix[start][j];
        pdisToStart[j] = disToStart[j];
    }


    for(int i = 0;i<vnum;i++)
        if(disToStart[i] != INFINITE)
            prev[0][i] = start;


    int usedVerNum;
    for(usedVerNum = 1;usedVerNum<verLimit -1;usedVerNum++)
    {
        for(int i = 0;i<vnum;i++)
        {
            for(int j = 0;j<vnum;j++)
            {
                if(disToStart[i]>pdisToStart[j] + disMatrix[i][j])
                {
                    disToStart[i] = pdisToStart[j] + disMatrix[i][j];
                    prev[usedVerNum][i] = j;
                }
            }
            if(pdisToStart[i] == disToStart[i])
                prev[usedVerNum][i] = prev[(usedVerNum-1)][i];
        }
        for(int i = 0;i<vnum;i++)
            pdisToStart[i] = disToStart[i];
    }
    usedVerNum -= 1;

    auto path = vector<pair<double,double>>();
    for(int current = final;current != start;current = prev[usedVerNum][current])
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

    delete isEdgeBoundary;
    delete disToStart;
    delete pdisToStart;

    return path;

}

bool Intersection(Vector2 a,Vector2 b, Vector2 c,Vector2 d)
{
    if((a.x == c.x&&a.y == c.y) || (a.x == d.x&&a.y == d.y)||(b.x == c.x&&b.y == c.y) || (b.x == d.x&&b.y == d.y))
        return false;
    b.x -= a.x; b.y -= a.y;
    c.x -= a.x; c.y -= a.y;
    d.x -= a.x; d.y -= a.y;

    double distAB = sqrt(b.x * b.x + b.y * b.y);

    double theCos = b.x / distAB;
    double theSin = b.y / distAB;
    double newX = c.x * theCos + c.y * theSin;
    c.y = c.y * theCos - c.x * theSin; c.x = newX;
    newX = d.x * theCos + d.y * theSin;
    d.y = d.y * theCos - d.x * theSin; d.x = newX;

    double posAB = d.x + (c.x - d.x) * d.y / (d.y - c.y);

    if (posAB <= 0 || posAB >= distAB)
        return false;

    if(c.y*d.y<0)
        return true;
    return false;
}

int pointInFace(Vector3 A, Vector3 B, Vector3 C, Vector3 P)
{
    Vector3 v0 = C - A ;
    Vector3 v1 = B - A ;
    Vector3 v2 = P - A ;

    float dot00 = v0&(v0) ;
    float dot01 = v0&(v1) ;
    float dot02 = v0&(v2) ;
    float dot11 = v1&(v1) ;
    float dot12 = v1&(v2) ;

    float inverDeno = 1 / (dot00 * dot11 - dot01 * dot01) ;

    float u = (dot11 * dot02 - dot01 * dot12) * inverDeno ;
    if (u < 0 || u > 1) //在面外
        return 0 ;

    float v = (dot00 * dot12 - dot01 * dot02) * inverDeno ;
    if (v < 0 || v > 1) //在面外
        return 0 ;

    if(u == 0||v == 0||abs(u+v - 1)<1e-6)//在边上
        return 1;

    if(u+v<1)//在面内
        return 2;
    else return 0;
};
