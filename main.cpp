#include <QApplication>
#include <QScreen>
#include <QDesktopWidget>
#include <QMouseEvent>
#include <QLabel>
#include <QSplitter>
#include <QMainWindow>
#include <QMenuBar>
#include <QAction>

class Magnifier : public QLabel {
Q_OBJECT
public:
    explicit Magnifier(QWidget* parent = nullptr)
            : QLabel(parent)
    {
    }
};

class MainWindow : public QMainWindow {
Q_OBJECT
    Magnifier *magnifier;
protected:
    void mouseMoveEvent(QMouseEvent* event) override;
public:
    explicit MainWindow(QWidget* parent = nullptr)
            : QMainWindow(parent)
    {
        magnifier = new Magnifier();
        setCentralWidget(magnifier);
        // Tworzy menu.
        QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
        QAction *exitAction = fileMenu->addAction(tr("E&xit"), this, &QWidget::close);
        exitAction->setShortcuts(QKeySequence::Quit);
    }
};

void MainWindow::mouseMoveEvent(QMouseEvent* event) {
    QScreen *screen = QApplication::screenAt(event->globalPos());
    if (screen) {
        const QPixmap screenshot = screen->grabWindow(0, event->globalX() - 100, event->globalY() - 100, 200, 200);  // Dostosuj do swojej lupy.
        magnifier->setPixmap(screenshot.scaled(magnifier->width(), magnifier->height()));
        magnifier->show();
    }
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}

#include "main.moc"
