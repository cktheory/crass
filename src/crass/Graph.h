//
//  Graph.h
//  crass
//
//  Created by Connor Skennerton on 10/01/14.
//  Copyright (c) 2014 Australian Centre for Ecogenomics. All rights reserved.
//

#ifndef __crass__Graph__
#define __crass__Graph__

#include <iostream>
#include <vector>
#include <unordered_map>
#include <deque>

#include "StringCheck.h"
#include "Sequence.h"
namespace crass {
    
    /*
        Node Types
        REPEAT: all bases in node originate from a repeat
        MIXED:  some bases are from both the repeat and spacer
        SPACER: all bases in node originate from a spacer
        UNDEF: For when we don't know yet
     
     */
    enum graph_node_t {
        REPEAT,     
        MIXED,      
        SPACER,
        UNDEF
    };
    
    
    /*
        Normal and jumping edges
        Normal:     This is the standard edge generated by a debruijn graph.  It will point to a
                    kmer which differs by a single base from the current node
        Jumping:    A jumping edge is entirely different.  This type of edge links two nodes that
                    are not next to each other in the naive graph, instead they link two nodes that
                    are on either side of a repeat sequence.  You can therefore think of a jumping
                    edge as being symbolic of the repeat sequence itself but allows for walking through
                    the graph without going though the repeat, which may make determining the corect 
                    path difficult.
     */
    struct Node {
        std::vector<Node *> mFwdEdges;                  // a list of nodes that are in front of this one
        std::vector<Node *> mRevEdges;                  // a list of nodes behind this one
        std::vector<Node *> mFwdJmpEdges;               // a list of nodes that are jumping forward
        std::vector<Node *> mRevJmpEdges;               // a list of nodes that are jumping in reverse
        std::vector<RawRead *> mReadsContainingNode;    // a list of reads which contain this node
    public:
        StringToken mId;
        int mCov;
        graph_node_t mType;
        
        Node(StringToken i) : mId(i), mCov(1), mType(UNDEF) {}
        
        int inDegree() {return mRevEdges.size();}
        int outDegree() {return mFwdEdges.size();}
        int inJmpDegree() {return mRevJmpEdges.size();}
        int outJmpDegree() {return mFwdJmpEdges.size();}
        
        void addFwdEdge(Node * n) {mFwdEdges.push_back(n);}
        void addRevEdge(Node * n) {mRevEdges.push_back(n);}
        void addFwdJmpEdge(Node * n) {mFwdJmpEdges.push_back(n);}
        void addRevJmpEdge(Node * n) {mRevJmpEdges.push_back(n);}
        
        Node * fwdEdge(int n) {return mFwdEdges.at(n);}
        Node * revEdge(int n) {return mRevEdges.at(n);}
        Node * fwdJmpEdge(int n) {return mFwdJmpEdges.at(n);}
        Node * revJmpEdge(int n) {return mRevJmpEdges.at(n);}


    };

    class Graph {
        std::unordered_map<StringToken, Node *> mNodes;
        StringCheck mNodeLookup; // hash of the node Ids to their string representations
        int mKmerLength;

    public:
        Graph(int _k) : mKmerLength(_k){}
        ~Graph();
        void addReadToGraph(crass::RawRead& read);
        Node * getNode(StringToken T);
        void identifyRepeatNodes(std::deque<Node *>& rpath);
        void toGraphviz(std::ostream& out);
        
    };
}
#endif /* defined(__crass__Graph__) */