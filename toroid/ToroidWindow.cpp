/*

Toroid design program.
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
#include <QMessageBox>
#include <QString>
#include "ToroidWindow.h"
#include "ui_toroid.h"


ToroidWindow::ToroidWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ToroidWindow)
{
    toroid = new Toroid;
    toroid->setCoreType(Toroid::powderedIron);

    setWindowIcon(QIcon(":/toroid.png"));

    ui->setupUi(this);
    ui->powderedIronButton->setChecked(true);
    ui->coreTypeComboBox->addItems(toroid->coreTypes());
    ui->coreMaterialComboBox->addItems(toroid->coreMaterials());

    // Use reasonable initial defaults
    ui->coreMaterialComboBox->setCurrentIndex(6);
    ui->inductanceSpinBox->setValue(10.0);

    connect(ui->powderedIronButton, SIGNAL(toggled(bool)), SLOT(toggleMode()));
    connect(ui->coreTypeComboBox, SIGNAL(currentIndexChanged(int)), SLOT(updateValues()));
    connect(ui->coreMaterialComboBox, SIGNAL(currentIndexChanged(int)), SLOT(updateValues()));
    connect(ui->inductanceSpinBox, SIGNAL(valueChanged(double)), SLOT(updateValues()));
    connect(ui->actionQuit, SIGNAL(triggered()), QApplication::instance(), SLOT(quit()));
    connect(ui->actionAbout, SIGNAL(triggered()), SLOT(about()));

    updateValues();
}


ToroidWindow::~ToroidWindow()
{
    delete ui;
    delete toroid;
}


// Return true if we are in powdered iron mode, false if ferrite mode.
bool ToroidWindow::powderedIronMode()
{
    return ui->powderedIronButton->isChecked();
}


// Return true if we are in ferrite mode, false if powered iron mode.
bool ToroidWindow::ferriteMode()
{
    return !powderedIronMode();
}


// Called when Iron Core / Ferrite mode changes.
void ToroidWindow::toggleMode()
{
    if (powderedIronMode()) {
        toroid->setCoreType(Toroid::powderedIron);
        ui->inductanceSpinBox->setSuffix(tr(" uH"));
    } else {
        toroid->setCoreType(Toroid::ferrite);
        ui->inductanceSpinBox->setSuffix(tr(" mH"));
    }
    ui->coreTypeComboBox->clear();
    ui->coreTypeComboBox->addItems(toroid->coreTypes());
    ui->coreMaterialComboBox->clear();
    ui->coreMaterialComboBox->addItems(toroid->coreMaterials());
    updateValues();
}


// Display About dialog
void ToroidWindow::about()
{
    QMessageBox::about(
            this,
            tr("About Toroid Designer"),
            tr(
    "Toroidal inductor design program version 0.3\n"
    "By Jeff Tranter <tranter@pobox.com>\n"
    "Copyright (C) 2009-2015 Jeff Tranter\n"
    "\n"
    "This program is free software: you can redistribute it and/or modify\n"
    "it under the terms of the GNU General Public License as published by\n"
    "the Free Software Foundation, either version 3 of the License, or\n"
    "(at your option) any later version.\n"
    "\n"
    "This program is distributed in the hope that it will be useful,\n"
    "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
    "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
    "GNU General Public License for more details.\n"
    "\n"
    "You should have received a copy of the GNU General Public License\n"
    "along with this program.  If not, see <http://www.gnu.org/licenses/>."
));

}


// Calculate design values and display in labels.
void ToroidWindow::updateValues()
{
    QString coreSize = ui->coreTypeComboBox->currentText();
    QString mix = ui->coreMaterialComboBox->currentText();
    double inductance = ui->inductanceSpinBox->value();
    double al = toroid->calculateAl(coreSize, mix);
    double turns = floor(toroid->calculateTurns(inductance, al) + 0.5);
    double actualInductance = toroid->calculateInductance(turns, al);

    QString units, alUnits;
    if (powderedIronMode()) {
        units = tr("uH");
        alUnits = tr("uH/100 turns");
    } else {
        units = tr("mH");
        alUnits = tr("mH/1000 turns");
    }

    if (al != -1) {
        ui->designInductanceLabel->setText(tr("Calculated inductance: %1 %2").arg(actualInductance).arg(units));
        if (powderedIronMode())
            ui->designCoreTypeLabel->setText(tr("Core type: Powdered Iron %1-%2 (%3)").arg(coreSize, mix, toroid->mixColour(mix)));
        else
            ui->designCoreTypeLabel->setText(tr("Core type: Ferrite %1-%2").arg(coreSize, mix));
        ui->designTurnsLabel->setText(tr("Turns: %1").arg(turns));
        if (toroid->maximumWireGauge(coreSize, turns) != -1)
            ui->designWireGaugeLabel->setText(tr("Maximum wire gauge: %1").arg(toroid->maximumWireGauge(coreSize, turns)));
        else
            ui->designWireGaugeLabel->setText(tr("Maximum wire gauge: unknown"));
        ui->designFreqRangeLabel->setText(tr("Frequency range: %1").arg(toroid->frequencyRange(mix)));
        ui->designMuLabel->setText(tr("Mu factor: %1").arg(toroid->mixMuValue(mix)));
        ui->designAlLabel->setText(tr("Al: %1%2").arg(al).arg(alUnits));
        statusBar()->clearMessage();
    } else {
        ui->designInductanceLabel->setText(tr("Calculated inductance: NA"));
        ui->designCoreTypeLabel->setText(tr("Core type: NA"));
        ui->designTurnsLabel->setText(tr("Turns: NA"));
        ui->designWireGaugeLabel->setText(tr("Maximum wire gauge: NA"));
        ui->designFreqRangeLabel->setText(tr("Frequency range: NA"));
        ui->designMuLabel->setText(tr("Mu factor: NA"));
        ui->designAlLabel->setText(tr("Al: NA"));
        statusBar()->showMessage(tr("Invalid core type / material."));
    }
}
