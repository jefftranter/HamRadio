#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <math.h>

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
  connect(ui->desiredValueSpinBox,
          QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &MainWindow::setDesired);
  connect(ui->r1SpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
          this, &MainWindow::setR1);
  connect(ui->r2SpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
          this, &MainWindow::setR2);
  connect(ui->r3SpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
          this, &MainWindow::setR3);
  connect(ui->desiredResistanceComboBox,
          QOverload<int>::of(&QComboBox::currentIndexChanged), this,
          &MainWindow::setDesired);
  connect(ui->r1ResistanceComboBox,
          QOverload<int>::of(&QComboBox::currentIndexChanged), this,
          &MainWindow::setR1);
  connect(ui->r2ResistanceComboBox,
          QOverload<int>::of(&QComboBox::currentIndexChanged), this,
          &MainWindow::setR2);
  connect(ui->r3ResistanceComboBox,
          QOverload<int>::of(&QComboBox::currentIndexChanged), this,
          &MainWindow::setR3);
  connect(ui->calculateButton, &QPushButton::clicked, this,
          &MainWindow::calculate);
  connect(ui->infoButton, &QPushButton::clicked, this, &MainWindow::info);
  connect(ui->modeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
          this, &MainWindow::setMode);
  connect(ui->standardValuesComboBox,
          QOverload<int>::of(&QComboBox::currentIndexChanged), this,
          &MainWindow::setSeries);

  ui->modeComboBox->setCurrentIndex(0);
  setMode();
  ui->standardValuesComboBox->setCurrentIndex(E12);
  calculate();
}

// Destructor
MainWindow::~MainWindow() { delete ui; }

void MainWindow::setR1() {
  m_r1 = ui->r1SpinBox->value() *
         multiplier(ui->r1ResistanceComboBox->currentIndex());
  qDebug() << "R1 =" << m_r1;
}

void MainWindow::setR2() {
  m_r2 = ui->r2SpinBox->value() *
         multiplier(ui->r2ResistanceComboBox->currentIndex());
  qDebug() << "R2 =" << m_r2;
}

void MainWindow::setR3() {
  m_r3 = ui->r3SpinBox->value() *
         multiplier(ui->r3ResistanceComboBox->currentIndex());
  qDebug() << "R3 =" << m_r3;
}

void MainWindow::setDesired() {
  m_desired = ui->desiredValueSpinBox->value() *
              multiplier(ui->desiredResistanceComboBox->currentIndex());
  qDebug() << "Desired =" << m_desired;
}

void MainWindow::setResult(double newResult) {
  m_result = newResult;
  qDebug() << "Result =" << m_result;
}

void MainWindow::setSeries() {
  qDebug() << "Series =" << ui->standardValuesComboBox->currentIndex();

  switch (ui->standardValuesComboBox->currentIndex()) {
  case E6:
    m_series = e6Series;
    m_seriesSize = sizeof(e6Series) / sizeof(e6Series[0]);
    break;
  case E12:
    m_series = e12Series;
    m_seriesSize = sizeof(e12Series) / sizeof(e12Series[0]);
    break;
  case E24:
    m_series = e24Series;
    m_seriesSize = sizeof(e24Series) / sizeof(e24Series[0]);
    break;
  case E48:
    m_series = e48Series;
    m_seriesSize = sizeof(e48Series) / sizeof(e48Series[0]);
    break;
  case E96:
    m_series = e96Series;
    m_seriesSize = sizeof(e96Series) / sizeof(e96Series[0]);
    break;
  case E192:
    m_series = e192Series;
    m_seriesSize = sizeof(e192Series) / sizeof(e192Series[0]);
    break;
  }
}

// Set controls based on operating mode.
void MainWindow::setMode() {
  m_mode = ui->modeComboBox->currentIndex();
  qDebug() << "Mode =" << m_mode;

  switch (m_mode) {
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
  // double bestDiff = 100e6;
  const int *series = nullptr;
  int size = 0;
  int firstDecade = 0;
  int lastDecade = 0;
  double value = 0;

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

  // solve();

  // solveS2(firstDecade, lastDecade, series, size, desiredValue, value, bestR1,
  // bestR2);

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

bool MainWindow::solve() { return true; }

bool MainWindow::solveS2() {
  double bestR1 = -1;
  double bestR2 = -1;
  // double bestR3 = -1;
  double bestDiff = 100e6;
  double value = -1;

  // Calculate by brute force.
  // TODO: Optimize by continuing if any single resistor or first resistor of
  // decade is greater than the desired value
  for (int decade1 = m_firstDecade; decade1 <= m_lastDecade; decade1++) {
    for (int i1 = 0; i1 < m_seriesSize; i1++) {
      for (int decade2 = m_firstDecade; decade2 <= m_lastDecade; decade2++) {
        for (int i2 = 0; i2 < m_seriesSize; i2++) {
          double r1 = m_series[i1] * exp10(decade1);
          double r2 = m_series[i2] * exp10(decade2);
          value = r1 + r2;
          double diff = fabs(m_desired - value);
          if (diff < bestDiff) {
            bestR1 = r1;
            bestR2 = r2;
            bestDiff = diff;
            qDebug() << "Best so far is" << r1 << "+" << r2;
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

done:
  // pr1 = bestR1;
  // pr2 = bestR2;
  // presult = value;
  return true;
}

bool MainWindow::solveS3() {
  /*
    // Calculate by brute force.
    // TODO: Optimize by continuing if any single resistor or first resistor of
    decade is greater than the desired value for (int decade1 = firstDecade;
    decade1 <= lastDecade; decade1++) { for (int i1 = 0; i1 < size; i1++) { for
    (int decade2 = firstDecade; decade2 <= lastDecade; decade2++) { for (int i2
    = 0; i2 < size; i2++) { for (int decade3 = firstDecade; decade3 <=
    lastDecade; decade3++) { for (int i3 = 0; i3 < size; i3++) { double r1 =
    series[i1] * exp10(decade1); double r2 = series[i2] * exp10(decade2); double
    r3 = series[i3] * exp10(decade3); double value = r1 + r2 + r3; double diff =
    fabs(desiredValue - value); if (diff < bestDiff) { bestR1 = r1; bestR2 = r2;
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
  */
  return true;
}

void MainWindow::solveP2() {}

void MainWindow::solveP3() {}

void MainWindow::solveSP3A() {}

void MainWindow::solveSP3B() {}

// Show resistor values.
void MainWindow::info() {
  QString text;

  switch (ui->standardValuesComboBox->currentIndex()) {
  case E6:
    text = tr("Standard resistor values in E6 (20%) series:\n\n");
    break;
  case E12:
    text = tr("Standard resistor values in E12 (10%) series:\n\n");
    break;
  case E24:
    text = tr("Standard resistor values in E24 (5%) series:\n\n");
    break;
  case E48:
    text = tr("Standard resistor values in E48 (2%) series:\n\n");
    break;
  case E96:
    text = tr("Standard resistor values in E96 (1%) series:\n\n");
    break;
  case E192:
    text = tr("Standard resistor values in E192 (0.5%) series:\n\n");
    break;
  }

  for (int i = 1; i < m_seriesSize; i++) {
    text += QString::number(m_series[i]);
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
         "Unless required by applicable law or agreed to in writing,<br>"
         "software distributed under the License is distributed on an<br>"
         "\"AS IS\" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF<br>"
         "ANY KIND either express or implied.<br>"
         "See the License for the specific language governing permissions<br>"
         "and limitations under the License."));
}
