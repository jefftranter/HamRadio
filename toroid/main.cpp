/*

Jeff Tranter <tranter@pobox.com>
Copyright (C) 2009-2015.

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

#include <QApplication>
#include <QDebug>
#include "ToroidWindow.h"

int main(int argc, char *argv[])
{
    // If invoked with --test option, generates test data end exits.
    if ((argc == 2) && !strcmp(argv[1], "--test")) {
        Toroid *toroid = new Toroid;
        toroid->printTestData();
        delete toroid;
        return 0;
    }

    QApplication a(argc, argv);
    ToroidWindow w;
    w.show();
    return a.exec();
}
