#pragma once

#include <math.h>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>
#include <algorithm>
#include <vector>

namespace DUMP
{
    namespace fs=std::filesystem;
    constexpr bool isEnable = true; // global turn off/turn on this tool (without clear code)
    constexpr float coneBaseSize = 1.f / 20.f;
    constexpr float oneSphereRadius = 0.1f;
    constexpr float ratioSphereRadius = 0.025f;
    constexpr std::string_view fileExtension = ".stl";
    constexpr char fileNameSeparator = '_';
}

namespace DUMP
{
    
    
#pragma pack(push, 4)
    template <typename T>
    struct Point3 {
        T x;
        T y;
        T z;

        Point3() = default;
        constexpr Point3(const T coordX, const T coordY, const T coordZ)
            : x(coordX), y(coordY), z(coordZ) {}
        Point3<T> normalize() const {
            T norm = sqrt(x * x + y * y + z * z);
            return Point3<T>(x / norm, y / norm, z / norm);
        }
        float distance(const Point3<T>& pt) const {
            return sqrt((x - pt.x) * (x - pt.x) + (y - pt.y) * (y - pt.y) +
                (z - pt.z) * (z - pt.z));
        }
        Point3<T> cross_product(const Point3<T>& pt) const {
            return { y * pt.z - z * pt.y, -(x * pt.z - pt.x * z), x * pt.y - pt.x * y };
        }

        Point3<T>& operator*=(const T val) {
            x *= val;
            y *= val;
            z *= val;
        }
        Point3<T>& operator/=(const T val) {
            x /= val;
            y /= val;
            z /= val;
            return *this;
        }
        Point3<T>& operator+=(const Point3<T>& pt) {
            x += pt.x;
            y += pt.y;
            z += pt.z;
            return *this;
        }
        Point3<T>& operator-=(const Point3<T>& pt) {
            x -= pt.x;
            y -= pt.y;
            z -= pt.z;
            return *this;
        }
        Point3<T> operator-(const Point3<T>& pt) const {
            return Point3<T>(x - pt.x, y - pt.y, z - pt.z);
        }
        Point3<T> operator+(const Point3<T>& pt) const {
            return Point3<T>(x + pt.x, y + pt.y, z + pt.z);
        }
        Point3<T> operator*(const T val) const {
            return Point3<T>(x * val, y * val, z * val);
        }
        Point3<T> operator/(const T val) const {
            return Point3<T>(x / val, y / val, z / val);
        }
    };
#pragma pack(pop)
    using Point3f = Point3<float>;

#pragma pack(push, 1)
    struct Triangle {
        Point3f norm = Point3f(0.f, 0.f, 1.f);  // for export STL
        Point3f vertex0;
        Point3f vertex1;
        Point3f vertex2;
        char dummy0 = 0;  // for export STL
        char dummy1 = 0;  // for export STL

        Triangle() = default;
        constexpr Triangle(const Point3f& pt1, const Point3f& pt2, const Point3f& pt3)
            : vertex0(pt1), vertex1(pt2), vertex2(pt3) {}
        constexpr Triangle(Point3f&& pt1, Point3f&& pt2, Point3f&& pt3)
            : vertex0(std::move(pt1)),
            vertex1(std::move(pt2)),
            vertex2(std::move(pt3)) {}
        static float getArea(const Point3f& pt1, const Point3f& pt2,
            const Point3f& pt3) {
            float a = pt1.distance(pt2);
            float b = pt1.distance(pt3);
            float c = pt2.distance(pt3);
            float p = (a + b + c) / 2;
            return sqrt(p * (p - a) * (p - b) * (p - c));
        }
        Point3f& operator[](const int index) {
            if (index == 0) return vertex0;
            if (index == 1) return vertex1;
            return vertex2;
        }
        const Point3f& operator[](const int index) const {
            if (index == 0) return vertex0;
            if (index == 1) return vertex1;
            return vertex2;
        }
    };
#pragma pack(pop)
    static_assert(sizeof(Triangle)==50,"Triangle size should be 50, see https://en.wikipedia.org/wiki/STL_(file_format)#Binary_STL"); 

    struct Model3D {

        struct STLHeader
        {
            // see https://en.wikipedia.org/wiki/STL_(file_format)#Binary_STL
            static constexpr unsigned header_size_bytes = 80;
            char Header[header_size_bytes];
            uint32_t NumOfTriangles;
        };
        static_assert(sizeof(STLHeader)==84,"STLHeader size should be 84, see https://en.wikipedia.org/wiki/STL_(file_format)#Binary_STL"); 

        std::vector<Triangle> triangles;
        static void exportTxt(fs::path filename,
            const std::vector<Triangle>& triangles) {
            std::ofstream file;
            file.open(filename, std::ios::out);
            file << "solid \n";
            for (const auto& triangle : triangles) {
                file << "\n";
                file << "facet normal " << 0.0 << " " << 0.0 << " " << 1.0 << '\n';
                file << "outer loop\n";
                file << "vertex " << triangle[0].x << " " << triangle[0].y << " "
                    << triangle[0].z << '\n';
                file << "vertex " << triangle[1].x << " " << triangle[1].y << " "
                    << triangle[1].z << '\n';
                file << "vertex " << triangle[2].x << " " << triangle[2].y << " "
                    << triangle[2].z << '\n';
                file << "endloop\n";
                file << "endfacet\n";
            }
            file.close();
        }
        void exportTxt(fs::path filename) const {
            filename.replace_extension(fileExtension);
            exportTxt(filename, triangles);
        }
        void exportTxt(fs::path&& filename) const {
            filename.replace_extension(fileExtension);
            exportTxt(filename, triangles);
        }
        static void exportBin(fs::path filename,
            const std::vector<Triangle>& triangles) {
            std::fstream file;
            std::cout<<"WriteFile: <"<<filename<<">"<<std::endl;
            file.open(filename, std::ios::out | std::ios::binary);
            if (!file.is_open()) return;
            STLHeader  dummy;
            dummy.NumOfTriangles = (unsigned int)triangles.size();
            file.write(reinterpret_cast<const char*>(&dummy), static_cast<std::streamsize>(sizeof(dummy)));
            file.write(reinterpret_cast<const char*>(triangles.data()), static_cast<std::streamsize>(triangles.size() * sizeof(Triangle)));
            file.close();
        }
        void exportBin(fs::path filename) const {
            if (triangles.empty()) return;
            filename.replace_extension(fileExtension);
            exportBin(filename, triangles);
        }
        void exportBin(fs::path&& filename) const {
            if (triangles.empty()) return;
            filename.replace_extension(fileExtension);
            exportBin(filename, triangles);
        }

        Model3D& addTriangle(const Point3f& pt1, const Point3f& pt2,
            const Point3f& pt3) {
            triangles.emplace_back(Triangle(pt1, pt2, pt3));
            return *this;
        }
        Model3D& addTriangle(const Triangle& triangle) {
            triangles.emplace_back(triangle);
            return *this;
        }
        Model3D& addTriangle(Triangle&& triangle) {
            triangles.emplace_back(triangle);
            return *this;
        }
        Model3D& addPoint(const Point3f& pt1) {
            triangles.emplace_back(Triangle(pt1, pt1, pt1));
            return *this;
        }
        Model3D& addEdge(const Point3f& pt1, const Point3f& pt2) {
            triangles.emplace_back(Triangle(pt1, pt2, pt2));
            return *this;
        }
        Model3D& addCone(const Point3f& pt1, const Point3f& pt2,
            const float baseSize = coneBaseSize) {
            const auto directionCone = (pt2 - pt1).normalize();
            std::vector<std::pair<float, Point3f>> bidders = {
                {directionCone.x, Point3f{1.f, 0.f, 0.f}},
                {directionCone.y, Point3f{0.f, 1.f, 0.f}},
                {directionCone.z, Point3f{0.f, 0.f, 1.f}},
            };
            std::sort(bidders.begin(), bidders.end(),
                [](const auto& a, const auto& b) { return a.first < b.first; });
            const auto basisX =
                directionCone.cross_product(bidders[1].second).normalize() * baseSize;
            const auto basisY =
                basisX.cross_product(directionCone).normalize() * baseSize;

            addQuad(pt1 + basisX, pt1 + basisY, pt1 - basisX, pt1 - basisY);
            addTriangle(pt1 + basisX, pt1 + basisY, pt2);
            addTriangle(pt1 + basisY, pt1 - basisX, pt2);
            addTriangle(pt1 - basisX, pt1 - basisY, pt2);
            addTriangle(pt1 - basisY, pt1 + basisX, pt2);
            return *this;
        }
        Model3D& addQuad(const Point3f& pt1, const Point3f& pt2, const Point3f& pt3,
            const Point3f& pt4) {
            if (fmin(Triangle::getArea(pt1, pt2, pt3),
                Triangle::getArea(pt1, pt3, pt4)) <
                fmin(Triangle::getArea(pt1, pt2, pt4),
                    Triangle::getArea(pt2, pt3, pt4))) {
                triangles.emplace_back(Triangle(pt1, pt2, pt4));
                triangles.emplace_back(Triangle(pt2, pt3, pt4));
            }
            else {
                triangles.emplace_back(Triangle(pt1, pt2, pt3));
                triangles.emplace_back(Triangle(pt1, pt3, pt4));
            }
            return *this;
        }
        Model3D& addSphere(const Point3f& center, const float radius) {
            constexpr float phi = 1.61803398875f;
            constexpr Triangle sphere[] = {
                Triangle(Point3f(0.f, -phi, 1.f), Point3f(-phi, -1.f, 0.f),
                         Point3f(-1.f, 0.f, phi)),
                Triangle(Point3f(0.f, -phi, 1.f), Point3f(-1.f, 0.f, phi),
                         Point3f(1.f, 0.f, phi)),
                Triangle(Point3f(0.f, -phi, 1.f), Point3f(1.f, 0.f, phi),
                         Point3f(phi, -1.f, 0.f)),
                Triangle(Point3f(0.f, -phi, 1.f), Point3f(phi, -1.f, 0.f),
                         Point3f(0.f, -phi, -1.f)),
                Triangle(Point3f(0.f, -phi, 1.f), Point3f(0.f, -phi, -1.f),
                         Point3f(-phi, -1.f, 0.f)),

                Triangle(Point3f(0.f, phi, -1.f), Point3f(-phi, 1.f, 0.f),
                         Point3f(0.f, phi, 1.f)),
                Triangle(Point3f(0.f, phi, -1.f), Point3f(0.f, phi, 1.f),
                         Point3f(phi, 1.f, 0.f)),
                Triangle(Point3f(0.f, phi, -1.f), Point3f(phi, 1.f, 0.f),
                         Point3f(1.f, 0.f, -phi)),
                Triangle(Point3f(0.f, phi, -1.f), Point3f(1.f, 0.f, -phi),
                         Point3f(-1.f, 0.f, -phi)),
                Triangle(Point3f(0.f, phi, -1.f), Point3f(-1.f, 0.f, -phi),
                         Point3f(-phi, 1.f, 0.f)),

                Triangle(Point3f(0.f, -phi, -1.f), Point3f(-1.f, 0.f, -phi),
                         Point3f(-phi, -1.f, 0)),
                Triangle(Point3f(-1.f, 0.f, -phi), Point3f(-phi, -1.f, 0),
                         Point3f(-phi, 1.f, 0)),
                Triangle(Point3f(-phi, -1.f, 0), Point3f(-phi, 1.f, 0),
                         Point3f(-1.f, 0.f, phi)),
                Triangle(Point3f(-phi, 1.f, 0), Point3f(-1.f, 0.f, phi),
                         Point3f(0.f, phi, 1)),
                Triangle(Point3f(-1.f, 0.f, phi), Point3f(0.f, phi, 1),
                         Point3f(1.f, 0.f, phi)),
                Triangle(Point3f(1.f, 0.f, phi), Point3f(0.f, phi, 1),
                         Point3f(phi, 1.f, 0)),
                Triangle(Point3f(phi, -1.f, 0), Point3f(phi, 1.f, 0),
                         Point3f(1.f, 0.f, phi)),
                Triangle(Point3f(phi, 1.f, 0), Point3f(phi, -1.f, 0),
                         Point3f(1.f, 0.f, -phi)),
                Triangle(Point3f(0.f, -phi, -1), Point3f(phi, -1.f, 0),
                         Point3f(1.f, 0.f, -phi)),
                Triangle(Point3f(0.f, -phi, -1), Point3f(-1.f, 0.f, -phi),
                         Point3f(1.f, 0.f, -phi)) };

            for (const auto& tr : sphere) {
                addTriangle(tr[0] * radius + center, tr[1] * radius + center,
                    tr[2] * radius + center);
            }
            return *this;
        }
    };
}

namespace DUMP // custom
{
    Model3D direction(const std::vector<Point3f>& points) {
        Model3D res;
        if (points.empty()) return res;
        for (auto point = points.begin(), pointNext = point + 1;
            pointNext != points.end(); ++point, ++pointNext) {
            res.addCone(*point, *pointNext);
        }
        return res;
    }
    Model3D line(const std::vector<Point3f>& points) {
        Model3D res;
        if (points.empty()) return res;
        for (auto point = points.begin(), pointNext = point + 1;
            pointNext != points.end(); ++point, ++pointNext) {
            res.addEdge(*point, *pointNext);
        }
        return res;
    }
    Model3D sphere(const std::vector<Point3f>& points) {
        if (points.empty()) return {};

        float radius = oneSphereRadius;
        if (points.size() > 1) {
            Point3f min = points[0], max = points[0];
            for (const auto& pt : points) {
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
        for (const auto& pt : points) {
            res.addSphere(pt, radius);
        }
        return res;
    }

    Model3D convert(const std::vector<Point3f>& points) {
        if (points.empty()) return {};
        Model3D res;
        for (auto& pt : points) {
            res.addPoint(pt);
        }
        return res;
    }
    Model3D convert(const Triangle& triangle) { return Model3D{ {triangle} }; }
    Model3D convert(const std::vector<Triangle>& triangles) {
        return Model3D{ triangles };
    }
}

namespace DUMP {
    template <typename... Args>
    void save(fs::path fileName, Args... args) {
        if (!isEnable) return;
        convert(args...).exportBin(fileName);
    }

    void save(fs::path fileName, const Model3D& model) {
        if (!isEnable) return;
        model.exportBin(fileName);
    }

    template <typename... Args>
    void saveInc(fs::path fileName, Args... args) {
        auto index = 0;
        fs::path fullFileName;
        auto generateFName = [&index, &fileName, &fullFileName]() {
            fullFileName= fileName.parent_path() / fs::path(fileName.stem().string() + fileNameSeparator + std::to_string(index)).replace_extension(fileExtension);
            
        };
        for (generateFName(); fs::exists(fullFileName); ++index, generateFName());
        save(fullFileName, args...);
    }
}  // namespace DUMP
