#pragma once

#include <math.h>
#include <string>
#include <fstream>
#include <filesystem>

namespace STL //settings
{
    constexpr bool isEnable = true;
    constexpr std::string_view folderSTL = "";
    //constexpr std::string_view folderSTL = "C:\\Repos\\STL\\";
    constexpr float coneBaseSize = 1.f / 20.f; //ratio length and size of base
    constexpr float oneSphereRadius = 0.1f; //
    constexpr float ratioSphereRadius = 0.025f;
}

namespace STL //header
{
#pragma pack(push, 4)
    template <typename T>
    struct Point3
    {
        T x;
        T y;
        T z;

        constexpr Point3(const T x, const T y, const T z);
        Point3() = default;

        Point3<T> normalize() const;
        float distance(const Point3<T>& pt) const;
        Point3<T> cross_product(const Point3<T>& pt) const;

        Point3<T>& operator+=(const Point3<T>&);
        Point3<T>& operator-=(const Point3<T>&);
        Point3<T>& operator/=(const T value);
        Point3<T>& operator*=(const T value);
    };
#pragma pack(pop)
    using Point3f = Point3<float>;

#pragma pack(push, 1)
    struct Triangle
    {
        Point3f norm = Point3f(0.f, 0.f, 1.f); //for export STL
        Point3f vertice0;
        Point3f vertice1;
        Point3f vertice2;
        char dummy0 = 0; //for export STL
        char dummy1 = 0; //for export STL

        Triangle() = default;
        constexpr Triangle(const Point3f& pt1, const Point3f& pt2, const Point3f& pt3);
        constexpr Triangle(Point3f&& pt1, Point3f&& pt2, Point3f&& pt3);

        static float getArea(const Point3f& pt1, const Point3f& pt2, const Point3f& pt3);
        Point3f& operator[] (const int index);
        const Point3f& operator[] (const int index) const;
    };
#pragma pack(pop)

    struct Model3D
    {
        std::vector<Triangle> triangles;

        void exportTxt(std::string_view str) const;
        void exportTxt(std::string&& str) const;
        void exportBin(std::string_view str) const;
        void exportBin(std::string&& str) const;

        void addTriangle(const Triangle& triangle);
        void addTriangle(Triangle&& triangle);
        void addTriangle(const Point3f& pt1, const Point3f& pt2, const Point3f& pt3);
        void addPoint(const Point3f& pt1);
        void addEdge(const Point3f& pt1, const Point3f& pt2);
        void addQuad(const Point3f& pt1, const Point3f& pt2, const Point3f& pt3, const Point3f& pt4);
        void addCone(const Point3f& pt1, const Point3f& pt2, const float baseSize = coneBaseSize);
        void addSphere(const Point3f& center, const float radius);

        static constexpr std::string_view extension = ".stl";
    };
}

namespace STL //source
{
    ///////////////////////////////////////////////// Point3
    template <typename T> constexpr Point3<T>::Point3(const T coordX, const T coordY, const T coordZ)
        : x(coordX), y(coordY), z(coordZ)
    {
    }
    template <typename T> Point3<T> Point3<T>::normalize() const
    {
        T norm = sqrt(x * x + y * y + z * z);
        return Point3<T>(x / norm, y / norm, z / norm);
    }
    template <typename T> Point3<T>& Point3<T>::operator+=(const Point3<T>& pt)
    {
        x += pt.x;
        y += pt.y;
        z += pt.z;
        return *this;
    }
    template <typename T> Point3<T>& Point3<T>::operator-=(const Point3<T>& pt)
    {
        x -= pt.x;
        y -= pt.y;
        z -= pt.z;
        return *this;
    }
    template <typename T> float Point3<T>::distance(const Point3<T>& pt) const
    {
        return sqrt((x - pt.x) * (x - pt.x)
            + (y - pt.y) * (y - pt.y)
            + (z - pt.z) * (z - pt.z));
    }
    template <typename T> Point3<T> Point3<T>::cross_product(const Point3<T>& pt) const
    {
        return { y * pt.z - z * pt.y,
            -(x * pt.z - pt.x * z),
            x * pt.y - pt.x * y };
    }
    template <typename T> Point3<T> operator-(const Point3<T>& pt1, const Point3<T>& pt2)
    {
        return Point3<T>(pt1.x - pt2.x, pt1.y - pt2.y, pt1.z - pt2.z);
    }
    template <typename T> Point3<T> operator+(const Point3<T>& pt1, const Point3<T>& pt2)
    {
        return Point3<T>(pt1.x + pt2.x, pt1.y + pt2.y, pt1.z + pt2.z);
    }

    template<typename T> Point3<T> operator*(const Point3<T>& pt, const T val)
    {
        return Point3<T>(pt.x * val, pt.y * val, pt.z * val);
    }
    template<typename T> Point3<T> operator*(const T val, const Point3<T>& pt)
    {
        return Point3<T>(pt.x * val, pt.y * val, pt.z * val);
    }
    template<typename T> Point3<T>& Point3<T>::operator*=(const T val)
    {
        x = *val;
        y = *val;
        z = *val;
    }
    template<typename T> Point3<T> operator/(const Point3<T>& pt, const T val)
    {
        return Point3<T>(pt.x / val, pt.y / val, pt.z / val);
    }
    template<typename T> Point3<T>& Point3<T>::operator/=(const T val)
    {
        x /= val;
        y /= val;
        z /= val;
        return *this;
    }

    ///////////////////////////////////////////////// Triangle
    constexpr Triangle::Triangle(const Point3f& pt1, const Point3f& pt2, const Point3f& pt3)
        : vertice0(pt1), vertice1(pt2), vertice2(pt3)
    {
    }
    constexpr Triangle::Triangle(Point3f&& pt1, Point3f&& pt2, Point3f&& pt3)
        : vertice0(std::move(pt1)), vertice1(std::move(pt2)), vertice2(std::move(pt3))
    {
    }
    float Triangle::getArea(const Point3f& pt1, const Point3f& pt2, const Point3f& pt3)
    {
        float a = pt1.distance(pt2);
        float b = pt1.distance(pt3);
        float c = pt2.distance(pt3);
        float p = (a + b + c) / 2;
        return sqrt(p * (p - a) * (p - b) * (p - c));
    }
    Point3f& Triangle::operator[] (const int index)
    {
        if (index == 0)
            return vertice0;
        if (index == 1)
            return vertice1;
        return vertice2;
    }
    const Point3f& Triangle::operator[] (const int index) const
    {
        if (index == 0)
            return vertice0;
        if (index == 1)
            return vertice1;
        return vertice2;
    }

    ///////////////////////////////////////////////// Model3D
    static void exportTxt(std::string_view filename, const std::vector<Triangle>& triangles)
    {
        std::ofstream file;
        file << "solid \n";
        for (const auto& triangle : triangles)
        {
            file << "\n";
            file << "facet normal " << 0.0 << " " << 0.0 << " " << 1.0 << '\n';
            file << "outer loop\n";
            file << "vertex " << triangle[0].x << " " << triangle[0].y << " " << triangle[0].z << '\n';
            file << "vertex " << triangle[1].x << " " << triangle[1].y << " " << triangle[1].z << '\n';
            file << "vertex " << triangle[2].x << " " << triangle[2].y << " " << triangle[2].z << '\n';
            file << "endloop\n";
            file << "endfacet\n";
        }
        file.close();
    }
    void Model3D::exportTxt(std::string_view str) const
    {
        std::string filename(str);
        filename += extension;
        STL::exportTxt(std::string_view(filename), triangles);
    }
    void Model3D::exportTxt(std::string&& filename) const
    {
        filename += extension;
        STL::exportTxt(std::string_view(filename), triangles);
    }
    static void exportBin(std::string_view filename, const std::vector<Triangle>& triangles)
    {
        std::fstream file;
        file.open(filename, std::ios::out | std::ios::binary);
        if (!file.is_open())
            return;
        unsigned int dummy[21];
        dummy[20] = (unsigned int)triangles.size();
        file.write((char*)dummy, 84);
        file.write((char*)&(triangles[0]), static_cast<std::streamsize>(triangles.size()) * 50);
        file.close();
    }
    void Model3D::exportBin(std::string_view str) const
    {
        if (triangles.empty())
            return;
        std::string filename(str);
        filename += extension;
        STL::exportBin(std::string_view(filename), triangles);
    }
    void Model3D::exportBin(std::string&& str) const
    {
        if (triangles.empty())
            return;
        str += extension;
        STL::exportBin(std::string_view(str), triangles);
    }

    void Model3D::addTriangle(const Point3f& pt1, const Point3f& pt2, const Point3f& pt3)
    {
        triangles.emplace_back(Triangle(pt1, pt2, pt3));
    }
    void Model3D::addTriangle(const Triangle& triangle)
    {
        triangles.emplace_back(triangle);
    }
    void Model3D::addTriangle(Triangle&& triangle)
    {
        triangles.emplace_back(triangle);
    }
    void Model3D::addPoint(const Point3f& pt1)
    {
        triangles.emplace_back(Triangle(pt1, pt1, pt1));
    }
    void Model3D::addEdge(const Point3f& pt1, const Point3f& pt2)
    {
        triangles.emplace_back(Triangle(pt1, pt2, pt2));
    }
    void Model3D::addCone(const Point3f& pt1, const Point3f& pt2, const float baseSize)
    {
        const auto directionCone = (pt2 - pt1).normalize();
        std::vector<std::pair<float, Point3f>> bidders = {
            {directionCone.x, Point3f{1.f, 0.f, 0.f} },
            {directionCone.y, Point3f{0.f, 1.f, 0.f} },
            {directionCone.z, Point3f{0.f, 0.f, 1.f} },
        };
        std::sort(bidders.begin(), bidders.end(), [](const auto& a, const auto& b) {
            return a.first < b.first;
            });
        const auto basisX = directionCone.cross_product(bidders[1].second).normalize() * baseSize;
        const auto basisY = basisX.cross_product(directionCone).normalize() * baseSize;

        addQuad(pt1 + basisX, pt1 + basisY, pt1 - basisX, pt1 - basisY);
        addTriangle(pt1 + basisX, pt1 + basisY, pt2);
        addTriangle(pt1 + basisY, pt1 - basisX, pt2);
        addTriangle(pt1 - basisX, pt1 - basisY, pt2);
        addTriangle(pt1 - basisY, pt1 + basisX, pt2);
    }
    void Model3D::addQuad(const Point3f& pt1, const Point3f& pt2, const Point3f& pt3, const Point3f& pt4)
    {
        if (fmin(Triangle::getArea(pt1, pt2, pt3), Triangle::getArea(pt1, pt3, pt4))
            < fmin(Triangle::getArea(pt1, pt2, pt4), Triangle::getArea(pt2, pt3, pt4)))
        {
            triangles.emplace_back(Triangle(pt1, pt2, pt4));
            triangles.emplace_back(Triangle(pt2, pt3, pt4));
        }
        else
        {
            triangles.emplace_back(Triangle(pt1, pt2, pt3));
            triangles.emplace_back(Triangle(pt1, pt3, pt4));
        }
    }
    void Model3D::addSphere(const Point3f& center, const float radius)
    {
        constexpr float phi = 1.61803398875f;
        constexpr Triangle sphere[] = {
            Triangle(Point3f(0.f, -phi, 1.f), Point3f(-phi, -1.f, 0.f), Point3f(-1.f, 0.f, phi)),
            Triangle(Point3f(0.f, -phi, 1.f), Point3f(-1.f, 0.f, phi), Point3f(1.f, 0.f, phi)),
            Triangle(Point3f(0.f, -phi, 1.f), Point3f(1.f, 0.f, phi), Point3f(phi, -1.f, 0.f)),
            Triangle(Point3f(0.f, -phi, 1.f), Point3f(phi, -1.f, 0.f), Point3f(0.f, -phi, -1.f)),
            Triangle(Point3f(0.f, -phi, 1.f), Point3f(0.f, -phi, -1.f), Point3f(-phi, -1.f, 0.f)),

            Triangle(Point3f(0.f, phi, -1.f), Point3f(-phi, 1.f, 0.f), Point3f(0.f, phi, 1.f)),
            Triangle(Point3f(0.f, phi, -1.f), Point3f(0.f, phi, 1.f), Point3f(phi, 1.f, 0.f)),
            Triangle(Point3f(0.f, phi, -1.f), Point3f(phi, 1.f, 0.f), Point3f(1.f, 0.f, -phi)),
            Triangle(Point3f(0.f, phi, -1.f), Point3f(1.f, 0.f, -phi), Point3f(-1.f, 0.f, -phi)),
            Triangle(Point3f(0.f, phi, -1.f), Point3f(-1.f, 0.f, -phi), Point3f(-phi, 1.f, 0.f)),

            Triangle(Point3f(0.f, -phi, -1.f), Point3f(-1.f,0.f,-phi), Point3f(-phi,-1.f,0)),
            Triangle(Point3f(-1.f, 0.f, -phi), Point3f(-phi,-1.f,0), Point3f(-phi,1.f,0)),
            Triangle(Point3f(-phi,-1.f,0), Point3f(-phi,1.f,0), Point3f(-1.f,0.f,phi)),
            Triangle(Point3f(-phi,1.f,0), Point3f(-1.f,0.f,phi), Point3f(0.f,phi,1)),
            Triangle(Point3f(-1.f,0.f,phi), Point3f(0.f,phi,1), Point3f(1.f,0.f,phi)),
            Triangle(Point3f(1.f,0.f,phi), Point3f(0.f,phi,1), Point3f(phi,1.f,0)),
            Triangle(Point3f(phi,-1.f,0), Point3f(phi,1.f,0), Point3f(1.f,0.f,phi)),
            Triangle(Point3f(phi,1.f,0), Point3f(phi,-1.f,0), Point3f(1.f,0.f,-phi)),
            Triangle(Point3f(0.f,-phi,-1), Point3f(phi,-1.f,0), Point3f(1.f,0.f,-phi)),
            Triangle(Point3f(0.f,-phi,-1), Point3f(-1.f,0.f,-phi), Point3f(1.f,0.f,-phi))
        };

        for (const auto& tr : sphere)
        {
            addTriangle(tr[0] * radius + center, tr[1] * radius + center, tr[2] * radius + center);
        }
    }
}

namespace STL //custom 
{
    Model3D direction(const std::vector<Point3f>& points)
    {
        Model3D res;
        for (int i = 0; i < (int)points.size() - 1; i++)
        {
            res.addCone(points[i], points[i + 1]);
        }
        return res;
    }
    Model3D line(const std::vector<Point3f>& points)
    {
        if (points.empty())
            return {};
        Model3D res;
        for (size_t i = 0; i < points.size() - 1; i++)
        {
            res.addEdge(points[i], points[i + 1]);
        }
        return res;
    }
    Model3D sphere(const std::vector<Point3f>& points)
    {
        if (points.empty())
            return {};

        float radius = oneSphereRadius;
        if (points.size() > 1)
        {
            Point3f min = points[0], max = points[0];
            for (const auto& pt : points)
            {
                min.x = std::fmin(pt.x, min.x);
                min.y = std::fmin(pt.y, min.y);
                min.z = std::fmin(pt.z, min.z);
                max.x = std::fmax(pt.x, max.x);
                max.y = std::fmax(pt.y, max.y);
                max.z = std::fmax(pt.z, max.z);
            }
            radius = max.distance(min) * ratioSphereRadius;
        }

        Model3D res;
        for (size_t i = 0; i < points.size(); i++)
        {
            res.addSphere(points[i], radius);
        }
        return res;
    }

    Model3D convert(const std::vector<Point3f>& points)
    {
        if (points.empty())
            return {};
        Model3D res;
        for (auto& pt : points)
        {
            res.addPoint(pt);
        }
        return res;
    }
    Model3D convert(const Triangle& triangle)
    {
        return Model3D{ {triangle} };
    }
    Model3D convert(const std::vector<Triangle>& triangles)
    {
        return Model3D{ triangles };
    }
}

namespace STL
{
    template<typename... Args>
    void save(std::string_view fpath, Args... args)
    {
        if (!isEnable)
            return;
        std::string filename(STL::folderSTL);
        filename += fpath;
        convert(args...).exportBin(std::move(filename));
    }

    void save(std::string_view fpath, const Model3D& model)
    {
        if (!isEnable)
            return;
        std::string filename(STL::folderSTL);
        filename += fpath;
        model.exportBin(std::move(filename));
    }

    template<typename... Args>
    void saveInc(std::string_view fpath, Args... args)
    {
        std::string fName(STL::folderSTL);
        fName += fpath;
        fName += "_";

        int index = 0;
        std::string indexStr = "0";
        while (std::filesystem::exists(fName + indexStr + ".stl"))
        {
            ++index;
            indexStr = std::to_string(index);
        }
        fName += indexStr;
        save(std::move(fName), args...);
    }
}

namespace STL
{
    void generateExamples()
    {
        save("stlExample_points", std::vector<Point3f>{ {0.f, 0.f, 0.f}, { 1.f,1.f,0.f }, { 2.f,0.f,0.f }, { 3.f,1.f,0.f }, { 4.f,0.f,0.f } });

        save("stlExample_spheres", sphere({ {0.f,0.f,0.f}, {5.f,5.f,5.f}, {5.f,0.f,0.f}, {0.f,5.f,0.f}, {0.f,0.f,5.f} }));

        save("stlExample_lineChain", line({ {0.f,0.f,0.f}, {1.f,2.f,3.f}, {1.f,3.f,4.f}, {4.f,0.f,0.f}, {0.f,0.f,0.f} }));

        saveInc("stlExample_directionChain", direction({ {0.f,0.f,0.f}, {1.f,2.f,3.f}, {1.f,3.f,4.f}, {4.f,0.f,0.f}, {0.f,0.f,0.f} }));
        saveInc("stlExample_directionChain", direction({ {0.f, 0.f, 0.f}, { 1.f,1.f,0.f }, { 2.f,0.f,0.f}, { 3,1.f,0.f }, { 4.f,0.f,0.f } }));
        saveInc("stlExample_directionChain", direction({ {0.f,0.f,0.f}, {5.f,5.f,5.f}, {5.f,0.f,0.f}, {0.f,5.f,0.f}, {0.f,0.f,5.f} }));

        const std::vector<Triangle> tetraedr = {
            Triangle({0.f,0.f,0.f},{0.f,1.f,1.f},{1.f,0.f,1.f}),
            Triangle({0.f,1.f,1.f},{1.f,1.f,0.f},{1.f,0.f,1.f}),
            Triangle({0.f,0.f,0.f},{1.f,1.f,0.f},{1.f,0.f,1.f}),
            Triangle({0.f,0.f,0.f},{1.f,1.f,0.f},{0.f,1.f,1.f})
        };
        save("stlExample_tetraedr", tetraedr);

        Model3D cube;
        cube.addQuad({ 0.f,0.f,0.f }, { 0.f,0.f,1.f }, { 1.f,0.f,1.f }, { 1.f,0.f,0.f });
        cube.addQuad({ 0.f,1.f,0.f }, { 0.f,1.f,1.f }, { 1.f,1.f,1.f }, { 1.f,1.f,0.f });
        cube.addQuad({ 0.f,0.f,0.f }, { 0.f,0.f,1.f }, { 0.f,1.f,1.f }, { 0.f,1.f,0.f });
        cube.addQuad({ 1.f,0.f,0.f }, { 1.f,1.f,0.f }, { 1.f,1.f,1.f }, { 1.f,0.f,1.f });
        cube.addQuad({ 0.f,1.f,0.f }, { 0.f,0.f,0.f }, { 1.f,0.f,0.f }, { 1.f,1.f,0.f });
        cube.addQuad({ 0.f,0.f,1.f }, { 0.f,1.f,1.f }, { 1.f,1.f,1.f }, { 1.f,0.f,1.f });
        save("stlExample_cube", cube);
    }
}