#ifndef GRADESHEET_HPP
#define GRADESHEET_HPP

#include <iostream>
using namespace std;

class GradeSheet {
protected:
    double* marks;
    int numStudents;
    double curveBoost;

public:
    GradeSheet();
    GradeSheet(int count, double initVal, double extraVal);
    GradeSheet(const GradeSheet& other);
    virtual ~GradeSheet();
    GradeSheet& operator=(const GradeSheet& other);

    double getMark(int index) const;
    virtual double classAverage() const;
};

#endif
