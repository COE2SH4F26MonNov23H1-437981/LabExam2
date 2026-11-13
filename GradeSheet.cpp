#include "GradeSheet.hpp"

GradeSheet::GradeSheet() : marks(nullptr), numStudents(0), curveBoost(5.0) {}

GradeSheet::GradeSheet(int count, double initVal, double extraVal) : marks(nullptr), numStudents(count), curveBoost(extraVal) {
    if (numStudents > 0) {
        marks = new double[numStudents];
        for (int i = 0; i < numStudents; ++i) {
            marks[i] = initVal + static_cast<double>(i);
        }
    } else {
        numStudents = 0;
    }
}

// TODO 1: Complete the Copy Constructor (Deep Copy)
GradeSheet::GradeSheet(const GradeSheet& other) {
    (void)other;
}

// TODO 2: Complete the Destructor (Release DMA)
GradeSheet::~GradeSheet() {
}

// TODO 3: Complete the Copy Assignment Operator (Deep Copy + Self-Assignment Guard)
GradeSheet& GradeSheet::operator=(const GradeSheet& other) {
    (void)other;
    return *this;
}

double GradeSheet::getMark(int index) const {
    if (index >= 0 && index < numStudents && marks != nullptr) {
        return marks[index];
    }
    return static_cast<double>(0);
}

// TODO 4: Implement classAverage() (Maximum 8 lines of implementation)
double GradeSheet::classAverage() const {
    return static_cast<double>(0);
}
