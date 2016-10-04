#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QDebug>
#include <QShortcut>

int         iarg;
int         iarg2;
std::string sarg;
int X1[4]= {0, 0, 0, 0};
int X2[4]= {0, 0, 0, 0};
int Y1[4]= {0, 0, 0, 0};
int Y2[4]= {0, 0, 0, 0};
int R[4]= {0, 0, 0, 0};
int rouge[4]= {0, 0, 0, 0};
int vert[4]= {0, 0, 0, 0};
int bleu[4]= {0, 0, 0, 0};
int epais[4]= {0, 0, 0, 0};


int channel = 61;



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    udpSocket = new QUdpSocket(this);
    udpSocket->bind(7002);

    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
    QShortcut* shortcutFS =  new QShortcut(QKeySequence(Qt::CTRL+Qt::Key_F), this);

    connect (shortcutFS,   SIGNAL(activated()), this, SLOT(fullscreen()));

    m_graphicsScene = new QGraphicsScene(this);
    m_graphicsScene->setSceneRect(0, 0, size().width(), size().height() );
    ui->view->setScene(m_graphicsScene);
    ui->view->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    ui->view->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );

    rect1 = new QGraphicsRectItem();
    m_graphicsScene->addItem(rect1);

    rect2 = new QGraphicsRectItem();
    m_graphicsScene->addItem(rect2);

    ellipse1= new QGraphicsEllipseItem();
    m_graphicsScene->addItem(ellipse1);

    ellipse2= new QGraphicsEllipseItem();
    m_graphicsScene->addItem(ellipse2);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::processPendingDatagrams()
{
PacketReader pr;
while (udpSocket->hasPendingDatagrams()) {
    oscpkt::Message *msg;
    QByteArray datagram;
    datagram.resize(udpSocket->pendingDatagramSize());
    udpSocket->readDatagram(datagram.data(), datagram.size());
    pr.init(datagram.data(), datagram.size());

    while (pr.isOk() && (msg = pr.popMessage()) != 0) {

/*        if (msg->match("/device").popStr(sarg).popInt32(iarg).popInt32(iarg2).isOkNoMoreArgs()) {
        if ((sarg == "/device/FOCUS/PAN")&&(channel<=iarg<channel+4))   pan  (iarg, iarg2);
       */

        if ((msg->match("/device/FOCUS/PAN").popInt32(iarg).popInt32(iarg2).isOkNoMoreArgs())&&(channel<=iarg<channel+4)) {pan(iarg, iarg2);}
        if ((msg->match("/device/FOCUS/TILT").popInt32(iarg).popInt32(iarg2).isOkNoMoreArgs())&&(channel<=iarg<channel+4)) {tilt (iarg, iarg2);}
        if ((msg->match("/device/BEAM/IRIS").popInt32(iarg).popInt32(iarg2).isOkNoMoreArgs())&&(channel<=iarg<channel+4)) {largeur  (iarg, iarg2);}
        if ((msg->match("/device/BEAM/FOCUS").popInt32(iarg).popInt32(iarg2).isOkNoMoreArgs())&&(channel<=iarg<channel+4)) {hauteur (iarg, iarg2);}
        if ((msg->match("/device/BEAM/FROST").popInt32(iarg).popInt32(iarg2).isOkNoMoreArgs())&&(channel<=iarg<channel+4))      {rotate(iarg, iarg2);}
        if ((msg->match("/device/COLOUR/RED").popInt32(iarg).popInt32(iarg2).isOkNoMoreArgs())&&(channel<=iarg<channel+4))     {red  (iarg, iarg2);}
        if ((msg->match("/device/COLOUR/GREEN").popInt32(iarg).popInt32(iarg2).isOkNoMoreArgs())&&(channel<=iarg<channel+4))   {green(iarg, iarg2);}
        if ((msg->match("/device/COLOUR/BLUE").popInt32(iarg).popInt32(iarg2).isOkNoMoreArgs())&&(channel<=iarg<channel+4))    {blue (iarg, iarg2);}
        if ((msg->match("/device/BEAM/ZOOM").popInt32(iarg).popInt32(iarg2).isOkNoMoreArgs())&&(channel<=iarg<channel+4))      {thickness(iarg, iarg2);}
           }
}
}

void MainWindow::pan (int ch, int value)
{
X1[ch-channel] = value;
ligneUpdate();
}

void MainWindow::tilt (int ch, int value)
{
Y1[ch-channel] = value;
ligneUpdate();
}
void MainWindow::largeur (int ch, int value)
{
X2[ch-channel] = value;
ligneUpdate();
}

void MainWindow::hauteur (int ch, int value)
{
Y2[ch-channel] = value;
ligneUpdate();
}

void MainWindow::rotate(int ch, int value)
{
R[ch-channel] = 360*value/65280;
ligneUpdate();
}

void MainWindow::red(int ch, int value)
{
rouge[ch-channel] = ((value+1)/256);
ligneUpdate();
}

void MainWindow::green(int ch, int value)
{
vert[ch-channel] = ((value+1)/256);
ligneUpdate();
}

void MainWindow::blue(int ch, int value)
{
bleu[ch-channel] = ((value+1)/256);
ligneUpdate();
}

void MainWindow::thickness(int ch, int value)
{
epais[ch-channel] = 2*value*size().width()/65000;
ligneUpdate();
}

void MainWindow::ligneUpdate()
{
    rect1->setRect(QRectF((X1[0]*size().width()/65280), (Y1[0]*size().height()/65280), (X2[0]*size().width()/65280), (Y2[0]*size().height()/65280)));
    rect1->setPen(QPen(QColor(rouge[0], vert[0], bleu[0], 255), epais[0]));
    rect1->setTransformOriginPoint(rect1->boundingRect().center());
    rect1->setRotation(R[0]);

    rect2->setRect(QRectF(((size().width()-X1[1]*size().width()/65280)-X2[1]*size().width()/65280), ((size().height()-Y1[1]*size().height()/65280)-Y2[1]*size().height()/65280), (X2[1]*size().width()/65280), (Y2[1]*size().height()/65280)));
    rect2->setPen(QPen(QColor(rouge[1], vert[1], bleu[1], 255), epais[1]));
    rect2->setTransformOriginPoint(rect2->boundingRect().center());
    rect2->setRotation(R[1]);

    ellipse1->setRect(QRectF(((size().width()-X1[2]*size().width()/65280)-X2[2]*size().width()/65280), (Y1[2]*size().height()/65280), (X2[2]*size().width()/65280), (Y2[2]*size().height()/65280)));
    ellipse1->setPen(QPen(QColor(rouge[2], vert[2], bleu[2], 255), epais[2]));
    ellipse1->setTransformOriginPoint(ellipse1->boundingRect().center());
    ellipse1->setRotation(R[2]);

    ellipse2->setRect(QRectF((X1[3]*size().width()/65280), ((size().height()-Y1[3]*size().height()/65280)-Y2[3]*size().height()/65280), (X2[3]*size().width()/65280), (Y2[3]*size().height()/65280)));
    ellipse2->setPen(QPen(QColor(rouge[3], vert[3], bleu[3], 255), epais[3]));
    ellipse2->setTransformOriginPoint(ellipse2->boundingRect().center());
    ellipse2->setRotation(R[3]);
}

void MainWindow::fullscreen()
{
    setWindowState(windowState() ^ Qt::WindowFullScreen);
}
