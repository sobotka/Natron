//  Natron
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */



#include "TableModelView.h"

#include <set>
#include <QHeaderView>
#include <QMouseEvent>

//////////////TableItem

TableItem::TableItem(const TableItem& other)
: values(other.values)
, view(0)
, id(-1)
, itemFlags(other.itemFlags)
{
    
}

TableItem::~TableItem()
{
    if (TableModel *model = (view ? qobject_cast<TableModel*>(view->model()) : 0)) {
        model->removeItem(this);
    }
    view = 0;
}

TableItem *TableItem::clone() const
{
    return new TableItem(*this);
}


void TableItem::setFlags(Qt::ItemFlags flags)
{
    itemFlags = flags;
    if (TableModel *model = (view ? qobject_cast<TableModel*>(view->model()) : 0))
        model->itemChanged(this);
}

QVariant TableItem::data(int role) const
{
    role = (role == Qt::EditRole ? Qt::DisplayRole : role);
    for (int i = 0; i < values.count(); ++i)
        if (values.at(i).role == role)
            return values.at(i).value;
    return QVariant();
}

void TableItem::setData(int role, const QVariant &value)
{
    bool found = false;
    role = (role == Qt::EditRole ? Qt::DisplayRole : role);
    for (int i = 0; i < values.count(); ++i) {
        if (values.at(i).role == role) {
            if (values[i].value == value)
                return;
            
            values[i].value = value;
            found = true;
            break;
        }
    }
    if (!found)
        values.append(ItemData(role, value));
    if (TableModel *model = (view ? qobject_cast<TableModel*>(view->model()) : 0)) {
        model->itemChanged(this);
    }
}


TableItem &TableItem::operator=(const TableItem &other)
{
    values = other.values;
    itemFlags = other.itemFlags;
    return *this;
}


///////////////TableModel
struct TableModelPrivate
{
    QVector<TableItem*> tableItems;
    QVector<TableItem*> horizontalHeaderItems;
    int rowCount;
    
    TableModelPrivate(int rows,int columns)
    : tableItems(rows * columns,0)
    , horizontalHeaderItems(columns,0)
    , rowCount(rows)
    {
    }
};


TableModel::TableModel(int rows,int columns,TableView* view)
: QAbstractTableModel(view)
, _imp(new TableModelPrivate(rows,columns))
{
    QObject::connect(this, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(onDataChanged(QModelIndex)));
}

TableModel::~TableModel()
{
    
}

void TableModel::onDataChanged(const QModelIndex& index)
{
    if (TableItem *i = item(index))
        emit s_itemChanged(i);
}

bool TableModel::insertRows(int row, int count, const QModelIndex &)
{
    if (count < 1 || row < 0 || row > _imp->rowCount) {
        return false;
    }
    beginInsertRows(QModelIndex(), row, row + count - 1);
    _imp->rowCount += count;
    int cc = _imp->horizontalHeaderItems.size();
    if (_imp->rowCount == 0) {
        _imp->tableItems.resize(cc * count);
    } else {
        _imp->tableItems.insert(tableIndex(row, 0), cc * count,0);
    }
    endInsertRows();
    return true;
}

bool TableModel::insertColumns(int column, int count, const QModelIndex &)
{
    if (count < 1 || column < 0 || column > _imp->horizontalHeaderItems.size()) {
        return false;
    }
    beginInsertColumns(QModelIndex(), column, column +  count - 1);
    int cc = _imp->horizontalHeaderItems.count();
    _imp->horizontalHeaderItems.insert(column, count, 0);
    if (cc == 0) {
        _imp->tableItems.resize(_imp->rowCount * count);
    } else {
        for (int row = 0; row < _imp->rowCount; ++row) {
            _imp->tableItems.insert(tableIndex(row, column), count, 0);
        }
    }
    endInsertColumns();
    return true;
}

bool TableModel::removeRows(int row, int count, const QModelIndex &)
{
    if (count < 1 || row < 0 || row + count > _imp->rowCount)
        return false;
    
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    int i = tableIndex(row, 0);
    int n = count * columnCount();
    TableItem *oldItem = 0;
    for (int j = i; j < n + i; ++j) {
        oldItem = _imp->tableItems.at(j);
        if (oldItem) {
            oldItem->view = 0;
        }
        delete oldItem;
    }
    _imp->rowCount -= count;
    _imp->tableItems.remove(std::max(i, 0), n);
    endRemoveRows();
    return true;
}

bool TableModel::removeColumns(int column, int count, const QModelIndex &)
{
    if (count < 1 || column < 0 || column + count >  _imp->horizontalHeaderItems.count())
        return false;
    
    beginRemoveColumns(QModelIndex(), column, column + count - 1);
    TableItem* oldItem = 0;
    for (int row = rowCount() - 1; row >= 0; --row) {
        int i = tableIndex(row, column);
        for (int j = i; j < i + count; ++j) {
            oldItem = _imp->tableItems.at(j);
            if (oldItem) {
                oldItem->view = 0;
            }
            delete oldItem;
        }
        _imp->tableItems.remove(i, count);
    }
    for (int h=column; h<column+count; ++h) {
        oldItem = _imp->horizontalHeaderItems.at(h);
        if (oldItem) {
            oldItem->view = 0;
        }
        delete oldItem;
    }
    _imp->horizontalHeaderItems.remove(column, count);
    endRemoveColumns();
    return true;
}

void TableModel::setItem(int row, int column, TableItem *item)
{
    int i = tableIndex(row, column);
    if (i < 0 || i >= _imp->tableItems.count())
        return;
    TableItem *oldItem = _imp->tableItems.at(i);
    if (item == oldItem) {
        return;
    }
    
    // remove old
    if (oldItem) {
        oldItem->view = 0;
    }
    delete _imp->tableItems.at(i);
    
    // set new
    if (item) {
        item->id = i;
    }
    _imp->tableItems[i] = item;
    
    QModelIndex idx = QAbstractTableModel::index(row, column);
    emit dataChanged(idx, idx);
}

TableItem *TableModel::takeItem(int row, int column)
{
    long i = tableIndex(row, column);
    TableItem *itm = _imp->tableItems.value(i);
    if (itm) {
        itm->view = 0;
        itm->id = -1;
        _imp->tableItems[i] = 0;
        QModelIndex ind = index(itm);
        emit dataChanged(ind, ind);
    }
    return itm;
}

QModelIndex TableModel::index(const TableItem *item) const
{
    if (!item) {
        return QModelIndex();
    }
    int i = -1;
    const int id = item->id;
    if (id >= 0 && id < _imp->tableItems.count() && _imp->tableItems.at(id) == item) {
        i = id;
    } else { // we need to search for the item
        i = _imp->tableItems.indexOf(const_cast<TableItem*>(item));
        if (i == -1) { // not found
            return QModelIndex();
        }
    }
    int ncols = columnCount();
    if (ncols == 0) {
        return QModelIndex();
    } else {
        int row = i / ncols;
        int col = i % ncols;
        return QAbstractTableModel::index(row, col);
    }
}

TableItem *TableModel::item(int row, int column) const
{
    return item(index(row, column));
}

TableItem *TableModel::item(const QModelIndex &index) const
{
    if (!isValid(index))
        return 0;
    return _imp->tableItems.at(tableIndex(index.row(), index.column()));
}

void TableModel::removeItem(TableItem *item)
{
    int i = _imp->tableItems.indexOf(item);
    if (i != -1) {
        _imp->tableItems[i] = 0;
        QModelIndex idx = index(item);
        emit dataChanged(idx, idx);
        return;
    }
    
    i = _imp->horizontalHeaderItems.indexOf(item);
    if (i != -1) {
        _imp->horizontalHeaderItems[i] = 0;
        emit headerDataChanged(Qt::Horizontal, i, i);
        return;
    }
}

void TableModel::setHorizontalHeaderItem(int section, TableItem *item)
{
    if (section < 0 || section >= _imp->horizontalHeaderItems.count()) {
        return;
    }
    
    TableItem *oldItem = _imp->horizontalHeaderItems.at(section);
    if (item == oldItem) {
        return;
    }
    
    if (oldItem) {
        oldItem->view = 0;
    }
    delete oldItem;
    
    TableView *view = qobject_cast<TableView*>(QObject::parent());
    
    if (item) {
        item->view = view;
        item->itemFlags = Qt::ItemFlags(int(item->itemFlags)|ItemIsHeaderItem);
    }
    _imp->horizontalHeaderItems[section] = item;
    emit headerDataChanged(Qt::Horizontal, section, section);
}

TableItem *TableModel::takeHorizontalHeaderItem(int section)
{
    if (section < 0 || section >= _imp->horizontalHeaderItems.count())
        return 0;
    TableItem *itm = _imp->horizontalHeaderItems.at(section);
    if (itm) {
        itm->view = 0;
        itm->itemFlags &= ~ItemIsHeaderItem;
        _imp->horizontalHeaderItems[section] = 0;
    }
    return itm;
}

TableItem *TableModel::horizontalHeaderItem(int section)
{
    return _imp->horizontalHeaderItems.value(section);
}

void TableModel::setRowCount(int rows)
{
    if (rows < 0 || _imp->rowCount == rows) {
        return;
    }
    if (_imp->rowCount < rows) {
        insertRows(std::max(_imp->rowCount, 0), rows - _imp->rowCount);
    } else {
        removeRows(std::max(rows, 0), _imp->rowCount - rows);
    }
}

void TableModel::setColumnCount(int columns)
{
    int cc = _imp->horizontalHeaderItems.count();
    if (columns < 0 || cc == columns) {
        return;
    }
    if (cc < columns) {
        insertColumns(std::max(cc, 0), columns - cc);
    } else {
        removeColumns(std::max(columns, 0), cc - columns);
    }
}

int TableModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : _imp->rowCount;
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : _imp->horizontalHeaderItems.count();
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    TableItem *itm = item(index);
    if (itm) {
        return itm->data(role);
    }
    return QVariant();
}

TableItem *TableModel::createItem() const
{
    return new TableItem;
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid()) {
        return false;
    }
    
    TableItem *itm = item(index);
    if (itm) {
        itm->setData(role, value);
        return true;
    }
    
    // don't create dummy table items for empty values
    if (!value.isValid()) {
        return false;
    }
    
    TableView *view = qobject_cast<TableView*>(QObject::parent());
    if (!view) {
        return false;
    }
    
    itm = createItem();
    itm->setData(role, value);
    view->setItem(index.row(), index.column(), itm);
    return true;
}

bool TableModel::setItemData(const QModelIndex &index, const QMap<int, QVariant> &roles)
{
    if (!index.isValid()) {
        return false;
    }
    
    TableView *view = qobject_cast<TableView*>(QObject::parent());
    TableItem *itm = item(index);
    if (itm) {
        itm->view = 0; // prohibits item from calling itemChanged()
        bool changed = false;
        for (QMap<int, QVariant>::ConstIterator it = roles.constBegin(); it != roles.constEnd(); ++it) {
            if (itm->data(it.key()) != it.value()) {
                itm->setData(it.key(), it.value());
                changed = true;
            }
        }
        itm->view = view;
        if (changed)
            itemChanged(itm);
        return true;
    }
    
    if (!view)
        return false;
    
    itm = createItem();
    for (QMap<int, QVariant>::ConstIterator it = roles.constBegin(); it != roles.constEnd(); ++it)
        itm->setData(it.key(), it.value());
    view->setItem(index.row(), index.column(), itm);
    return true;
}

QMap<int, QVariant> TableModel::itemData(const QModelIndex &index) const
{
    QMap<int, QVariant> roles;
    TableItem *itm = item(index);
    if (itm) {
        for (int i = 0; i < itm->values.count(); ++i) {
            roles.insert(itm->values.at(i).role,
                         itm->values.at(i).value);
        }
    }
    return roles;
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (section < 0)
        return QVariant();
    
    TableItem *itm = 0;
    if (orientation == Qt::Horizontal && section < _imp->horizontalHeaderItems.count())
        itm = _imp->horizontalHeaderItems.at(section);
    else
        return QVariant(); // section is out of bounds
    
    if (itm)
        return itm->data(role);
    if (role == Qt::DisplayRole)
        return section + 1;
    return QVariant();
}

bool TableModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (section < 0 ||
        (orientation == Qt::Horizontal && _imp->horizontalHeaderItems.size() <= section))
        return false;
    
    TableItem *itm = 0;
    if (orientation == Qt::Horizontal)
        itm = _imp->horizontalHeaderItems.at(section);
    
    if (itm) {
        itm->setData(role, value);
        return true;
    }
    return false;
}

long TableModel::tableIndex(int row, int column) const { return (row * _imp->horizontalHeaderItems.count()) + column; }

void TableModel::clear()
{
    beginResetModel();
    for (int i = 0; i < _imp->tableItems.count(); ++i) {
        if (_imp->tableItems.at(i)) {
            _imp->tableItems.at(i)->view = 0;
            delete _imp->tableItems.at(i);
            _imp->tableItems[i] = 0;
        }
    }
    endResetModel();
}

bool TableModel::isValid(const QModelIndex &index) const
{
    return (index.isValid()
            && index.row() < _imp->rowCount
            && index.column() < _imp->horizontalHeaderItems.count());
}

void TableModel::itemChanged(TableItem *item)
{
    if (!item) {
        return;
    }
    
    if (item->flags() & ItemIsHeaderItem) {
        int column = _imp->horizontalHeaderItems.indexOf(item);
        if (column >= 0)
            emit headerDataChanged(Qt::Horizontal, column, column);
        
    } else {
        QModelIndex idx = index(item);
        if (idx.isValid()) {
            emit dataChanged(idx, idx);
        }
    }
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsDropEnabled;
    if (TableItem *itm = item(index))
        return itm->flags();
    return (Qt::ItemIsEditable
            |Qt::ItemIsSelectable
            |Qt::ItemIsUserCheckable
            |Qt::ItemIsEnabled
            |Qt::ItemIsDragEnabled
            |Qt::ItemIsDropEnabled);
}
////////////////TableViewPrivae

struct TableViewPrivate
{
    TableModel* model;
    
    TableViewPrivate()
    : model(0)
    {
        
    }
};


/////////////// TableView

TableView::TableView(QWidget* parent)
: QTreeView(parent)
, _imp(new TableViewPrivate())
{
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setRootIsDecorated(false);
    setItemsExpandable(false);
    setSortingEnabled(true);
    
    ///The table model here doesn't support sorting
    setSortingEnabled(false);
    
    header()->setStretchLastSection(false);
    setTextElideMode(Qt::ElideMiddle);
    setEditTriggers(QAbstractItemView::EditKeyPressed | QAbstractItemView::SelectedClicked | QAbstractItemView::DoubleClicked);
    setContextMenuPolicy(Qt::CustomContextMenu);
    setDragDropMode(QAbstractItemView::NoDragDrop);
    setAttribute(Qt::WA_MacShowFocusRect,0);
    setAcceptDrops(true);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    
}

TableView::~TableView()
{
    
}

void TableView::setTableModel(TableModel* model)
{
    _imp->model = model;
    setModel(model);
}

void TableView::setRowCount(int rows)
{
    _imp->model->setRowCount(rows);
}

int TableView::rowCount() const
{
    return _imp->model->rowCount();
}

void TableView::setColumnCount(int columns)
{
    _imp->model->setColumnCount(columns);
}

int TableView::columnCount() const
{
    return _imp->model->columnCount();
}

int TableView::row(const TableItem *item) const
{
    return _imp->model->index(item).row();
}

int TableView::column(const TableItem *item) const
{
    return _imp->model->index(item).column();
}

TableItem *TableView::item(int row, int column) const
{
    return _imp->model->item(row, column);
}

void TableView::setItem(int row, int column, TableItem *item)
{
    if (item) {
        if (item->view != 0) {
            qWarning("TableView: cannot insert an item that is already owned by another TableView");
        } else {
            item->view = this;
            _imp->model->setItem(row, column, item);
        }
    } else {
        delete takeItem(row, column);
    }
}

TableItem *TableView::takeItem(int row, int column)
{
    TableItem* item =  _imp->model->takeItem(row, column);
    if (item)
        item->view = 0;
    return item;
}

void TableView::setHorizontalHeaderLabels(const QStringList &labels)
{
    TableItem *item = 0;
    for (int i = 0; i < _imp->model->columnCount() && i < labels.count(); ++i) {
        item = _imp->model->horizontalHeaderItem(i);
        if (!item) {
            item = _imp->model->createItem();
            setHorizontalHeaderItem(i, item);
        }
        item->setText(labels.at(i));
    }
}

TableItem *TableView::horizontalHeaderItem(int column) const
{
    return _imp->model->horizontalHeaderItem(column);
}

TableItem *TableView::takeHorizontalHeaderItem(int column)
{
    TableItem *itm = _imp->model->takeHorizontalHeaderItem(column);
    if (itm)
        itm->view = 0;
    return itm;
}

void TableView::setHorizontalHeaderItem(int column, TableItem *item)
{
    if (item) {
        item->view = this;
        _imp->model->setHorizontalHeaderItem(column, item);
    } else {
        delete takeHorizontalHeaderItem(column);
    }
}

void TableView::editItem(TableItem *item)
{
    if (!item)
        return;
    edit(_imp->model->index(item));
}

void TableView::openPersistentEditor(TableItem *item)
{
    if (!item)
        return;
    QModelIndex index = _imp->model->index(item);
    QAbstractItemView::openPersistentEditor(index);
}

void TableView::closePersistentEditor(TableItem *item)
{
    if (!item)
        return;
    QModelIndex index = _imp->model->index(item);
    QAbstractItemView::closePersistentEditor(index);
}

QWidget *TableView::cellWidget(int row, int column) const
{
    QModelIndex index = model()->index(row, column, QModelIndex());
    return QAbstractItemView::indexWidget(index);
}

void TableView::setCellWidget(int row, int column, QWidget *widget)
{
    QModelIndex index = model()->index(row, column, QModelIndex());
    QAbstractItemView::setIndexWidget(index, widget);
}

void TableView::removeCellWidget(int row, int column)
{
    setCellWidget(row, column, 0);
}

TableItem *TableView::itemAt(const QPoint &p) const
{
    return _imp->model->item(indexAt(p));
}

inline TableItem *TableView::itemAt(int x, int y) const
{
    return itemAt(QPoint(x,y));
}

QRect TableView::visualItemRect(const TableItem *item) const
{
    if (!item)
        return QRect();
    QModelIndex index = _imp->model->index(const_cast<TableItem*>(item));
    assert(index.isValid());
    return visualRect(index);
}

void TableView::mousePressEvent(QMouseEvent* event)
{
    TableItem* item = itemAt(event->pos());
    if (!item) {
        selectionModel()->clear();
    } else {
        QTreeView::mousePressEvent(event);
    }
}

void TableView::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace) {
        emit deleteKeyPressed();
        event->accept();
    }
    
    QTreeView::keyPressEvent(event);
}