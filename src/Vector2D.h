#pragma once

template <typename T>
struct Vector2D {

    T x;
    T y;

    Vector2D() : x(0), y(0) {};
    Vector2D(T x, T y) : x(x), y(y) {};

    friend Vector2D operator+(const Vector2D& thisVector, const Vector2D& givenVector) {
        return {thisVector.x + givenVector.x, thisVector.y + givenVector.y};
    }

    friend Vector2D operator-(const Vector2D& thisVector, const Vector2D& givenVector) {
        return {thisVector.x - givenVector.x, thisVector.y - givenVector.y};
    }

};