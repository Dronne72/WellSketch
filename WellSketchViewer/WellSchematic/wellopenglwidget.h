#ifndef WELLOPENGLWIDGET_H
#define WELLOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QVector3D>
#include <QMatrix4x4>
#include <QVector>
#include <QMap>
#include <cmath>

class WellComponentItem;

struct WellComponent {
    QString type;
    QString name;
    float topDepth;
    float bottomDepth;
    float outerDiameter;
    float innerDiameter;
    QVector3D color;
    QVector3D position;

    WellComponent() : topDepth(0), bottomDepth(0), outerDiameter(0), innerDiameter(0) {}
};

class WellOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit WellOpenGLWidget(QWidget *parent = nullptr);
    ~WellOpenGLWidget();

    void setWellTree(class WellSchematicTree *tree);
    void updateWellData();

public slots:
    void onTreeChanged();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    void setupShaders();
    void createGeometry();
    void renderWellComponents();
    void renderDefaultWell();
    void extractComponentsFromTree();
    void addCylinder(float x, float y, float top, float bottom, float radius, int segments, const QVector3D &color);
    void addVertex(float x, float y, float z, const QVector3D &color);
    QVector3D getColorForType(const QString &type);

    QOpenGLShaderProgram *m_program;
    QOpenGLBuffer m_vbo;
    QOpenGLVertexArrayObject m_vao;

    QVector<WellComponent> m_components;
    WellSchematicTree *m_wellTree;

    QMatrix4x4 m_projection;
    QMatrix4x4 m_view;
    QMatrix4x4 m_model;

    // Shader attribute locations
    int m_vertexLocation;
    int m_colorLocation;
    int m_matrixLocation;

    // Geometry data
    QVector<float> m_vertices;
    QVector<float> m_colors;
};

#endif // WELLOPENGLWIDGET_H
