#include "tictactoemainwindow.h"
#include "ui_tictactoemainwindow.h"

#include <QtGui>
#include <QPalette>
#include<QMessageBox>
#include<QFont>
#include<QToolBar>

const int DEFAULT_X_OFFSET= 100;
const int DEFAULT_Y_OFFSET= 100;
const int DEFAULT_WIDTH  =  100;
const int DEFAULT_HEIGHT =  100;


TicTacToeMainWindow::TicTacToeMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TicTacToeMainWindow)
{
    ui->setupUi(this);
    Initialize();
    m_pClientSocket=new QTcpSocket(this);

}

TicTacToeMainWindow::~TicTacToeMainWindow()
{
    delete ui;
}


void TicTacToeMainWindow::paintEvent(QPaintEvent *pEvent)
{
    QWidget::paintEvent(pEvent);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::red);
     QFont font;
     font.setPixelSize(22);
     painter.setFont(font);
    painter.setPen(Qt::blue);

    for (int iRowIndex = 0; iRowIndex<m_NoOfRows; iRowIndex++)
    {
        for (int iColIndex = 0; iColIndex<m_NoOfCols; iColIndex++)
        {
            painter.drawRect(m_CellArray[iRowIndex][iColIndex].m_CellRect);

            if(m_CellArray[iRowIndex][iColIndex].m_strText!="i")
            {
                painter.drawText(m_CellArray[iRowIndex][iColIndex].m_CellRect.left()-m_Height/2,
                                m_CellArray[iRowIndex][iColIndex].m_CellRect.top()-m_width/2,
                                m_CellArray[iRowIndex][iColIndex].m_strText);
            }

        }
    }

}

void TicTacToeMainWindow ::Initialize()
{
     m_NoOfRows=DEFAULT_NO_ROWS;
     m_NoOfCols=DEFAULT_NO_COLS;
     m_width=DEFAULT_WIDTH;
     m_Height=DEFAULT_HEIGHT;
     m_XOffset=DEFAULT_X_OFFSET;
     m_YOffset=DEFAULT_Y_OFFSET;
     //Initialize the Rectange

    for (int iRowIndex = 0; iRowIndex<m_NoOfRows; iRowIndex++)
    {
        for (int iColIndex = 0; iColIndex<m_NoOfCols; iColIndex++)
        {
             m_CellArray[iRowIndex][iColIndex].m_CellRect.setSize(QSize(m_width, m_Height));
             m_CellArray[iRowIndex][iColIndex].m_CellRect.setTopLeft(QPoint(m_XOffset + (m_width*(iColIndex+1)), m_YOffset + (m_Height*(iRowIndex+1))));

        }
    }
}

QString TicTacToeMainWindow ::CreateMessage()
{
    m_Message="";
    for(int i=0;i<DEFAULT_NO_ROWS;i++)
    {
        for(int j=0;j<DEFAULT_NO_COLS;j++)
        {
            m_Message+=m_CellArray[i][j].m_strText;
        }
    }
    return m_Message;
}






void TicTacToeMainWindow::DisplayStatusMessage(QString Message)
{
    ui->labelResult->setText(Message);
}

void TicTacToeMainWindow::SendMovesToRemotePC(QString Message)
{
    //This is a simple protocol message ie Just storing only cell moves
    //User might need to expand the protocol struct
    //to accomodate other settings paramaters eg Message="GridSize"+"#"+"X or Y"+"#"+Message;
    //based on the requirements
    m_pClientSocket->write(QString(Message + "\n").toUtf8());
}
void TicTacToeMainWindow::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError)
    {
            case QAbstractSocket::RemoteHostClosedError:
                break;
            case QAbstractSocket::HostNotFoundError:
                QMessageBox::information(this, tr("Fortune Client"),
                                         tr("The host was not found. Please check the "
                                            "host name and port settings."));
                break;
            case QAbstractSocket::ConnectionRefusedError:
                QMessageBox::information(this, tr("Fortune Client"),
                                         tr("The connection was refused by the peer. "
                                            "Make sure the fortune server is running, "
                                            "and check that the host name and port "
                                            "settings are correct."));
                break;
            default:
                QMessageBox::information(this, tr("Fortune Client"),
                                         tr("The following error occurred: %1.")
                                         .arg(m_pClientSocket->errorString()));
            }

}
void TicTacToeMainWindow::reset()
{
    for(int i=0;i<DEFAULT_NO_ROWS;i++)
        {
            for(int j=0;j<DEFAULT_NO_COLS;j++)
            {
                m_CellArray[i][j].m_strText="";

                this->repaint();
            }

        }
}

void TicTacToeMainWindow::CheckWinner()

{	if ((m_CellArray[0][0].m_strText == 'X' && m_CellArray[0][1].m_strText == 'X'&&m_CellArray[0][2].m_strText == 'X')||
            (m_CellArray[1][0].m_strText == 'X' && m_CellArray[1][1].m_strText == 'X' && m_CellArray[1][2].m_strText == 'X') ||
            (m_CellArray[2][0].m_strText == 'X' && m_CellArray[2][1].m_strText == 'X' && m_CellArray[2][2].m_strText == 'X') ||
            (m_CellArray[0][0].m_strText == 'X' && m_CellArray[1][1].m_strText == 'X' && m_CellArray[2][2].m_strText == 'X') ||
            (m_CellArray[0][2].m_strText == 'X' && m_CellArray[1][1].m_strText == 'X' && m_CellArray[2][0].m_strText == 'X') ||
            (m_CellArray[0][0].m_strText == 'X' && m_CellArray[1][0].m_strText == 'X' && m_CellArray[2][0].m_strText == 'X') ||
            (m_CellArray[0][1].m_strText == 'X' && m_CellArray[1][1].m_strText == 'X' && m_CellArray[2][1].m_strText == 'X') ||
            (m_CellArray[0][2].m_strText == 'X' && m_CellArray[1][2].m_strText == 'X' && m_CellArray[2][2].m_strText == 'X'))
   {



     QMessageBox::information(this, tr("Message"),
                              tr("X Win."));
     QMessageBox::information(this, tr("Message"),
                              tr(" GameOver"));
    newGame();
    }

       if ((m_CellArray[0][0].m_strText == 'O' && m_CellArray[0][1].m_strText == 'O' &&m_CellArray[0][2].m_strText == 'O')||
              (m_CellArray[1][0].m_strText == 'O' && m_CellArray[1][1].m_strText == 'O' && m_CellArray[1][2].m_strText == 'O') ||
              (m_CellArray[2][0].m_strText == 'O' && m_CellArray[2][1].m_strText == 'O' && m_CellArray[2][2].m_strText == 'O') ||
              (m_CellArray[0][0].m_strText == 'O' && m_CellArray[1][1].m_strText == 'O' && m_CellArray[2][2].m_strText == 'O') ||
              (m_CellArray[0][2].m_strText == 'O' && m_CellArray[1][1].m_strText == 'O' && m_CellArray[2][0].m_strText == 'O') ||
              (m_CellArray[0][0].m_strText == 'O' && m_CellArray[1][0].m_strText == 'O' && m_CellArray[2][0].m_strText == 'O') ||
              (m_CellArray[0][1].m_strText == 'O' && m_CellArray[1][1].m_strText == 'O' && m_CellArray[2][1].m_strText == 'O') ||
              (m_CellArray[0][2].m_strText == 'O' && m_CellArray[1][2].m_strText == 'O' && m_CellArray[2][2].m_strText == 'O'))
   {

     QMessageBox::information(this, tr("Message"),
                              tr(" O Win."));

     QMessageBox::information(this, tr("Message"),
                              tr(" GameOver"));
   newGame();
    }


}
void TicTacToeMainWindow::newGame()
{
    clickCount = 0;
       alreadyClicked = false;
       for(int i=0;i<m_NoOfRows;i++)
       {
           for(int j=0;j<m_NoOfCols;j++)
           {
               m_CellArray[i][j].m_strText = "";
           }
       }

       for(int i=0;i<m_NoOfRows;i++)
       {
           for(int j=0;j<m_NoOfCols;j++)
           {
               statusX[i][j] = 0;
           }
       }

       for(int i=0;i<m_NoOfRows;i++)
       {
           for(int j=0;j<m_NoOfCols;j++)
           {
               statusO[i][j] = 0;
           }
       }

}
void TicTacToeMainWindow::on_pushButtonStart_clicked()
{
    if(ui->pushButtonStart->text()=="Start")
    {
         m_pBoxServer= new HelloWorldServer(this);
        bool success = m_pBoxServer->listen(QHostAddress::Any, quint16(ui->plainTextEditPort->toPlainText().toInt()));
        if(!success)
        {
            DisplayStatusMessage("Server failed...");
        }
        else
        {
            DisplayStatusMessage("Server Started...");
            this->setWindowTitle(this->windowTitle()+"|"+ui->plainTextEditPort->toPlainText());
            ui->pushButtonStart->setText("Stop");
        }

    }
    else
    {
        m_pBoxServer->close();
        DisplayStatusMessage("Server Stopped...");
        ui->pushButtonStart->setText("Start");
    }

}
void TicTacToeMainWindow::on_pushButtonConnect_clicked()
{
    try
    {
        if(ui->pushButtonConnect->text()=="Connect")
        {
            m_pClientSocket->connectToHost(ui->plainTextEditRemoteIPAddr->toPlainText(),quint16(ui->plainTextEditRemotePort->toPlainText().toInt()) );
                            connect(m_pClientSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
                            ui->pushButtonConnect->setText("Disconnect");
                            DisplayStatusMessage("Connected to Host =>" + ui->plainTextEditRemoteIPAddr->toPlainText() + "Port =>" + ui->plainTextEditRemotePort->toPlainText());
                            QMessageBox::information(this,"Message","Connection Successfull..");
        }
        else
        {
             m_pClientSocket->disconnectFromHost();
             m_pClientSocket->close();
             ui->pushButtonConnect->setText("Connect");
             DisplayStatusMessage("Disconnected from Host =>" + ui->plainTextEditRemoteIPAddr->toPlainText());
             QMessageBox::information(this,"Message","Disconnect Successfull..");
        }
    }
    catch(QException* ex)
    {
        DisplayStatusMessage(QString("Error in Connection=>") + QString(ex->what()));
    }
}



void TicTacToeMainWindow::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);


    if (event->button() == Qt::LeftButton)
    {
        for(int i=0;i<m_NoOfRows;i++)
        {
            for(int j=0;j<m_NoOfCols;j++)
            {

                QRect widgetRect =m_CellArray[i][j].m_CellRect;
                widgetRect.moveTopLeft(this->parentWidget()->mapToGlobal(widgetRect.topLeft()));
                QPoint CurretPoint=event->pos();
                if(widgetRect.contains(CurretPoint) && alreadyClicked == false)
                {
                    clickCount++;
                    alreadyClicked = true;
                    m_CellArray[i][j].m_strText="X";
                    statusX[i][j] = 1;

                    this->repaint();

                    if(i == 0 && j == 0){
                        SendMovesToRemotePC("00");

                    }
                    if(i == 0 && j == 1){
                        SendMovesToRemotePC("01");
                    }
                    if(i == 0 && j == 2){
                        SendMovesToRemotePC("02");
                    }
                    if(i == 1 && j == 0){
                        SendMovesToRemotePC("10");
                    }
                    if(i == 1 && j == 1){
                        SendMovesToRemotePC("11");
                    }
                    if(i == 1 && j == 2){
                        SendMovesToRemotePC("12");
                    }
                    if(i == 2 && j == 0){
                        SendMovesToRemotePC("20");
                    }
                    if(i == 2 && j == 1){
                        SendMovesToRemotePC("21");
                    }
                    if(i == 2 && j == 2){
                        SendMovesToRemotePC("22");
                    }
                    CheckWinner();
                    return;

                }
                else if(alreadyClicked == true){
                    QMessageBox::information(this,tr("Warning"),tr("Wait for Opponent to play"));
                    return;
                }
            }
        }
    }

}

void TicTacToeMainWindow::DisplayRemotePCMessage(QString Message)
{
    if(Message == "00"){
        m_CellArray[0][0].m_strText="O";
        statusO[0][0]=1;
        clickCount++;
    }
    if(Message == "01"){
        m_CellArray[0][1].m_strText="O";
        statusO[0][1]=1;
        clickCount++;
    }
    if(Message == "02"){
        m_CellArray[0][2].m_strText="O";
        statusO[0][2]=1;
        clickCount++;
    }
    if(Message == "10"){
        m_CellArray[1][0].m_strText="O";
        statusO[1][0]=1;
        clickCount++;
    }
    if(Message == "11"){
        m_CellArray[1][1].m_strText="O";
        statusO[1][1]=1;
        clickCount++;
    }
    if(Message == "12"){
        m_CellArray[1][2].m_strText="O";
        statusO[1][2]=1;
        clickCount++;
    }
    if(Message == "20"){
        m_CellArray[2][0].m_strText="O";
        statusO[2][0]=1;
        clickCount++;
    }
    if(Message == "21"){
        m_CellArray[2][1].m_strText="O";
        statusO[2][1]=1;
        clickCount++;
    }
    if(Message == "22"){
        m_CellArray[2][2].m_strText="O";
        statusO[2][2]=1;
        clickCount++;
    }

   CheckWinner();
    alreadyClicked = false;
    this->repaint();

}

void TicTacToeMainWindow::on_actionNew_Game_triggered()
{
   newGame();
    reset();
}




