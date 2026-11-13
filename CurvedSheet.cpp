#include "CurvedSheet.hpp"

CurvedSheet::CurvedSheet() : GradeSheet() {}

CurvedSheet::CurvedSheet(int count, double initVal, double extraVal) : GradeSheet(count, initVal, extraVal) {}

CurvedSheet::CurvedSheet(const CurvedSheet& other) : GradeSheet(other) {}

CurvedSheet::~CurvedSheet() {}

CurvedSheet& CurvedSheet::operator=(const CurvedSheet& other) {
    if (this != &other) {
        GradeSheet::operator=(other);
    }
    return *this;
}

// TODO 5: Override classAverage() (Maximum 8 lines of implementation)
double CurvedSheet::classAverage() const {
    return static_cast<double>(0);
}
