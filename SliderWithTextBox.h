#ifndef QMAGNIFIER_SLIDERWITHTEXTBOX_H
#define QMAGNIFIER_SLIDERWITHTEXTBOX_H


#include <QSlider>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QWidget>
#include <QApplication>

class SliderWithTextBox : public QWidget {
//Q_OBJECT

public:
    SliderWithTextBox(QWidget *parent = nullptr)
            : QWidget(parent), slider(new QSlider(Qt::Vertical)), lineEdit(new QLineEdit)
    {
        auto *layout = new QVBoxLayout;
        lineEdit->setMaximumWidth(24);
        layout->addWidget(slider);
        layout->addWidget(lineEdit);
        layout->setContentsMargins(0, 0, 0, 0);
        connect(slider, &QSlider::valueChanged, this, &SliderWithTextBox::updateLineEdit);

        slider->setRange(8, 32);
        lineEdit->setReadOnly(true);

        setLayout(layout);
        adjustSize();
    }

public slots:
    void updateLineEdit(int value) {
        lineEdit->setText(QString::number(int(value*value/50.0)));
    }

private:
    QSlider *slider;
    QLineEdit *lineEdit;
};

#endif //QMAGNIFIER_SLIDERWITHTEXTBOX_H
