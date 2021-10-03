#ifndef Annotaions_H
#define Annotaions_H

#include <common.h>
#include <vtkSmartPointer.h>

using namespace std;

enum class DatasetFormat
{
    STANDARD,
    KITTI
};


struct BoxLabel
{
    BoxLabel()
    {
        type = "unknown";
        this->detail.center_x = this->detail.center_y = this->detail.center_z = 0;
        this->detail.yaw = 2;
        this->detail.length = this->detail.width = this->detail.height = 1;
    }

    BoxLabel(const double p1[3], const double p2[3], string type_ = "unknown")
    {
        type = type_;
        this->detail.center_x = (p1[0] + p2[0]) / 2;
        this->detail.center_y = (p1[1] + p2[1]) / 2;
        this->detail.center_z = (p1[2] + p2[2]) / 2;
        this->detail.yaw = 0;
        this->detail.length = p2[0] - p1[0];
        this->detail.width = p2[1] - p1[1];
        this->detail.height = p2[2] - p1[2];
    }

    string type;
    double cameraPosX;
    double cameraPosY;
    double cameraPosZ;
    union
    {
        double data[7];
        struct
        {
            /**
             * Float from 0 (non-truncated) to 1 (truncated), where
             * truncated refers to the object leaving image boundaries
             */
            double truncated;
            /**
             * Integer (0,1,2,3) indicating occlusion state:
             * 0 = fully visible, 1 = partly occluded
             * 2 = largely occluded, 3 = unknown
             */
            double occluded;

            /**
             * Observation angle of object, ranging [-pi..pi]
             */
            double alpha;

            /**
             * 2D bounding box of object in the image (0-based index):
             * contains left, top, right, bottom pixel coordinates
             */
            double left;
            double top;
            double right;
            double bottom;

            /**
             * 3D object dimensions: height, width, length (in meters)
             */
            double length;
            double width;
            double height;

            /**
             * 3D object location x,y,z in camera coordinates (in meters)
             */
            double center_x;
            double center_y;
            double center_z;

            /**
             * Rotation ry around Y-axis in camera coordinates [-pi..pi]
             */
            double yaw;
        } detail;
    };

    string toString()
    {
        char buffer[200];
        sprintf(buffer, "%s %f %f %f %f %f %f %f",
                type.c_str(), data[0], data[1], data[2], data[3], data[4], data[5], data[6]);
        return std::string(buffer);;
    }
};


class vtkBoxWidgetCallback0;

class vtkBoxWidgetCallback1;

class vtkAnnotationBoxSource;

class vtkBoxWidgetRestricted;

class vtkTransform;

class vtkRenderWindowInteractor;

class vtkActor;

class vtkPolyDataMapper;

class Annotation
{
public:
    /**
     * @brief Annotation  construct from boxlabel which load from label file
     * @param label
     * @param visible_
     * @param lock_
     */
    Annotation(const BoxLabel &label, bool visible_ = true, bool lock_ = false);

    /**
     * @brief Annotation construct from part of cloud points
     * @param cloud
     * @param slice
     * @param type_
     */
    Annotation(const PointCloudTPtr cloud, vector<int> &slice, string type_);

    ~Annotation();

    /**
     * @brief getBoxLabel get boxLabel from annotaion tranformation
     * @return
     */
    BoxLabel getBoxLabel();

    /**
     * @brief apply transform to annotation
     * @param t
     */
    void applyTransform(vtkSmartPointer<vtkTransform> t);

    /**
     * @brief enter picked state, show boxwidget which allow to adjust annotation
     * @param interactor
     */
    void picked(vtkRenderWindowInteractor *interactor);

    /**
     * @brief disable boxWidget
     */
    void unpicked();

    /**
     * @brief keep current orientation, re-compute the center and scale
     * to make annotation fit to selected point well enough
     */
    void adjustToAnchor();

    /**
     * @brief change the type of annotation, and color too
     * @param value
     */
    void setType(const string value);

    vtkSmartPointer<vtkActor> getActor() const;

    string getType() const;


protected:
    void initial();

    /**
     * @brief color the annotation with given color
     * @param color_index
     * if color_index>=0,refer to @ref pcl::GlasbeyLUT
     * otherwise use color already mapped by type
     */
    void colorAnnotation(int color_index = -1);

    /**
     * @brief copy selected points as anchor to current annotation
     * @param cloud
     * @param slice
     */
    void setAnchorPoint(const PointCloudTPtr cloud, const vector<int> &slice);

    /**
     * @brief computeScaleAndCenterShift
     * @param o direction
     * @param scs ["scale", "center shift"]
     * @return scale
     */
    double computeScaleAndCenterShift(double o[3], double scs[2]);


private:
    string m_type;
    vtkSmartPointer<vtkAnnotationBoxSource> m_source;
    vtkSmartPointer<vtkActor> m_actor;
    vtkSmartPointer<vtkPolyDataMapper> m_mapper;
    vtkSmartPointer<vtkTransform> m_transform;

    vtkSmartPointer<vtkBoxWidgetRestricted> m_boxWidget;
    vtkSmartPointer<vtkBoxWidgetCallback0> m_boxWidgetCallback0;
    vtkSmartPointer<vtkBoxWidgetCallback1> m_boxWidgetCallback1;

    vector<double *> m_anchorPoints;
    double m_center[3];

    // Position of sensor
    double m_cameraPos[3];

    // NOTE not used
    bool m_visible;
    bool m_lock;


public:
    /**
     * @brief get types vector pointer
     * @return
     */
    static vector<string> *getTypes();

    /**
     * @brief getTypeIndex  auto add to vector map if has not
     * @param type_
     * @return
     */
    static int getTypeIndex(string type_);

    /**
     * @brief getColor map type to color in pcl::GlasbeyLUT
     * @param type_
     * @return
     */
    static pcl::RGB getColor(string type_);

    /**
     * @brief computeOBB compute max,min [x,y,z] aligned to xyz axis
     * @param cloud
     * @param slice
     * @param p1 min [x,y,z]
     * @param p2 max [x,y,z]
     */
    static void
    computeOBB(const PointCloudTPtr cloud, vector<int> &slice, double p1[3], double p2[3]);

private:
    /**
     * @brief types all annotation type here
     */
    static vector<string> *m_types;

};


class Annotations
{
public:
    /**
     * @brief from annotatin box actor to find annotation itself
     * @param actor
     * @return
     */
    Annotation *getAnnotation(vtkActor *actor);

    explicit Annotations();

    void push_back(Annotation *anno);

    void remove(Annotation *anno);

    void clear();

    int getSize();

    /**
     * @brief load annotations from file
     * @param filename
     */
    void loadAnnotations(string filename);

    /**
     * @brief save annotations to file
     * @param filename
     */
    void saveAnnotations(string filename);

    vector<Annotation *> &getAnnotations();

protected:
    /**
     * @brief keep all annotation from current cloud
     */
    vector<Annotation *> m_annotations;
    DatasetFormat m_datasetType;
};


#endif //Annotaions_H
