#include <iostream>
#include <DumpSTL/DumpSTL.h>

using namespace DUMP;

int main() {
    save("dumpStlExample_points", std::vector<Point3f>{ {0.f, 0.f, 0.f},
        { 1.f, 1.f, 0.f },
        { 2.f, 0.f, 0.f },
        { 3.f, 1.f, 0.f },
        { 4.f, 0.f, 0.f }});

    save("dumpStlExample_spheres", sphere({ {0.f, 0.f, 0.f},
                                           {5.f, 5.f, 5.f},
                                           {5.f, 0.f, 0.f},
                                           {0.f, 5.f, 0.f},
                                           {0.f, 0.f, 5.f} }));

    save("dumpStlExample_lineChain", line({ {0.f, 0.f, 0.f},
                                           {1.f, 2.f, 3.f},
                                           {1.f, 3.f, 4.f},
                                           {4.f, 0.f, 0.f},
                                           {0.f, 0.f, 0.f} }));

    saveInc("dumpStlExample_directionChain", direction({ {0.f, 0.f, 0.f},
                                                        {1.f, 2.f, 3.f},
                                                        {1.f, 3.f, 4.f},
                                                        {4.f, 0.f, 0.f},
                                                        {0.f, 0.f, 0.f} }));
    saveInc("dumpStlExample_directionChain", direction({ {0.f, 0.f, 0.f},
                                                        {1.f, 1.f, 0.f},
                                                        {2.f, 0.f, 0.f},
                                                        {3, 1.f, 0.f},
                                                        {4.f, 0.f, 0.f} }));
    saveInc("dumpStlExample_directionChain", direction({ {0.f, 0.f, 0.f},
                                                        {5.f, 5.f, 5.f},
                                                        {5.f, 0.f, 0.f},
                                                        {0.f, 5.f, 0.f},
                                                        {0.f, 0.f, 5.f} }));

    const std::vector<Triangle> tetraedr = {
        Triangle({0.f, 0.f, 0.f}, {0.f, 1.f, 1.f}, {1.f, 0.f, 1.f}),
        Triangle({0.f, 1.f, 1.f}, {1.f, 1.f, 0.f}, {1.f, 0.f, 1.f}),
        Triangle({0.f, 0.f, 0.f}, {1.f, 1.f, 0.f}, {1.f, 0.f, 1.f}),
        Triangle({0.f, 0.f, 0.f}, {1.f, 1.f, 0.f}, {0.f, 1.f, 1.f}) };
    save("dumpStlExample_tetraedr", tetraedr);

    Model3D cube;
    cube.addQuad({ 0.f, 0.f, 0.f }, { 0.f, 0.f, 1.f }, { 1.f, 0.f, 1.f },
        { 1.f, 0.f, 0.f });
    cube.addQuad({ 0.f, 1.f, 0.f }, { 0.f, 1.f, 1.f }, { 1.f, 1.f, 1.f },
        { 1.f, 1.f, 0.f });
    cube.addQuad({ 0.f, 0.f, 0.f }, { 0.f, 0.f, 1.f }, { 0.f, 1.f, 1.f },
        { 0.f, 1.f, 0.f });
    cube.addQuad({ 1.f, 0.f, 0.f }, { 1.f, 1.f, 0.f }, { 1.f, 1.f, 1.f },
        { 1.f, 0.f, 1.f });
    cube.addQuad({ 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }, { 1.f, 0.f, 0.f },
        { 1.f, 1.f, 0.f });
    cube.addQuad({ 0.f, 0.f, 1.f }, { 0.f, 1.f, 1.f }, { 1.f, 1.f, 1.f },
        { 1.f, 0.f, 1.f });
    save("dumpStlExample_cube", cube);
}