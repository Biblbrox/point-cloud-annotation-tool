#ifndef VISUALIZER_H
#define VISUALIZER_H

#include "common.h"
#include <QMainWindow>
#include <pcl/visualization/PointCloudColorHandlerLUT.h>
#include <Annotation.h>

namespace Ui {
class MainWindow;
}

using namespace std;

class Visualizer:public QMainWindow
{
public:
    explicit Visualizer(QWidget *parent = 0);
    ~Visualizer();
    void highlightPoint(vector<int>& slice);
    void defaultColorPoint(vector<int>& slice);
    void groundColorPoint(vector<int>& slice);
    void pickAnnotation(double x,double y);
    void createAnnotationFromSelectPoints(string type="unknown");
    void typeButtonClickedProcess(string type);
    void updateCloud();

protected:
    Ui::MainWindow* m_ui;
    PCLViewerPtr m_viewer;
    string m_pointcloudFileName;
    string m_annotationFileName;
    string m_calMatrixFileName;
    Eigen::MatrixXd m_calMatrix;
    /**
     * @brief the loaded cloud
     */
    PointCloudTPtr m_cloud;
    /**
     * @brief state of each point to identity color or selectable
     */
    int* m_cloudLabel;
    PointCloudColorHandlerLUT<PointT> m_colorHandler;
    vector<int> m_last_selected_slice;
    // manage annotations
    boost::shared_ptr<Annotations> m_annoManager;

    /**
     * @brief show loaded cloud and annotations
     */
    void refresh();

    /**
     * @brief initialize UI, slot connection and something else
     */
    void initialize();
    /**
     * @brief open cloud file
     */
    void openFile();
    /**
     * @brief clear state before load new cloud and annotation
     */
    void clear();
    /**
     * @brief save annotations
     */
    void save();

    /**
     * @brief load APPLI or KITTI  cloud bin file
     * @param filename_
     * @param cloud_
     */
    void loadBinFile(string filename_, PointCloudT &cloud_);
    void planeDetect();
    void threshold();


private:
    void AreaPickingEventProcess(const pcl::visualization::AreaPickingEvent& event);
    void MouseEventProcess (const pcl::visualization::MouseEvent& event);
    void KeyboardEventProcess(const pcl::visualization::KeyboardEvent& event);

    // visibility
    void showAnnotation();
    void showAnnotation(const Annotation* anno);
    void removeAnnotation();
    void removeAnnotation(Annotation* anno);
    Eigen::MatrixXd loadMatFromFile(const std::string& fileName) const;

    // axes
    vtkSmartPointer<vtkOrientationMarkerWidget> m_axesWidget;

    // for pick
    Annotation *m_currPickedAnnotation;
};

#endif // VISUALIZER_H
