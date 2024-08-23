#include "Presenter.h"
#include "ViewModel.h"

#define LOGW(x) std::wcout << x << std::endl
#define PRINTW(x) std::wcout << x

int main() {

	return EXIT_SUCCESS;
}

//#include <iostream>
//#include <memory>
//#include <variant>
//
//// Base class
//class Shape {
//protected:
//    Shape() {}  // Protected constructor to prevent direct instantiation
//
//public:
//    virtual ~Shape() = default;
//
//    // Force derived classes to implement this method
//    virtual double area() const = 0;
//};
//
//// Derived class: Circle
//class Circle : public Shape {
//public:
//    Circle(double r) : radius(r) {}
//    double radius;
//};
//
//// Derived class: Square
//class Square : public Shape {
//public:
//    Square(double s) : side(s) {}
//    double side;
//};
//
//// Derived class: NotAShape
//class NotAShape : public Shape {
//public:
//    double area() const override {
//        return 0.0;
//    }
//};
//
//// Function to demonstrate usage
//double calculateArea(const Shape& shape) {
//    return shape.area();
//}
//
//int main() {
//    std::unique_ptr<Shape> circle = std::make_unique<Circle>(5.0);
//    std::unique_ptr<Shape> square = std::make_unique<Square>(4.0);
//    std::unique_ptr<Shape> notAShape = std::make_unique<NotAShape>();
//
//    std::cout << "Circle area: " << calculateArea(*circle) << std::endl;
//    std::cout << "Square area: " << calculateArea(*square) << std::endl;
//    std::cout << "NotAShape area: " << calculateArea(*notAShape) << std::endl;
//
//    return 0;
//}