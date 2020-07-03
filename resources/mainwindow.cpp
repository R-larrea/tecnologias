
#include "mainwindow.h"
#include "ui_mainwindow.h"
//                            0   1	  2   3   4   5   6   7   8   9
char diccionario[63]={ 	     ' ','/','0','@','0','0','0','0','0','0'//0
                            ,'0','0','0','0','0','#',',','0',':','.'//1
                            ,'i','0','j','w',',','?','!','0','s','0'//2
                            ,'t','0','a','0','e','0','c','0','d','0'//3
                            ,'k','u','o','z','m','x','n','y','b','0'//4
                            ,'h','0','f','0','g','w','l','v','r','0'//5
                            ,'p','0','q'};							//6

char LtoN[10]= {'j','a','b','c','d','e','f','g','h','i'};
//char Letras[26]={ '32','48','36','38','34','52','54'
  //               ,'50','26','22','40','56','44','46','42','60','62','58','28','30','41','57','55','45','47','43'};



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Port = NULL;    //indica que el objeto puerto no esta creado;
    Portname = "";
    EnumerarPuertos();
    QPixmap pix_abc("C:/Users/alumno/Desktop/interfaz/brl/abc.gif");
    ui->label_abc->setPixmap(pix_abc.scaled(787,400,Qt::IgnoreAspectRatio));

    inicializar();
}

void MainWindow::inicializar()
{

    ui->boton_sig->setEnabled(false);
    ui->boton_atras->setEnabled(false);
    ui->textEdit_pal->setEnabled(false);
    ui->plain_escribir_text->setEnabled(false);
    ui->plainText_recibido->setVisible(false);

    ui->textEdit_brl->setEnabled(false);

    //ventana ayuda

}

int MainWindow::index=0;
int MainWindow::cant=0;
MainWindow::~MainWindow()
{
    delete ui;
    delete Port;
}



//tab conectar
void MainWindow::on_boton_conectar_clicked()
{
    if (!Port)
        {

            Port = new QSerialPort(Portname);
          //  Port->setBaudRate(QSerialPort::Baud9600);
            //Port->setFlowControl(QSerialPort::NoFlowControl);
            //Port->setParity(QSerialPort::NoParity);
           // Port->setDataBits(QSerialPort::Data8);
            //Port->setStopBits(QSerialPort::OneStop);
            if(!Port->open(QIODevice::ReadWrite))
                {
                     QMessageBox::critical(this,"Error","No se puede abrir el puerto "+Port->portName());
                     delete Port;
                     Port = nullptr;

                }
            if(!Port->setBaudRate(QSerialPort::Baud9600))
                {
                   qDebug()<<"Error de Baudrate";
                }
            if(!Port->setDataBits(QSerialPort::Data8))
               {
                   qDebug()<<"Error de Databits";
               }
            if(!Port->setFlowControl(QSerialPort::NoFlowControl))
                {
                   qDebug()<<"Error de Control de flujo";
                }
             if(!Port->setParity(QSerialPort::NoParity))
                {
                   qDebug()<<"Error de Paridad";
                }
            if(!Port->setStopBits(QSerialPort::OneStop))
                {
                   qDebug()<<"Error de stopbits";
                }
            else
                {
                    ui->boton_conectar->setText("Desconectar");
                    connect(Port,SIGNAL(readyRead()),this,SLOT(Bandeja_entrada()));
                }
        }
        else
        {

            Port = NULL;
            ui->boton_conectar->setText("mjghfng");
            delete Port;
        }


        //connect(ui->boton_conectar,Test_window->accepted(),this,display_status());


}
void MainWindow::on_comboBox_ports_currentIndexChanged(int index)
{
    Portname = ui->comboBox_ports->itemData(index).toString();    //recupero la data del item de indice index
}
void MainWindow::EnumerarPuertos()
{
    ui->comboBox_ports->clear();

    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();

    for (int i = 0; i < ports.size(); i++)
    {
        ui->comboBox_ports->addItem(ports.at(i).portName(), ports.at(i).portName());
    }
}
void MainWindow::on_boton_refrescar_clicked()
{
    EnumerarPuertos();

}


//tab leer
void MainWindow::on_boton_Abrir_clicked()
{
    QString filename=QFileDialog::getOpenFileName(
                       this,
                       tr("Abrir archivo"),
                       "/home/adrian/Documentos/leer", //path que abre
                       "All files (*.*);;Text File (*.txt);" //veo todos los archivos .txt

                       );
        QFile file(filename);
       if(!file.open(QFile::ReadOnly ))
       {
          QMessageBox::warning(this,"Error de Lectura","El archivo no pudo abrirse");

       }
       QTextStream in(&file);
       QString   text = in.readAll();
       ui->textEdit_texto->setPlainText(text);
       file.close();

}
void MainWindow::on_boton_crear_clicked()
{
    QString newfilename = QFileDialog::getSaveFileName(this,tr("Guardar archivo"),"/home/adrian/Documentos/leer/","*.txt",nullptr);


        QFile file(newfilename);

       if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
       {
          QMessageBox::warning(this,"Error de Escritura","Archivo no guardado");


       }
       QTextStream out(&file);
       QString   text = ui->textEdit_texto->toPlainText();
       out<<text;
       file.close();
}
void MainWindow::on_boton_Enviar_clicked()
{

    QByteArray aux;



    aux.append(ui->textEdit_brl->toPlainText());

    Trasmitir(aux);


}
void MainWindow::on_boton_sig_clicked()
{
   if(index==(cant-2))

   {
       ui->boton_sig->setEnabled(false);
   }

    ui->boton_atras->setEnabled(true);
    Obtener_palabra(List,cant,ADELANTE);

}
void MainWindow::on_boton_atras_clicked()
{
    if(index==1)

    {
        ui->boton_atras->setEnabled(false);
    }

     ui->boton_sig->setEnabled(true);
     Obtener_palabra(List,cant,ATRAS);
}
void MainWindow::Obtener_palabra(QStringList List,int tam,int direccion)
{

    QString aux2;
    if(direccion==ADELANTE)
     {
        if(index==(tam-1))//esta en el ultimo elemento
        {
            aux2=traducir(List[index]);
            ui->textEdit_pal->setPlainText(List[index]);
            ui->textEdit_brl->setPlainText(aux2);
        }else
        {
            index++;
            aux2=traducir(List[index]);
            ui->textEdit_pal->setPlainText(List[index]);
            ui->textEdit_brl->setPlainText(aux2);


        }
     }
    if(direccion==ATRAS)
    {
        if(index==0)//primer elemento
        {
            aux2=traducir(List[index]);
            ui->textEdit_pal->setPlainText(List[index]);
            ui->textEdit_brl->setPlainText(aux2);

        }else
        {
            index--;
            aux2=traducir(List[index]);
            ui->textEdit_pal->setPlainText(List[index]);
            ui->textEdit_brl->setPlainText(aux2);
        }
    }



    //List= text.split(QRegularExpression("\\s+"),QString::SkipEmptyParts);






    return ;
}
void MainWindow::on_boton_confirmar_clicked()
{
    //llamar correccion
    QString text,aux,aux2;
    ui->textEdit_pal->clear();
    ui->textEdit_brl->clear();
    aux.append(ui->textEdit_texto->toPlainText());
    text=puntuacion(aux);
    if(!corrector_pal(text))
    {
        QMessageBox::warning(this,"Error","Por favor verifique que los caracteres ingresados sean validos");
    }
    else
    {
        List = text.split(" ", QString::SkipEmptyParts);
        cant=List.count();

        if(!cant)
        {
            QMessageBox::warning(this,"Error de Lectura","El espacio esta vacio");
        }else
        {
            if(cant==1)
            {
                aux2=traducir(List[0]);
                ui->boton_sig->setEnabled(false);
                ui->boton_atras->setEnabled(false);
                ui->textEdit_pal->setPlainText(List[0]);
                ui->textEdit_brl->setPlainText(aux2);
            }else
            {
                aux2=traducir(List[index]);
                ui->textEdit_pal->setPlainText(List[index]);
                ui->textEdit_brl->setPlainText(aux2);
                ui->boton_sig->setEnabled(true);
            }
        }
        }
}
void MainWindow::Trasmitir(QByteArray aux)
{
    if(Port)
        {

            //añadir funcion que separa palabras
            //palabra_actual=pal_actual(int pal_index);
            //palaux=armartrama(palabra actual)
            Port->write("&");
            Port->write("1");

            Port->write(aux/*palaux*/);
            Port->write("$");



        }
        else
        {
            QMessageBox::critical(this,"Error",QString::fromLatin1("No hay ningun puerto abierto"));
        }
}



//tab escribir
void MainWindow::Bandeja_entrada()
{
    QByteArray aux;

    aux.resize(Port->bytesAvailable());
    Port->read(aux.data(),aux.size());
    //imprimir aux procesar trama()
  //  ui->plainText_recibido->appendpla
    ui->plainText_recibido->appendPlainText("Recibi:""\n");
    ui->plainText_recibido->appendPlainText(aux+"\n");
    procesar_trama(aux);
}
void MainWindow::procesar_trama(QByteArray aux)
{

        //ui->plain_escribir_text->appendPlainText("aux:"+(QString)aux);
        int limite=aux.size();

        QString palabra;
        //static int recibo_lsb = 1;

        for (int i=0;i<limite;i++)
        {
            static int estado = ESPERANDO;

            switch (estado) {
            case ESPERANDO:



                if(aux.at(i)=='&')
                {

                    estado=RECIBIENDO;
                }

                break;
            case RECIBIENDO:

                if(aux.at(i) == '&')
                {
                    estado=ESPERANDO;
                    //valor = valor_msb*256 + valor_lsb ;


                    ui->plainText_recibido->appendPlainText((QString)aux.at(i));
                    ui->plainText_recibido->appendPlainText("\nFin de la recepcion\n");

                }

                if(aux.at(i) != '&' && aux.at(i)!='$')
                {
                    palabra.append(aux.at(i));
                }
                if(aux.at(i) == '$')//termino
                {
                    estado=ESPERANDO;
                    //valor = valor_msb*256 + valor_lsb ;


                    ui->plain_escribir_text->appendPlainText(" "+procesar_palabra(palabra));
                    ui->plainText_recibido->appendPlainText("Fin de la recepcion\n");


                    ui->plainText_recibido->appendPlainText(("palabra "+palabra));


                }

                break;

            default:
                estado = ESPERANDO;


                break;
            }
        }
}
QString MainWindow::procesar_palabra(QString(palabra))
{
    int num=0,j=0;

    QString aux;

        if((palabra[0]=="/") &&(palabra[1]=="/"))
        {

                palabra.remove(0,2);
                aux=palabra.toUpper();



        }else
        if(palabra[0]=="#")
        {
            palabra.remove(0,1);

            for(j=0;j<palabra.size();j++)
            {
                num=LeterstoNumbers(palabra[j]);
                aux[j]='0'+num;
            }
        }else
        {
            for(j=0;j<palabra.size();j++)
            {
                aux=palabra;
            }
        }



    return aux;
}
void MainWindow::on_boton_prueba_clicked()
{
    QString aux;
    QByteArray data;
    aux=ui->plain_prueba->toPlainText();
    data=aux.toUtf8();
    procesar_trama("&"+data+"$");

}
void MainWindow::on_boton_limpiar_clicked()
{
    ui->plain_prueba->clear();
    ui->plainText_recibido->clear();
    ui->plain_escribir_text->clear();
}
void MainWindow::on_boton_Trasmitir_clicked()
{
    //QByteArray aux="&2$";
    ui->plain_prueba->clear();
    ui->plainText_recibido->clear();
    ui->plain_escribir_text->clear();

    Port->write("&");
    Port->write("2");
    Port->write("$");

    ui->boton_Trasmitir->setText("REINICIAR");
}
void MainWindow::on_boton_crear_2_clicked()
{
    QString newfilename = QFileDialog::getSaveFileName(this,tr("Guardar archivo"),"/home/adrian/Documentos/leer/","*.txt",nullptr);


        QFile file(newfilename);

       if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
       {
          QMessageBox::warning(this,"Error de Escritura","Archivo no guardado");


       }
       QTextStream out(&file);
       QString   text = ui->plain_escribir_text->toPlainText();
       out<<text;
       file.close();
}


//funciones adicionales
int MainWindow::verificar_caracter(QChar c)
{

    int codigo=ERROR,i,aux=0;
   // ui->plain_escribir_text->appendPlainText("aux al inicio:"+(QString)c);
    for (i=0;i<=63;i++)
    {
        if(c==diccionario[i])
        {
            aux=i;

        }
    }
 //ui->plain_escribir_text->appendPlainText("aux:"+(QString)aux);
    if(aux)//el caracter esta permitido
    {
        if(c.isLetter())
        {
            codigo=LETERS;
        }
        else if(c=='#')
        {
            codigo=NUMBERS;
        }
        else if(c=='^')
        {
            codigo=CAPS_ON1;
        }
        else
        {
            codigo=SIMBOLS;
        }


    }




    return codigo;
}
int MainWindow::LeterstoNumbers(QChar c)
{
    int num,j;
    for (j=0;j<=10;j++)
    {
        if(c==LtoN[j])
        {
            num=j;
        }

    }
    return num;
}
int MainWindow::corrector_pal(QString text)
{
    int aux=1,i=0,j=0,check=0;
    for (i=0;i<=text.size();i++)
     {

        for (j=0;j<=63;j++)
        {
            if( text[i]>= '0' && text[i]<= '9')
            {
                check=1;
            }
            else if(text[i].isLetter())
            {
                check=1;

            }
            else if(text[i]==diccionario[j])
            {
                check=1;
            }

        }

        if(check==0)
        {
            aux=0;
            return aux;
        }

     }
    return aux;
}
QString MainWindow::puntuacion(QString text)
{
    int i=0,cont=1;
    QString aux;
    aux=text;

    for (i=0;i<text.size();i++)
    {

            if(text[i]=='.')
            {
                aux.insert(i+cont," ");
                cont++;
            }
            if(text[i]==',')
            {
                aux.insert(i+cont," ");
                cont++;
            }
            if(text[i]==';')
            {
                aux.insert(i+cont," ");
                cont++;
            }
            if(text[i]==':')
            {
                aux.insert(i+cont," ");
                cont++;
            }
            if(text[i]=='?')
            {
                aux.insert(i+cont," ");
                cont++;
            }
            if(text[i]=='!')
            {
                aux.insert(i+cont," ");
                cont++;
            }

    }
    ui->textEdit_texto->setPlainText(aux);
    return aux;
}
QString MainWindow::traducir(QString text)
{
    int i=0,j=0,car2;

    QString aux,car;
   //if(text.isLower())

    if(isMin(text))//solo son minisculas
    {
        aux=text;
    }
    else
    //if(text.isUpper())
    if(isMay(text))//si solo son mayusculas
    {
        aux.append("//");

        aux.append(text.toLower());

    }

    else if(text[0].isNumber())
        {
            aux.append("#");
            for (j=0;j<text.size();j++)
            {
                   car=text[j];
                   car2=car.toInt();
                   aux.append(LtoN[car2]);
            }

        }else
            {
                for (i=0;i<text.size();i++)
                   {
                        if(text[i].isUpper())
                            {
                                aux.append("/");
                                aux.append(text[i]);
                            }
                        else if(text[i].isNumber())
                                {
                                    aux.append("#");
                                    aux.append(text[i]);
                                 }
                        else
                            {
                                aux.append(text[i]);
                            }

                   }
            }



    return aux;
}





int MainWindow::isMin(QString text)
{

    int i=0,cont=0,aux=0;
    for(i=0;i<text.size();i++)
    {
        if(text[i]>='a'&& text[i]<='z')
        {
            cont++;

        }
    }
    if(cont==text.size())
      {
        aux=1;
      }
    return aux;
}
int MainWindow::isMay(QString text)
{

    int i=0,cont=0,aux=0;
    for(i=0;i<text.size();i++)
    {
        if(text[i]>='A'&& text[i]<='Z')
        {
            cont++;

        }
    }
    if(cont==text.size())
      {
        aux=1;
      }
    return aux;
}






