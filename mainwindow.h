#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<signal.h>

//#include<usuario.h>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
   void Crear_boton(QString name);
   void crear_lista_de_usuarios();
   QList<QString> Obtener_usuarios();
   void Crear_UserCard(QList<QString> usuarios);
   void Mostrar_usuarios(QList<QString> usuarios);
   QList<QString> MainWindow::Cortar_lista(int tam,QList<QString> usuarios);
   void limpiar_UserCard();
   int bucle(int k);
private slots:


    void on_pushButton_clicked();

    void on_boton_atras_clicked();

    void on_boton_adelante_clicked();

private:
    Ui::MainWindow *ui;
    static unsigned int indice;
    static unsigned int max;
     static unsigned int k;
    QStringList usuarios;


};
#endif // MAINWINDOW_H
