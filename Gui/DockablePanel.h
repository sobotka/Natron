//  Powiter
//
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
/*
*Created by Alexandre GAUTHIER-FOICHAT on 6/1/2012. 
*contact: immarespond at gmail dot com
*
*/

#ifndef POWITER_GUI_SETTINGSPANEL_H_
#define POWITER_GUI_SETTINGSPANEL_H_

#include <map>

#include <QFrame>

#include <boost/shared_ptr.hpp>

#include "Global/GlobalDefines.h"

class LineEdit;
class Node;
class Knob;
class KnobGui;
class NodeGui;
class QVBoxLayout;
class QGridLayout;
class QTabWidget;
class Button;
class QHBoxLayout;
class QLabel;
class QUndoStack;
class QUndoCommand;

namespace Powiter{
    class Project;
};

/**
 * @brief An abstract class that defines a dockable properties panel that can be found in the Property bin pane.
**/
class DockablePanel : public QFrame{
    Q_OBJECT
    
    QVBoxLayout* _container; /*!< ptr to the layout containing this DockablePanel*/
    
    /*global layout*/
    QVBoxLayout* _mainLayout;
    
    /*Header related*/
    QFrame* _headerWidget;
    QHBoxLayout *_headerLayout;
    
    LineEdit* _nameLineEdit; /*!< if the name is editable*/
    QLabel* _nameLabel; /*!< if the name is read-only*/
    
    /*Tab related*/
    QTabWidget* _tabWidget;
    
    Button* _helpButton;
    Button* _minimize;
    Button* _cross;
    
    Button* _undoButton;
    Button* _redoButton;
    
    bool _minimized; /*!< true if the panel is minimized*/
    QUndoStack* _undoStack; /*!< undo/redo stack*/
    
    /*a map storing for each knob a pointer to their GUI.*/
    std::map<Knob*,KnobGui*> _knobs;
    
    /* map<tab name, pair<tab , row count> >*/
    std::map<QString,std::pair<QWidget*,int> > _tabs;
    
public:
    
    explicit DockablePanel(QVBoxLayout* container
                  ,bool readOnlyName
                  ,const QString& initialName
                  ,const QString& helpToolTip
                  ,const QString& defaultTab
                  ,QWidget *parent = 0);
    
    virtual ~DockablePanel();
    
    bool isMinimized() const {return _minimized;}
    
    /*inserts a new tab to the dockable panel.*/
    void addTab(const QString& name);
    
    /*Creates a new button and inserts it in the header
     at position headerPosition. You can then take
     the pointer to the Button to customize it the
     way you want. The ownership of the Button remains
     to the DockablePanel.*/
    Button* insertHeaderButton(int headerPosition);
    
    void pushUndoCommand(QUndoCommand* cmd);
    
public slots:
    
    /*Internal slot, not meant to be called externally.*/
    void closePanel();
    
    /*Internal slot, not meant to be called externally.*/
    void minimizeOrMaximize(bool toggled);
    
    /*Internal slot, not meant to be called externally.*/
    void showHelp();
    
    /*You can connect to this when you want to change
     the name externally.*/
    void onNameChanged(const QString& str);
    
    /*initializes the knobs GUI*/
    void initializeKnobs();
    
    /*Internal slot, not meant to be called externally.*/
    void onKnobDeletion(KnobGui* k);
    
    /*Internal slot, not meant to be called externally.*/
    void onUndoPressed();
    
    /*Internal slot, not meant to be called externally.*/
    void onRedoPressed();
    

protected:
    
    virtual void mousePressEvent(QMouseEvent* e){
        emit selected();
        QFrame::mousePressEvent(e);
    }
    
    /*Must return a vector of knobs*/
    virtual const std::vector<Knob*>&  getKnobs() const = 0;
    
signals:
    
    /*emitted when the panel is clicked*/
    void selected();
    
    /*emitted when the name changed on the line edit*/
    void nameChanged(QString);
    
    /*emitted when the user pressed undo*/
    void undoneChange();
    
    /*emitted when the user pressed redo*/
    void redoneChange();
    
    /*emitted when the panel is minimized*/
    void minimized();
    
    /*emitted when the panel is maximized*/
    void maximized();
    
    /*emitted when the panel is closed.*/
    void closed();
    
private:
    
    /*Search an existing knob GUI in the map, otherwise creates
     the gui for the knob.*/
    KnobGui* findKnobGuiOrCreate(Knob* knob);
       
};

class NodeSettingsPanel : public DockablePanel
{
    Q_OBJECT
    
    Q_PROPERTY( bool _selected READ isSelected WRITE setSelected)
    
    /*Pointer to the node GUI*/
    NodeGui* _nodeGUI;
    
    bool _selected;

public:

    explicit NodeSettingsPanel(NodeGui* NodeUi, QWidget *parent = 0);
    
    virtual ~NodeSettingsPanel(){}
    
    void setSelected(bool s);
    
    bool isSelected() const {return _selected;}
    
protected:
   
    /*Must return a vector of knobs*/
    virtual const std::vector<Knob*>&  getKnobs() const OVERRIDE;
};

class ProjectSettingsPanel : public DockablePanel{
    
    boost::shared_ptr<Powiter::Project> _project;
public:
    explicit ProjectSettingsPanel(boost::shared_ptr<Powiter::Project> project,QVBoxLayout* container,QWidget *parent = 0);
    
    virtual  ~ProjectSettingsPanel(){}
    
protected:

    /*Must return a vector of knobs*/
    virtual const std::vector<Knob*>&  getKnobs() const OVERRIDE ;
};

#endif // POWITER_GUI_SETTINGSPANEL_H_