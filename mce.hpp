
#ifndef MCE__HPP
#define MCE__HPP

#include <assert.h>
#include <cstdlib>
#include <vector>
#include <map>
#include "inputbuffer.hpp"

using std::vector;
using std::map;

typedef int vid;
typedef vector<vid>::iterator vIt;

struct vtype
{
    vid key;
    vid *nbv;
    vid deg;
};

struct graph_t 
{
    public:
        graph_t():data(NULL), nodenum(0){};
        void init_g(FILE *gfile);
        vtype &operator[] (const vid index);
        const vtype &operator[] (const vid index) const;
        ~graph_t();

    vtype *data;
    vid   nodenum;
};

/*
 * initialize the graph structure
 * the vertices order can be not sorted
 */
void
graph_t::init_g(FILE *gfile)
{
    inputbuffer gbuffer(gfile);
    char *start = NULL, *end = NULL;
    gbuffer.getline(start, end);
    nodenum = atoi(start);

    data = (vtype *)malloc(sizeof(vtype) * nodenum);
    assert( data != NULL );

    vid count = nodenum;
    while( --count >= 0 )
    {
        start = NULL;
        end   = NULL;
        if( gbuffer.getline(start, end) < 0 ) break;

        vid v = 0;
        while( *start == '\n' ) ++start;

        while( *(start) != ',' )
        {
            v = (10 * v) + int(*start) - 48;
            ++start;
        }
        assert( v < nodenum );

        vid deg = 0;
        while( *(++start) != ':' && *start != '\n' )
            deg = (10 * deg) + int(*start) - 48;

        vid *adjlist = (vid *)malloc(sizeof(vid) * deg);
        data[v].nbv = adjlist;
        data[v].deg = deg;

        for( int i = 0; i < deg ; ++i )
        {
            vid u = 0;
            while( *(++start) != ':' && *(start) != '\n' )
                u = (10 * u) + int(*start) - 48;
            data[v].nbv[i] = u;
        }
    }
}

vtype &
graph_t::operator[] (const vid index)
{
    return data[index];
}

const vtype &
graph_t::operator[] (const vid index) const
{
    return data[index];
}

graph_t::~graph_t()
{
    for(int i = 0; i < nodenum; ++i)
        free( data[i].nbv );
    free(data);
}

void get_vertex_dd_map(vector<vid>&, vid&, map<vid,vid>&, inputbuffer&);
void get_neighbor_cc(graph_t& , vid, vector<vid>&);
vid  binaray_search(vtype&, vid);
int  wcc(graph_t&, vector<int>&);
void mark_cc(graph_t&, vid, int *, int);
void get_neibor_sg(graph_t &, graph_t &, vid);
void init_g_withddmap(graph_t &g, FILE *gfile, map<vid,vid> &ddmap);

#endif

