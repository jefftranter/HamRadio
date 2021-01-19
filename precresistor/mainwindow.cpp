#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <math.h>
#include <QString>
#include <QDebug>


// List of standard resistor values.
// See https://eepower.com/resistor-guide/resistor-standards-and-codes/resistor-values/
const int e6Series[] = { 10, 15, 22, 33, 47, 68 };

const int e12Series[] = { 10, 12, 15, 18, 22, 27, 33, 39, 47, 56, 68, 82 };

const int e24Series[] = { 10, 11, 12, 13, 15, 16, 18, 20, 22, 24, 27, 30,
                          33, 36, 39, 43, 47, 51, 56, 62, 68, 75, 82, 91 };

const int e24Decades[] = { 0, 1, 2, 3, 4, 5, 6 };

const int e48Series[] = { 100, 105, 110, 115, 121, 127, 133, 140, 147, 154, 162, 169,
                          178, 187, 196, 205, 215, 226, 237, 249, 261, 274, 287, 301,
                          316, 332, 348, 365, 383, 402, 422, 442, 464, 487, 511, 536,
                          562, 590, 619, 649, 681, 715, 750, 787, 825, 866, 909, 953 };

const int e96Series[] = { 100, 102, 105, 107, 110, 113, 115, 118, 121, 124, 127, 130,
                          133, 137, 140, 143, 147, 150, 154, 158, 162, 165, 169, 174,
                          178, 182, 187, 191, 196, 200, 205, 210, 215, 221, 226, 232,
                          237, 243, 249, 255, 261, 267, 274, 280, 287, 294, 301, 309,
                          316, 324, 332, 340, 348, 357, 365, 374, 383, 392, 402, 412,
                          422, 432, 442, 453, 464, 475, 487, 499, 511, 523, 536, 549,
                          562, 576, 590, 604, 619, 634, 649, 665, 681, 698, 715, 732,
                          750, 768, 787, 806, 825, 845, 866, 887, 909, 931, 953, 976 };

const int e192Series[] = { 100, 101, 102, 104, 105, 106, 107, 109, 110, 111, 113, 114,
                           115, 117, 118, 120, 121, 123, 124, 126, 127, 129, 130, 132,
                           133, 135, 137, 138, 140, 142, 143, 145, 147, 149, 150, 152,
                           154, 156, 158, 160, 162, 164, 165, 167, 169, 172, 174, 176,
                           178, 180, 182, 184, 187, 189, 191, 193, 196, 198, 200, 203,
                           205, 208, 210, 213, 215, 218, 221, 223, 226, 229, 232, 234,
                           237, 240, 243, 246, 249, 252, 255, 258, 261, 264, 267, 271,
                           274, 277, 280, 284, 287, 291, 294, 298, 301, 305, 309, 312,
                           316, 320, 324, 328, 332, 336, 340, 344, 348, 352, 357, 361,
                           365, 370, 374, 379, 383, 388, 392, 397, 402, 407, 412, 417,
                           422, 427, 432, 437, 442, 448, 453, 459, 464, 470, 475, 481,
                           487, 493, 499, 505, 511, 517, 523, 530, 536, 542, 549, 556,
                           562, 569, 576, 583, 590, 597, 604, 612, 619, 626, 634, 642,
                           649, 657, 665, 673, 681, 690, 698, 706, 715, 723, 732, 741,
                           750, 759, 768, 777, 787, 796, 806, 816, 825, 835, 845, 856,
                           866, 876, 887, 898, 909, 920, 931, 942, 953, 965, 976, 988 };


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionQuit, &QAction::triggered, qApp, &QApplication::quit);
    connect(ui->quitButton, &QPushButton::clicked, qApp, &QApplication::quit);
    connect(ui->actionCalculate, &QAction::triggered, this, &MainWindow::calculate);
    connect(ui->calculateButton, &QPushButton::clicked, this, &MainWindow::calculate);
    connect(ui->modeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::setMode);

    ui->modeComboBox->setCurrentIndex(0);
    ui->standardValuesComboBox->setCurrentIndex(1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setMode(int mode)
{
    switch(mode) {
    case 0:
        ui->circuitLabel->setPixmap(QPixmap(QString::fromUtf8(":/images/circuit-s2.png")));
        ui->r3SpinBox->setEnabled(false);
        break;
    case 1:
        ui->circuitLabel->setPixmap(QPixmap(QString::fromUtf8(":/images/circuit-s3.png")));
        ui->r3SpinBox->setEnabled(true);
        break;
    case 2:
        ui->circuitLabel->setPixmap(QPixmap(QString::fromUtf8(":/images/circuit-p2.png")));
        ui->r3SpinBox->setEnabled(false);
        break;
    case 3:
        ui->circuitLabel->setPixmap(QPixmap(QString::fromUtf8(":/images/circuit-p3.png")));
        ui->r3SpinBox->setEnabled(true);
        break;
    case 4:
        ui->circuitLabel->setPixmap(QPixmap(QString::fromUtf8(":/images/circuit-sp3a.png")));
        ui->r3SpinBox->setEnabled(true);
        break;
    case 5:
        ui->circuitLabel->setPixmap(QPixmap(QString::fromUtf8(":/images/circuit-sp3b.png")));
        ui->r3SpinBox->setEnabled(true);
        break;
    }
}


// Return value multiplier based on units in combobox.
int MainWindow::multiplier(int units)
{
    switch(units) {
    case 0: // Ohms
        return 1;
    case 1: // Kilohms
        return 1000;
    case 2: // Megohms
        return 1000000;
    default:
        return -1;
    }
}

void MainWindow::calculate()
{
    double desiredValue = ui->desiredValueSpinBox->value() * multiplier(ui->desiredResistanceComboBox->currentIndex());
    double bestR1 = -1;
    double bestR2 = -1;
    double bestDiff = 1e6;

    qDebug() << "Desired value is" << desiredValue << "Ohms.";

  // Calculate by brute force.
    for (int decade1 = -1; decade1 <= 6; decade1++) {
        for (int i1 = 0; i1 < 24; i1++) {
            for (int decade2 = -1; decade2 <= 6; decade2++) {
                for (int i2 = 0; i2 < 24; i2++) {
                    double r1 = e24Series[i1] * exp10(decade1);
                    double r2 = e24Series[i2] *exp10(decade2);
                    double value = r1 + r2;
                    double diff = fabs(desiredValue - value);
                    if (diff < bestDiff) {
                        bestR1 = r1;
                        bestR2 = r2;
                        bestDiff = diff;
                        //qDebug() << "Best so far is" << r1 << "+" << r2;
                    }
                }
            }
        }
    }

    qDebug() << "Best R1 =" << bestR1;
    qDebug() << "Best R2 =" << bestR2;

    ui->r1SpinBox->setValue(bestR1);
    ui->r2SpinBox->setValue(bestR2);
    QString s = QString::number(bestR1 + bestR2) + " Ohms (" + QString::number(100 * bestDiff / (bestR1 + bestR2)) + "%)";
    ui->actualValueLabel->setText(s);


}
