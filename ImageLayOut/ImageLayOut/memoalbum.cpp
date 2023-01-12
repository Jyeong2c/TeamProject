//#include <QSplitter>
//#include <QListWidget>
//#include <QDir>
//#include <QFileInfo>

//#include "imageview.h"
//#include "memoalbum.h"

//MemoAlbum::MemoAlbum(QWidget *parent)
//    : QSplitter(parent)
//{
////    QPalette palette;
////    palette.setColor(QPalette::Window, Qt::white);
////    palette.setColor(QPalette::WindowText, Qt::blue);

////    graphicsScene = new QGraphicsScene;
////    imageView = new ImageView(this);
////    imageView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
////    imageView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
////    imageView->setScene(graphicsScene);
////    imageView->grabGesture(Qt::PinchGesture);

////    listWidget = new QListWidget(this);
////    listWidget->setIconSize(QSize(120, 80));
////    listWidget->setResizeMode(QListWidget::Adjust);
////    listWidget->setFlow(QListWidget::LeftToRight);
////    listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    connect(listWidget, SIGNAL(itemClicked(QListWidgetItem*)), SLOT(selectItem(QListWidgetItem*)));

////    setOrientation(Qt::Vertical);
////    addWidget(imageView);
////    addWidget(listWidget);
////    setStretchFactor(0, 3);

////    QList<int> list;
////    list << 520 << 100;
////    setSizes(list);

//    reloadImages();
//}

//void MemoAlbum::reloadImages(){
//    QDir dir(".");
//    QStringList filters;
//    filters << "*.png" << "*.jpg" << "*.bmp";
//    QFileInfoList fileInfoList = dir.entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);
//    graphicsScene->clear();

//    ui->listWidget->clear();
//    for(int i=0; i<fileInfoList.count(); i++){
//        QListWidgetItem* item = new QListWidgetItem(QIcon(fileInfoList.at(i).fileName()), NULL, ui->listWidget);
//        item->setStatusTip(fileInfoList.at(i).fileName());
//        ui->listWidget->addItem(item);
//    };
//}

//void MemoAlbum::selectItem(QListWidgetItem *item){
//    imageView->resetTransform();
//    graphicsScene->clear();
//    graphicsScene->addPixmap(QPixmap(item->statusTip()).scaled(640, 480, Qt::KeepAspectRatio, Qt::SmoothTransformation));
//}

//void MemoAlbum::setImage(QString path){
//    imageView->resetTransform();
//    graphicsScene->clear();
//    graphicsScene->addPixmap(QPixmap(path).scaled(640, 480, Qt::KeepAspectRatio, Qt::SmoothTransformation));

//    QFileInfo fileInfo(path);
//    for(int i=0; i<listWidget->count(); i++){
//        if(listWidget->item(i)->statusTip() == fileInfo.fileName()){
//            listWidget->setCurrentRow(i);
//            break;
//        }
//    }
//}

//QString MemoAlbum::currentImage(){
//    return (listWidget->currentRow() >= 0)?listWidget->currentItem()->statusTip():"";
//}

//void MemoAlbum::reset(){
//    imageView->resetTransform();
//}
