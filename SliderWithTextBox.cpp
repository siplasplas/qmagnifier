#include "SliderWithTextBox.h"
#include <cmath>
#include "SliderWithTextBox.h"

SliderWithTextBox::SliderWithTextBox(QWidget *parent)
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

void SliderWithTextBox::updateLineEdit(int value) {
    int newValue = int(value*value/50.0);
    lineEdit->setText(QString::number(newValue));
    emit valueChanged(newValue);
}

void SliderWithTextBox::setValue(int val) {
    slider->setValue((int)((round(sqrt(val*50.0)))));
}
