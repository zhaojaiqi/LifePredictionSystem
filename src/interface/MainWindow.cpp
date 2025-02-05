#include "MainWindow.h"

#include "page/pages.h"

MainWindow::MainWindow(QWidget* parent)
        : QMainWindow(parent)
        , m_stackedWidget(new QStackedWidget(this))
{
    setWindowTitle("机器寿命预测系统");
    setFixedSize(300, 200);

    auto welcomePage = new WelcomePage(this);
    auto modelSelectPage = new ModelSelectPage(this);

    m_stackedWidget->addWidget(welcomePage);
    m_stackedWidget->addWidget(modelSelectPage);

    connect(welcomePage, &WelcomePage::onBtnStartClicked, this, &MainWindow::switchToPage);

    setCentralWidget(m_stackedWidget);

    switchToPage(ePage::WELCOME);
}

void MainWindow::switchToPage(ePage index) {
    m_stackedWidget->setCurrentIndex(static_cast<int>(index));
}
