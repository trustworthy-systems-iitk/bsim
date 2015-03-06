#ifndef _UTILITY_H_DEFINED_
#define _UTILITY_H_DEFINED_

#include <vector>
#include <algorithm>
#include <assert.h>
#include <string>

class node_t;

struct nodetriple_t {
    node_t* a;
    node_t* b;
    node_t* c;

private:
    void init(node_t* x, node_t* y, node_t* z) {
        std::vector<node_t*> nodes(3);
        nodes[0] = x;
        nodes[1] = y;
        nodes[2] = z;
        std::sort(nodes.begin(), nodes.end());
        a = nodes[0];
        b = nodes[1];
        c = nodes[2];
    }
public:
    nodetriple_t(node_t* x, node_t* y, node_t* z) {
        init(x,y,z);
    }

    nodetriple_t(std::vector<node_t*>& nodelist) {
        assert(nodelist.size() == 3);
        init(nodelist[0], nodelist[1], nodelist[2]);
    }


    bool operator==(const nodetriple_t& other) const
    {
        return(a == other.a && 
               b == other.b &&
               c == other.c);

    }
    bool operator<(const nodetriple_t& other) const
    {
        if(a < other.a) return true;
        else if(a > other.a) return false;

        // only gets here if a == other.a
        assert(a == other.a);
        if(b < other.b) return true;
        else if(b > other.b) return false;

        // only gets here if a == other.a && b == other.b
        assert(a == other.a && b == other.b);
        if(c < other.c) return true;
        else return false;
    }
};

struct nodequad_t {
    node_t* a;
    node_t* b;
    node_t* c;
    node_t* d;

private:
    void init(node_t* x, node_t* y, node_t* z, node_t* w) {
        std::vector<node_t*> nodes(4);
        nodes[0] = x;
        nodes[1] = y;
        nodes[2] = z;
        nodes[3] = w;
        std::sort(nodes.begin(), nodes.end());
        a = nodes[0];
        b = nodes[1];
        c = nodes[2];
        d = nodes[3];
    }
public:
    nodequad_t(node_t* x, node_t* y, node_t* z, node_t* w) {
        init(x,y,z,w);
    }

    nodequad_t(std::vector<node_t*>& nodelist) {
        assert(nodelist.size() == 4);
        init(nodelist[0], nodelist[1], nodelist[2], nodelist[3]);
    }


    bool operator==(const nodequad_t& other) const
    {
        return(a == other.a && 
               b == other.b &&
               c == other.c &&
               d == other.d);

    }
    bool operator<(const nodequad_t& other) const
    {
        if(a < other.a) return true;
        else if(a > other.a) return false;

        // only gets here if a == other.a
        assert(a == other.a);
        if(b < other.b) return true;
        else if(b > other.b) return false;

        // only gets here if a == other.a && b == other.b
        assert(a == other.a && b == other.b);
        if(c < other.c) return true;
        else if(c > other.c) return false;

        // only gets here if a == other.a && b == other.b && c == other.c
        assert(a == other.a && b == other.b && c == other.c);
        if(d < other.d) return true;
        else return false;
    }
};

struct node5tup_t {
    node_t* a;
    node_t* b;
    node_t* c;
    node_t* d;
	node_t* e;

private:
    void init(node_t* x, node_t* y, node_t* z, node_t* w, node_t* u) {
        std::vector<node_t*> nodes(5);
        nodes[0] = x;
        nodes[1] = y;
        nodes[2] = z;
        nodes[3] = w;
		nodes[4] = u;
        std::sort(nodes.begin(), nodes.end());
        a = nodes[0];
        b = nodes[1];
        c = nodes[2];
        d = nodes[3];
		e = nodes[4];
    }
public:
    node5tup_t(node_t* x, node_t* y, node_t* z, node_t* w, node_t* u) {
        init(x,y,z,w,u);
    }

    node5tup_t(std::vector<node_t*>& nodelist) {
        assert(nodelist.size() == 5);
        init(nodelist[0], nodelist[1], nodelist[2], nodelist[3], nodelist[4]);
    }


    bool operator==(const node5tup_t& other) const
    {
        return(a == other.a && 
               b == other.b &&
               c == other.c &&
               d == other.d &&
			   e == other.e);

    }
    bool operator<(const node5tup_t& other) const
    {
        if(a < other.a) return true;
        else if(a > other.a) return false;

        // only gets here if a == other.a
        assert(a == other.a);
        if(b < other.b) return true;
        else if(b > other.b) return false;

        // only gets here if a == other.a && b == other.b
        assert(a == other.a && b == other.b);
        if(c < other.c) return true;
        else if(c > other.c) return false;

        // only gets here if a == other.a && b == other.b && c == other.c
        assert(a == other.a && b == other.b && c == other.c);
        if(d < other.d) return true;
        else if(d > other.d) return false;

		// a through d need to be equal.
        assert(a == other.a && b == other.b && c == other.c && d == other.d);
		if(e < other.e) return true;
		else return false;
    }
};

struct node_tuple_t {
	std::vector<node_t*> nodes;

    node_tuple_t() { }
    void sort() { std::sort(nodes.begin(), nodes.end()); }

    node_tuple_t(const std::vector<node_t*>& nodelist) 
		: nodes(nodelist)
	{
		std::sort(nodes.begin(), nodes.end());
    }

	unsigned size() const { return nodes.size(); }

    bool operator==(const node_tuple_t& other) const
    {
		if(nodes.size() != other.size()) return false;
		for(unsigned i=0; i != nodes.size(); i++)
		{
			if(nodes[i] != other.nodes[i]) return false;
		}
		return true;
    }

    bool operator<(const node_tuple_t& other) const
    {
		if(size() < other.size()) return true;
		else if(size() > other.size()) return false;
		else {
			unsigned i;
			for(i=0; i != size(); i++) {
				if(nodes[i] < other.nodes[i]) return true;
				else if(nodes[i] > other.nodes[i]) return false;
			}
			// all equal at this point.
			// a = b => ~(a < b)
			return false;
		}
    }
};

void split_lines(std::vector<std::string>& result, std::string& line, int cols);

#endif
