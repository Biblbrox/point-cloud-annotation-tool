#include <vtk/vtkBoxWidgetCallback.h>
#include <Annotation.h>
#include <vtkBoxWidget.h>
#include <vtkTransform.h>

vtkBoxWidgetCallback0 *vtkBoxWidgetCallback0::New()
{
    return new vtkBoxWidgetCallback0;
}

void vtkBoxWidgetCallback0::Execute(vtkObject *caller, unsigned long, void *)
{
    // Here we use the vtkBoxWidget to transform the underlying coneActor
    // (by manipulating its transformation matrix).
    vtkSmartPointer<vtkTransform> t =
            vtkSmartPointer<vtkTransform>::New();
    vtkBoxWidget *widget = reinterpret_cast<vtkBoxWidget*>(caller);

    widget->GetTransform( t );
    if (m_anno){
        // widget->GetProp3D()->SetUserTransform(t);
        m_anno->applyTransform(t);

    }
}

void vtkBoxWidgetCallback0::setAnno(Annotation *value)
{
    m_anno = value;
}


vtkBoxWidgetCallback1 *vtkBoxWidgetCallback1::New()
{
    return new vtkBoxWidgetCallback1;
}

void vtkBoxWidgetCallback1::Execute(vtkObject *caller, unsigned long, void *)
{
    if (m_anno)
        m_anno->adjustToAnchor();
}

void vtkBoxWidgetCallback1::setAnno(Annotation *value)
{
    m_anno = value;
}
