#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <math.h>

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
  ui->standardValuesComboBox->setCurrentIndex(E12);

  setMode();
  setSeries();
  setR1();
  setR2();
  setR3();
  setDesired();
  setResult(0);
  calculate();
}

// Destructor
MainWindow::~MainWindow() { delete ui; }

void MainWindow::setR1() {
  m_r1 = ui->r1SpinBox->value() *
         multiplier(ui->r1ResistanceComboBox->currentIndex());
  // qDebug() << "_R1 =" << m_r1;
}

void MainWindow::setR2() {
  m_r2 = ui->r2SpinBox->value() *
         multiplier(ui->r2ResistanceComboBox->currentIndex());
  // qDebug() << "_R2 =" << m_r2;
}

void MainWindow::setR3() {
  m_r3 = ui->r3SpinBox->value() *
         multiplier(ui->r3ResistanceComboBox->currentIndex());
  // qDebug() << "_R3 =" << m_r3;
}

void MainWindow::setDesired() {
  m_desired = ui->desiredValueSpinBox->value() *
              multiplier(ui->desiredResistanceComboBox->currentIndex());
  // qDebug() << "Desired =" << m_desired;
}

void MainWindow::setResult(double newResult) {
  m_result = newResult;
  // qDebug() << "Result =" << m_result;
}

void MainWindow::setSeries() {
  // qDebug() << "Series =" << ui->standardValuesComboBox->currentIndex();

  switch (ui->standardValuesComboBox->currentIndex()) {
  case E6:
    m_series = e6Series;
    m_seriesSize = sizeof(e6Series) / sizeof(e6Series[0]);
    m_firstDecade = -1;
    m_lastDecade = 6;
    break;
  case E12:
    m_series = e12Series;
    m_seriesSize = sizeof(e12Series) / sizeof(e12Series[0]);
    m_firstDecade = -1;
    m_lastDecade = 6;
    break;
  case E24:
    m_series = e24Series;
    m_seriesSize = sizeof(e24Series) / sizeof(e24Series[0]);
    m_firstDecade = -1;
    m_lastDecade = 6;
    break;
  case E48:
    m_series = e48Series;
    m_seriesSize = sizeof(e48Series) / sizeof(e48Series[0]);
    m_firstDecade = -2;
    m_lastDecade = 5;
    break;
  case E96:
    m_series = e96Series;
    m_seriesSize = sizeof(e96Series) / sizeof(e96Series[0]);
    m_firstDecade = -2;
    m_lastDecade = 5;
    break;
  case E192:
    m_series = e192Series;
    m_seriesSize = sizeof(e192Series) / sizeof(e192Series[0]);
    m_firstDecade = -2;
    m_lastDecade = 5;
    break;
  }
}

// Set controls based on operating mode.
void MainWindow::setMode() {
  m_mode = ui->modeComboBox->currentIndex();
  // qDebug() << "Mode =" << m_mode;

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

// Return optimal scaled value based on value.
double MainWindow::optimalScale(double val)
{
  if (val >= 1e6) {
    return val / 1e6;
  } else if (val >= 1e3) {
    return val / 1e3;
  } else {
    return val;
  }
}


// Return optimal units for value
int MainWindow::optimalUnits(double val)
{
  if (val >= 1e6) {
    return MEGOHMS;
  } else if (val >= 1e3) {
    return KILOHMS;
  } else {
    return OHMS;
  }
}

// Calculate result based on input values.
void MainWindow::calculate() {

  if (m_desired > 100e6) {
    QMessageBox::warning(this, tr("Out of Range"),
                         tr("The desired value is too high. Please specify a "
                            "value of no more than 100 Megohms."));
    return;
  }

  qDebug() << "Desired value is" << m_desired << "Ohms.";

  QApplication::setOverrideCursor(Qt::WaitCursor);
  solve();
  QApplication::restoreOverrideCursor();

  qDebug() << "R1 =" << m_r1;
  qDebug() << "R2 =" << m_r2;
  qDebug() << "R3 =" << m_r3;

  ui->r1SpinBox->setValue(optimalScale(m_r1));
  ui->r1ResistanceComboBox->setCurrentIndex(optimalUnits(m_r1));

  ui->r2SpinBox->setValue(optimalScale(m_r2));
  ui->r2ResistanceComboBox->setCurrentIndex(optimalUnits(m_r2));

  ui->r3SpinBox->setValue(optimalScale(m_r3));
  ui->r3ResistanceComboBox->setCurrentIndex(optimalUnits(m_r3));

  double error = (m_result - m_desired) / m_desired;
  QString s;
  if (m_result >= 1e6) {
    s = tr("<b>%1 Megohms").arg(QString::number(m_result / 1e6));
  } else if (m_result >= 1e3) {
    s = tr("<b>%1 Kilohms").arg(QString::number(m_result / 1e3));
  } else {
    s = tr("<b>%1 Ohms").arg(QString::number(m_result));
  }
  if (qFuzzyCompare(error, 0)) {
    s += " (exact)";
  } else {
    s += tr(" (error %1%)</b>").arg(QString::number(100 * error, 'g', 2));
  }
  ui->actualValueLabel->setText(s);
}

void MainWindow::solve() {
  switch (m_mode) {
  case S2:
    solveS2();
    break;
  case S3:
    solveS3();
    break;
  case P2:
    solveP2();
    break;
  case P3:
    solveP3();
    break;
  case SP3A:
    solveSP3A();
    break;
  case SP3B:
    solveSP3B();
    break;
  }
}

void MainWindow::solveS2() {
  double bestR1 = -1;
  double bestR2 = -1;
  double bestDiff = 100e6;
  double value = -1;

  // Calculate by brute force.
  // TODO: Optimize by breaking if any single resistor or first resistor of
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
            //qDebug() << "Best so far is" << r1 << "+" << r2;
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
  m_r1 = bestR1;
  m_r2 = bestR2;
  m_result = bestR1 + bestR2;
  qDebug() << "R1 =" << m_r1;
  qDebug() << "R2 =" << m_r2;
  qDebug() << "Result =" << m_result;
}

void MainWindow::solveS3() {
  double bestR1 = -1;
  double bestR2 = -1;
  double bestR3 = -1;
  double bestDiff = 100e6;
  double value = -1;

  // Calculate by brute force.
  // TODO: Optimize by breaking if any single resistor or first resistor of
  // decade is greater than the desired value
  for (int decade1 = m_firstDecade; decade1 <= m_lastDecade; decade1++) {
    for (int i1 = 0; i1 < m_seriesSize; i1++) {
      for (int decade2 = m_firstDecade; decade2 <= m_lastDecade; decade2++) {
        for (int i2 = 0; i2 < m_seriesSize; i2++) {
          for (int decade3 = m_firstDecade; decade3 <= m_lastDecade;
               decade3++) {
            for (int i3 = 0; i3 < m_seriesSize; i3++) {
              double r1 = m_series[i1] * exp10(decade1);
              double r2 = m_series[i2] * exp10(decade2);
              double r3 = m_series[i3] * exp10(decade3);
              value = r1 + r2 + r3;
              double diff = fabs(m_desired - value);
              if (diff < bestDiff) {
                bestR1 = r1;
                bestR2 = r2;
                bestR3 = r3;
                bestDiff = diff;
                //qDebug() << "Best so far is" << r1 << "+" << r2 << "+" << r3;
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
  m_r1 = bestR1;
  m_r2 = bestR2;
  m_r3 = bestR3;
  m_result = bestR1 + bestR2 + bestR3;
  qDebug() << "R1 =" << m_r1;
  qDebug() << "R2 =" << m_r2;
  qDebug() << "R3 =" << m_r3;
  qDebug() << "Result =" << m_result;
}

void MainWindow::solveP2()
{
  double bestR1 = -1;
  double bestR2 = -1;
  double bestDiff = 100e6;
  double value = -1;

  // Calculate by brute force.
  // TODO: Optimize by breaking if any single resistor or first resistor of
  // decade is less than the desired value
  // TODO: Handle case where optimal solution is to leave one resistor open.
  for (int decade1 = m_firstDecade; decade1 <= m_lastDecade; decade1++) {
    for (int i1 = 0; i1 < m_seriesSize; i1++) {
      for (int decade2 = m_firstDecade; decade2 <= m_lastDecade; decade2++) {
        for (int i2 = 0; i2 < m_seriesSize; i2++) {
          double r1 = m_series[i1] * exp10(decade1);
          double r2 = m_series[i2] * exp10(decade2);
          if (r1 == 0 && r2 == 0) {
            continue; // Avoid divide by zero
          }
          value = (r1 * r2) / (r1 + r2);
          double diff = fabs(m_desired - value);
          if (diff < bestDiff) {
            bestR1 = r1;
            bestR2 = r2;
            bestDiff = diff;
            //qDebug() << "Best so far is" << r1 << "+" << r2;
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
  m_r1 = bestR1;
  m_r2 = bestR2;
  m_result = (bestR1 * bestR2) / (bestR1 + bestR2);
  qDebug() << "R1 =" << m_r1;
  qDebug() << "R2 =" << m_r2;
  qDebug() << "Result =" << m_result;
}

void MainWindow::solveP3()
{
  double bestR1 = -1;
  double bestR2 = -1;
  double bestR3 = -1;
  double bestDiff = 100e6;
  double value = -1;

  // Calculate by brute force.
  // TODO: Optimize by breaking if any single resistor or first resistor of
  // decade is less than the desired value
  // TODO: Handle case where optimal solution is to leave one or two resistors open.
  for (int decade1 = m_firstDecade; decade1 <= m_lastDecade; decade1++) {
    for (int i1 = 0; i1 < m_seriesSize; i1++) {
      for (int decade2 = m_firstDecade; decade2 <= m_lastDecade; decade2++) {
        for (int i2 = 0; i2 < m_seriesSize; i2++) {
          for (int decade3 = m_firstDecade; decade3 <= m_lastDecade;
               decade3++) {
            for (int i3 = 0; i3 < m_seriesSize; i3++) {
              double r1 = m_series[i1] * exp10(decade1);
              double r2 = m_series[i2] * exp10(decade2);
              double r3 = m_series[i3] * exp10(decade3);
              if (r1 == 0 || r2 == 0 || r3 == 0) {
                continue; // Avoid divide by zero
              }
              value = 1 / (1 / r1 + 1 / r2 + 1 / r3);
              double diff = fabs(m_desired - value);
              if (diff < bestDiff) {
                bestR1 = r1;
                bestR2 = r2;
                bestR3 = r3;
                bestDiff = diff;
                //qDebug() << "Best so far is" << r1 << "+" << r2 << "+" << r3;
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
  m_r1 = bestR1;
  m_r2 = bestR2;
  m_r3 = bestR3;
  m_result = 1 / (1 / bestR1 + 1 / bestR2 + 1 / bestR3);
  qDebug() << "R1 =" << m_r1;
  qDebug() << "R2 =" << m_r2;
  qDebug() << "R3 =" << m_r3;
  qDebug() << "Result =" << m_result;
}

void MainWindow::solveSP3A()
{
  double bestR1 = -1;
  double bestR2 = -1;
  double bestR3 = -1;
  double bestDiff = 100e6;
  double value = -1;

  // Calculate by brute force.
  // TODO: Optimize by breaking if any single resistor or first resistor of
  // decade is greater than or less than the desired value, depending position.
  // TODO: Handle case where optimal solution is to leave one or two resistors open.
  for (int decade1 = m_firstDecade; decade1 <= m_lastDecade; decade1++) {
    for (int i1 = 0; i1 < m_seriesSize; i1++) {
      for (int decade2 = m_firstDecade; decade2 <= m_lastDecade; decade2++) {
        for (int i2 = 0; i2 < m_seriesSize; i2++) {
          for (int decade3 = m_firstDecade; decade3 <= m_lastDecade;
               decade3++) {
            for (int i3 = 0; i3 < m_seriesSize; i3++) {
              double r1 = m_series[i1] * exp10(decade1);
              double r2 = m_series[i2] * exp10(decade2);
              double r3 = m_series[i3] * exp10(decade3);
              if (r1 == 0 && r2 == 0 && r3 == 0) {
                  continue; // Avoid divide by zero
              }
              value = ((r1 + r2) * r3) / (r1 + r2 + r3);
              double diff = fabs(m_desired - value);
              if (diff < bestDiff) {
                bestR1 = r1;
                bestR2 = r2;
                bestR3 = r3;
                bestDiff = diff;
                //qDebug() << "Best so far is" << r1 << "+" << r2 << "+" << r3;
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
  m_r1 = bestR1;
  m_r2 = bestR2;
  m_r3 = bestR3;
  m_result = ((bestR1 + bestR2) * bestR3) / (bestR1 + bestR2 + bestR3);

  qDebug() << "R1 =" << m_r1;
  qDebug() << "R2 =" << m_r2;
  qDebug() << "R3 =" << m_r3;
  qDebug() << "Result =" << m_result;
}

void MainWindow::solveSP3B()
{
  double bestR1 = -1;
  double bestR2 = -1;
  double bestR3 = -1;
  double bestDiff = 100e6;
  double value = -1;

  // Calculate by brute force.
  // TODO: Optimize by breaking if any single resistor or first resistor of
  // decade is greater than or less than the desired value, depending position.
  // TODO: Handle case where optimal solution is to leave one or two resistors open.
  for (int decade1 = m_firstDecade; decade1 <= m_lastDecade; decade1++) {
    for (int i1 = 0; i1 < m_seriesSize; i1++) {
      for (int decade2 = m_firstDecade; decade2 <= m_lastDecade; decade2++) {
        for (int i2 = 0; i2 < m_seriesSize; i2++) {
          for (int decade3 = m_firstDecade; decade3 <= m_lastDecade;
               decade3++) {
            for (int i3 = 0; i3 < m_seriesSize; i3++) {
              double r1 = m_series[i1] * exp10(decade1);
              double r2 = m_series[i2] * exp10(decade2);
              double r3 = m_series[i3] * exp10(decade3);
              if (r2 == 0 && r3 == 0) {
                  continue; // Avoid divide by zero
              }
              value = r1 + (r2 * r3) / (r2 + r3);
              double diff = fabs(m_desired - value);
              if (diff < bestDiff) {
                bestR1 = r1;
                bestR2 = r2;
                bestR3 = r3;
                bestDiff = diff;
                //qDebug() << "Best so far is" << r1 << "+" << r2 << "+" << r3;
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
  m_r1 = bestR1;
  m_r2 = bestR2;
  m_r3 = bestR3;
  m_result = value = bestR1 + (bestR2 * bestR3) / (bestR2 + bestR3);

  qDebug() << "R1 =" << m_r1;
  qDebug() << "R2 =" << m_r2;
  qDebug() << "R3 =" << m_r3;
  qDebug() << "Result =" << m_result;
}

// Show resistor values.
void MainWindow::info() {
  QString text = tr("Standard resistor values in ");

  switch (ui->standardValuesComboBox->currentIndex()) {
  case E6:
    text += tr("E6 (20%)");
    break;
  case E12:
    text += tr("E12 (10%)");
    break;
  case E24:
    text += tr("E24 (5%)");
    break;
  case E48:
    text += tr("E48 (2%)");
    break;
  case E96:
    text += tr("E96 (1%)");
    break;
  case E192:
    text += tr("E192 (0.5%)");
    break;
  }

  text += tr(" series:\n\n");

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
         "Copyright (C) 2021 by Jeff Tranter "
         "&lt;tranter@pobox.com&gt;.<br><br>"
         "Licensed under the Apache License, Version 2.0 (the "
         "\"License\");<br>"
         "you may not use this file except in compliance with the "
         "License.<br>"
         "You may obtain a copy of the License at<br><br>"
         "  http://www.apache.org/licenses/LICENSE-2.0<br><br>"
         "Unless required by applicable law or agreed to in writing,<br>"
         "software distributed under the License is distributed on an<br>"
         "\"AS IS\" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF<br>"
         "ANY KIND either express or implied.<br>"
         "See the License for the specific language governing "
         "permissions<br>"
         "and limitations under the License."));
}
