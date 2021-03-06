#pragma once

struct Vector2 {
    int x;
    int y;

    Vector2()
        : x(0), y(0) {
    }

    Vector2(int x, int y)
        : x(x), y(y) {
    }

    static Vector2 zero() {
        return Vector2(0, 0);
    }

    Vector2 operator*(int operand) {
        return Vector2(x * operand, y * operand);
    }
};