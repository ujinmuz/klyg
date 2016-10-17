#include "glwi.h"
#include "l19maxwell.h"

#include <QtDebug>
#include <QPainter>
#include <QGLFormat>
#include <QToolTip>
#include <QOpenGLFramebufferObjectFormat>
#include <QOpenGLFramebufferObject>
#include <QFileDialog>
#include <QDir>
#include <string>

GLWi::GLWi(QWidget *parent) : QOpenGLWidget(parent)
{
    showgrid = false;
    setAutoFillBackground(false);  
    setMouseTracking(true);
    cent_x = 0;
    cent_y = 0;
    mash = 1;
    this->setFocusPolicy(Qt::StrongFocus);
    selected_pid=-1;
    selected_lay=-1;

    Cnt = new ContourCollection();

    Cnt->addNew();
    selected_lay=0;
//    Cnt->getById(selected_lay)->addXY(0.0f,1.0f);  //0
//    Cnt->getById(selected_lay)->addXY(0.3f,1.0f);  //1
//    Cnt->getById(selected_lay)->addXY(0.8f,0.7f);  //2
//    Cnt->getById(selected_lay)->addXY(0.8f,0.5f);  //3
//    Cnt->getById(selected_lay)->addXY(0.5f,0.4f);  //4
//    Cnt->getById(selected_lay)->addXY(0.6f,0.5f);  //5
//    Cnt->getById(selected_lay)->addXY(0.2f,0.35f);  //6
//    Cnt->getById(selected_lay)->addXY(1.0f,0.0f);  //7
//    Cnt->getById(selected_lay)->addXY(1.5f,0.0f);  //8
//    Cnt->getById(selected_lay)->addXY(2.0f,-0.2f);  //9
//    Cnt->getById(selected_lay)->addXY(2.3f,-0.3f);  //10
//    Cnt->getById(selected_lay)->addXY(2.2f,-0.5f);  //11
//    Cnt->getById(selected_lay)->addXY(1.9f,-0.7f);  //12
//    Cnt->getById(selected_lay)->addXY(1.8f,-0.7f);  //13
//    Cnt->getById(selected_lay)->addXY(1.9f,-0.9f);  //14
//    Cnt->getById(selected_lay)->addXY(2.2f,-1.2f);  //15
//    Cnt->getById(selected_lay)->addXY(2.8f,-1.5f);  //16

    Cnt->getById(selected_lay)->addXY(1.5f,-1.5f);
    Cnt->getById(selected_lay)->addXY(0.5f,-1.5f);
    Cnt->getById(selected_lay)->addXY(0.5f,-1.0f);
    Cnt->getById(selected_lay)->addXY(1.5f,-1.0f);
    Cnt->getById(selected_lay)->addXY(1.5f,-0.5f);
    Cnt->getById(selected_lay)->addXY(0.5f,-0.5f);
    Cnt->getById(selected_lay)->addXY(0.5f,0.0f);
    Cnt->getById(selected_lay)->addXY(1.5f,0.0f);
    Cnt->getById(selected_lay)->addXY(1.5f,0.5f);
    Cnt->getById(selected_lay)->addXY(0.5f,0.5f);
    Cnt->getById(selected_lay)->addXY(0.5f,1.0f);
    Cnt->getById(selected_lay)->addXY(1.5f,1.0f);


    lnpresX =0;
    lnpresY =0;
    xLines.clear();
    yLines.clear();

}


void GLWi::initializeGL()
{
    initializeOpenGLFunctions();
    initShaders();    
    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);
    // Enable back face culling
    //glEnable(GL_CULL_FACE);

}



static const char *vertexShaderSource =
        "attribute vec2 coord;\n"
        "uniform highp mat4 mvp_matrix;\n"
        "void main() {\n"
        "   gl_Position = mvp_matrix * vec4(coord, 0.0, 1.0);\n"
        "}\n";


static const char *fragmentShaderSource =
        "uniform vec4 color;\n"
        "void main() {\n"
        "    gl_FragColor = color;\n"
        "}\n";


void GLWi::initShaders()
{
    // Compile vertex shader
    if (!program.addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource))
        close();
    // Compile fragment shader
    if (!program.addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource))
        close();
    // Link shader pipeline
    if (!program.link())
        close();
    // Bind shader pipeline for use
    if (!program.bind())
        close();
}


void GLWi::resizeGL(int w, int h)
{
    MakeMtx(w,h);
    CalcGrid();
}

void GLWi::MakeMtx(int w, int h)
{

    projection.setToIdentity();
    GLdouble dWt;
    GLdouble dHt;
    if(w > h)
    {
        dHt = 1*mash;
        dWt = ((double)w*mash) / h;
    }
    else
    {
        dHt = ((double)h*mash) / w;
        dWt = 1*mash;
    }
    r2Mx.setMatrix(2*dWt/w,0,0,-2*dHt/h,cent_x-dWt,cent_y+dHt);
    projection.ortho(cent_x-dWt, cent_x+dWt, cent_y-dHt, cent_y+dHt, -5, 5);
    vMx = projection;
}

void GLWi::paintGL()
{
    QPainter painter(this);
    painter.beginNativePainting();

    if (!program.bind())
        close();
    // Clear color and depth buffer
    glClearColor(1.0f, 1.0f, 1.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    QColor color(0, 0, 250, 255);
    program.setUniformValue("color", color);
    program.setUniformValue("mvp_matrix", vMx);
    if (selected_lay>-1)
    {
        GLfloat *Vertices;
        Vertices=Cnt->getById(selected_lay)->fgetdata();
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, Vertices);
        glEnableVertexAttribArray(0);
        glLineWidth(1);
        glDrawArrays(GL_LINE_STRIP, 0, Cnt->getById(selected_lay)->getPointCount());
        delete [] Vertices;
    }
    painter.endNativePainting();

    if (showgrid)
    {
        painter.setRenderHint(QPainter::Antialiasing,true);
        painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
        painter.setBrush(QBrush(Qt::green,Qt::SolidPattern));

        for(int i=0;i<xLines.count();i++)
        {
            painter.drawLine(0,xLines[i].win_val,width(),xLines[i].win_val);
            painter.drawText(5,xLines[i].win_val-5,QString::number(xLines[i].real_val,'f',lnpresX));
        }

        for(int i=0;i<yLines.count();i++)
        {
            painter.drawLine(yLines[i].win_val,0,yLines[i].win_val,height());
            painter.drawText(yLines[i].win_val+5,height()-5,QString::number(yLines[i].real_val,'f',lnpresY));
        }
    }
}

void GLWi::paintGLoffscreen(int layid, QString dir)
{

    makeCurrent();
    //////
    QOpenGLFramebufferObjectFormat fboFormat;
    fboFormat.setSamples(4);
    fboFormat.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);

    const QRect drawRect(0, 0, width(), height());
    const QSize drawRectSize = drawRect.size();

    QOpenGLFramebufferObject fbo(drawRectSize, fboFormat);
    bool fbnd = fbo.bind();
    if (!fbnd)
    {
        qDebug()<<"fbnd: false";
    }
    //////

    if (!program.bind())
        close();
    // Clear color and depth buffer
    glClearColor(1.0f, 1.0f, 1.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    QColor color(0, 0, 250, 255);
    program.setUniformValue("color", color);
    program.setUniformValue("mvp_matrix", vMx);
    if ((layid>-1)&&(layid<Cnt->getCount()))
    {
        GLfloat *Vertices;
        Vertices=Cnt->getById(layid)->fgetdata();
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, Vertices);
        glEnableVertexAttribArray(0);
        glLineWidth(1);
        glDrawArrays(GL_LINE_STRIP, 0, Cnt->getById(layid)->getPointCount());
        delete [] Vertices;
    }


    QImage img = fbo.toImage();

    if (showgrid)
    {
        QPainter painter(&img);
        painter.setRenderHint(QPainter::Antialiasing,true);
        painter.setPen(QPen(Qt::black,1,Qt::DotLine,Qt::RoundCap));
        painter.setBrush(QBrush(Qt::green,Qt::SolidPattern));

        for(int i=0;i<xLines.count();i++)
        {
            painter.drawLine(0,xLines[i].win_val,width(),xLines[i].win_val);
            painter.drawText(5,xLines[i].win_val-5,QString::number(xLines[i].real_val,'f',lnpresX));
        }

        for(int i=0;i<yLines.count();i++)
        {
            painter.drawLine(yLines[i].win_val,0,yLines[i].win_val,height());
            painter.drawText(yLines[i].win_val+5,height()-5,QString::number(yLines[i].real_val,'f',lnpresY));
        }
        painter.end();
    }


    img.save(dir+"\\img_"+QString::number(layid)+".png","PNG");
}

void GLWi::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
      {
        case (Qt::Key_PageUp):
           mash=mash*1.5;
           break;
        case (Qt::Key_PageDown):
           mash=mash/1.5;
           break;
        case (Qt::Key_Left):
           cent_x+=-0.1*mash;
           break;
        case (Qt::Key_Right):
           cent_x+=0.1*mash;
           break;
        case (Qt::Key_Up):
           cent_y+=0.1*mash;
           break;
        case (Qt::Key_Down):
           cent_y+=-0.1*mash;
           break;
        case (Qt::Key_Delete):
           if (lstate==1)
           {
               if (selected_lay>-1)
                Cnt->getById(selected_lay)->removeLast();
           }
           break;
      }       
      MakeMtx(width(),height());
      CalcGrid();
      this->update();
}


void GLWi::mousePressEvent(QMouseEvent *e)
{
    if (lstate==1)
    {
        QPointF p = r2Mx.map(e->localPos());
        if (selected_lay>-1)
            Cnt->getById(selected_lay)->addXY(p.x(),p.y());
        this->update();
    }
    else if (lstate==2)
    {
        QPointF p = r2Mx.map(e->localPos());
        float delta = 4*r2Mx.m11();
        if (selected_lay>-1)
            selected_pid=Cnt->getById(selected_lay)->findAtXY(p.x(),p.y(),delta);

    }

}


void GLWi::mouseReleaseEvent(QMouseEvent *e)
{
    if (lstate==2)
    {
        selected_pid=-1;
    }
}


void GLWi::mouseMoveEvent(QMouseEvent *e)
{

    QPointF p = r2Mx.map(e->localPos());

    if ((lstate==2)&&(selected_lay>-1))
    {
        float delta = 4*r2Mx.m11();
        Cnt->getById(selected_lay)->setById(selected_pid,p.x(),p.y());
        this->update();
    }
    else
    {
        QToolTip::hideText();
        QToolTip::showText( this->mapToGlobal(e->localPos().toPoint()), QString::number(p.x()) + QString (" : ") + QString::number(p.y()));
    }
}


void GLWi::setLayerId(int id)
{
    selected_lay=id;
}

int GLWi::getLayerId()
{
    return selected_lay;
}


void GLWi::addNewLayer()
{
    Cnt->addNew();
}


int GLWi::getLayerCount()
{
    return Cnt->getCount();
}


void GLWi::deleteLayerById(int id)
{
    Cnt->deleteById(id);    
}


void GLWi::CenterByCur()
{
    double minx,maxx,miny,maxy;
    Cnt->getMinMaxByID(selected_lay,minx,miny,maxx,maxy);
    calcCenter(minx,miny,maxx,maxy);
    this->update();
}

void GLWi::CenterByAll()
{
    double minx,maxx,miny,maxy;
    Cnt->getMinMax(minx,miny,maxx,maxy);
    calcCenter(minx,miny,maxx,maxy);
    this->update();
}


void GLWi::calcCenter(double minx, double miny, double maxx, double maxy)
{
    cent_x=(maxx+minx)/2;
    cent_y=(maxy+miny)/2;

    int w = width();
    int h = height();

    GLint dHt;
    GLint dWt;

    if(w > h)
    {
        dHt = 1;
        dWt = ((double)w) / h;
    }
    else
    {
        dHt = ((double)h) / w;
        dWt = 1;
    }

    if ((maxx-minx)/w>(maxy-miny)/h)
    {
        mash = 1.1*(maxx-minx) / (2*dWt);
    }
    else
    {
        mash = 1.1*(maxy-miny) / (2*dHt);
    }

    MakeMtx(width(),height());
    CalcGrid();
}


void GLWi::saveToFile(QTextStream& out)
{
    Cnt->saveToFile(out);
}


void GLWi::loadFromFile(QTextStream& in)
{
    Cnt->loadFromFile(in);
}


void GLWi::calc()
{    
    int c=Cnt->getCount();    
    for (int j=0;j<c;j++)
    {
        ResolveByMaxwellRule(Cnt->getById(j));
    }
    this->update();
}

void GLWi::ResolveByMaxwellRule(Contour *Cont)
{
    L19Maxwell::l19interval v;
    bool solved;
    double res;
    int lp,fp;
    double fy,ly;
    int start=0;
    int stop=Cont->getPointCount()-1;
    int strt;
    int stp;
    double minx,miny,maxx,maxy;
    Cont->getMinMax(minx,miny,maxx,maxy);
    solved = true;
    while (L19Maxwell::separateReverseInterval(Cont->getdata(),start,stop,v, strt, stp) && solved)
    {
        solved = false;
        double x1=v.x2;
        double x2=v.x1;
        double eps=fabs((maxx-minx)*(maxy-miny)/100000000);
        if (L19Maxwell::MaxwellRule(Cont->getdata(),strt,stp,x1,x2,eps,res,fp,lp,fy,ly))
        {
            solved = true;
            Cont->setById(fp,res,fy);
            Cont->setById(lp,res,ly);
            for (int i=fp+1;i<lp;i++)
            {
                Cont->removeAt(fp+1);
            }
            stop=Cont->getPointCount()-1;
        }
    }
}


void GLWi::CalcGrid()
{
    xLines.clear();
    yLines.clear();
    QPointF p;
    p.setX(0);
    p.setY(0);
    QPointF p1 = r2Mx.map(p);
    p.setX(width());
    p.setY(height());
    QPointF p2 = r2Mx.map(p);
    qDebug()<<"///////////";
    double dx = (p2.x() - p1.x())/10;
    double dy = (p1.y() - p2.y())/10;

    qDebug()<<dx << " - " << dy;

    int exx, exy;
    dx = L19Maxwell::frexp10(dx,&exx);
    dy = L19Maxwell::frexp10(dy,&exy);

    double dxx = round(10*dx)*pow(10,exx-1);
    double dyy = round(10*dy)*pow(10,exy-1);
    qDebug()<<dx << " - " << dy;
    qDebug()<<dxx << " - " << dyy;

    double x1 = ceil(p1.x()/dxx)*dxx;
    double y1 = ceil(p2.y()/dyy)*dyy;


    qDebug()<<x1 << " - " << y1;

    axisLine al;

    lnpresX = abs(exx)+1;
    lnpresY = abs(exy)+1;

    double v=y1;
    while (v<=p1.y()) {
        al.real_val=v;
        al.win_val=(al.real_val-r2Mx.dy())/r2Mx.m22();
        xLines.append(al);
        v+=dyy;
    }

    v=x1;
    while (v<=p2.x()) {
        al.real_val=v;
        al.win_val=(al.real_val-r2Mx.dx())/r2Mx.m11();
        yLines.append(al);
        v+=dxx;
    }

}

void GLWi::SwitchGrid()
{
    showgrid = !showgrid;
    this->update();    
}


void GLWi::save_png()
{
    CenterByAll();
    int c = Cnt->getCount();
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),QString(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    QDir dr(dir);
    dr.setNameFilters(QStringList() << "*img_*.png");
    dr.setFilter(QDir::Files);
    foreach(QString dirFile, dr.entryList())
    {
        dir.remove(dirFile);
    }
    for (int i=0;i<c;i++)
        paintGLoffscreen(i, dir);
}
