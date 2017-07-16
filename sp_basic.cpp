//#include "./sp.h"
//#include <GLFW/glfw3.h>
//
//using namespace std;
//bool Intersection(Vector2 a,Vector2 b, Vector2 c,Vector2 d);
//int pointInFace(Vector3 A, Vector3 B, Vector3 C, Vector3 P);
//
//vector<pair<double,double> >GetShortestPath(
//        vector<pair<double,double> > const& points,
//        vector<pair<int,int> > const& edges,
//        vector<tuple<int,int,int> > const& faces,
//        pair<double,double> from,
//        pair<double,double> to)
//{
//    int vnum = points.size();
//    int edgeNum = edges.size();
//
//    int* prev = new int[vnum];
//    double* disToStart = new double[vnum]();
//
//    double* disMatrix = new double[vnum*vnum]();
//    bool* isEdgeBoundary = new bool[edgeNum]();
//
//    bool* haveBeenMarked = new bool[vnum]();
//
//    const int start = find(points.begin(),points.end(),from) - points.begin();
//    const int final = find(points.begin(),points.end(),to) - points.begin();
//
//    for(auto face : faces)
//    {
//        int a,b,c;
//        std::tie(a,b,c) = face;
//
//        isEdgeBoundary[a] = !isEdgeBoundary[a];
//        isEdgeBoundary[b] = !isEdgeBoundary[b];
//        isEdgeBoundary[c] = !isEdgeBoundary[c];
//    }
//
//    for(auto edge : edges) {
//        disMatrix[edge.first*vnum+edge.second] = (Vector2(points[edge.first].first,points[edge.first].second) - Vector2(points[edge.second].first,points[edge.second].second)).len();
//        disMatrix[edge.second*vnum+edge.first] = disMatrix[edge.first*vnum+edge.second];
//    }
//
//    Vector2 a(points[start].first,points[start].second);
//    for(int i = 0;i<vnum;i++)
//    {
//        if(!disMatrix[start*vnum+i])
//        {
//            Vector2 b(points[i].first,points[i].second);
//            int crossMask = 0;
//            for(int k = 0;k<edgeNum;k++)
//            {
//                if(isEdgeBoundary[k])
//                {
//                    Vector2 c(points[edges[k].first].first,points[edges[k].first].second);
//                    Vector2 d(points[edges[k].second].first,points[edges[k].second].second);
//                    if(Intersection(a,b,c,d))
//                    {
//                        disToStart[i] = INFINITE;
//                        crossMask = 1;
//                        break;
//                    }
//                }
//            }
//            if(!crossMask)
//            {
//                int infacemask = 0;
//                Vector2 mid = (a+b)/2;
//                for(auto face : faces)
//                {
//                    int a,b,c;
//                    std::tie(a,b,c) = face;
//                    int A,B,C;
//                    A = edges[a].first;
//                    B = edges[a].second;
//                    if(edges[b].first == A||edges[b].first == B)
//                        C = edges[b].second;
//                    else C = edges[b].first;
//                    if(pointInFace(Vector2(points[A].first,points[A].second) ,Vector2(points[B].first,points[B].second),Vector2(points[C].first,points[C].second),mid))
//                    {
//                        infacemask = 1;
//                        disToStart[i] = INFINITE;
//                        break;
//                    }
//                }
//                if(infacemask)
//                {
//                    disToStart[i] = (a-b).len();
//                }
//            }
//        }
//        else disToStart[i] = disMatrix[start*vnum+i];
//
//        haveBeenMarked[i] = false;
//        if(disToStart[i] == INFINITE)
//            prev[i] = -1;
//        else prev[i] = start;
//    }
//
//    haveBeenMarked[start] = true;
//    disToStart[start] = 0;
//    for(int i = 0;i<vnum;i++)
//    {
//        double mindist = INFINITE;
//        int closestVer = -1;
//        for(int j = 0;j<vnum;j++)
//        {
//            if((!haveBeenMarked[j])&&disToStart[j]<mindist)
//            {
//                closestVer = j;
//                mindist = disToStart[j];
//            }
//        }
//        if(closestVer == -1)
//            break;
//
//        haveBeenMarked[closestVer] = true;
//
//        for(int j = 0;j<vnum;j++)
//        {
//            if(haveBeenMarked[j])
//                continue;
//            double predis = Vector2(points[j].first-points[closestVer].first,points[j].second-points[closestVer].second).len();
//            if(predis + disToStart[closestVer]<disToStart[j])
//            {
//
//                int  crossWithEdge = 0;
//                Vector2 a(points[closestVer].first,points[closestVer].second);
//                Vector2 b(points[j].first,points[j].second);
//                for(int k = 0;k<edgeNum;k++)
//                {
//                    if(isEdgeBoundary[k])
//                    {
//                        Vector2 c(points[edges[k].first].first,points[edges[k].first].second);
//                        Vector2 d(points[edges[k].second].first,points[edges[k].second].second);
//                        if(Intersection(a,b,c,d))
//                        {
//                            crossWithEdge = 1;
//                            break;
//                        }
//                    }
//                }
//                if( crossWithEdge)
//                    continue;
//
//                auto mid = Vector2((points[closestVer].first+points[j].first)*0.5,(points[closestVer].second+points[j].second)*0.5);
//                int inAFace = 0;
//                for(auto face : faces)
//                {
//                    int a,b,c;
//                    std::tie(a,b,c) = face;
//                    int p1,p2,p3;
//                    p1 = edges[a].first;
//                    p2 = edges[a].second;
//                    if(edges[b].first == p1||edges[b].first == p2)
//                        p3 = edges[b].second;
//                    else p3 = edges[b].first;
//                    if(pointInFace(Vector2(points[p1].first,points[p1].second) ,Vector2(points[p2].first,points[p2].second),Vector2(points[p3].first,points[p3].second),mid))
//                    {
//                        inAFace = 1;
//                        break;
//                    }
//                }
//                if(!inAFace)
//                    continue;
//
//                disToStart[j] = predis + disToStart[closestVer];
//                prev[j] = closestVer;
//            }
//        }
//    }
//
//    auto path = vector<pair<double,double>>();
//    for(int current = final;current != start;current = prev[current])
//    {
//        if(current == -1)
//        {
//            filelog<<"\nno way between start and final\n";
//            console<<"\nno way between start and final\n";
//
//            return vector<pair<double,double>>();
//        }
//
//        path.push_back(pair<double,double>(points[current].first,points[current].second));
//    }
//    path.push_back(pair<double,double>(points[start].first,points[start].second));
//
//    delete prev;
//    delete disToStart;
//    delete disMatrix;
//    delete isEdgeBoundary;
//    delete haveBeenMarked;
//
//    return path;
//
//}
//
//bool Intersection(Vector2 a,Vector2 b, Vector2 c,Vector2 d)
//{
//    if((a.x == c.x&&a.y == c.y) || (a.x == d.x&&a.y == d.y)||(b.x == c.x&&b.y == c.y) || (b.x == d.x&&b.y == d.y))
//        return false;
//    b.x -= a.x; b.y -= a.y;
//    c.x -= a.x; c.y -= a.y;
//    d.x -= a.x; d.y -= a.y;
//
//    double distAB = sqrt(b.x * b.x + b.y * b.y);
//
//    double theCos = b.x / distAB;
//    double theSin = b.y / distAB;
//    double newX = c.x * theCos + c.y * theSin;
//    c.y = c.y * theCos - c.x * theSin; c.x = newX;
//    newX = d.x * theCos + d.y * theSin;
//    d.y = d.y * theCos - d.x * theSin; d.x = newX;
//
//    double posAB = d.x + (c.x - d.x) * d.y / (d.y - c.y);
//
//    if (posAB <= 0 || posAB >= distAB)
//        return false;
//
//    if(c.y*d.y<0)
//        return true;
//    return false;
//}
//
//int pointInFace(Vector3 A, Vector3 B, Vector3 C, Vector3 P) {
//    Vector3 v0 = C - A;
//    Vector3 v1 = B - A;
//    Vector3 v2 = P - A;
//
//    float dot00 = v0 & v0;
//    float dot01 = v0 & v1;
//    float dot02 = v0 & v2;
//    float dot11 = v1 & v1;
//    float dot12 = v1 & v2;
//
//    float inverDeno = 1 / (dot00 * dot11 - dot01 * dot01);
//
//    float u = (dot11 * dot02 - dot01 * dot12) * inverDeno;
//    if (u < 0 || u > 1) //在面外
//        return 0;
//
//    float v = (dot00 * dot12 - dot01 * dot02) * inverDeno;
//    if (v < 0 || v > 1) //在面外
//        return 0;
//
//    if (u == 0 || v == 0 || abs(u + v - 1) < 1e-6)//在边上
//        return 1;
//
//    if (u + v < 1)//在面内
//        return 2;
//    else return 0;
//};
