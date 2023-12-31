#include <QApplication>
#include <QScreen>
#include <QDebug>
#include <QDesktopWidget>
#include <QMouseEvent>
#include <QLabel>
#include <QSplitter>
#include <QMainWindow>
#include <QMenuBar>
#include <QAction>
#include <QStatusBar>
#include <cmath>
#include <algorithm>
#include "SliderWithTextBox.h"

class Magnifier : public QLabel {
Q_OBJECT
public:
    explicit Magnifier(QWidget* parent = nullptr)
            : QLabel(parent)
    {
        setMouseTracking(true);
    }
    void mouseMoveEvent(QMouseEvent* event) override;
};

class MainWindow : public QMainWindow {
Q_OBJECT
    Magnifier *magnifier;
    float scale = 8;
    QPoint globalPos;
protected:
    void mouseMoveEvent(QMouseEvent* event) override;
public:
    explicit MainWindow(QWidget* parent = nullptr)
            : QMainWindow(parent)
    {
        QWidget *centralWidget = new QWidget(this);
        auto *layout = new QHBoxLayout;
        magnifier = new Magnifier(this);
        auto *sliderText = new SliderWithTextBox(this);
        sliderText->setValue((int)scale);
        connect(sliderText, &SliderWithTextBox::valueChanged, this, [this](int value) {
            scale = value;
            grab(globalPos);
        });


        layout->addWidget(magnifier);
        layout->addWidget(sliderText);
        layout->setStretchFactor(magnifier, 1);
        layout->setStretchFactor(sliderText, 0);
        layout->setContentsMargins(0, 0, 0, 0);

        centralWidget->setLayout(layout);
        setCentralWidget(centralWidget);
        QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
        QAction *exitAction = fileMenu->addAction(tr("E&xit"), this, &QWidget::close);
        exitAction->setShortcuts(QKeySequence::Quit);
        statusBar()->showMessage(tr("Ready"));
    }

    void grab(QPoint globalPos);
};

void Magnifier::mouseMoveEvent(QMouseEvent* event) {
    QPixmap pixmap = grab();
    QImage image = pixmap.toImage();
    int x = event->x();
    int y = event->y();
    if (x>=0 && x<width() && y>=0 && y<height()) {
        QColor color = image.pixelColor(x, event->y());
        int red = color.red();
        int green = color.green();
        int blue = color.blue();
        QMainWindow* mwin = dynamic_cast<QMainWindow*>(parent()->parent());
        QString s = QString("%1, %2, %3 ").arg(red).arg(green).arg(blue);
        mwin->statusBar()->showMessage(s);
    }
    QLabel::mouseMoveEvent(event);
}

void MainWindow::grab(QPoint globalPos) {
    QScreen *screen = QApplication::screenAt(globalPos);
    if (screen) {
        int mw = magnifier->width();
        int mh = magnifier->height();
        int scrW = round(mw/scale);
        int scrH = round(mh/scale);
        int cx = globalPos.x();
        int cy = globalPos.y();
        int startX = cx-scrW/2;
        int startY = cy-scrH/2;
        startX = std::max(startX, 0);
        startY = std::max(startY, 0);
        if (startX+scrW > screen->geometry().right())
            startX = screen->geometry().right() - scrW;
        if (startY+scrH > screen->geometry().bottom())
            startY = screen->geometry().bottom() - scrH;
        const QPixmap screenshot = screen->grabWindow(0, startX, startY, scrW, scrH);
        magnifier->setPixmap(screenshot.scaled(mw, mh));
        magnifier->show();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent* event) {
    auto pos = event->pos();
    if (pos.x()>=0 && pos.x()<width() && pos.y()>=0 && pos.y()<height())
        ;
    else
        globalPos = event->globalPos();
    grab(globalPos);
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}

#include "main.moc"
