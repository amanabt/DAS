/***************************************************************************
**                                                                        **
**  QCustomPlot, an easy to use, modern plotting widget for Qt            **
**  Copyright (C) 2011-2016 Emanuel Eichhammer                            **
**                                                                        **
**  This program is free software: you can redistribute it and/or modify  **
**  it under the terms of the GNU General Public License as published by  **
**  the Free Software Foundation, either version 3 of the License, or     **
**  (at your option) any later version.                                   **
**                                                                        **
**  This program is distributed in the hope that it will be useful,       **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of        **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         **
**  GNU General Public License for more details.                          **
**                                                                        **
**  You should have received a copy of the GNU General Public License     **
**  along with this program.  If not, see http://www.gnu.org/licenses/.   **
**                                                                        **
****************************************************************************
**           Author: Emanuel Eichhammer                                   **
**  Website/Contact: http://www.qcustomplot.com/                          **
**             Date: 13.09.16                                             **
**          Version: 2.0.0-beta                                           **
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextDocument>
#include <QFileDialog>
#include "qcpdocumentobject.h"
#include <vector>
#include <utility>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	typedef double data_type;
	
public:
	explicit MainWindow (QWidget *parent = 0);
	~MainWindow ();
	
public:
	void add_datapoint (double x, double y)
	{ _x.push_back (x); _y.push_back (y); }
	void clear_dataset (void)
    { _x.clear(); _y.clear(); }
	void update_plot (void) const;

private:
	void init_plot (void);

private slots:
	void on_actionInsert_Plot_triggered (void);
	void on_actionSave_Document_triggered (void);

private:
	Ui::MainWindow *ui;
	QVector<data_type> _x, _y;
};

#endif // MAINWINDOW_H
