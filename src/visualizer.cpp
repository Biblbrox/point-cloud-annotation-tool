#include "visualizer.h"
#include "ui_mainwindow.h"
#include "annotation.h"
#include "pcl/visualization/pcl_visualizer_extented.h"

#include <pcl/io/pcd_io.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/visualization/point_cloud_color_handlers.h>
#include <pcl/visualization/common/actor_map.h>
#include <QFileDialog>
#include <vtkOrientationMarkerWidget.h>
#include <vtkAxesActor.h>
#include <vtkTransform.h>
#include <pcl/visualization/mouse_event.h>
#include <vtkPropPicker.h>
#include <QMessageBox>
#include <vtkRenderWindow.h>
#include <view/flowlayout.h>
#include <QStandardPaths>

using namespace pcl::visualization;
using namespace std;
//#define connect(...) QObject::connect(__VA_ARGS__);

#define DEFAULT_POINT 0
#define SELECTED_POINT 1
#define GROUND_POINT 2

Visualizer::Visualizer(DatasetFormat datasetType, QWidget *parent)
        : QMainWindow(parent),
          m_datasetType(datasetType),
          m_ui(new Ui::MainWindow)
{

    m_ui->setupUi(this);
    this->setWindowTitle("3D annotation tool");

    initialize();

    // register selection http://www.pcl-users.org/Select-set-of-points-using-mouse-td3424113.html

    m_viewer->registerKeyboardCallback(boost::bind(&Visualizer::KeyboardEventProcess, this, _1));
    m_viewer->registerAreaPickingCallback(
            boost::bind(&Visualizer::AreaPickingEventProcess, this, _1));
    m_viewer->registerMouseCallback(boost::bind(&Visualizer::MouseEventProcess, this, _1));

    // UI
    connect(m_ui->action_Open, &QAction::triggered, this, &Visualizer::openFile);
    connect(m_ui->action_Save, &QAction::triggered, this, &Visualizer::save);
    connect(m_ui->action_detect_plane, &QAction::triggered, m_ui->PlaneDetect_dockWidget,
            &QDockWidget::show);
    connect(m_ui->action_Threshold, &QAction::triggered, m_ui->threshold_dockWidget,
            &QDockWidget::show);

    m_ui->PlaneDetect_dockWidget->hide();
    m_ui->distanceThreshold_lineEdit->setText("0.1");
    m_ui->distanceThreshold_lineEdit->setValidator(new QDoubleValidator(-5, 5, 2));
    connect(m_ui->applyPlaneDetection_pushButton, &QPushButton::clicked, this,
            &Visualizer::planeDetect);

    m_ui->threshold_dockWidget->hide();
    m_ui->threshold_lineEdit->setText("-1.5");
    m_ui->threshold_lineEdit->setValidator(new QDoubleValidator(-100, 100, 2));
    connect(m_ui->threshold_pushButton, &QPushButton::clicked, this, &Visualizer::threshold);
}

Visualizer::~Visualizer()
{
    delete m_ui;
}

void Visualizer::initialize()
{
    // init viewer
    m_viewer.reset(new PCLViewer("", false));
    // BUG under windows, this cause execution exception
    m_ui->qvtkWidget->SetRenderWindow(m_viewer->getRenderWindow());
    m_viewer->setupInteractor(m_ui->qvtkWidget->GetInteractor(), m_ui->qvtkWidget->GetRenderWindow());
    m_ui->qvtkWidget->update();

    // init label type
    Annotation::getTypes()->clear();
    if (m_datasetType == DatasetFormat::KITTI) {
        Annotation::getTypes()->push_back("DontCare");
        Annotation::getTypes()->push_back("Cyclist");
        Annotation::getTypes()->push_back("Pedestrian");
        Annotation::getTypes()->push_back("Car");
        Annotation::getTypes()->push_back("Van");
        Annotation::getTypes()->push_back("Truck");
        Annotation::getTypes()->push_back("Person_sitting");
        Annotation::getTypes()->push_back("Tram");
    } else {
        Annotation::getTypes()->push_back("dontCare");
        Annotation::getTypes()->push_back("cyclist");
        Annotation::getTypes()->push_back("pedestrian");
        Annotation::getTypes()->push_back("vehicle");
        Annotation::getTypes()->push_back("unknown");
    }

    FlowLayout *layout = new FlowLayout();
    for (const auto& type : *(Annotation::getTypes())) {
        QPushButton *button = new QPushButton(QString::fromStdString(type));
        connect(button, &QPushButton::clicked, this,
                [=]() { this->typeButtonClickedProcess(button->text().toStdString()); });

        pcl::RGB c = Annotation::getColor(type);
        QPalette pal = button->palette();
        pal.setColor(QPalette::Button, QColor(c.r, c.g, c.b, c.a));
        button->setAutoFillBackground(true);
        button->setPalette(pal);
        button->update();
        layout->addWidget(button);
    }
    m_ui->groupBox_Types->setLayout(layout);
    //m_ui->groupBox_Types->setMaximumWidth(200);
    //m_ui->groupBox_Types->setMaximumHeight(200);


    //init annotation
    m_annoManager.reset(new Annotations());
    m_currPickedAnnotation = nullptr;

    //axes
    vtkSmartPointer<vtkAxesActor> axes = vtkSmartPointer<vtkAxesActor>::New();
    m_axesWidget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
    m_axesWidget->SetOrientationMarker(axes);
//  m_axesWidget->SetInteractor( viewer->getInteractorStyle()->GetInteractor() );
    m_axesWidget->SetInteractor(m_viewer->getRenderWindowInteractor());
    m_axesWidget->SetViewport(0.0, 0.0, 0.2, 0.2);
    m_axesWidget->SetEnabled(1);
    m_axesWidget->InteractiveOff();

    // init cloud
    m_cloud.reset(new PointCloudT);
    m_cloudLabel = nullptr;
}

void Visualizer::refresh()
{
    m_cloudLabel = new int[m_cloud->size()];
    memset(m_cloudLabel, 0, m_cloud->size() * sizeof(int));

    m_ui->label_filename->setText(QString::fromStdString(m_pointcloudFileName));
    m_colorHandler.setInputCloud(m_cloud);
    m_colorHandler.setLabel(m_cloudLabel);
    m_viewer->addPointCloud<PointT>(m_cloud, m_colorHandler, "cloud", 0);

//    // show cloud
//    //http://www.pcl-users.org/how-to-add-color-to-a-xyz-cloud-td4040030.html
//    pcl::visualization::PointCloudColorHandlerCustom<PointT> colorHandler(cloud, 255, 255, 255);
//    //    pcl::visualization::PointCloudColorHandlerGenericField<PointT> colorHandler(cloud, "intensity");


//    PointCloudColorHandlerLabelField<PointT> colorHandler;
//    colorHandler.

    m_viewer->resetCamera();
    m_ui->qvtkWidget->update();

    // show annotation if exists
    showAnnotation();
}

void Visualizer::pickAnnotation(double x, double y)
{
    vtkSmartPointer<vtkPropPicker> picker = vtkSmartPointer<vtkPropPicker>::New();
    picker->Pick(x, y, 0, m_viewer->getRendererCollection()->GetFirstRenderer());
    vtkActor *pickedActor = picker->GetActor();

    if (m_currPickedAnnotation) {
        m_currPickedAnnotation->unpicked();
        m_currPickedAnnotation = nullptr;
    }
    // get the correspond annotation
    m_currPickedAnnotation = m_annoManager->getAnnotation(pickedActor);
    if (m_currPickedAnnotation)
        m_currPickedAnnotation->picked(m_viewer->getRenderWindowInteractor());
}

void Visualizer::highlightPoint(std::vector<int> &slice)
{
    if (slice.empty())
        return;

    for (int& it : slice)
        m_cloudLabel[it] = SELECTED_POINT;
}


void Visualizer::defaultColorPoint(std::vector<int> &slice)
{
    if (slice.empty())
        memset(m_cloudLabel, DEFAULT_POINT, m_cloud->size() * sizeof(int));

    for (int& it : slice)
        m_cloudLabel[it] = DEFAULT_POINT;
}

void Visualizer::groundColorPoint(std::vector<int> &slice)
{
    if (slice.empty())
        return;

    for (int& it : slice)
        m_cloudLabel[it] = GROUND_POINT;
}


void Visualizer::createAnnotationFromSelectPoints(const string& type)
{
    if (m_last_selected_slice.size() > 3) {
        Annotation *anno = new Annotation(m_cloud, m_last_selected_slice, type, m_img);
        m_annoManager->push_back(anno);
        showAnnotation(anno);
        m_ui->qvtkWidget->update();
    } else {
        std::printf("%s", "no points selected");
    }
}

void Visualizer::typeButtonClickedProcess(const string& type)
{
    // QMessageBox::information(this, QString::fromStdString("information"),QString::fromStdString(type));
    if (m_currPickedAnnotation) {
        m_currPickedAnnotation->setType(type);
        m_ui->qvtkWidget->update();
        return;
    }

    if (m_last_selected_slice.size() > 3)
        createAnnotationFromSelectPoints(type);
}

void Visualizer::updateCloud()
{
    m_viewer->updatePointCloud<PointT>(m_cloud, m_colorHandler, "cloud");
    m_ui->qvtkWidget->update();
}

vector<int> intersectionVector(vector<int> &a, vector<int> &b)
{
    vector<int> c;
    sort(a.begin(), a.end());
    sort(b.begin(), b.end());
    set_intersection(a.begin(), a.end(), b.begin(), b.end(), back_inserter(c));
    return c;
}

vector<int> unionVector(vector<int> &a, vector<int> &b)
{
    vector<int> c;
    set_union(a.begin(), a.end(), b.begin(), b.end(), back_inserter(c));
    return c;
}

vector<int> diffVector(vector<int> a, vector<int> b)
{
    vector<int> c;
    sort(a.begin(), a.end());
    sort(b.begin(), b.end());
    set_difference(a.begin(), a.end(), b.begin(), b.end(), back_inserter(c));
    return c;
}


void Visualizer::AreaPickingEventProcess(const pcl::visualization::AreaPickingEvent &event)
{
    vector<int> new_selected_slice;
    event.getPointsIndices(new_selected_slice);

    if (new_selected_slice.empty())
        return;

    int s = m_viewer->getRenderWindowInteractor()->GetShiftKey();
    int a = m_viewer->getRenderWindowInteractor()->GetControlKey();

    // remove ground points
    vector<int> r;
    for (auto x:new_selected_slice)
        if (m_cloudLabel[x] != GROUND_POINT)
            r.push_back(x);

    new_selected_slice = r;

    if (!m_last_selected_slice.empty())
        defaultColorPoint(m_last_selected_slice);

    if (s && a) { // intersection
        m_last_selected_slice = intersectionVector(m_last_selected_slice, new_selected_slice);
    } else if (s) { // union
        m_last_selected_slice = unionVector(m_last_selected_slice, new_selected_slice);
    } else if (a) { // remove
        m_last_selected_slice = diffVector(m_last_selected_slice, new_selected_slice);
    } else { // new
        m_last_selected_slice = new_selected_slice;
    }

    highlightPoint(m_last_selected_slice);
    updateCloud();
}


void Visualizer::MouseEventProcess(const pcl::visualization::MouseEvent &event)
{
    if (event.getButton() == pcl::visualization::MouseEvent::LeftButton
        && event.getType() == pcl::visualization::MouseEvent::MouseButtonPress) {
        pickAnnotation(event.getX(), event.getY());
    }
}

void Visualizer::KeyboardEventProcess(const KeyboardEvent &event)
{
    std::cout << event.getKeySym() << std::endl;

    // delete annotation
    if (event.getKeySym() == "Delete" && m_currPickedAnnotation)
        removeAnnotation(m_currPickedAnnotation);
}

void Visualizer::showAnnotation()
{
    for (auto anno:m_annoManager->getAnnotations())
        showAnnotation(anno);
}

void Visualizer::threshold()
{
    double threhold_;
    if (m_ui->threshold_lineEdit->text().isEmpty())
        return;

    threhold_ = m_ui->threshold_lineEdit->text().toDouble();

    vector<int> slice;
    for (int i = 0; i < m_cloud->size(); i++)
        if (m_cloud->points[i].z < threhold_)
            slice.push_back(i);

    memset(m_cloudLabel, DEFAULT_POINT, m_cloud->size() * sizeof(int));
    groundColorPoint(slice);
    updateCloud();
}

void Visualizer::planeDetect()
{
    if (m_ui->distanceThreshold_lineEdit->text().isEmpty())
        return;

    double distanceThreshold = m_ui->distanceThreshold_lineEdit->text().toDouble();

    pcl::ModelCoefficients::Ptr coefficients(new pcl::ModelCoefficients);
    pcl::PointIndices::Ptr inliers(new pcl::PointIndices);
    // Create the segmentation object
    pcl::SACSegmentation<PointT> seg;
    // Optional
    seg.setOptimizeCoefficients(true);
    // Mandatory
    seg.setModelType(pcl::SACMODEL_PLANE);
    seg.setMethodType(pcl::SAC_RANSAC);
    seg.setDistanceThreshold(distanceThreshold);
    seg.setInputCloud(m_cloud);
    seg.segment(*inliers, *coefficients);

    std::cout << "plane detection: " << inliers->indices.size() << std::endl;

    memset(m_cloudLabel, DEFAULT_POINT, m_cloud->size() * sizeof(int));
    groundColorPoint(inliers->indices);
    updateCloud();




//    PointCloudT::Ptr groundPlaneCloud;
//    groundPlaneCloud.reset(new PointCloudT);

//    // http://docs.pointclouds.org/trunk/classpcl_1_1_extract_indices.html
//    pcl::ExtractIndices<PointT> eifilter (true); // Initializing with true will allow us to extract the removed indices
//    eifilter.setInputCloud (cloud);
//    eifilter.setIndices (inliers);
//    eifilter.filter (*groundPlaneCloud);

//    //    PointCloudT::Ptr groundPlaneCloud;
//    //    groundPlaneCloud.reset(new PointCloudT);
//    //    for (size_t i = 0; i < inliers->indices.size (); ++i){
//    //        groundPlaneCloud->push_back(cloud->points[inliers->indices[i]]);
//    //    }

//    viewer->removeAllPointClouds();
//    ui->qvtkWidget->update();
//    if (ui->removePlane_checkBox->isChecked()){
//        pcl::visualization::PointCloudColorHandlerCustom<PointT> colorHandler1(remainCloud, colors.at(1).r,colors.at(1).g,colors.at(1).b);
//        viewer->addPointCloud<PointT>(remainCloud,colorHandler1,"remainCloud");
//        ui->qvtkWidget->update();
//    }else{
//        pcl::visualization::PointCloudColorHandlerCustom<PointT> colorHandler1(remainCloud, colors.at(1).r,colors.at(1).g,colors.at(1).b);
//        pcl::visualization::PointCloudColorHandlerCustom<PointT> colorHandler2(groundPlaneCloud, colors.at(2).r,colors.at(2).g,colors.at(2).b);
//        viewer->addPointCloud<PointT>(remainCloud,colorHandler1,"remainCloud");
//        viewer->addPointCloud<PointT>(groundPlaneCloud,colorHandler2,"groundPlaneCloud");
//        ui->qvtkWidget->update();
//    }
}

void Visualizer::openFile()
{
    QString homeDir = QStandardPaths::displayName(QStandardPaths::HomeLocation);
    m_pointcloudFileName = QFileDialog::getOpenFileName(this, tr("Open PCD file"), homeDir,
                                                      tr("PCD Files (*.pcd *.bin)")).toStdString();
//    m_calMatrixFileName = QFileDialog::getOpenFileName(this, tr("Open calibration matrix file"),
//                                                     homeDir,
//                                                     tr("Txt Files (*.txt)")).toStdString();

    const char* imgTypes = "Image Files (*.png *bmp *.jpg *.jpeg)";
    m_imgFileName = QFileDialog::getOpenFileName(this, tr("Open image file name"),
                                                 homeDir, tr(imgTypes)).toStdString();

    if (m_pointcloudFileName.empty())
        return;

    clear();
    QFileInfo file(QString::fromStdString(m_pointcloudFileName));
    QString ext = file.completeSuffix();  // ext = "bin" ,"pcd"

    if (ext == "pcd") {
        pcl::io::loadPCDFile(m_pointcloudFileName, *m_cloud);
    } else {
        loadBinFile(m_pointcloudFileName, *m_cloud);
    }

    std::cout << m_pointcloudFileName << "cloud point loaded" << endl;
    std::cout << "cloud point number: " << m_cloud->width * m_cloud->height << endl;

    // Load calibration matrix
//    m_calMatrix = loadMatFromFile(m_calMatrixFileName);
    m_img = cv::imread(m_imgFileName);

    m_annotationFileName = m_pointcloudFileName + ".txt";
    if (QFile::exists(QString::fromStdString(m_annotationFileName)))
        m_annoManager->loadAnnotations(m_annotationFileName);

    refresh();
}

void Visualizer::clear()
{
    removeAnnotation();
    m_viewer->removeAllPointClouds();
    m_annoManager->clear();

    m_currPickedAnnotation = nullptr;
    m_last_selected_slice.clear();

    if (m_cloudLabel) {
        delete[] m_cloudLabel;
        m_cloudLabel = nullptr;
    }
}

void Visualizer::save()
{
    m_annoManager->saveAnnotations(m_annotationFileName);
}

void Visualizer::loadBinFile(const string& filename, PointCloudT &cloud)
{
    std::ifstream input(filename.c_str(), std::ios_base::binary);
    if (!input.good()) {
        std::cerr << "Cannot open file : " << filename << std::endl;
        return;
    }

    cloud.clear();
    cloud.height = 1;

    for (int i = 0; input.good() && !input.eof(); i++) {
        PointT point;
        input.read((char *) &point.x, 3 * sizeof(double));
        input.read((char *) &point.intensity, sizeof(double));
        cloud.push_back(point);
    }
    input.close();
}

void Visualizer::showAnnotation(const Annotation *anno)
{
    m_viewer->addActorToRenderer(anno->getActor());
}

void Visualizer::removeAnnotation()
{
    for (auto anno: m_annoManager->getAnnotations())
        removeAnnotation(anno);
}

void Visualizer::removeAnnotation(Annotation *anno)
{
    if (m_currPickedAnnotation) {
        m_currPickedAnnotation->unpicked();
        m_currPickedAnnotation = nullptr;
    }
    m_viewer->removeActorFromRenderer(anno->getActor());
}

Eigen::MatrixXd Visualizer::loadMatFromFile(const std::string& fileName) const
{
    int cols = 0, rows = 0;
    double buff[100];

    // Read numbers from file into buffer.
    ifstream infile;
    infile.open(fileName);
    while (!infile.eof()) {
        string line;
        getline(infile, line);

        int temp_cols = 0;
        stringstream stream(line);
        while (!stream.eof())
            stream >> buff[cols * rows + temp_cols++];

        if (temp_cols == 0)
            continue;

        if (cols == 0)
            cols = temp_cols;

        rows++;
    }

    infile.close();

    rows--;

    // Populate matrix with numbers.
    Eigen::MatrixXd result(rows, cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            result(i, j) = buff[cols * i + j];

    return result;
}

