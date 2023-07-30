#ifndef QMAGNIFIER_SLIDERWITHTEXTBOX_H
#define QMAGNIFIER_SLIDERWITHTEXTBOX_H


#include <QSlider>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QWidget>
#include <QApplication>

class SliderWithTextBox : public QWidget {
Q_OBJECT

public:
    SliderWithTextBox(QWidget *parent = nullptr);

public slots:
    void updateLineEdit(int value);

signals:
    void valueChanged(int value);

private:
    QSlider *slider;
    QLineEdit *lineEdit;
};

#endif //QMAGNIFIER_SLIDERWITHTEXTBOX_H
