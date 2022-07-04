#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>

#include <liblas/liblas.hpp>
#include <vtkGenericOpenGLRenderWindow.h>

namespace {
QString currentFilePath;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), red(0), green(0), blue(0) {
    ui->setupUi(this);

    auto renderer = vtkSmartPointer<vtkRenderer>::New();
    auto renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    renderWindow->AddRenderer(renderer);
    viewer.reset(new pcl::visualization::PCLVisualizer(renderer, renderWindow,
                                                       "viewer", false));
    ui->qVtkOpenGlNativeWidget->setRenderWindow(viewer->getRenderWindow());
    viewer->setupInteractor(ui->qVtkOpenGlNativeWidget->interactor(),
                            ui->qVtkOpenGlNativeWidget->renderWindow());
    viewer->setShowFPS(false);

    connect(ui->horizontalSlider_red, &QSlider::sliderReleased, this,
            &MainWindow::colorSliderReleased);
    connect(ui->horizontalSlider_green, &QSlider::sliderReleased, this,
            &MainWindow::colorSliderReleased);
    connect(ui->horizontalSlider_blue, &QSlider::sliderReleased, this,
            &MainWindow::colorSliderReleased);
}

MainWindow::~MainWindow() { delete ui; }

// Обработчик нажатия на кнопку "Случайные цвета"
void MainWindow::on_pushButton_setRandomColors_pressed() {
    if (cloud == nullptr)
        return;

    for (auto &point : *cloud) {
        point.r = 255 * (1024 * rand() / (RAND_MAX + 1.0f));
        point.g = 255 * (1024 * rand() / (RAND_MAX + 1.0f));
        point.b = 255 * (1024 * rand() / (RAND_MAX + 1.0f));
    }

    viewer->updatePointCloud(cloud);
    viewer->spin();
}

// Обработчик перемещения слайдеров "Красный", "Синий", "Зеленый"
void MainWindow::colorSliderReleased() {
    for (auto &point : *cloud) {
        point.r = red;
        point.g = green;
        point.b = blue;
    }

    viewer->updatePointCloud(cloud);
    viewer->spin();
}

// Обработчик перемещения слайдера "Красный"
void MainWindow::on_horizontalSlider_red_valueChanged(int value) {
    red = value;
}

// Обработчик перемещения слайдера "Зеленый"
void MainWindow::on_horizontalSlider_green_valueChanged(int value) {
    green = value;
}

// Обработчик перемещения слайдера "Синий"
void MainWindow::on_horizontalSlider_blue_valueChanged(int value) {
    blue = value;
}

// Обработчик перемещения слайдера "Размер точек"
void MainWindow::on_horizontalSlider_pointSize_valueChanged(int value) {
    viewer->setPointCloudRenderingProperties(
                pcl::visualization::PCL_VISUALIZER_POINT_SIZE, value);

    viewer->spin();
}

// Обработчик нажатия на пункт меню "Загрузить..."
void MainWindow::on_actionLoad_triggered() {
    QString filePath;
    filePath = QFileDialog::getOpenFileName(this, "Choose point cloud file",
                                            QString(), "*.las");

    if (filePath.isEmpty())
        return;

    std::ifstream ifs;
    ifs.open(filePath.toStdString(), std::ios::in | std::ios::binary);

    liblas::ReaderFactory f;
    liblas::Reader reader = f.CreateWithStream(ifs);
    liblas::Header const &header = reader.GetHeader();

    cloud.reset(new PointCloudT);
    cloud->resize(header.GetPointRecordsCount());

    for (auto &point : *cloud) {
        reader.ReadNextPoint();
        liblas::Point const &p = reader.GetPoint();

        point.x = p.GetX();
        point.y = p.GetY();
        point.z = p.GetZ();

        point.r = p.GetColor().GetRed() / 255.0;
        point.g = p.GetColor().GetGreen() / 255.0;
        point.b = p.GetColor().GetBlue() / 255.0;
    }

    viewer->removePointCloud();
    viewer->addPointCloud(cloud);
    viewer->resetCamera();
    viewer->spin();

    currentFilePath = filePath;
    statusBar()->showMessage(filePath);
}

// Обработчик нажатия на пункт меню "Демонстрационное облако"
void MainWindow::on_actionDemo_Cloud_triggered() {
    cloud.reset(new PointCloudT);
    cloud->resize(1000);

    for (auto &point : *cloud) {
        point.x = 1024 * rand() / (RAND_MAX + 1.0f);
        point.y = 1024 * rand() / (RAND_MAX + 1.0f);
        point.z = 1024 * rand() / (RAND_MAX + 1.0f);

        point.r = red;
        point.g = green;
        point.b = blue;
    }

    viewer->removePointCloud();
    viewer->addPointCloud(cloud);
    viewer->resetCamera();
    viewer->spin();

    currentFilePath = QString();
}

// Обработчик нажатия на кнопку "Цвета из LAS"
void MainWindow::on_pushButton_setLasColors_pressed() {
    if (cloud == nullptr || currentFilePath.isEmpty())
        return;

    for (auto &point : *cloud) {
        point.r = red;
        point.g = green;
        point.b = blue;
    }

    std::ifstream ifs;
    ifs.open(currentFilePath.toStdString(), std::ios::in | std::ios::binary);

    liblas::ReaderFactory f;
    liblas::Reader reader = f.CreateWithStream(ifs);

    for (auto &point : *cloud) {
        reader.ReadNextPoint();
        liblas::Point const &p = reader.GetPoint();

        point.r = p.GetColor().GetRed() / 255.0;
        point.g = p.GetColor().GetGreen() / 255.0;
        point.b = p.GetColor().GetBlue() / 255.0;
    }

    viewer->updatePointCloud(cloud);
    viewer->spin();
}

// Обработчик нажатия пункта меню "О программе..."
void MainWindow::on_actionAbout_Program_triggered() {
    QMessageBox::information(
                this, "О программе",
                "Point Cloud Viewer v1.0 \nПрограмма была разработана студентом КубГАУ "
                "группы ИТ1902 Черкашиным Андреем Романовичем (c) 2022\n\nИспользованные "
                "библиотеки: Point Cloud Library, LibLAS, Visualzation Toolkit");
}

// Обработчик нажатия пункта меню "О Qt..."
void MainWindow::on_actionAbout_Qt_triggered() { QMessageBox::aboutQt(this); }
