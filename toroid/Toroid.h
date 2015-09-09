#ifndef TOROID_H
#define TOROID_H

/*

 Class to encapsulate toroid calculations.

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

#include <QString>
#include <QStringList>
#include <QHash>

class Toroid
{
public:
    typedef enum { powderedIron, ferrite } core_t;
    Toroid();
    ~Toroid();
    QStringList coreTypes();
    QStringList coreMaterials();
    double calculateTurns(double inductance, double al);
    double calculateInductance(double turns, double al);
    double calculateAl(QString coreSize, QString mix);
    QString mixColour(QString mix);
    int mixMuValue(QString mix);
    QString frequencyRange(QString mix);
    int maximumWireGauge(QString coreSize, int turns);
    core_t coreType() { return m_coreType; }
    void setCoreType(core_t newType) { m_coreType = newType; }
    void printTestData();

private:
    void initializeAlTable();
    core_t m_coreType;
    static const char *m_powderedIronCoreSizes[];
    static const char *m_ferriteCoreSizes[];
    static const char *m_powderedIronCoreMixes[];
    static const char *m_ferriteCoreMixes[];
    static const char *m_powderedIronMixColours[];
    static const int m_powderedIronMixMuValues[];
    static const int m_ferriteMixMuValues[];
    static const char *m_powderedIronFrequencyRanges[];
    static const double m_powderedIronAlTable[];
    static const double m_ferriteAlTable[];
    static const int m_wireSizeTable[];
    static const char *m_wireSizeTableCoreSizes[];
    static const char *m_wireSizeTableWireSizes[];
    QHash<QString, double> m_aLValue;
    QHash<QString, int> m_wireSizes;
};

#endif // TOROID_H
