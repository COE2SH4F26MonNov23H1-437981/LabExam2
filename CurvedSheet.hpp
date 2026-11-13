#ifndef CURVEDSHEET_HPP
#define CURVEDSHEET_HPP

#include "GradeSheet.hpp"

class CurvedSheet : public GradeSheet {
public:
    CurvedSheet();
    CurvedSheet(int count, double initVal, double extraVal);
    CurvedSheet(const CurvedSheet& other);
    virtual ~CurvedSheet();
    CurvedSheet& operator=(const CurvedSheet& other);

    virtual double classAverage() const override;
};

#endif
