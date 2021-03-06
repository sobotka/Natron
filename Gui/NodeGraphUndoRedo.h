//  Natron
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
/*
 *Created by Alexandre GAUTHIER-FOICHAT on 6/1/2012.
 *contact: immarespond at gmail dot com
 *
 */

#ifndef NODEGRAPHUNDOREDO_H
#define NODEGRAPHUNDOREDO_H

#include "Global/GlobalDefines.h"

#include <vector>
#include <QPointF>
#include <boost/shared_ptr.hpp>

#include <QUndoCommand>

/**
 * This file gathers undo/redo command associated to the node graph. Each of them triggers an autosave when redone/undone
 * except the move command.
 **/

class Edge;
class NodeGui;
class NodeGraph;
class NodeBackDrop;
namespace Natron
{
class Node;
}


class MoveMultipleNodesCommand : public QUndoCommand
{
public:
    
    struct NodeToMove
    {
        boost::shared_ptr<NodeGui> node;
        bool isWithinBD;
    };
    
    MoveMultipleNodesCommand(const std::list<NodeToMove>& nodes,
                             const std::list<NodeBackDrop*>& bds,
                             double dx,double dy,
                             const QPointF& mouseScenePos,
                             QUndoCommand *parent = 0);
    virtual void undo();
    virtual void redo();
    virtual int id() const { return kNodeGraphMoveNodeCommandCompressionID; }
    virtual bool mergeWith(const QUndoCommand *command);
    
    
private:
    
    void move(bool skipMagnet,double dx,double dy);
    
    bool _firstRedoCalled;
    std::list<NodeToMove> _nodes;
    std::list<NodeBackDrop*> _bds;
    QPointF _mouseScenePos;
    double _dx,_dy;
};


/**
 * @class The first redo() call will do nothing, it assumes the node has already been added.
 * undo() just call deactivate() on all nodes, and following calls to redo() call activate()
 *
 * This class is used for many similar action such as add/duplicate/paste,etc...
 **/
class AddMultipleNodesCommand : public QUndoCommand
{
public:
    
    AddMultipleNodesCommand(NodeGraph* graph,
                            const std::list<boost::shared_ptr<NodeGui> >& nodes,
                            const std::list<NodeBackDrop*>& bds,
                            QUndoCommand *parent = 0);
    
    AddMultipleNodesCommand(NodeGraph* graph,
                            const boost::shared_ptr<NodeGui>& node,
                            QUndoCommand* parent = 0);
    
    AddMultipleNodesCommand(NodeGraph* graph,
                            NodeBackDrop* bd,
                            QUndoCommand* parent = 0);
    
    virtual ~AddMultipleNodesCommand();
    
    virtual void undo();
    virtual void redo();
    
private:
    
    std::list<boost::shared_ptr<NodeGui> > _nodes;
    std::list<NodeBackDrop*> _bds;
    NodeGraph* _graph;
    bool _firstRedoCalled;
    bool _isUndone;
};


class RemoveMultipleNodesCommand : public QUndoCommand
{
public:
    
    RemoveMultipleNodesCommand(NodeGraph* graph,
                  const std::list<boost::shared_ptr<NodeGui> >& nodes,
                  const std::list<NodeBackDrop*>& bds,
                  QUndoCommand *parent = 0);
    
    virtual ~RemoveMultipleNodesCommand();
    
    virtual void undo();
    virtual void redo();
    
private:
    struct NodeToRemove
    {
        ///This contains the output nodes whose input should be reconnected to this node afterwards.
        ///This list contains only nodes that are not part of the selection: we restore only the
        ///inputs of the outputs nodes of the graph that were not removed
        std::list<boost::shared_ptr<Natron::Node> > outputsToRestore;
        boost::shared_ptr<NodeGui> node;
    };
    
    std::list<NodeToRemove> _nodes;
    std::list<NodeBackDrop*> _bds;
    NodeGraph* _graph;
    bool _isRedone;
};

class ConnectCommand : public QUndoCommand{
public:
    ConnectCommand(NodeGraph* graph,Edge* edge,const boost::shared_ptr<NodeGui>&oldSrc,
                   const boost::shared_ptr<NodeGui>& newSrc,QUndoCommand *parent = 0);
    
    virtual void undo();
    virtual void redo();
    
private:
    Edge* _edge;
    boost::shared_ptr<NodeGui> _oldSrc,_newSrc;
    boost::shared_ptr<NodeGui> _dst;
    NodeGraph* _graph;
    int _inputNb;
};


class ResizeBackDropCommand : public QUndoCommand {
    
public:
    
    ResizeBackDropCommand(NodeBackDrop* bd,int w,int h,QUndoCommand *parent = 0);
    virtual ~ResizeBackDropCommand();
    
    virtual void undo();
    virtual void redo();
    virtual int id() const { return kNodeGraphResizeNodeBackDropCommandCompressionID; }
    virtual bool mergeWith(const QUndoCommand *command);
    
    
private:
    
    NodeBackDrop* _bd;
    int _w,_h;
    int _oldW,_oldH;
    
};


class DecloneMultipleNodesCommand : public QUndoCommand
{
public:
    
    DecloneMultipleNodesCommand(NodeGraph* graph,
                            const std::list<boost::shared_ptr<NodeGui> >& nodes,
                            const std::list<NodeBackDrop*>& bds,
                            QUndoCommand *parent = 0);
    
    
    virtual ~DecloneMultipleNodesCommand();
    
    virtual void undo();
    virtual void redo();
    
private:
    
    struct NodeToDeclone
    {
        boost::shared_ptr<NodeGui> node;
        boost::shared_ptr<Natron::Node> master;
    };
    
    struct BDToDeclone
    {
        NodeBackDrop* bd;
        NodeBackDrop* master;
    };
    
    std::list<NodeToDeclone> _nodes;
    std::list<BDToDeclone> _bds;
    NodeGraph* _graph;
};



class RearrangeNodesCommand : public QUndoCommand
{
public:
    
    struct NodeToRearrange
    {
        boost::shared_ptr<NodeGui> node;
        QPointF oldPos,newPos;
    };
    
    RearrangeNodesCommand(const std::list<boost::shared_ptr<NodeGui> >& nodes,
                          QUndoCommand *parent = 0);
    virtual void undo();
    virtual void redo();
    
private:
    
    std::list<NodeToRearrange> _nodes;
};


#endif // NODEGRAPHUNDOREDO_H
