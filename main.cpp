//#include <QCoreApplication>

#include <iostream>

#define GL_SILENCE_DEPRECATION

#include <osgViewer/Viewer>
#include <osgViewer/config/SingleWindow>
#include <osg/ShapeDrawable>

#include <imgui.h>
#include <imgui_impl_opengl3.h>

#include "OsgImGuiHandler.h"

//int main(int argc, char *argv[])
//{
//    QCoreApplication a(argc, argv);

//    return a.exec();
//}

class ImGuiInitOperation : public osg::Operation
{
public:
    ImGuiInitOperation()
        : osg::Operation("ImGuiInitOperation", false)
    {
    }

    void operator()(osg::Object* object) override
    {
        osg::GraphicsContext* context = dynamic_cast<osg::GraphicsContext*>(object);
        if (!context)
            return;

        if (!ImGui_ImplOpenGL3_Init("#version 410"))
        {
            std::cout << "ImGui_ImplOpenGL3_Init() failed\n";
        }
    }
};


osg::ref_ptr<osg::Geometry> createAxis() {
    osg::ref_ptr<osg::Geometry> geometry (new osg::Geometry());

    osg::ref_ptr<osg::Vec3Array> vertices (new osg::Vec3Array());
    vertices->push_back (osg::Vec3 ( 0.0, 0.0, 0.0));
    vertices->push_back (osg::Vec3 ( 10.0, 0.0, 0.0));
    vertices->push_back (osg::Vec3 ( 0.0, 0.0, 0.0));
    vertices->push_back (osg::Vec3 ( 0.0, 10.0, 0.0));
    vertices->push_back (osg::Vec3 ( 0.0, 0.0, 0.0));
    vertices->push_back (osg::Vec3 ( 0.0, 0.0, 10.0));
    geometry->setVertexArray (vertices.get());

    osg::ref_ptr<osg::Vec4Array> colors (new osg::Vec4Array());
    colors->push_back (osg::Vec4 (1.0f, 0.0f, 0.0f, 1.0f));
    colors->push_back (osg::Vec4 (1.0f, 0.0f, 0.0f, 1.0f));
    colors->push_back (osg::Vec4 (0.0f, 1.0f, 0.0f, 1.0f));
    colors->push_back (osg::Vec4 (0.0f, 1.0f, 0.0f, 1.0f));
    colors->push_back (osg::Vec4 (0.0f, 0.0f, 1.0f, 1.0f));
    colors->push_back (osg::Vec4 (0.0f, 0.0f, 1.0f, 1.0f));
    geometry->setColorArray (colors.get(), osg::Array::BIND_PER_VERTEX);
    geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES,0,6));
    return geometry.release();
}

osg::ref_ptr<osg::Geometry> createPoints()
{
    // create Geometry object to store all the vertices and lines primitive.
    osg::Geometry* polyGeom = new osg::Geometry();

    // note, first coord at top, second at bottom, reverse to that buggy OpenGL image..
    osg::Vec3 myCoords[] =
    {
        // TRIANGLES 6 vertices, v0..v5
        // note in anticlockwise order.
        osg::Vec3(-1.12056, -2.15188e-09, -0.840418),
        osg::Vec3(-0.95165, -2.15188e-09, -0.840418),
        osg::Vec3(-1.11644, 9.18133e-09, -0.716827),

        // note in anticlockwise order.
        osg::Vec3(-0.840418, 9.18133e-09, -0.778623),
        osg::Vec3(-0.622074, 9.18133e-09, -0.613835),
        osg::Vec3(-1.067, 9.18133e-09, -0.609715),

        // TRIANGLE STRIP 6 vertices, v6..v11
        // note defined top point first,
        // then anticlockwise for the next two points,
        // then alternating to bottom there after.
        osg::Vec3(-0.160668, -2.15188e-09, -0.531441),
        osg::Vec3(-0.160668, -2.15188e-09, -0.749785),
        osg::Vec3(0.0617955, 9.18133e-09, -0.531441),
        osg::Vec3(0.168908, -2.15188e-09, -0.753905),
        osg::Vec3(0.238942, -2.15188e-09, -0.531441),
        osg::Vec3(0.280139, -2.15188e-09, -0.823939),

        // TRIANGLE FAN 5 vertices, v12..v16
        // note defined in anticlockwise order.
        osg::Vec3(0.844538, 9.18133e-09, -0.712708),
        osg::Vec3(1.0258, 9.18133e-09, -0.799221),
        osg::Vec3(1.03816, -2.15188e-09, -0.692109),
        osg::Vec3(0.988727, 9.18133e-09, -0.568518),
        osg::Vec3(0.840418, -2.15188e-09, -0.506723),

    };

    int numCoords = sizeof(myCoords)/sizeof(osg::Vec3);

    osg::Vec3Array* vertices = new osg::Vec3Array(numCoords,myCoords);

    polyGeom->setVertexArray(vertices);

    polyGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES,0,6));
    polyGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_STRIP,6,6));
    polyGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_FAN,12,5));

    return polyGeom;
}


class OsgManager {
public:
    static OsgManager* getInstance() {
        if (instance == nullptr) {
            instance = new OsgManager();
        }
        return instance;
    }

    void setView(osgViewer::Viewer &pviewer) {
        m_pViewer = &pviewer;
        m_pViewer->setSceneData(root);
    }

    void show() {
        root->addChild(createPoints());
    }

    void reset() {
        root->removeChildren(0, root->getNumChildren());
    }

    osg::ref_ptr<osg::Geode> getRoot() {
        return root;
    }

private:
    OsgManager() {
        root = new osg::Geode;
        root->addChild(createAxis());
    }

    static OsgManager* instance;
    osg::ref_ptr<osgViewer::Viewer> m_pViewer;
    osg::ref_ptr<osg::Geode> root;
};


OsgManager* OsgManager::instance;

class ImGuiDemo : public OsgImGuiHandler
{
protected:
    void drawUi() override
    {
        // ImGui code goes here...
        //ImGui::ShowDemoWindow();
        ImGui::Begin("Hello, world!");
        if (ImGui::Button("click button")) {
            OsgManager::getInstance()->show();
        }
        if (ImGui::Button("reset button")) {
            OsgManager::getInstance()->reset();
        }

        ImGui::End();
    }
};

int main() {
    osgViewer::Viewer pviewer;

    const std::string version( "3.0" );
    osg::ref_ptr< osg::GraphicsContext::Traits > traits = new osg::GraphicsContext::Traits();
        traits->x = 20; traits->y = 30;
        traits->width = 800; traits->height = 450;
        traits->windowDecoration = true;
        traits->doubleBuffer = true;
        traits->glContextVersion = version;
        traits->readDISPLAY();
        traits->setUndefinedScreenDetailsToDefaultScreen();
    osg::ref_ptr< osg::GraphicsContext > gc = osg::GraphicsContext::createGraphicsContext( traits.get() );
    osg::Camera* cam = pviewer.getCamera();
        cam->setGraphicsContext( gc.get() );

    pviewer.apply(new osgViewer::SingleWindow(100, 100, 640, 480));
    pviewer.addEventHandler(new ImGuiDemo);
    pviewer.setRealizeOperation(new ImGuiInitOperation);

    OsgManager::getInstance()->setView(pviewer);

//    osg::ref_ptr<osg::Geode> root = new osg::Geode;
//    root->addChild(createAxis());
//    pviewer.setSceneData(root);

    return pviewer.run();
}

