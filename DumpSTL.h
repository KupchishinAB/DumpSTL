#pragma once

#include <math.h>
#include <string>
#include <fstream>
#include <filesystem>

namespace STL //settings
{
    bool isEnable = true;
    const std::string folderSTL = "C:\\Repos\\STL\\";
    const float coneBaseSize = 1.0f / 20.0f; //ratio length and size of base
    const float oneSphereRadius = 0.1f; //
    const float ratioSphereRadius = 0.025f;
}

namespace STL //header
{
#pragma pack(push, 4)
    template <typename T>
    struct Point3
    {
        T _x;
        T _y;
        T _z;

        constexpr Point3(T x, T y, T z);
        Point3() = default;

        Point3<T> normalize() const;
        float distance(const Point3<T>& pt) const;
        Point3<T> cross_product(const Point3<T>& pt) const;

        Point3<T> & operator+=(const Point3<T>&);
        Point3<T> & operator-=(const Point3<T>&);
        Point3<T> & operator/=(T value);
        Point3<T> & operator*=(T value);
    };
#pragma pack(pop)
    using Point3f = Point3<float>;

#pragma pack(push, 1)
    struct Triangle
    {
        Point3f _norm = Point3f(0.0f, 0.0f, 1.0f); //for export STL
        Point3f _vertice0;
        Point3f _vertice1;
        Point3f _vertice2;
        char dummy0 = 0; //for export STL
        char dummy1 = 0; //for export STL

        Triangle() = default;
        constexpr Triangle(const Point3f& pt1, const Point3f& pt2, const Point3f& pt3);
        constexpr Triangle(Point3f&& pt1, Point3f&& pt2, Point3f&& pt3);

        static float getArea(const Point3f& pt1, const Point3f& pt2, const Point3f& pt3);
        Point3f& operator[] (int index);
        const Point3f& operator[] (int index) const;
    };
#pragma pack(pop)

    struct Model3D
    {
        std::vector<Triangle> _triangles;

        void exportTxt(const std::string& str) const;
        void exportBin(const std::string& str) const;

        void addTriangle(const Triangle& triangle);
        void addTriangle(Triangle&& triangle);
        void addTriangle(const Point3f& pt1, const Point3f& pt2, const Point3f& pt3);
        void addPoint(const Point3f& pt1);
        void addEdge(const Point3f& pt1, const Point3f& pt2);
        void addQuad(const Point3f& pt1, const Point3f& pt2, const Point3f& pt3, const Point3f& pt4);
        void addCone(const Point3f& pt1, const Point3f& pt2, float baseSize = coneBaseSize);
        void addSphere(const Point3f& center, float radius);
    };
}

namespace STL //source
{
///////////////////////////////////////////////// Point3
    template <typename T> constexpr Point3<T>::Point3<T>(T x, T y, T z)
        : _x(x), _y(y), _z(z)
    {
    }
    template <typename T> Point3<T> Point3<T>::normalize() const
    {
        T norm = sqrt(_x * _x + _y * _y + _z * _z);
        return Point3<T>(_x / norm, _y / norm, _z / norm);
    }
    template <typename T> Point3<T>& Point3<T>::operator+=(const Point3<T>& pt)
    {
        _x += pt._x;
        _y += pt._y;
        _z += pt._z;
        return *this;
    }
    template <typename T> Point3<T>& Point3<T>::operator-=(const Point3<T>& pt)
    {
        _x -= pt._x;
        _y -= pt._y;
        _z -= pt._z;
        return *this;
    }
    template <typename T> float Point3<T>::distance(const Point3<T>& pt) const
    {
        return sqrt((_x - pt._x) * (_x - pt._x)
            + (_y - pt._y) * (_y - pt._y)
            + (_z - pt._z) * (_z - pt._z));
    }
    template <typename T> Point3<T> Point3<T>::cross_product(const Point3<T>& pt) const
    {
        return { _y * pt._z - _z * pt._y,
            -(_x * pt._z - pt._x * _z),
            _x * pt._y - pt._x * _y };
    }
    template <typename T> Point3<T> operator-(const Point3<T>& pt1, const Point3<T>& pt2)
    {
        return Point3<T>(pt1._x - pt2._x, pt1._y - pt2._y, pt1._z - pt2._z);
    }
    template <typename T> Point3<T> operator+(const Point3<T>& pt1, const Point3<T>& pt2)
    {
        return Point3<T>(pt1._x + pt2._x, pt1._y + pt2._y, pt1._z + pt2._z);
    }

    template<typename T> Point3<T> operator*(const Point3<T>& pt, T val)
    {
        return Point3<T>(pt._x * val, pt._y * val, pt._z * val);
    }
    template<typename T> Point3<T> operator*(T val, const Point3<T>& pt)
    {
        return Point3<T>(pt._x * val, pt._y * val, pt._z * val);
    }
    template<typename T> Point3<T>& Point3<T>::operator*=(T val)
    {
        _x = *val;
        _y = *val;
        _z = *val;
    }
    template<typename T> Point3<T> operator/(const Point3<T>& pt, T val)
    {
        return Point3<T>(pt._x / val, pt._y / val, pt._z / val);
    }
    template<typename T> Point3<T>& Point3<T>::operator/=(T val)
    {
        _x /= val;
        _y /= val;
        _z /= val;
        return *this;
    }

///////////////////////////////////////////////// Triangle
    constexpr Triangle::Triangle(const Point3f& pt1, const Point3f& pt2, const Point3f& pt3)
        : _vertice0(pt1), _vertice1(pt2), _vertice2(pt3)
    {
    }
    constexpr Triangle::Triangle(Point3f&& pt1, Point3f&& pt2, Point3f&& pt3)
        : _vertice0(std::move(pt1)), _vertice1(std::move(pt2)), _vertice2(std::move(pt3))
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
    Point3f& Triangle::operator[] (int index)
    {
        if (index == 0)
            return _vertice0;
        if (index == 1)
            return _vertice1;
        return _vertice2;
    }
    const Point3f& Triangle::operator[] (int index) const
    {
        if (index == 0)
            return _vertice0;
        if (index == 1)
            return _vertice1;
        return _vertice2;
    }
     
///////////////////////////////////////////////// Model3D
    void Model3D::exportTxt(const std::string& str) const
    {
        std::ofstream file;
        file.open(str + ".stl", std::ios::out);
        file << "solid \n";
        for (const auto& triangle : _triangles)
        {
            file << "\n";
            file << "facet normal " << 0.0 << " " << 0.0 << " " << 1.0 << '\n';
            file << "outer loop\n";
            file << "vertex " << triangle[0]._x << " " << triangle[0]._y << " " << triangle[0]._z << '\n';
            file << "vertex " << triangle[1]._x << " " << triangle[1]._y << " " << triangle[1]._z << '\n';
            file << "vertex " << triangle[2]._x << " " << triangle[2]._y << " " << triangle[2]._z << '\n';
            file << "endloop\n";
            file << "endfacet\n";
        }
        file.close();
    }
    void Model3D::exportBin(const std::string& str) const
    {
        if (_triangles.empty())
            return;
        std::fstream file;
        file.open(str + ".stl", std::ios::out | std::ios::binary);
        if (!file.is_open())
            return;

        unsigned int dummy[21];
        dummy[20] = (unsigned int)_triangles.size();
        file.write((char*)dummy, 84);
        file.write((char*)&(_triangles[0]), static_cast<std::streamsize>(_triangles.size()) * 50);
        file.close();
    }

    void Model3D::addTriangle(const Point3f& pt1, const Point3f& pt2, const Point3f& pt3)
    {
        _triangles.emplace_back(Triangle(pt1, pt2, pt3));
    }
    void Model3D::addTriangle(const Triangle& triangle)
    {
        _triangles.emplace_back(triangle);
    }
    void Model3D::addTriangle(Triangle&& triangle)
    {
        _triangles.emplace_back(triangle);
    }
    void Model3D::addPoint(const Point3f& pt1)
    {
        _triangles.emplace_back(Triangle(pt1, pt1, pt1));
    }
    void Model3D::addEdge(const Point3f& pt1, const Point3f& pt2)
    {
        _triangles.emplace_back(Triangle(pt1, pt2, pt2));
    }
    void Model3D::addCone(const Point3f& pt1, const Point3f& pt2, float baseSize)
    {
        const auto directionCone = (pt2 - pt1).normalize();
        std::vector<std::pair<float, Point3f>> bidders = {
            {directionCone._x, Point3f{1.0f, 0.0f, 0.0f} },
            {directionCone._y, Point3f{0.0f, 1.0f, 0.0f} },
            {directionCone._z, Point3f{0.0f, 0.0f, 1.0f} },
        };
        std::sort(bidders.begin(), bidders.end(), [](const auto& a, const auto& b) {
            return a.first < b.first;
        });
        const auto basisX = directionCone.cross_product(bidders[1].second).normalize()* baseSize;
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
            _triangles.emplace_back(Triangle(pt1, pt2, pt4));
            _triangles.emplace_back(Triangle(pt2, pt3, pt4));
        }
        else
        {
            _triangles.emplace_back(Triangle(pt1, pt2, pt3));
            _triangles.emplace_back(Triangle(pt1, pt3, pt4));
        }
    }
    void Model3D::addSphere(const Point3f& center, float radius)
    {
        constexpr float phi = 1.61803398875f;
        constexpr Triangle sphere[] = {
            Triangle(Point3f(0.0f, -phi, 1.0f), Point3f(-phi, -1.0f, 0.0f), Point3f(-1.0f, 0.0f, phi)),
            Triangle(Point3f(0.0f, -phi, 1.0f), Point3f(-1.0f, 0, phi), Point3f(1.0f, 0.0f, phi)),
            Triangle(Point3f(0.0f, -phi, 1.0f), Point3f(1.0f, 0.0f, phi), Point3f(phi, -1.0f, 0)),
            Triangle(Point3f(0.0f, -phi, 1.0f), Point3f(phi, -1.0f, 0), Point3f(0.0f, -phi, -1.0f)),
            Triangle(Point3f(0.0f, -phi, 1.0f), Point3f(0.0f, -phi, -1.0f), Point3f(-phi, -1.0f, 0.0f)),

            Triangle(Point3f(0.0f, phi, -1.0f), Point3f(-phi, 1.0f, 0.0f), Point3f(0.0f, phi, 1.0f)),
            Triangle(Point3f(0.0f, phi, -1.0f), Point3f(0.0f, phi, 1.0f), Point3f(phi, 1.0f, 0.0f)),
            Triangle(Point3f(0.0f, phi, -1.0f), Point3f(phi, 1.0f, 0.0f), Point3f(1.0f, 0.0f, -phi)),
            Triangle(Point3f(0.0f, phi, -1.0f), Point3f(1.0f, 0.0f, -phi), Point3f(-1.0f, 0.0f, -phi)),
            Triangle(Point3f(0.0f, phi, -1.0f), Point3f(-1.0f, 0.0f, -phi), Point3f(-phi, 1.0f, 0.0f)),

            Triangle(Point3f(0.0f, -phi, -1.0f), Point3f(-1,0,-phi), Point3f(-phi,-1,0)),
            Triangle(Point3f(-1, 0, -phi), Point3f(-phi,-1,0), Point3f(-phi,1,0)),
            Triangle(Point3f(-phi,-1,0), Point3f(-phi,1,0), Point3f(-1,0,phi)),
            Triangle(Point3f(-phi,1,0), Point3f(-1,0,phi), Point3f(0,phi,1)),
            Triangle(Point3f(-1,0,phi), Point3f(0,phi,1), Point3f(1,0,phi)),
            Triangle(Point3f(1,0,phi), Point3f(0,phi,1), Point3f(phi,1,0)),
            Triangle(Point3f(phi,-1,0), Point3f(phi,1,0), Point3f(1,0,phi)),
            Triangle(Point3f(phi,1,0), Point3f(phi,-1,0), Point3f(1,0,-phi)),
            Triangle(Point3f(0,-phi,-1), Point3f(phi,-1,0), Point3f(1,0,-phi)),
            Triangle(Point3f(0,-phi,-1), Point3f(-1,0,-phi), Point3f(1,0,-phi))
        };

        for (const auto& tr : sphere)
        {
            addTriangle(tr[0]* radius + center, tr[1] * radius + center, tr[2] * radius + center );
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
                min._x = std::fmin(pt._x, min._x);
                min._y = std::fmin(pt._y, min._y);
                min._z = std::fmin(pt._z, min._z);
                max._x = std::fmax(pt._x, max._x);
                max._y = std::fmax(pt._y, max._y);
                max._z = std::fmax(pt._z, max._z);
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
    void save(const std::string& fpath, Args... args)
    {
        if (!isEnable)
            return;
        convert(args...).exportBin(STL::folderSTL + fpath);
    }

    void save(const std::string& fpath, const Model3D& model)
    {
        if (!isEnable)
            return;
        model.exportBin(STL::folderSTL + fpath);
    }

    template<typename... Args>
    void saveInc(const std::string& fpath, Args... args)
    {
        std::string fName = STL::folderSTL + fpath;
        int index = 0;
        while (std::filesystem::exists(fName + "_" + std::to_string(index) + ".stl"))
        {
            index++;
        }
        save(fpath + "_" + std::to_string(index), args...);
    }
}

namespace STL
{
    void generateExamples()
    {
        save("stlExample_points", std::vector<Point3f>{ {0,0,0}, {1,1,0}, {2,0,0}, {3,1,0}, {4,0,0} });

        save("stlExample_spheres", sphere({ {0,0,0}, {5,5,5}, {5,0,0}, {0,5,0}, {0,0,5} }));

        save("stlExample_lineChain", line({ {0,0,0}, {1,2,3}, {1,3,4}, {4,0,0}, {0,0,0} }));

        saveInc("stlExample_directionChain", direction({ {0,0,0}, {1,2,3}, {1,3,4}, {4,0,0}, {0,0,0} }));
        saveInc("stlExample_directionChain", direction({ {0, 0, 0}, { 1,1,0 }, { 2,0,0 }, { 3,1,0 }, { 4,0,0 } }));
        saveInc("stlExample_directionChain", direction({ {0,0,0}, {5,5,5}, {5,0,0}, {0,5,0}, {0,0,5} }));

        const std::vector<Triangle> tetraedr = {
            Triangle({0.0f,0.0f,0.0f},{0.0f,1.0f,1.0f},{1.0f,0.0f,1.0f}),
            Triangle({0.0f,1.0f,1.0f},{1.0f,1.0f,0.0f},{1.0f,0.0f,1.0f}),
            Triangle({0.0f,0.0f,0.0f},{1.0f,1.0f,0.0f},{1.0f,0.0f,1.0f}),
            Triangle({0.0f,0.0f,0.0f},{1.0f,1.0f,0.0f},{0.0f,1.0f,1.0f})
        };
        save("stlExample_tetraedr", tetraedr);

        Model3D cube;
        cube.addQuad({0,0,0}, {0,0,1}, {1,0,1}, {1,0,0});
        cube.addQuad({0,1,0}, {0,1,1}, {1,1,1}, {1,1,0});
        cube.addQuad({0,0,0}, {0,0,1}, {0,1,1}, {0,1,0});
        cube.addQuad({1,0,0}, {1,1,0}, {1,1,1}, {1,0,1});
        cube.addQuad({0,1,0}, {0,0,0}, {1,0,0}, {1,1,0});
        cube.addQuad({0,0,1}, {0,1,1}, {1,1,1}, {1,0,1});
        save("stlExample_cube", cube);
    }
}