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

#include <math.h>
#include <stdio.h>
#include <QString>
#include <QApplication>
#include "Toroid.h"

// Handy macro to return number of elements in an array
#define arraysizeof(x) sizeof(x)/sizeof(x[0])

// Data below comes from the 2006 ARRL Radio Amateur's Handbook p7.5
const char *Toroid::m_powderedIronCoreSizes[] = {
    "T-12",  "T-16", "T-20", "T-25", "T-30", "T-37", "T-44", "T-50",
    "T-68", "T-80", "T-94", "T-106", "T-130", "T-157", "T-184", "T-200"
};

const char *Toroid::m_ferriteCoreSizes[] = {
    "FT-23", "FT-37", "FT-50", "FT-82", "FT-114"
};

const char *Toroid::m_powderedIronCoreMixes[] = {
    "26", "3", "15", "1", "2", "7", "6", "10", "12", "17", "0"
        };

const char *Toroid::m_ferriteCoreMixes[] = {
    "63/37", "61", "43", "77(72)", "J(75)"
};

const char *Toroid::m_powderedIronMixColours[] = {
    "Yellow/white", "Gray", "Red/white", "Blue", "Red", "White",
    "Yellow", "Black", "Green/white", "Blue/yellow", "Tan"
};

const int Toroid::m_powderedIronMixMuValues[] = { 75, 35, 25, 20, 10, 9, 8, 6, 4, 4, 1 };

const int Toroid::m_ferriteMixMuValues[] = { 40, 125, 850, 2000, 5000 };

const char *Toroid::m_powderedIronFrequencyRanges[] = {
    "DC-1 MHz", "0.05-0.50 MHz", "0.10-2 MHz", "0.5-5 MHz", "2-30 MHz", "3-35 MHz",
    "10-50 MHz", "30-100 MHz", "50-200 MHz", "40-180 MHz", "100-300 MHz"
};

const int na = -1;

const double Toroid::m_powderedIronAlTable[] = {
    na,   60,  50,  48,  20,  18,  17,  12, 7.5,  7.5,  3.0,
    145,  61,  55,  44,  22,  na,  19,  13, 8.0,  8.0,  3.0,
    180,  76,  65,  52,  27,  24,  22,  16, 10.0, 10.0, 3.5,
    235,  100, 85,  70,  34,  29,  27,  19, 12.0, 12.0, 4.5,
    325,  140, 93,  85,  43,  37,  36,  25, 16.0, 16.0, 6.0,
    275,  120, 90,  80,  40,  32,  30,  25, 15.0, 15.0, 4.9,
    360,  180, 160, 105, 52,  46,  42,  33, 18.5, 18.5, 6.5,
    320,  175, 135, 100, 49,  43,  40,  31, 18.0, 18.0, 6.4,
    420,  195, 180, 115, 57,  52,  47,  32, 21.0, 21.0, 7.5,
    450,  180, 170, 115, 55,  50,  45,  32, 22.0, 22.0, 8.5,
    590,  248, 200, 160, 84,  na,  70,  58, 32.0, na,   10.6,
    900,  450, 345, 325, 135, 133, 116, na, na,   na,   19.0,
    785,  350, 250, 200, 110, 103, 96,  na, na,   na,   15.0,
    870,  420, 360, 320, 140, na,  115, na, na,   na,   na,
    1640, 720, na,  500, 240, na,  195, na, na,   na,   na,
    895,  425, na,  250, 120, 105, 100, na, na,   na,   na
};

const double Toroid::m_ferriteAlTable[] = {
    7.9,  24.8, 188.0, 396,  980,
    19.7, 55.3, 420.0, 884,  2196,
    22.0, 68.0, 523.0, 1100, 2715,
    22.4, 73.3, 557.0, 1170, na,
    25.4, 79.3, 603.0, 1270, 3170
};

const char *Toroid::m_wireSizeTableCoreSizes[] = {
    "T-200", "T-130", "T-106", "T-94", "T-80",
    "T-68", "T-50", "T-37", "T-25", "T-12"
};

const char *Toroid::m_wireSizeTableWireSizes[] = {
    "10", "12", "14", "16", "18",
    "20", "22", "24", "26", "28",
    "30", "32", "34", "36", "38", "40"
};

const int Toroid::m_wireSizeTable[] = {
    33,  20,  12,  12,  10,  6,   4,   1,   na,  na,
    43,  25,  16,  16,  14,  9,   6,   3,   na,  na,
    54,  32,  21,  21,  18,  13,  8,   5,   1,   na,
    69,  41,  28,  28,  24,  17,  13,  7,   2,   na,
    88,  53,  37,  37,  32,  23,  18,  10,  4,   1,
    111, 67,  47,  47,  41,  29,  23,  14,  6,   1,
    140, 86,  60,  60,  53,  38,  30,  19,  9,   2,
    177, 109, 77,  77,  67,  49,  39,  25,  13,  4,
    223, 137, 97,  97,  85,  63,  50,  33,  17,  7,
    281, 173, 123, 123, 108, 80,  64,  42,  23,  9,
    355, 217, 154, 154, 136, 101, 81,  54,  29,  13,
    439, 272, 194, 194, 171, 127, 103, 68,  38,  17,
    557, 346, 247, 247, 218, 162, 132, 88,  49,  23,
    683, 424, 304, 304, 268, 199, 162, 108, 62,  30,
    875, 544, 389, 389, 344, 256, 209, 140, 80,  39,
    1103,687, 492, 492, 434, 324, 264, 178, 102, 51
};


Toroid::Toroid()
{
    setCoreType(powderedIron);
    initializeAlTable();
}


Toroid::~Toroid()
{
}


// Return list of core types/sizes.
QStringList Toroid::coreTypes()
{
    static QStringList powderedIronTypes;
    static QStringList ferriteTypes;

    if (powderedIronTypes.isEmpty()) {
        for (unsigned int i = 0; i < arraysizeof(m_powderedIronCoreSizes); ++i) {
            powderedIronTypes << m_powderedIronCoreSizes[i];
        }
    }

    if (ferriteTypes.isEmpty()) {
        for (unsigned int i = 0; i < arraysizeof(m_ferriteCoreSizes); ++i) {
            ferriteTypes << m_ferriteCoreSizes[i];
        }
    }

    if (coreType() == powderedIron) {
        return powderedIronTypes;
    } else {
        return ferriteTypes;
    }
}


// Return list of core materials/mixes.
QStringList Toroid::coreMaterials()
{
    static QStringList powderedIronMaterials;
    static QStringList ferriteMaterials;

    if (powderedIronMaterials.isEmpty()) {
        for (unsigned int i = 0; i < arraysizeof(m_powderedIronCoreMixes); ++i) {
            powderedIronMaterials << m_powderedIronCoreMixes[i];
        }
    }

    if (ferriteMaterials.isEmpty()) {
        for (unsigned int i = 0; i < arraysizeof(m_ferriteCoreMixes); ++i) {
            ferriteMaterials << m_ferriteCoreMixes[i];
        }
    }

    if (coreType() == powderedIron) {
        return powderedIronMaterials;
    } else {
        return ferriteMaterials;
    }
}


// Calculate number of turns for a given inductance and Al.
double Toroid::calculateTurns(double inductance, double al)
{
    double multiplier;

    if (coreType() == powderedIron) {
        multiplier = 100;
    } else {
        multiplier = 1000;
    }

    if (al != 0) {
        return multiplier * sqrt(inductance / al);
    } else {
        return 0;
    }
}


// Calculate actual inductance based on number of turns and Al.
double Toroid::calculateInductance(double turns, double al)
{
    double divisor;

    if (coreType() == powderedIron) {
        divisor = 10000;
    } else {
        divisor = 100000;
    }

    return al * turns * turns / divisor;
}


// Calculate Al value given core size and mix.
double Toroid::calculateAl(QString coreSize, QString mix)
{
    QString key = coreSize + "-" + mix;
    if (m_aLValue.contains(key))
        return m_aLValue[key];
    else
        return -1;
}


// Initialize the table of Al values for the given type of toroid.
void Toroid::initializeAlTable()
{
    int i = 0;
    m_aLValue.clear();

    Q_ASSERT(arraysizeof(m_powderedIronAlTable) == arraysizeof(m_powderedIronCoreSizes) * arraysizeof(m_powderedIronCoreMixes));
    Q_ASSERT(arraysizeof(m_ferriteAlTable) == arraysizeof(m_ferriteCoreSizes) * arraysizeof(m_ferriteCoreMixes));

    for (unsigned int s = 0; s < arraysizeof(m_powderedIronCoreSizes); ++s) {
        for (unsigned int m = 0; m < arraysizeof(m_powderedIronCoreMixes); ++m) {
            QString key = m_powderedIronCoreSizes[s] + QString("-") + m_powderedIronCoreMixes[m];
            m_aLValue[key] = m_powderedIronAlTable[i++];
        }
    }

    i = 0;
    for (unsigned int s = 0; s < arraysizeof(m_ferriteCoreSizes); ++s) {
        for (unsigned int m = 0; m < arraysizeof(m_ferriteCoreMixes); ++m) {
            QString key = m_ferriteCoreSizes[s] + QString("-") + m_ferriteCoreMixes[m];
            m_aLValue[key] = m_ferriteAlTable[i++];
        }
    }
}


// Return mix colour.
QString Toroid::mixColour(QString mix)
{
    // Only valid for powdered iron toroids.
    if (coreType() != powderedIron) {
        return QApplication::tr("NA");
    }

    Q_ASSERT(arraysizeof(m_powderedIronCoreMixes) == arraysizeof(m_powderedIronMixColours));

    for (unsigned int i = 0; i < arraysizeof(m_powderedIronCoreMixes); ++i) {
        if (mix == m_powderedIronCoreMixes[i])
            return m_powderedIronMixColours[i];
    }
    return QApplication::tr("Unknown");
}


// Return mu value for a mix.
int Toroid::mixMuValue(QString mix)
{
    if (coreType() == powderedIron) {
        Q_ASSERT(arraysizeof(m_powderedIronCoreMixes) == arraysizeof(m_powderedIronMixMuValues));
        for (unsigned int i = 0; i < arraysizeof(m_powderedIronCoreMixes); ++i) {
            if (mix == m_powderedIronCoreMixes[i])
                return m_powderedIronMixMuValues[i];
        }
    } else {
        Q_ASSERT(arraysizeof(m_ferriteCoreMixes) == arraysizeof(m_ferriteMixMuValues));
        for (unsigned int i = 0; i < arraysizeof(m_ferriteCoreMixes); ++i) {
            if (mix == m_ferriteCoreMixes[i]) {
                return m_ferriteMixMuValues[i];
            }
        }
    }
    return 0;
}


// Return frequency range for a core mix.
QString Toroid::frequencyRange(QString mix)
{
    // Only valid for powdered iron toroids.
    if (coreType() != powderedIron) {
        return QApplication::tr("NA");
    }

    Q_ASSERT(arraysizeof(m_powderedIronCoreMixes) == arraysizeof(m_powderedIronFrequencyRanges));
    for (unsigned int i = 0; i < arraysizeof(m_powderedIronCoreMixes); ++i) {
        if (mix == m_powderedIronCoreMixes[i])
            return m_powderedIronFrequencyRanges[i];
    }
    return 0;
}


// Returns largest wire gauge that will fit on given core size with number of turns.
// We don't have data for all core sizes.
int Toroid::maximumWireGauge(QString coreSize, int turns)
{
    // Only valid for powdered iron toroids.
    if (coreType() != powderedIron) {
        return -1;
    }

    static QStringList coreSizes;
    static QStringList wireSizes;

    if (coreSizes.isEmpty() || wireSizes.isEmpty()) {
        for (unsigned int i = 0; i < arraysizeof(m_wireSizeTableCoreSizes); ++i) {
            coreSizes << m_wireSizeTableCoreSizes[i];
        }
        for (unsigned int i = 0; i < arraysizeof(m_wireSizeTableWireSizes); ++i) {
            wireSizes << m_wireSizeTableWireSizes[i];
        }

        Q_ASSERT(arraysizeof(m_wireSizeTable) == arraysizeof(m_wireSizeTableCoreSizes) * arraysizeof(m_wireSizeTableWireSizes));
        int i = 0;
        foreach(QString wireSize, wireSizes) {
            foreach(QString coreSize, coreSizes) {
                QString key = coreSize + "," + wireSize;
                Toroid::m_wireSizes[key] = m_wireSizeTable[i];
                ++i;
            }
        }
    }

    foreach (QString wireSize, wireSizes) {
        QString key = coreSize + "," + wireSize;
        if (Toroid::m_wireSizes.contains(key) && Toroid::m_wireSizes[key] >= turns) {
            return wireSize.toInt();
        }
    }

    // No data or no core large enough.
    return -1;
}


// Display a table of calculations. This can be compared to previous data for regression testing.
void Toroid::printTestData()
{
    setCoreType(ferrite);
    foreach (QString coreType, coreTypes()) {
        foreach (QString coreMaterial, coreMaterials()) {
            for (double L = 1; L < 100; L++) {
                double al = calculateAl(coreType, coreMaterial);
                if (al != -1) {
                    long turns = calculateTurns(L, al);
                    printf("%s %s %4.2f mH %ld turns\n", qPrintable(coreType), qPrintable(coreMaterial), L, turns);
                }
            }
        }
    }

    foreach (QString coreType, coreTypes()) {
        foreach (QString coreMaterial, coreMaterials()) {
            for (long turns = 10; turns < 1000; turns += 10) {
                double al = calculateAl(coreType, coreMaterial);
                if (al != -1) {
                    double L = calculateInductance(turns, al);
                    printf("%s %s %4.2f mH %ld turns\n", qPrintable(coreType), qPrintable(coreMaterial), L, turns);
                }
            }
        }
    }

    setCoreType(powderedIron);
    foreach (QString coreType, coreTypes()) {
        foreach (QString coreMaterial, coreMaterials()) {
            for (double L = 1; L < 100; L++) {
                double al = calculateAl(coreType, coreMaterial);
                if (al != -1) {
                    long turns = calculateTurns(L, al);
                    printf("%s %s %4.2f uH %ld turns\n", qPrintable(coreType), qPrintable(coreMaterial), L, turns);
                }
            }
        }
    }

    foreach (QString coreType, coreTypes()) {
        foreach (QString coreMaterial, coreMaterials()) {
            for (long turns = 10; turns < 1000; turns += 10) {
                double al = calculateAl(coreType, coreMaterial);
                if (al != -1) {
                    double L = calculateInductance(turns, al);
                    printf("%s %s %4.2f uH %ld turns\n", qPrintable(coreType), qPrintable(coreMaterial), L, turns);
                }
            }
        }
    }
}
