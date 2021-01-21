#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <math.h>

// List of standard resistor values.
const int e6Series[] = {0, 10, 15, 22, 33, 47, 68};

const int e12Series[] = {0, 10, 12, 15, 18, 22, 27, 33, 39, 47, 56, 68, 82};

const int e24Series[] = {0,  10, 11, 12, 13, 15, 16, 18, 20, 22, 24, 27, 30,
                         33, 36, 39, 43, 47, 51, 56, 62, 68, 75, 82, 91};

const int e48Series[] = {0,   100, 105, 110, 115, 121, 127, 133, 140, 147,
                         154, 162, 169, 178, 187, 196, 205, 215, 226, 237,
                         249, 261, 274, 287, 301, 316, 332, 348, 365, 383,
                         402, 422, 442, 464, 487, 511, 536, 562, 590, 619,
                         649, 681, 715, 750, 787, 825, 866, 909, 953};

const int e96Series[] = {
    0,   100, 102, 105, 107, 110, 113, 115, 118, 121, 124, 127, 130, 133,
    137, 140, 143, 147, 150, 154, 158, 162, 165, 169, 174, 178, 182, 187,
    191, 196, 200, 205, 210, 215, 221, 226, 232, 237, 243, 249, 255, 261,
    267, 274, 280, 287, 294, 301, 309, 316, 324, 332, 340, 348, 357, 365,
    374, 383, 392, 402, 412, 422, 432, 442, 453, 464, 475, 487, 499, 511,
    523, 536, 549, 562, 576, 590, 604, 619, 634, 649, 665, 681, 698, 715,
    732, 750, 768, 787, 806, 825, 845, 866, 887, 909, 931, 953, 976};

const int e192Series[] = {
    0,   100, 101, 102, 104, 105, 106, 107, 109, 110, 111, 113, 114, 115, 117,
    118, 120, 121, 123, 124, 126, 127, 129, 130, 132, 133, 135, 137, 138, 140,
    142, 143, 145, 147, 149, 150, 152, 154, 156, 158, 160, 162, 164, 165, 167,
    169, 172, 174, 176, 178, 180, 182, 184, 187, 189, 191, 193, 196, 198, 200,
    203, 205, 208, 210, 213, 215, 218, 221, 223, 226, 229, 232, 234, 237, 240,
    243, 246, 249, 252, 255, 258, 261, 264, 267, 271, 274, 277, 280, 284, 287,
    291, 294, 298, 301, 305, 309, 312, 316, 320, 324, 328, 332, 336, 340, 344,
    348, 352, 357, 361, 365, 370, 374, 379, 383, 388, 392, 397, 402, 407, 412,
    417, 422, 427, 432, 437, 442, 448, 453, 459, 464, 470, 475, 481, 487, 493,
    499, 505, 511, 517, 523, 530, 536, 542, 549, 556, 562, 569, 576, 583, 590,
    597, 604, 612, 619, 626, 634, 642, 649, 657, 665, 673, 681, 690, 698, 706,
    715, 723, 732, 741, 750, 759, 768, 777, 787, 796, 806, 816, 825, 835, 845,
    856, 866, 876, 887, 898, 909, 920, 931, 942, 953, 965, 976, 988};

// Constants for combobox values
const int OHMS = 0;
const int KILOHMS = 1;
const int MEGOHMS = 2;

const int E6 = 0;
const int E12 = 1;
const int E24 = 2;
const int E48 = 3;
const int E96 = 4;
const int E192 = 5;

const int S2 = 0;
const int S3 = 1;
const int P2 = 2;
const int P3 = 3;
const int SP3A = 4;
const int SP3B = 5;


// Constructor
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  connect(ui->actionQuit, &QAction::triggered, qApp, &QApplication::quit);
  connect(ui->quitButton, &QPushButton::clicked, qApp, &QApplication::quit);
  connect(ui->actionCalculate, &QAction::triggered, this,
          &MainWindow::calculate);
  connect(ui->actionInfo, &QAction::triggered, this, &MainWindow::info);
  connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::about);

  connect(ui->calculateButton, &QPushButton::clicked, this,
          &MainWindow::calculate);
  connect(ui->infoButton, &QPushButton::clicked, this, &MainWindow::info);
  connect(ui->modeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
          this, &MainWindow::setMode);

  ui->modeComboBox->setCurrentIndex(0);
  setMode(0);
  ui->standardValuesComboBox->setCurrentIndex(E12);
  calculate();
}

// Destructor
MainWindow::~MainWindow() { delete ui; }

// Set controls based on operating mode.
void MainWindow::setMode(int mode) {
  switch (mode) {
  case S2:
    ui->circuitLabel->setPixmap(
        QPixmap(QString::fromUtf8(":/images/circuit-s2.png")));
    ui->r3SpinBox->setEnabled(false);
    ui->r3CheckBox->setEnabled(false);
    ui->r3ResistanceComboBox->setEnabled(false);
    ui->r3Label->setEnabled(false);
    break;
  case S3:
    ui->circuitLabel->setPixmap(
        QPixmap(QString::fromUtf8(":/images/circuit-s3.png")));
    ui->r3SpinBox->setEnabled(true);
    ui->r3CheckBox->setEnabled(true);
    ui->r3ResistanceComboBox->setEnabled(true);
    ui->r3Label->setEnabled(true);
    break;
  case P2:
    ui->circuitLabel->setPixmap(
        QPixmap(QString::fromUtf8(":/images/circuit-p2.png")));
    ui->r3SpinBox->setEnabled(false);
    ui->r3CheckBox->setEnabled(false);
    ui->r3ResistanceComboBox->setEnabled(false);
    ui->r3Label->setEnabled(false);
    break;
  case P3:
    ui->circuitLabel->setPixmap(
        QPixmap(QString::fromUtf8(":/images/circuit-p3.png")));
    ui->r3SpinBox->setEnabled(true);
    ui->r3CheckBox->setEnabled(true);
    ui->r3ResistanceComboBox->setEnabled(true);
    ui->r3Label->setEnabled(true);
    break;
  case SP3A:
    ui->circuitLabel->setPixmap(
        QPixmap(QString::fromUtf8(":/images/circuit-sp3a.png")));
    ui->r3SpinBox->setEnabled(true);
    ui->r3CheckBox->setEnabled(true);
    ui->r3ResistanceComboBox->setEnabled(true);
    ui->r3Label->setEnabled(true);
    break;
  case SP3B:
    ui->circuitLabel->setPixmap(
        QPixmap(QString::fromUtf8(":/images/circuit-sp3b.png")));
    ui->r3SpinBox->setEnabled(true);
    ui->r3CheckBox->setEnabled(true);
    ui->r3ResistanceComboBox->setEnabled(true);
    ui->r3Label->setEnabled(true);
    break;
  }
}

// Return value multiplier based on units in combobox.
int MainWindow::multiplier(int units) {
  switch (units) {
  case OHMS:
    return 1;
  case KILOHMS:
    return 1e3;
  case MEGOHMS:
    return 1e6;
  default:
    return -1;
  }
}

// Calculate result based on input values.
void MainWindow::calculate() {
  double desiredValue =
      ui->desiredValueSpinBox->value() *
      multiplier(ui->desiredResistanceComboBox->currentIndex());
  double bestR1 = -1;
  double bestR2 = -1;
  double bestR3 = -1;
  double bestDiff = 100e6;
  const int *series = nullptr;
  int size = 0;
  int firstDecade = 0;
  int lastDecade = 0;

  if (desiredValue > 100e6) {
    QMessageBox::warning(this, tr("Out of Range"),
                         tr("The desired value is too high, please specify a "
                            "value of no more than 100 Megohms."));
    return;
  }

  // TODO: Below code is common with Info, refactor.
  switch (ui->standardValuesComboBox->currentIndex()) {
  case E6:
    series = e6Series;
    size = sizeof(e6Series) / sizeof(e6Series[0]);
    firstDecade = -1;
    lastDecade = 6;
    break;
  case E12:
    series = e12Series;
    size = sizeof(e12Series) / sizeof(e12Series[0]);
    firstDecade = -1;
    lastDecade = 6;
    break;
  case E24:
    series = e24Series;
    size = sizeof(e24Series) / sizeof(e24Series[0]);
    firstDecade = -1;
    lastDecade = 6;
    break;
  case E48:
    series = e48Series;
    size = sizeof(e48Series) / sizeof(e48Series[0]);
    firstDecade = -2;
    lastDecade = 5;
    break;
  case E96:
    series = e96Series;
    size = sizeof(e96Series) / sizeof(e96Series[0]);
    firstDecade = -2;
    lastDecade = 5;
    break;
  case E192:
    series = e192Series;
    size = sizeof(e192Series) / sizeof(e192Series[0]);
    firstDecade = -2;
    lastDecade = 5;
    break;
  }

  // qDebug() << "Desired value is" << desiredValue << "Ohms.";

  QApplication::setOverrideCursor(Qt::WaitCursor);

  // Calculate by brute force.
  // TODO: Optimize by continuing if any single resistor or first resistor of decade is greater than the desired value
  for (int decade1 = firstDecade; decade1 <= lastDecade; decade1++) {
    for (int i1 = 0; i1 < size; i1++) {
      for (int decade2 = firstDecade; decade2 <= lastDecade; decade2++) {
        for (int i2 = 0; i2 < size; i2++) {
          for (int decade3 = firstDecade; decade3 <= lastDecade; decade3++) {
            for (int i3 = 0; i3 < size; i3++) {
              double r1 = series[i1] * exp10(decade1);
              double r2 = series[i2] * exp10(decade2);
              double r3 = series[i3] * exp10(decade3);
              double value = r1 + r2 + r3;
              double diff = fabs(desiredValue - value);
              if (diff < bestDiff) {
                bestR1 = r1;
                bestR2 = r2;
                bestR3 = r3;
                bestDiff = diff;
                qDebug() << "Best so far is" << r1 << "+" << r2 << "+" << r3;
                // Optimization: stop if exact solution found.
                if (qFuzzyCompare(bestDiff, 0)) {
                    qDebug() << "Exact solution found!";
                    goto done;
                }
              }
            }
          }
        }
      }
    }
  }

  done:

  QApplication::restoreOverrideCursor();

  qDebug() << "Best R1 =" << bestR1;
  qDebug() << "Best R2 =" << bestR2;
  qDebug() << "Best R3 =" << bestR3;

  if (bestR1 > 1e6) {
    ui->r1ResistanceComboBox->setCurrentIndex(MEGOHMS);
    ui->r1SpinBox->setValue(bestR1 / 1e6);
  } else if (bestR1 > 1e3) {
    ui->r1ResistanceComboBox->setCurrentIndex(KILOHMS);
    ui->r1SpinBox->setValue(bestR1 / 1e3);
  } else {
    ui->r1ResistanceComboBox->setCurrentIndex(OHMS);
    ui->r1SpinBox->setValue(bestR1);
  }

  if (bestR2 > 1e6) {
    ui->r2ResistanceComboBox->setCurrentIndex(MEGOHMS);
    ui->r2SpinBox->setValue(bestR2 / 1e6);
  } else if (bestR2 > 1e3) {
    ui->r2ResistanceComboBox->setCurrentIndex(KILOHMS);
    ui->r2SpinBox->setValue(bestR2 / 1e3);
  } else {
    ui->r2ResistanceComboBox->setCurrentIndex(OHMS);
    ui->r2SpinBox->setValue(bestR2);
  }

  if (bestR3 > 1e6) {
    ui->r3ResistanceComboBox->setCurrentIndex(MEGOHMS);
    ui->r3SpinBox->setValue(bestR3 / 1e6);
  } else if (bestR3 > 1e3) {
    ui->r3ResistanceComboBox->setCurrentIndex(KILOHMS);
    ui->r3SpinBox->setValue(bestR3 / 1e3);
  } else {
    ui->r3ResistanceComboBox->setCurrentIndex(OHMS);
    ui->r3SpinBox->setValue(bestR3);
  }

  double value = bestR1 + bestR2 + bestR3;
  double error = (value - desiredValue) / desiredValue;
  QString s;
  if (value > 1e6) {
    s = tr("<b>%1 Megohms").arg(QString::number(value / 1e6));
  } else if (value > 1e3) {
    s = tr("<b>%1 Kilohms").arg(QString::number(value / 1e3));
  } else {
    s = tr("<b>%1 Ohms").arg(QString::number(value));
  }
  s += tr(" (error %1%)</b>").arg(QString::number(100 * error, 'g', 2));
  ui->actualValueLabel->setText(s);
}

// Show resistor values.
void MainWindow::info() {
  QString text;
  const int *series = nullptr;
  int size = 0;

  switch (ui->standardValuesComboBox->currentIndex()) {
  case E6:
    text = tr("Standard resistor values in E6 (20%) series:\n\n");
    series = e6Series;
    size = sizeof(e6Series) / sizeof(e6Series[0]);
    break;
  case E12:
    text = tr("Standard resistor values in E12 (10%) series:\n\n");
    series = e12Series;
    size = sizeof(e12Series) / sizeof(e12Series[0]);
    break;
  case E24:
    text = tr("Standard resistor values in E24 (5%) series:\n\n");
    series = e24Series;
    size = sizeof(e24Series) / sizeof(e24Series[0]);
    break;
  case E48:
    text = tr("Standard resistor values in E48 (2%) series:\n\n");
    series = e48Series;
    size = sizeof(e48Series) / sizeof(e48Series[0]);
    break;
  case E96:
    text = tr("Standard resistor values in E96 (1%) series:\n\n");
    series = e96Series;
    size = sizeof(e96Series) / sizeof(e96Series[0]);
    break;
  case E192:
    text = tr("Standard resistor values in E192 (0.5%) series:\n\n");
    series = e192Series;
    size = sizeof(e192Series) / sizeof(e192Series[0]);
    break;
  }

  for (int i = 1; i < size; i++) {
    text += QString::number(series[i]);
    if (i % 12) {
      text += tr(" ");
    } else {
      text += tr("\n");
    }
  }

  QMessageBox::information(this, tr("Info"), text);
}

// About dialog.
void MainWindow::about() {
  QMessageBox::about(
      this, tr("About Precresistor"),
      tr("<b>Precision Resistor application by Jeff Tranter.</b><br><br>"
         "Copyright (C) 2021 by Jeff Tranter &lt;tranter@pobox.com&gt;.<br><br>"
         "Licensed under the Apache License, Version 2.0 (the \"License\");<br>"
         "you may not use this file except in compliance with the License.<br>"
         "You may obtain a copy of the License at<br><br>"
         "  http://www.apache.org/licenses/LICENSE-2.0<br><br>"
         "Unless required by applicable law or agreed to in writing, "
         "software<br>"
         "distributed under the License is distributed on an \"AS IS\" "
         "BASIS,<br>"
         "WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or "
         "implied.<br>"
         "See the License for the specific language governing permissions "
         "and<br>"
         "limitations under the License.<br>"));
}
