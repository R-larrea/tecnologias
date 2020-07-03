#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //ui->frame->setStyleSheet("");
    ui->boton_atras->hide();

    ui->boton_adelante->hide();

}

MainWindow::~MainWindow()
{
    delete ui;
}
//cambiar por funcion dentro de la clase
unsigned int MainWindow::indice=0; //cambiar por #def
unsigned int MainWindow::max=0;
void MainWindow::on_pushButton_clicked()
{
    crear_lista_de_usuarios();

}


void MainWindow::crear_lista_de_usuarios(){
    QList<QString> usuarios;
    usuarios=Obtener_usuarios();
    if(usuarios.size()>4){
        ui->boton_adelante->show();
    }
    max=usuarios.size();
    Crear_UserCard(usuarios);


}
QList<QString> MainWindow::Obtener_usuarios(){//esta funcion busca los archivos xml de cada usuario y los convierte en objeto,
                                  //por ahora solo crea una lista con los nombres
    //QList<QString> usuarios;
    QString t=ui->plain_user->toPlainText();
    int tam=t.toUInt();
    usuarios.append("ADMIN");
    for (int i=1;i<tam;i++) {

        usuarios.append("usuario"+QString::number(i));
    }




    return usuarios;
}
void MainWindow::Crear_UserCard(QList<QString> usuarios){

   QList<QString> aux;
   aux=usuarios.mid(indice,4);

   Mostrar_usuarios(aux);

}

void MainWindow::Mostrar_usuarios(QList<QString> aux){
    int i=0;

       for(i=0;i<aux.size();i++){

           Crear_boton(aux.at(i));

       }

}
void MainWindow::Crear_boton(QString name){
    QPushButton *button = new QPushButton();
    QPixmap icon(":/resources/resources/e.png");
      QIcon buttonicon(icon);
    button->setText(name);
    button->setIcon(buttonicon);
    button->setStyleSheet("background-color: #89A6FB");
    button->setIconSize(QSize(150,200));
    button->setFixedSize(250,300);

    ui->usersLayout->addWidget(button);

}



void MainWindow::on_boton_atras_clicked()
{
    indice=indice-4;
    ui->boton_adelante->show();
    if(indice<1){
        ui->boton_atras->hide();
    }
    limpiar_UserCard();
    Crear_UserCard(usuarios);
}


void MainWindow::on_boton_adelante_clicked()
{
    indice=indice+4;
    ui->boton_atras->show();
    if(indice>=(usuarios.size()-4)){
        ui->boton_adelante->hide();
    }

    limpiar_UserCard();
    Crear_UserCard(usuarios);
}
void MainWindow::limpiar_UserCard(){
   // QList<QPushButton *> allPButtons = ui->usersLayout->findChildren<QPushButton *>();
    QLayoutItem *child;
    while ((child = ui->usersLayout->takeAt(0)) != nullptr) {

        delete child->widget();
    }

}
