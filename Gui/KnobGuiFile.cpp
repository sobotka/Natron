//  Natron
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
/*
 *Created by Alexandre GAUTHIER-FOICHAT on 6/1/2012.
 *contact: immarespond at gmail dot com
 *
 */

#include "Gui/KnobGuiFile.h"

#include <QtGui/QLabel>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>

#include "Global/AppManager.h"
#include "Engine/Settings.h"
#include "Engine/KnobFile.h"

#include "Gui/Button.h"
#include "Gui/SequenceFileDialog.h"
#include "Gui/LineEdit.h"
#include "Gui/KnobUndoCommand.h"

using namespace Natron;

//===========================FILE_KNOB_GUI=====================================
File_KnobGui::File_KnobGui(Knob *knob, DockablePanel *container)
: KnobGui(knob, container)
{
    File_Knob *fileKnob = dynamic_cast<File_Knob *>(knob);
    QObject::connect(fileKnob, SIGNAL(shouldOpenFile()), this, SLOT(open_file()));
}

File_KnobGui::~File_KnobGui()
{
    delete _lineEdit;
    delete _descriptionLabel;
    delete _openFileButton;
}

void File_KnobGui::createWidget(QGridLayout *layout, int row)
{

    _descriptionLabel = new QLabel(QString(QString(getKnob()->getDescription().c_str()) + ":"), layout->parentWidget());
    _descriptionLabel->setToolTip(getKnob()->getHintToolTip().c_str());
    layout->addWidget(_descriptionLabel, row, 0, Qt::AlignRight);

    _lineEdit = new LineEdit(layout->parentWidget());
    _lineEdit->setPlaceholderText("File path...");
    _lineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    _lineEdit->setToolTip(getKnob()->getHintToolTip().c_str());
    QObject::connect(_lineEdit, SIGNAL(returnPressed()), this, SLOT(onReturnPressed()));

    _openFileButton = new Button(layout->parentWidget());

    QPixmap pix;
    appPTR->getIcon(Natron::OPEN_FILE_PIXMAP, &pix);
    _openFileButton->setIcon(QIcon(pix));
    _openFileButton->setFixedSize(20, 20);
    QObject::connect(_openFileButton, SIGNAL(clicked()), this, SLOT(open_file()));


    QWidget *container = new QWidget(layout->parentWidget());
    QHBoxLayout *containerLayout = new QHBoxLayout(container);
    container->setLayout(containerLayout);
    containerLayout->setContentsMargins(0, 0, 0, 0);
    containerLayout->addWidget(_lineEdit);
    containerLayout->addWidget(_openFileButton);

    layout->addWidget(container, row, 1);
}

void File_KnobGui::updateGUI(int /*dimension*/, const Variant &variant)
{
    std::string pattern = SequenceFileDialog::patternFromFilesList(variant.toStringList()).toStdString();
    _lineEdit->setText(pattern.c_str());
}

void File_KnobGui::open_file()
{
    QStringList filesList;
    std::vector<std::string> filters = appPTR->getCurrentSettings()._readersSettings.supportedFileTypes();

    SequenceFileDialog dialog(_lineEdit->parentWidget(), filters, true, SequenceFileDialog::OPEN_DIALOG, _lastOpened.toStdString());
    if (dialog.exec()) {
        filesList = dialog.selectedFiles();
    }
    if (!filesList.isEmpty()) {
        updateLastOpened(filesList.at(0));
        pushUndoCommand(new KnobUndoCommand(this, 0, getKnob()->getValue(0), Variant(filesList)));
    }
}

void File_KnobGui::onReturnPressed()
{
    QString str = _lineEdit->text();
    if (str.isEmpty()) {
        return;
    }
    QStringList newList = SequenceFileDialog::filesListFromPattern(str);
    if (newList.isEmpty()) {
        return;
    }
    pushUndoCommand(new KnobUndoCommand(this, 0, getKnob()->getValue(0), Variant(newList)));
}

void File_KnobGui::updateLastOpened(const QString &str)
{
    QString withoutPath = SequenceFileDialog::removePath(str);
    int pos = str.indexOf(withoutPath);
    _lastOpened = str.left(pos);
}

void File_KnobGui::_hide()
{
    _openFileButton->hide();
    _descriptionLabel->hide();
    _lineEdit->hide();
}

void File_KnobGui::_show()
{
    _openFileButton->show();
    _descriptionLabel->show();
    _lineEdit->show();
}

void File_KnobGui::addToLayout(QHBoxLayout *layout)
{
    layout->addWidget(_descriptionLabel);
    layout->addWidget(_lineEdit);
    layout->addWidget(_openFileButton);
}


//============================OUTPUT_FILE_KNOB_GUI====================================
OutputFile_KnobGui::OutputFile_KnobGui(Knob *knob, DockablePanel *container)
: KnobGui(knob, container)
{
    OutputFile_Knob *fileKnob = dynamic_cast<OutputFile_Knob *>(knob);
    QObject::connect(fileKnob, SIGNAL(shouldOpenFile()), this, SLOT(open_file()));
}

OutputFile_KnobGui::~OutputFile_KnobGui()
{
    delete _descriptionLabel;
    delete _lineEdit;
    delete _openFileButton;
}

void OutputFile_KnobGui::createWidget(QGridLayout *layout, int row)
{
    _descriptionLabel = new QLabel(QString(QString(getKnob()->getDescription().c_str()) + ":"), layout->parentWidget());
    _descriptionLabel->setToolTip(getKnob()->getHintToolTip().c_str());
    layout->addWidget(_descriptionLabel, row, 0, Qt::AlignRight);

    _lineEdit = new LineEdit(layout->parentWidget());
    _lineEdit->setPlaceholderText(QString("File path..."));
    _lineEdit->setToolTip(getKnob()->getHintToolTip().c_str());
    _lineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    _openFileButton = new Button(layout->parentWidget());

    QPixmap pix;
    appPTR->getIcon(Natron::OPEN_FILE_PIXMAP, &pix);
    _openFileButton->setIcon(QIcon(pix));
    _openFileButton->setFixedSize(20, 20);
    QObject::connect(_openFileButton, SIGNAL(clicked()), this, SLOT(open_file()));


    QWidget *container = new QWidget(layout->parentWidget());
    QHBoxLayout *containerLayout = new QHBoxLayout(container);
    container->setLayout(containerLayout);
    containerLayout->setContentsMargins(0, 0, 0, 0);

    containerLayout->addWidget(_lineEdit);
    containerLayout->addWidget(_openFileButton);

    layout->addWidget(container, row, 1);
}

void OutputFile_KnobGui::updateGUI(int /*dimension*/, const Variant &variant)
{
    _lineEdit->setText(variant.toString());
}

void OutputFile_KnobGui::open_file()
{
    std::vector<std::string> filters = appPTR->getCurrentSettings()._readersSettings.supportedFileTypes();
    SequenceFileDialog dialog(_lineEdit->parentWidget(), filters, true, SequenceFileDialog::SAVE_DIALOG, _lastOpened.toStdString());
    if (dialog.exec()) {
        QString oldPattern = _lineEdit->text();
        QString newPattern = dialog.filesToSave();
        updateLastOpened(SequenceFileDialog::removePath(oldPattern));

        pushUndoCommand(new KnobUndoCommand(this, 0, getKnob()->getValue(0), Variant(newPattern)));
    }
}

void OutputFile_KnobGui::onReturnPressed()
{
    QString newPattern = _lineEdit->text();

    pushUndoCommand(new KnobUndoCommand(this, 0, getKnob()->getValue(0), Variant(newPattern)));
}

void OutputFile_KnobGui::updateLastOpened(const QString &str)
{
    QString withoutPath = SequenceFileDialog::removePath(str);
    int pos = str.indexOf(withoutPath);
    _lastOpened = str.left(pos);
}

void OutputFile_KnobGui::_hide()
{
    _openFileButton->hide();
    _descriptionLabel->hide();
    _lineEdit->hide();
}

void OutputFile_KnobGui::_show()
{
    _openFileButton->show();
    _descriptionLabel->show();
    _lineEdit->show();
}

void OutputFile_KnobGui::addToLayout(QHBoxLayout *layout)
{
    layout->addWidget(_descriptionLabel);
    layout->addWidget(_lineEdit);
    layout->addWidget(_openFileButton);
}