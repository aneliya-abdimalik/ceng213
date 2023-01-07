#include "MeshGraph.h"
#include "BinaryHeap.h"

// For printing
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

MeshGraph::MeshGraph(const std::vector<Double3>& vertexPositions, const std::vector<IdPair>& edges)
{
    int vertix_size = vertexPositions.size();
    vertices.resize(vertix_size);
    adjList.resize(vertix_size);
    std::vector<IdPair>::const_iterator iter;
    for(int i=0;i<vertix_size;i++)
    {
        vertices[i].id = i;
        vertices[i].position3D = vertexPositions[i];
    }
    
    for (iter = edges.begin(); iter != edges.end(); ++iter)
    {
        int id_1 = iter->vertexId0;
        int id_2 = iter->vertexId1;
        adjList[id_1].push_back(&vertices[id_2]);
        adjList[id_2].push_back(&vertices[id_1]);
    }
}

double MeshGraph::AverageDistanceBetweenVertices() const
{
    double distance=0;
    int total=0,adj_size=adjList.size();
    std::list<Vertex*>::const_iterator iter;
    for(int i=0;i<adj_size;i++)
    {
        for(iter=adjList[i].begin();iter!=adjList[i].end();iter++)
        {
                total++;
                distance+=Double3::Distance(vertices[i].position3D,(*iter)->position3D);
        }
    }
    return distance/total;
}

double MeshGraph::AverageEdgePerVertex() const
{
    int adj_size=adjList.size();
    double total=0;
    std::list<Vertex*> :: const_iterator iter;
    for(int i=0;i<adj_size;i++)
        for(iter=adjList[i].begin();iter!=adjList[i].end();iter++)
                total++;
    return (total/adj_size)/2;
}

int MeshGraph::TotalVertexCount() const
{
    return (int) adjList.size();
}

int MeshGraph::TotalEdgeCount() const
{
    int total=0,adj_size=adjList.size();
    std::list<Vertex*> :: const_iterator iter;
    for(int i=0;i<adj_size;i++)
        for(iter=adjList[i].begin();iter!=adjList[i].end();iter++)
            total++;
    return total/2;
}

int MeshGraph::VertexEdgeCount(int vertexId) const
{
    if(vertexId<0 || vertexId>TotalEdgeCount())
        return -1;
    
    int result=0,adj_size=adjList.size();
    std::list<Vertex*>::const_iterator iter;
    for(int i=0;i<adj_size;i++)
        if(i==vertexId)
            for(iter=adjList[i].begin();iter!=adjList[i].end();iter++)
                result++;
    return result;
}

void MeshGraph::ImmediateNeighbours(std::vector<int>& outVertexIds, int vertexId) const
{
    if(vertexId>=0  && vertexId<TotalEdgeCount())
    {
        int adj_size=adjList.size();
        std::list<Vertex*> :: const_iterator iter;
        for(int i=0;i<adj_size;i++)
            if(i==vertexId)
                for(iter=adjList[i].begin();iter!=adjList[i].end();iter++)
                    outVertexIds.push_back((*iter)->id);
    }
    
}

void MeshGraph::PaintInBetweenVertex(std::vector<Color>& outputColorAllVertex, int vertexIdFrom, int vertexIdTo, const Color& color) const
{
    outputColorAllVertex.clear();
    int vertix_size=vertices.size();
    if (vertexIdFrom < 0 || vertexIdFrom >= vertix_size || vertexIdTo < 0 || vertexIdTo >= vertix_size)
        return;

    outputColorAllVertex.resize(vertix_size);
    for (int i = 0; i < vertix_size; i++)
    {
        outputColorAllVertex[i].r = 0;
        outputColorAllVertex[i].g = 0;
        outputColorAllVertex[i].b = 0;
    }

    BinaryHeap heap;
    heap.Add(vertexIdFrom, 0);
    std::vector<double> distances(vertix_size, INFINITY);
    distances[vertexIdFrom] = 0;
    std::vector<int> previous(vertix_size, -1);
    
    while (heap.HeapSize())
    {
        int curr_id;
        double distance;
        heap.PopHeap(curr_id, distance);
        std::list<Vertex*>::const_iterator iter;
        for (iter = adjList[curr_id].begin(); iter != adjList[curr_id].end(); iter++)
        {
            double new_dist = distance + Double3::Distance(vertices[curr_id].position3D, vertices[(*iter)->id].position3D);
            
            if (new_dist < distances[(*iter)->id])
            {
                distances[(*iter)->id] = new_dist;
                previous[(*iter)->id] = curr_id;
    
                if (!heap.ChangePriority((*iter)->id, new_dist))
                    heap.Add((*iter)->id, new_dist);
            }
        }
    }
    
    int j = vertexIdTo;
    while (j != vertexIdFrom)
    {
        outputColorAllVertex[j] = color;
        j = previous[j];
    }
    outputColorAllVertex[vertexIdFrom] = color;
}

void MeshGraph::PaintInRangeGeodesic(std::vector<Color>& outputColorAllVertex, int vertexId, const Color& color, int maxDepth, FilterType type, double alpha) const
{
    outputColorAllVertex.clear();
    int vertix_size=vertices.size();
    if (vertexId < 0 || vertexId >= vertix_size)
        return;

    outputColorAllVertex.resize(vertix_size);
    for (int i = 0; i < vertix_size; i++)
    {
        outputColorAllVertex[i].r = 0;
        outputColorAllVertex[i].g = 0;
        outputColorAllVertex[i].b = 0;
    }
    
    int c=0;

    BinaryHeap heap;
    heap.Add(vertexId, c++);
    
    std::vector<bool> visited(vertix_size,false);
    visited[vertexId]=true;
    std::vector<double> distances(vertix_size,0);
    std::vector<int> previous(vertix_size, vertexId);
    std::vector<int> depth;
    depth.resize(vertix_size);
    depth[vertexId]=0;
    std::vector<int> neigh;
    

    while(heap.HeapSize())
    {
        int curr_id;
        double distance;
        heap.PopHeap(curr_id,distance);
        
        if(depth[curr_id]<=maxDepth)
        {
        
            distances[curr_id]=Double3::Distance(vertices[previous[curr_id]].position3D,vertices[curr_id].position3D)+distances[previous[curr_id]];
            
            double f=Filter(type, distances[curr_id], alpha);
            outputColorAllVertex[curr_id].r=(unsigned char) color.r*f;
            outputColorAllVertex[curr_id].g=(unsigned char) color.g*f;
            outputColorAllVertex[curr_id].b=(unsigned char) color.b*f;
            
            ImmediateNeighbours(neigh,curr_id);
            std::sort(neigh.begin(),neigh.end());

            for(int i=0;i<(int)neigh.size();i++)
            {
                if(!visited[neigh[i]])
                {
                    visited[neigh[i]]=true;
                    previous[neigh[i]]=curr_id;
                    heap.Add(neigh[i],c++);
                    depth[neigh[i]]=depth[curr_id]+1;
                }
            }
        }
    }
}

void MeshGraph::PaintInRangeEuclidian(std::vector<Color>& outputColorAllVertex, int vertexId, const Color& color, int maxDepth, FilterType type, double alpha) const
{
    outputColorAllVertex.clear();
    int vertix_size=vertices.size();
    if (vertexId < 0 || vertexId >= vertix_size)
        return;

    outputColorAllVertex.resize(vertix_size);
    for (int i = 0; i < vertix_size; i++)
    {
        outputColorAllVertex[i].r = 0;
        outputColorAllVertex[i].g = 0;
        outputColorAllVertex[i].b = 0;
    }
        
    BinaryHeap heap;
    heap.Add(vertexId, 0);
    std::vector<bool> visited(vertix_size,false);
    std::vector<int> neigh;
    int curr_id;
    double distance;
    
    while(heap.HeapSize())
    {
        heap.PopHeap(curr_id,distance);
        int temp=distance;
        if(temp++ <= maxDepth)
        {
            distance=Double3::Distance(vertices[curr_id].position3D,vertices[vertexId].position3D);
            double f=Filter(type, distance, alpha);
            outputColorAllVertex[curr_id].r=(unsigned char) color.r*f;
            outputColorAllVertex[curr_id].g=(unsigned char) color.g*f;
            outputColorAllVertex[curr_id].b=(unsigned char) color.b*f;
            ImmediateNeighbours(neigh,curr_id);
            std::sort(neigh.begin(),neigh.end());
            for(int i=0;i<(int)neigh.size();i++)
            {
                if(!visited[neigh[i]])
                {
                    visited[neigh[i]]=true;
                    heap.Add(neigh[i],temp);
                }
            }
        }
    }
}

    
double MeshGraph::Filter(FilterType type, double x, double a) const
{
    if(type==FILTER_BOX)
    {
        if(x<=a && -a<=x)
            return 1;
        else
            return 0;
    }
    
    else if(type==FILTER_GAUSSIAN)
        return std::exp((-1)*(x*x)/(a*a));
}


void MeshGraph::WriteColorToFile(const std::vector<Color>& colors,
                                 const std::string& fileName)
{
    // IMPLEMENTED
    std::stringstream s;
    for(int i = 0; i < static_cast<int>(colors.size()); i++)
    {
        int r = static_cast<int>(colors[i].r);
        int g = static_cast<int>(colors[i].g);
        int b = static_cast<int>(colors[i].b);

        s << r << ", " << g << ", " << b << "\n";
    }
    std::ofstream f(fileName.c_str());
    f << s.str();
}

void MeshGraph::PrintColorToStdOut(const std::vector<Color>& colors)
{
    // IMPLEMENTED
    for(int i = 0; i < static_cast<int>(colors.size()); i++)
    {
        std::cout << static_cast<int>(colors[i].r) << ", "
                  << static_cast<int>(colors[i].g) << ", "
                  << static_cast<int>(colors[i].b) << "\n";
    }
}