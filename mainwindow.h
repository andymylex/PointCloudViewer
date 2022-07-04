#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/visualization/pcl_visualizer.h>

typedef pcl::PointXYZRGBA PointT;
typedef pcl::PointCloud<PointT> PointCloudT;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionDemo_Cloud_triggered();
    void on_actionLoad_triggered();
    void on_horizontalSlider_pointSize_valueChanged(int value);
    void on_horizontalSlider_blue_valueChanged(int value);
    void on_horizontalSlider_green_valueChanged(int value);
    void on_horizontalSlider_red_valueChanged(int value);
    void on_pushButton_setRandomColors_pressed();
    void colorSliderReleased();

private:
    Ui::MainWindow *ui;
    pcl::visualization::PCLVisualizer::Ptr viewer;
    PointCloudT::Ptr cloud;
    unsigned int red;
    unsigned int green;
    unsigned int blue;

};
#endif // MAINWINDOW_H
