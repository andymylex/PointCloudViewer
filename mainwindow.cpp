#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFileDialog>

#include <vtkGenericOpenGLRenderWindow.h>

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

    // pSliderValueChanged (2);
    // viewer->resetCamera ();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_pushButton_setRandomColors_pressed() {
    for (auto &point : *cloud) {
        point.r = 255 * (1024 * rand() / (RAND_MAX + 1.0f));
        point.g = 255 * (1024 * rand() / (RAND_MAX + 1.0f));
        point.b = 255 * (1024 * rand() / (RAND_MAX + 1.0f));
    }

    viewer->updatePointCloud(cloud);
    viewer->spin();
}

void MainWindow::colorSliderReleased() {
    for (auto &point : *cloud) {
        point.r = red;
        point.g = green;
        point.b = blue;
    }

    viewer->updatePointCloud(cloud);
    viewer->spin();
}

void MainWindow::on_horizontalSlider_red_valueChanged(int value) {
    red = value;
}

void MainWindow::on_horizontalSlider_green_valueChanged(int value) {
    green = value;
}

void MainWindow::on_horizontalSlider_blue_valueChanged(int value) {
    blue = value;
}

void MainWindow::on_horizontalSlider_pointSize_valueChanged(int value) {}

void MainWindow::on_actionLoad_triggered() {
    QString filePath;
    filePath = QFileDialog::getOpenFileName(this, "Choose point cloud file",
                                            QString(), "*las");

    if (filePath.isEmpty())
        return;
}

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
}
