#pragma once

#include <ostream>
#include <cmath>

template<int N, class T = double>
class Vector {
    private:
        T arr[N];

    public:
        Vector () {
            for (int i = 0; i < N; i++)
                arr[i] = 0;
        }

        Vector (const T (&proto)[N]) {
            for (int i = 0; i < N; i++)
                arr[i] = proto[i];
        }

        const T& operator[] (int n) const {
            return arr[n];
        }

        T& operator[] (int n) {
            return arr[n];
        }

        //Length of vector
        T length () const {
            T result = 0.0;

            for (int i = 0; i < N; i++)
                result += arr[i] * arr[i];

            return sqrt(result);
        }

        //Vector addition
        Vector<N, T> operator+ (const Vector<N, T>& other) const {
            Vector<N, T> v;

            for (int i = 0; i < N; i++)
                v[i] = arr[i] + other[i];

            return v;
        }

        const Vector<N, T>& operator+= (const Vector<N, T>& other) {
            for (int i = 0; i < N; i++)
                arr[i] += other[i];

            return *this;
        }

        Vector<N, T> operator- (const Vector<N, T>& other) const {
            Vector<N, T> v;

            for (int i = 0; i < N; i++)
                v[i] = arr[i] - other[i];

            return v;
        }

        //Scalar multiple
        Vector<N, T> operator* (T other) const {
            Vector<N, T> v;

            for (int i = 0; i < N; i++)
                v[i] = other*arr[i];

            return v;
        }

        //Dot product
        T operator* (const Vector<N, T>& other) const {
            T result = 0.0;

            for (int i = 0; i < N; i++)
                result += arr[i] * other[i];

            return result;
        }

        bool operator== (const Vector<N, T>& other) const {
            bool result = true;

            for (int i = 0; i < N; i++)
                result = result && arr[i] == other[i];

            return result;
        }

        bool operator!= (const Vector<N, T>& other) const {
            bool result = false;

            for (int i = 0; i < N; i++)
                result = result || arr[i] != other[i];

            return result;
        }

        //Cast
        template<class S>
        operator const Vector<N, S> () {
            Vector<N, S> v;

            for (int i = 0; i < N; i++)
                v[i] = arr[i];

            return v;
        }
};

template<int N, class T = double>
Vector<N, T> operator* (T left, Vector<N, T> right) {
    return right*left;
}

template<int N, class T = double>
std::ostream& operator<< (std::ostream& os, const Vector<N, T>& v) {
    os << "[";

    if (N >= 1) {
        os << v[0];

        for (int i = 1; i < N; i++)
            os << ", " << v[i];
    }

    os << "]";

    return os;
}

template<class T = double>
Vector<2, T> fromPolar (T length, double angle) {
    return Vector<2>({length*cos(angle), length*sin(angle)});
}

template<class T>
double angleBetween (const Vector<2, T>& a, const Vector<2, T>& b) {
    return atan2(b[1], b[0]) - atan2(a[1], a[0]);
}