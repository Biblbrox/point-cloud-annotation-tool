#include <Annotation.h>
#include <vtkAnnotationBoxSource.h>
#include <vtkBoxWidgetRestricted.h>
#include <vtkBoxWidgetCallback.h>

using namespace std;

Annotation::Annotation(const BoxLabel &label, bool visible_, bool lock_)
        : m_visible(visible_), m_lock(lock_)
{
    //m_type
    m_type = label.type;

    // init variable
    initial();

    // apply transform
    vtkSmartPointer<vtkTransform> cubeTransform = vtkSmartPointer<vtkTransform>::New();
    cubeTransform->PostMultiply();
    cubeTransform->Scale(label.detail.length, label.detail.width, label.detail.height);
    //        cubeTransform->RotateZ(label.detail.yaw);
    cubeTransform->RotateZ(label.detail.yaw * 180 / vtkMath::Pi());
    cubeTransform->Translate(label.detail.center_x, label.detail.center_y, label.detail.center_z);

    applyTransform(cubeTransform);
}

Annotation::Annotation(const PointCloudTPtr cloud, vector<int> &slice, string type_)
{
    double p1[3];
    double p2[3];
    computeOBB(cloud, slice, p1, p2);
    BoxLabel label(p1, p2, type_);

    setAnchorPoint(cloud, slice);

    m_type = type_;

    // init variable
    initial();

    // apply transform
    vtkSmartPointer<vtkTransform> cubeTransform = vtkSmartPointer<vtkTransform>::New();
    cubeTransform->PostMultiply();
    cubeTransform->Scale(label.detail.length, label.detail.width, label.detail.height);
    //        cubeTransform->RotateZ(label.detail.yaw);
    cubeTransform->RotateZ(label.detail.yaw * 180 / vtkMath::Pi());
    cubeTransform->Translate(label.detail.center_x, label.detail.center_y, label.detail.center_z);

    applyTransform(cubeTransform);


//    label.detail.left
}

Annotation::~Annotation()
{
    // release anchorPoints
    for (auto p: m_anchorPoints)
        delete[] p;

    m_anchorPoints.clear();
}

BoxLabel Annotation::getBoxLabel()
{
    BoxLabel label;
    label.type = m_type;

    double pos[3];
    double scale[3];
    double orientation[3];
    m_transform->GetPosition(pos);
    m_transform->GetScale(scale);
    m_transform->GetOrientation(orientation);
    memcpy(label.data, pos, 3 * sizeof(double));
    memcpy(label.data + 3, scale, 3 * sizeof(double));
    label.detail.yaw = orientation[2] / 180 * vtkMath::Pi();
    return label;
}

void Annotation::applyTransform(vtkSmartPointer<vtkTransform> t)
{
    if (t == m_transform) return;

    m_transform = t;
    m_actor->SetUserTransform(t);

}

void Annotation::picked(vtkRenderWindowInteractor *interactor)
{
    // enable box widget
    m_boxWidget = vtkSmartPointer<vtkBoxWidgetRestricted>::New();
    m_boxWidgetCallback0 = vtkSmartPointer<vtkBoxWidgetCallback0>::New();
    m_boxWidgetCallback0->setAnno(this);
    m_boxWidgetCallback1 = vtkSmartPointer<vtkBoxWidgetCallback1>::New();
    m_boxWidgetCallback1->setAnno(this);

    m_boxWidget->SetInteractor(interactor);
    // boxWidget->SetProp3D( cubeActor );
    // boxWidget->SetPlaceFactor( 1.25 ); // Make the box 1.25x larger than the actor
    // boxWidget->PlaceWidget();

    // default is [-0.5, 0.5], NOTE
    // [-1,1] makes boxwidget fit to annotion,
    // but [-0.5, 0.5] should be in the correct way, may be some bug
    double bounds[6] = {-1, 1, -1, 1, -1, 1};
    m_boxWidget->PlaceWidget(bounds);

    vtkSmartPointer<vtkTransform> t = vtkSmartPointer<vtkTransform>::New();
    t->DeepCopy(m_actor->GetUserTransform());
    m_boxWidget->SetTransform(t);
    m_boxWidget->SetHandleSize(0.01);
    m_boxWidget->GetOutlineProperty()->SetAmbientColor(1.0, 0.0, 0.0);

    m_boxWidget->AddObserver(vtkCommand::InteractionEvent, m_boxWidgetCallback0);
    m_boxWidget->AddObserver(vtkCommand::EndInteractionEvent, m_boxWidgetCallback1);
    m_boxWidget->On();
}

void Annotation::unpicked()
{
    m_boxWidget->Off();
}

void Annotation::adjustToAnchor()
{
    if (m_anchorPoints.empty())
        return;

    m_transform->GetPosition(m_center);

    double r[3], x[3], y[3], z[3] = {0, 0, 1};
    double s[3]; // scale

    m_transform->GetOrientation(r);
    x[0] = cos(vtkMath::RadiansFromDegrees(r[2]));
    x[1] = sin(vtkMath::RadiansFromDegrees(r[2]));
    x[2] = 0; // direction
    vtkMath::Cross(z, x, y);

    double scs[2];
    s[0] = computeScaleAndCenterShift(x, scs);
    vtkMath::MultiplyScalar(x, scs[1]);
    s[1] = computeScaleAndCenterShift(y, scs);
    vtkMath::MultiplyScalar(y, scs[1]);
    s[2] = computeScaleAndCenterShift(z, scs);
    vtkMath::MultiplyScalar(z, scs[1]);

    // apply center shift
    vtkMath::Add(m_center, x, m_center);
    vtkMath::Add(m_center, y, m_center);
    vtkMath::Add(m_center, z, m_center);

    vtkSmartPointer<vtkTransform> t = vtkSmartPointer<vtkTransform>::New();
    t->Translate(m_center);
    t->RotateZ(r[2]);
    t->Scale(s);

    //        transform->Scale(s);
    m_boxWidget->SetTransform(t);
    applyTransform(t);
}


string Annotation::getType() const
{
    return m_type;
}

void Annotation::setType(const string value)
{
    if (value != m_type) {
        m_type = value;
        colorAnnotation();
    }
}

vtkSmartPointer<vtkActor> Annotation::getActor() const
{
    return m_actor;
}

void Annotation::initial()
{
    // Cube
    m_source = vtkSmartPointer<vtkAnnotationBoxSource>::New();
    m_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    m_mapper->SetInputConnection(m_source->GetOutputPort());
    m_actor = vtkSmartPointer<vtkActor>::New();
    m_actor->SetMapper(m_mapper);
    m_transform = vtkSmartPointer<vtkTransform>::New();
    colorAnnotation();
}

void Annotation::colorAnnotation(int color_index)
{
    vtkSmartPointer<vtkLookupTable> lut = vtkSmartPointer<vtkLookupTable>::New();
    lut->SetNumberOfTableValues(2);
    lut->Build();
    if (color_index < 0) {
        pcl::RGB c = getColor(m_type);
        lut->SetTableValue(0, c.r / 255.0, c.g / 255.0, c.b / 255.0, 0);
        lut->SetTableValue(1, c.r / 255.0, c.g / 255.0, c.b / 255.0, 1);
    } else {
        pcl::RGB c = pcl::GlasbeyLUT::at(color_index);
        lut->SetTableValue(0, c.r / 255.0, c.g / 255.0, c.b / 255.0, 0);
        lut->SetTableValue(1, c.r / 255.0, c.g / 255.0, c.b / 255.0, 1);
    }

    vtkSmartPointer<vtkFloatArray> cellData =
            vtkSmartPointer<vtkFloatArray>::New();

    //line color
    for (int i = 0; i < 12; i++) {
        cellData->InsertNextValue(1);
    }

    //face color
    for (int i = 0; i < 6; i++) {
        cellData->InsertNextValue(0);
    }

    // plusX face
    cellData->InsertValue(12, 1);

    m_source->Update();
    m_source->GetOutput()->GetCellData()->SetScalars(cellData);

    m_actor->GetProperty()->SetLineWidth(2);
    m_actor->GetProperty()->SetLighting(false);

    m_mapper->SetLookupTable(lut);
}

void Annotation::setAnchorPoint(const PointCloudTPtr cloud, const vector<int> &slice)
{
    m_anchorPoints.clear();
    for (auto i:slice) {
        double *p = new double[3];
        p[0] = cloud->points[i].x;
        p[1] = cloud->points[i].y;
        p[2] = cloud->points[i].z;
        m_anchorPoints.push_back(p);
    }
}

double Annotation::computeScaleAndCenterShift(double o[], double scs[])
{
    vtkMath::Normalize(o);

    double a, b;
    a = -std::numeric_limits<double>::max();
    b = std::numeric_limits<double>::max();

    double t[3];
    for (auto x: m_anchorPoints) {
        vtkMath::Subtract(x, m_center, t);
        double s = vtkMath::Dot(t, o);
        a = std::max(a, s);
        b = std::min(b, s);
    }
    scs[0] = a - b;
    scs[1] = (a + b) / 2;
    return a - b;
}

vector<string> *Annotation::getTypes()
{
    return m_types;
}

int Annotation::getTypeIndex(string type_)
{
    for (int i = 0; i < m_types->size(); i++)
        if (m_types->at(i) == type_) return i;

    m_types->push_back(type_);
    return m_types->size() - 1;
}

pcl::RGB Annotation::getColor(string type_)
{
    return pcl::GlasbeyLUT::at(getTypeIndex(type_));
}

void
Annotation::computeOBB(const PointCloudTPtr cloud, vector<int> &slice, double p1[3], double p2[3])
{
    p1[0] = std::numeric_limits<double>::max();
    p1[1] = std::numeric_limits<double>::max();
    p1[2] = std::numeric_limits<double>::max();

    //std::numeric_limits <double>::min (); is a number close enough to 0
    p2[0] = -std::numeric_limits<double>::max();
    p2[1] = -std::numeric_limits<double>::max();
    p2[2] = -std::numeric_limits<double>::max();

    for (auto i:slice) {
        p1[0] = std::min(p1[0], (double) cloud->points[i].x);
        p1[1] = std::min(p1[1], (double) cloud->points[i].y);
        p1[2] = std::min(p1[2], (double) cloud->points[i].z);

        p2[0] = std::max(p2[0], (double) cloud->points[i].x);
        p2[1] = std::max(p2[1], (double) cloud->points[i].y);
        p2[2] = std::max(p2[2], (double) cloud->points[i].z);

    }
}

// static variable
vector<string> *Annotation::m_types = new vector<string>();


Annotation *Annotations::getAnnotation(vtkActor *actor)
{
    for (auto *anno : m_annotations)
        if (anno->getActor() == actor)
            return anno;

    return 0;
}

void Annotations::push_back(Annotation *anno)
{
    m_annotations.push_back(anno);
}

void Annotations::remove(Annotation *anno)
{
    m_annotations.erase(std::remove(m_annotations.begin(), m_annotations.end(), anno),
                        m_annotations.end());
}

void Annotations::clear()
{
    for (auto p:m_annotations)
        delete p;

    m_annotations.clear();
}

int Annotations::getSize()
{
    return m_annotations.size();
}

void Annotations::loadAnnotations(string filename)
{
    m_annotations.clear();

    std::ifstream input(filename.c_str(), std::ios_base::in);
    if (!input.good()) {
        std::cerr << "Cannot open file : " << filename << std::endl;
        return;
    }
    std::string type;
    while (input >> type) {
        BoxLabel label;
        label.type = type;
        for (double &j : label.data)
            input >> j;

        m_annotations.push_back(new Annotation(label));
    }
    std::cout << filename << ": load " << m_annotations.size() << " boxes" << std::endl;
    input.close();
}

void Annotations::saveAnnotations(string filename)
{
    if (m_annotations.empty())
        return;

    std::ofstream output(filename.c_str(), std::ios_base::out);
    for (auto anno: m_annotations) {
        if (m_datasetType == DatasetFormat::KITTI) {
            auto box = anno->getBoxLabel();
            output << box.type << " "; // Class object
            output << 0.0 << " "; // Truncated
            output << 0 << " "; // Occluded
            output << 123 << " "; // Observation angle of object TODO: fix this
            output << 0 << " " << 0 << " "; // 2D bounding box TODO: fix this
            output << box.data[5] << " " << box.data[4] << " " << box.data[3] << " "; // height, width, length
            output << box.data[0] - box.cameraPosX << " " << box.data[1] - box.cameraPosY << " "
                   << box.data[2] - box.cameraPosZ << " "; // Position in camera coords
            output << box.detail.yaw << " "; // Rotation about vertical axis(yaw)
        } else {
            output << anno->getBoxLabel().toString() << std::endl;
        }
    }
    output.close();
}

vector<Annotation *> &Annotations::getAnnotations()
{
    return m_annotations;
}

Annotations::Annotations() : m_datasetType(DatasetFormat::KITTI)
{
}


