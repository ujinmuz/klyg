#include "mainwindow.h"
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QToolBar>
#include <QPixmap>
#include <QApplication>
#include <QIcon>
#include <QMessageBox>
#include <QFileDialog>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("KLyG"));  // Установить заголовок окна
    setMinimumSize(600,400);

    openGL2D = new GLWi(this);
    QWidget *secondPageWidget = new QWidget;

    {   // Создать меню
        QAction *m_quit = new QAction("&"+tr("Выход"), this);
        QAction *m_open = new QAction("&"+tr("Открыть"), this);
        QAction *m_save = new QAction("&"+tr("Сохранить"), this);
        QAction *m_save_png = new QAction("&"+tr("Сохранить в PNG"), this);

        QMenu *m_file;
        m_file = menuBar()->addMenu("&"+tr("Файл"));
        m_file->addAction(m_open);
        m_file->addAction(m_save);
        m_file->addAction(m_save_png);
        m_file->addAction(m_quit);

        connect(m_quit, SIGNAL(triggered()), qApp, SLOT(quit()));
        connect(m_open, SIGNAL(triggered()), this, SLOT(open()));
        connect(m_save, SIGNAL(triggered()), this, SLOT(save()));
        connect(m_save_png, SIGNAL(triggered()), openGL2D, SLOT(save_png()));

    }

    {  // Установка центральноговиджета







        stackedWidget = new QStackedWidget();

        stackedWidget->addWidget(openGL2D);
        stackedWidget->addWidget(secondPageWidget);
        stackedWidget->setCurrentIndex(0);

        setCentralWidget(stackedWidget);

    }

    { // создать Toolbar
        QPixmap addln(":/res/provedenie_granic.png");
        QPixmap rmln(":/res/udalenie_granic2.png");
        QPixmap d2(":/res/2d.png");
        QPixmap d3(":/res/3d.png");
        QPixmap plus(":/res/plus.png");
        QPixmap minus(":/res/minus.png");
        QPixmap center(":/res/graf_frag1.png");
        QPixmap centerN(":/res/graf_frag2.png");
        QPixmap calc(":/res/calc.png");
        QPixmap grid(":/res/grid.png");

        QToolBar *toolbar = addToolBar("main toolbar");
        m_2d = toolbar->addAction(QIcon(d2), "2D");
        m_3d = toolbar->addAction(QIcon(d3), "3D");
        m_2d->setCheckable(true);
        m_3d->setCheckable(true);

        m_2d->setChecked(true);
        toolbar->addSeparator();
        m_adl = toolbar->addAction(QIcon(addln), tr("Изменить"));
        m_rml = toolbar->addAction(QIcon(rmln), tr("Передвинуть"));
        m_adl->setCheckable(true);
        m_rml->setCheckable(true);
        m_sep2 = toolbar->addSeparator();
        lb1=new QLabel();
        lb1->setText(tr("Cлой"));
        lb1->setMargin(5);
        m_lbBox = toolbar->addWidget(lb1);
        spinBox = new QSpinBox(this);
        spinBox->setMaximum(0);
        m_sBox = toolbar->addWidget(spinBox);

        m_plus = toolbar->addAction(QIcon(plus), tr("Добавить"));
        m_minus = toolbar->addAction(QIcon(minus), tr("Удалить"));



        connect(m_plus, SIGNAL(triggered()), this, SLOT(plusClick()));
        connect(m_minus, SIGNAL(triggered()), this, SLOT(minusClick()));

        connect(spinBox, SIGNAL(valueChanged(int)),this, SLOT(chLayer(int)));

        connect(m_2d, SIGNAL(triggered()), this, SLOT(d2Click()));
        connect(m_3d, SIGNAL(triggered()), this, SLOT(d3Click()));
        connect(m_adl, SIGNAL(triggered()), this, SLOT(adlClick()));
        connect(m_rml, SIGNAL(triggered()), this, SLOT(rmlClick()));

        m_center = toolbar->addAction(QIcon(center), tr("Выровнять по слою"));
        m_centerN = toolbar->addAction(QIcon(centerN), tr("Выровнять по всем слоям"));

        connect(m_center, SIGNAL(triggered()), openGL2D, SLOT(CenterByCur()));
        connect(m_centerN, SIGNAL(triggered()), openGL2D, SLOT(CenterByAll()));

        m_calc = toolbar->addAction(QIcon(calc), tr("Считать"));
        connect(m_calc, SIGNAL(triggered()), openGL2D, SLOT(calc()));

        m_grid = toolbar->addAction(QIcon(grid), tr("Сетка"));
        connect(m_grid, SIGNAL(triggered()), openGL2D, SLOT(SwitchGrid()));
    }


    after2DClick();
}

MainWindow::~MainWindow()
{

}

void MainWindow::open()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("Открыть"));
    QFile f( filename );
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox m;
        m.setText(tr("Ошибка при открытии файла!"));
        m.exec();
        return;
    }
    QTextStream in(&f);
    openGL2D->loadFromFile(in);
    f.close();
    spinBox->setMaximum(openGL2D->getLayerCount()-1);
    spinBox->setValue(spinBox->minimum());
    openGL2D->CenterByAll();
}

void MainWindow::save()
{
    QString filename = QFileDialog::getSaveFileName(this,tr("Сохранить"));
    QFile f( filename );
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox m;
        m.setText(tr("Ошибка при открытии файла!"));
        m.exec();
        return;
    }
    QTextStream out(&f);
    openGL2D->saveToFile(out);

    f.close();

}

void MainWindow::d2Click()
{
    if (dstate==1)
    {
        m_2d->setChecked(true);
    }
    else
    {
        m_3d->setChecked(false);
        after2DClick();
    }
    dstate = 1;
}

void MainWindow::d3Click()
{
    if (dstate==2)
    {
        m_3d->setChecked(true);
    }
    else
    {
        m_2d->setChecked(false);
        after3DClick();
    }
    dstate = 2;
}

void MainWindow::adlClick()
{
    if (openGL2D->lstate==1)
    {
        openGL2D->lstate = 0;
    }
    else
    {
        m_rml->setChecked(false);
        openGL2D->lstate = 1;
    }

}

void MainWindow::rmlClick()
{
    if (openGL2D->lstate==2)
    {
        openGL2D->lstate = 0;
    }
    else
    {
        m_adl->setChecked(false);
        openGL2D->lstate = 2;
        openGL2D->selected_pid=-1;
    }

}


void MainWindow::after2DClick()
{
    dstate = 1;
    openGL2D->lstate = 0;
    m_adl->setVisible(true);
    m_rml->setVisible(true);
    m_plus->setVisible(true);
    m_minus->setVisible(true);
    m_sBox->setVisible(true);
    m_lbBox->setVisible(true);
    m_center->setVisible(true);
    m_centerN->setVisible(true);
    m_sep2->setVisible(true);
    m_adl->setChecked(false);
    m_rml->setChecked(false);
    m_calc->setVisible(true);
    m_grid->setVisible(true);
    stackedWidget->setCurrentIndex(0);
}

void MainWindow::after3DClick()
{
    dstate = 2;
    openGL2D->lstate = 0;
    m_adl->setVisible(false);
    m_rml->setVisible(false);
    m_adl->setChecked(false);
    m_rml->setChecked(false);
    m_plus->setVisible(false);
    m_sBox->setVisible(false);
    m_lbBox->setVisible(false);
    m_minus->setVisible(false);
    m_sep2->setVisible(false);
    m_center->setVisible(false);
    m_centerN->setVisible(false);
    m_calc->setVisible(false);
    m_grid->setVisible(false);
    stackedWidget->setCurrentIndex(1);

}

void MainWindow::plusClick()
{
    openGL2D->addNewLayer();
    int mx=openGL2D->getLayerCount()-1;
    spinBox->setMaximum(mx);

}



void MainWindow::minusClick()
{
    openGL2D->deleteLayerById(openGL2D->getLayerId());
    int mx=openGL2D->getLayerCount()-1;
    if (mx==-1)
    {
        openGL2D->addNewLayer();
        mx=0;
    }
    spinBox->setMaximum(mx);
    openGL2D->update();
}


void MainWindow::chLayer(int i)
{
    openGL2D->setLayerId(i);
    openGL2D->update();

}
