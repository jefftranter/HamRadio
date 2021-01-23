#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

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

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  void setMode();
  void setR1();
  void setR2();
  void setR3();
  void setDesired();
  void setResult(double newResult);
  void setSeries();
  void calculate();
  int multiplier(int units);
  double optimalScale(double val);
  int optimalUnits(double val);
  void info();
  void about();
  void solve();
  void solveS2();
  void solveS3();
  void solveP2();
  void solveP3();
  void solveSP3A();
  void solveSP3B();

private:
  Ui::MainWindow *ui;  // UI form
  int m_mode;          // Circuit mode
  double m_r1;         // Value of R1
  double m_r2;         // Value of R2
  double m_r3;         // Value of R3
  double m_desired;    // Desired total resistance value
  double m_result;     // Actual resistance value
  const int *m_series; // Resistor series
  int m_seriesSize;    // Size of resistor series
  int m_firstDecade;   // First decade of series
  int m_lastDecade;    // Last decade of series
};
#endif // MAINWINDOW_H
