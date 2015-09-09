#ifndef TOROIDWINDOW_H
#define TOROIDWINDOW_H

/*

Jeff Tranter <tranter@pobox.com>
Copyright (C) 2009.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <QStringList>
#include <QHash>
#include <QtGui/QMainWindow>
#include "Toroid.h"

namespace Ui
{
    class ToroidWindow;
}

class ToroidWindow : public QMainWindow
{
    Q_OBJECT

public:
    ToroidWindow(QWidget *parent = 0);
    ~ToroidWindow();
    bool powderedIronMode();
    bool ferriteMode();

public slots:
    void updateValues();
    void toggleMode();
    void about();

private:
    Ui::ToroidWindow *ui;
    Toroid *toroid;
};

#endif // TOROIDWINDOW_H
