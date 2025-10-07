#include "WellOpenGLWidget.h"
#include "../WellSchematicTree/WellSchematicTree.h"
#include "../PropertiesWidget/WellComponentItem.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLContext>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDebug>

WellOpenGLWidget::WellOpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent), m_wellTree(nullptr), m_program(nullptr)
{
    setMinimumSize(400, 600);
}

WellOpenGLWidget::~WellOpenGLWidget()
{
    makeCurrent();
    m_vbo.destroy();
    delete m_program;
    doneCurrent();
}

void WellOpenGLWidget::setWellTree(WellSchematicTree *tree)
{
    m_wellTree = tree;
    updateWellData();
}

void WellOpenGLWidget::updateWellData()
{
    extractComponentsFromTree();
    update();
}

void WellOpenGLWidget::onTreeChanged()
{
    updateWellData();
}

void WellOpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    setupShaders();
    createGeometry();
}

void WellOpenGLWidget::setupShaders()
{
    m_program = new QOpenGLShaderProgram(this);

    // Vertex shader
    const char *vshader =
        "#version 330 core\n"
        "layout(location = 0) in vec3 vertexPosition;\n"
        "layout(location = 1) in vec3 vertexColor;\n"
        "uniform mat4 matrix;\n"
        "out vec3 fragColor;\n"
        "void main() {\n"
        "    fragColor = vertexColor;\n"
        "    gl_Position = matrix * vec4(vertexPosition, 1.0);\n"
        "}\n";

    // Fragment shader
    const char *fshader =
        "#version 330 core\n"
        "in vec3 fragColor;\n"
        "out vec4 color;\n"
        "void main() {\n"
        "    color = vec4(fragColor, 1.0);\n"
        "}\n";

    if (!m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vshader)) {
        qDebug() << "Vertex shader compilation failed:" << m_program->log();
    }

    if (!m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fshader)) {
        qDebug() << "Fragment shader compilation failed:" << m_program->log();
    }

    if (!m_program->link()) {
        qDebug() << "Shader program linking failed:" << m_program->log();
    }

    m_vertexLocation = 0;
    m_colorLocation = 1;
    m_matrixLocation = m_program->uniformLocation("matrix");
}

void WellOpenGLWidget::createGeometry()
{
    // Create and bind VAO
    m_vao.create();
    m_vao.bind();

    // Create and bind VBO
    m_vbo.create();
    m_vbo.bind();

    // Set vertex attribute pointers
    m_program->enableAttributeArray(m_vertexLocation);
    m_program->setAttributeBuffer(m_vertexLocation, GL_FLOAT, 0, 3, 6 * sizeof(float));

    m_program->enableAttributeArray(m_colorLocation);
    m_program->setAttributeBuffer(m_colorLocation, GL_FLOAT, 3 * sizeof(float), 3, 6 * sizeof(float));

    m_vbo.release();
    m_vao.release();
}

void WellOpenGLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);

    m_projection.setToIdentity();
    float aspect = float(w) / float(h ? h : 1);
    m_projection.ortho(-aspect, aspect, -1.0f, 1.0f, -10.0f, 10.0f);

    m_view.setToIdentity();
    m_view.translate(0.0f, 0.0f, -2.0f);
}

void WellOpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!m_program->bind()) {
        return;
    }

    m_model.setToIdentity();
    QMatrix4x4 matrix = m_projection * m_view * m_model;
    m_program->setUniformValue(m_matrixLocation, matrix);

    renderWellComponents();

    m_program->release();
}

void WellOpenGLWidget::renderWellComponents()
{
    if (m_components.isEmpty()) {
        renderDefaultWell();
        return;
    }

    m_vertices.clear();
    m_colors.clear();

    // Create geometry for all components
    for (const WellComponent &component : m_components) {
        float top = component.topDepth / 3000.0f; // Normalize depth
        float bottom = component.bottomDepth / 3000.0f;
        float od = component.outerDiameter * 0.1f; // Scale diameter
        float id = component.innerDiameter * 0.1f;

        QVector3D color = component.color;

        // Create cylinder geometry for the component
        // Outer cylinder
        addCylinder(0.0f, 0.0f, top, bottom, od, 32, color);

        // Inner cylinder (if applicable)
        if (id > 0.01f) {
            addCylinder(0.0f, 0.0f, top + 0.01f, bottom - 0.01f, id, 32, QVector3D(0.1f, 0.1f, 0.1f));
        }
    }

    // Upload data to GPU
    m_vbo.bind();
    m_vbo.allocate(m_vertices.data(), m_vertices.size() * sizeof(float));
    m_vbo.release();

    // Render
    m_vao.bind();
    glDrawArrays(GL_TRIANGLES, 0, m_vertices.size() / 6);
    m_vao.release();
}

void WellOpenGLWidget::addCylinder(float x, float y, float top, float bottom, float radius, int segments, const QVector3D &color)
{
    float height = bottom - top;
    float angleStep = 2.0f * M_PI / segments;

    for (int i = 0; i < segments; ++i) {
        float angle1 = i * angleStep;
        float angle2 = (i + 1) * angleStep;

        // Calculate vertices for current segment
        float x1 = radius * cos(angle1);
        float z1 = radius * sin(angle1);
        float x2 = radius * cos(angle2);
        float z2 = radius * sin(angle2);

        // Top triangle
        addVertex(x, top, 0.0f, color);
        addVertex(x + x1, top, z1, color);
        addVertex(x + x2, top, z2, color);

        // Bottom triangle
        addVertex(x, bottom, 0.0f, color);
        addVertex(x + x2, bottom, z2, color);
        addVertex(x + x1, bottom, z1, color);

        // Side quad (two triangles)
        addVertex(x + x1, top, z1, color);
        addVertex(x + x2, top, z2, color);
        addVertex(x + x1, bottom, z1, color);

        addVertex(x + x1, bottom, z1, color);
        addVertex(x + x2, top, z2, color);
        addVertex(x + x2, bottom, z2, color);
    }
}

void WellOpenGLWidget::addVertex(float x, float y, float z, const QVector3D &color)
{
    m_vertices << x << y << z;
    m_vertices << color.x() << color.y() << color.z();
}

void WellOpenGLWidget::renderDefaultWell()
{
    m_vertices.clear();
    m_colors.clear();

    // Default well representation
    QVector3D casingColor(0.7f, 0.7f, 0.7f); // Silver
    QVector3D openHoleColor(0.55f, 0.27f, 0.07f); // Brown

    // Example casing sections
    addCylinder(0.0f, 0.0f, -0.9f, -0.7f, 0.3f, 32, casingColor);
    addCylinder(0.0f, 0.0f, -0.7f, -0.5f, 0.25f, 32, casingColor);
    addCylinder(0.0f, 0.0f, -0.5f, -0.3f, 0.2f, 32, casingColor);

    // Open hole section
    addCylinder(0.0f, 0.0f, -0.3f, 0.0f, 0.15f, 32, openHoleColor);

    // Upload data to GPU
    m_vbo.bind();
    m_vbo.allocate(m_vertices.data(), m_vertices.size() * sizeof(float));
    m_vbo.release();

    // Render
    m_vao.bind();
    glDrawArrays(GL_TRIANGLES, 0, m_vertices.size() / 6);
    m_vao.release();
}

void WellOpenGLWidget::extractComponentsFromTree()
{
    m_components.clear();

    if (!m_wellTree) return;

    // Traverse tree and extract component data
    // This is a simplified implementation - you would need to adapt it to your tree structure

    WellComponent casing1;
    casing1.type = "Casing";
    casing1.name = "Casing - 9.5/8";
    casing1.topDepth = 0.0f;
    casing1.bottomDepth = 1000.0f;
    casing1.outerDiameter = 9.625f;
    casing1.innerDiameter = 8.835f;
    casing1.color = getColorForType("Casing");
    m_components.append(casing1);

    WellComponent cement1;
    cement1.type = "Cement";
    cement1.name = "Cement";
    cement1.topDepth = 0.0f;
    cement1.bottomDepth = 1000.0f;
    cement1.outerDiameter = 12.0f;
    cement1.innerDiameter = 9.625f;
    cement1.color = getColorForType("Cement");
    m_components.append(cement1);

    WellComponent casing2;
    casing2.type = "Casing";
    casing2.name = "Casing - 7";
    casing2.topDepth = 1000.0f;
    casing2.bottomDepth = 2000.0f;
    casing2.outerDiameter = 7.0f;
    casing2.innerDiameter = 6.366f;
    casing2.color = getColorForType("Casing");
    m_components.append(casing2);

    WellComponent openHole;
    openHole.type = "OpenHole";
    openHole.name = "Open Hole - 6.5/8";
    openHole.topDepth = 2000.0f;
    openHole.bottomDepth = 2950.0f;
    openHole.outerDiameter = 6.625f;
    openHole.innerDiameter = 6.5f;
    openHole.color = getColorForType("OpenHole");
    m_components.append(openHole);
}

QVector3D WellOpenGLWidget::getColorForType(const QString &type)
{
    static QMap<QString, QVector3D> colorMap = {
        {"Casing", QVector3D(0.7f, 0.7f, 0.7f)},      // Silver
        {"Cement", QVector3D(0.82f, 0.71f, 0.55f)},   // Tan
        {"OpenHole", QVector3D(0.55f, 0.27f, 0.07f)}, // Brown
        {"Tubing", QVector3D(0.27f, 0.51f, 0.71f)},   // Steel blue
        {"Packer", QVector3D(1.0f, 0.65f, 0.0f)},     // Orange
        {"Perforation", QVector3D(1.0f, 0.0f, 0.0f)}  // Red
    };

    return colorMap.value(type, QVector3D(0.5f, 0.5f, 0.5f)); // Default gray
}
