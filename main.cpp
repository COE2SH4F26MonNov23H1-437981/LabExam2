#include <iostream>
#include "GradeSheet.hpp"
#include "CurvedSheet.hpp"

using namespace std;

// Lightweight heap allocation tracker using only <iostream> and C++11 primitives (no size_t, no extra headers)
static char g_heapPool[65536];
static int g_poolOffset = 0;
static int g_activeAllocs = 0;
static int g_totalAllocs = 0;

void* operator new(decltype(sizeof(0)) bytes) {
    int req = static_cast<int>(bytes);
    if (req <= 0) req = 1;
    // 8-byte alignment
    int rem = req % 8;
    if (rem != 0) req += (8 - rem);
    if (g_poolOffset + req > 65536) return nullptr;
    void* ptr = &g_heapPool[g_poolOffset];
    g_poolOffset += req;
    ++g_activeAllocs;
    ++g_totalAllocs;
    return ptr;
}

void* operator new[](decltype(sizeof(0)) bytes) {
    int req = static_cast<int>(bytes);
    if (req <= 0) req = 1;
    int rem = req % 8;
    if (rem != 0) req += (8 - rem);
    if (g_poolOffset + req > 65536) return nullptr;
    void* ptr = &g_heapPool[g_poolOffset];
    g_poolOffset += req;
    ++g_activeAllocs;
    ++g_totalAllocs;
    return ptr;
}

void operator delete(void* ptr) noexcept {
    if (ptr != nullptr) {
        --g_activeAllocs;
    }
}

void operator delete[](void* ptr) noexcept {
    if (ptr != nullptr) {
        --g_activeAllocs;
    }
}

void operator delete(void* ptr, decltype(sizeof(0))) noexcept {
    if (ptr != nullptr) {
        --g_activeAllocs;
    }
}

void operator delete[](void* ptr, decltype(sizeof(0))) noexcept {
    if (ptr != nullptr) {
        --g_activeAllocs;
    }
}

bool approxEqual(double a, double b) {
    double diff = a - b;
    if (diff < 0.0) diff = -diff;
    return diff < 0.001;
}

int main() {
    int passed = 0;
    int failed = 0;

    // Expected reference values computed dynamically
    // Array of 3 elements initialized as: initVal + 0, initVal + 1, initVal + 2
    double initV = static_cast<double>(70.0);
    double extV = static_cast<double>(5.0);

    // --- PARENT CLASS TEST CASES (1 to 5) ---
    // TC 1: Default constructor & empty compute
    {
        GradeSheet pDef;
        if (approxEqual(static_cast<double>(pDef.classAverage()), 0.0) &&
            approxEqual(static_cast<double>(pDef.getMark(0)), 0.0)) {
            cout << "[PASS] TC 01: GradeSheet default constructor" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 01: GradeSheet default constructor" << endl;
            ++failed;
        }
    }

    // TC 2: Parameterized constructor & base compute()
    {
        GradeSheet p1(3, initV, extV);
        bool ok = approxEqual(static_cast<double>(p1.getMark(0)), static_cast<double>(initV)) &&
                  approxEqual(static_cast<double>(p1.getMark(2)), static_cast<double>(initV + static_cast<double>(2)));
        // Verify compute() is non-trivial when initV > 0
        if (ok && p1.getMark(1) == static_cast<double>(initV + static_cast<double>(1))) {
            // Compare against a second identical instance
            GradeSheet pCheck(3, initV, extV);
            if (approxEqual(static_cast<double>(p1.classAverage()), static_cast<double>(pCheck.classAverage())) &&
                !approxEqual(static_cast<double>(p1.classAverage()), 0.0)) {
                cout << "[PASS] TC 02: GradeSheet parameterized constructor & classAverage()" << endl;
                ++passed;
            } else {
                cout << "[FAIL] TC 02: GradeSheet classAverage() returned unexpected value" << endl;
                ++failed;
            }
        } else {
            cout << "[FAIL] TC 02: GradeSheet element initialization mismatch" << endl;
            ++failed;
        }
    }

    // TC 3: Parent Copy Constructor (Deep Copy verification)
    {
        GradeSheet orig(3, initV, extV);
        GradeSheet copyObj(orig);
        if (approxEqual(static_cast<double>(copyObj.classAverage()), static_cast<double>(orig.classAverage())) &&
            approxEqual(static_cast<double>(copyObj.getMark(1)), static_cast<double>(orig.getMark(1))) &&
            !approxEqual(static_cast<double>(copyObj.classAverage()), 0.0)) {
            cout << "[PASS] TC 03: GradeSheet copy constructor deep copy" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 03: GradeSheet copy constructor failed" << endl;
            ++failed;
        }
    }

    // TC 4: Parent Copy Assignment Operator & Self-Assignment
    {
        GradeSheet a(3, initV, extV);
        GradeSheet b(2, initV, extV);
        b = a;
        GradeSheet* selfPtr = &b;
        b = *selfPtr; // Self-assignment check
        if (approxEqual(static_cast<double>(b.classAverage()), static_cast<double>(a.classAverage())) &&
            approxEqual(static_cast<double>(b.getMark(2)), static_cast<double>(a.getMark(2))) &&
            !approxEqual(static_cast<double>(b.classAverage()), 0.0)) {
            cout << "[PASS] TC 04: GradeSheet copy assignment & self-assignment" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 04: GradeSheet copy assignment failed" << endl;
            ++failed;
        }
    }

    // TC 5: Parent Destructor Heap Release Check
    {
        int before = g_activeAllocs;
        {
            GradeSheet temp(4, initV, extV);
        }
        int after = g_activeAllocs;
        if (before == after && g_totalAllocs > 0) {
            cout << "[PASS] TC 05: GradeSheet destructor released heap memory" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 05: GradeSheet destructor leaked heap memory" << endl;
            ++failed;
        }
    }

    // --- CHILD CLASS TEST CASES (6 to 10) ---
    // TC 6: Child Parameterized Construction & Overridden compute()
    {
        GradeSheet baseObj(3, initV, extV);
        CurvedSheet childObj(3, initV, extV);
        if (!approxEqual(static_cast<double>(childObj.classAverage()), 0.0) &&
            !approxEqual(static_cast<double>(childObj.classAverage()), static_cast<double>(baseObj.classAverage()))) {
            cout << "[PASS] TC 06: CurvedSheet overridden classAverage()" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 06: CurvedSheet overridden classAverage() failed" << endl;
            ++failed;
        }
    }

    // TC 7: Polymorphic Dispatch via Base Pointer
    {
        CurvedSheet directChild(3, initV, extV);
        GradeSheet* polyPtr = &directChild;
        if (!approxEqual(static_cast<double>(polyPtr->classAverage()), 0.0) &&
            approxEqual(static_cast<double>(polyPtr->classAverage()), static_cast<double>(directChild.classAverage()))) {
            cout << "[PASS] TC 07: CurvedSheet polymorphic dispatch via GradeSheet*" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 07: CurvedSheet polymorphic dispatch failed" << endl;
            ++failed;
        }
    }

    // TC 8: Child Copy Constructor
    {
        CurvedSheet c1(3, initV, extV);
        CurvedSheet c2(c1);
        if (approxEqual(static_cast<double>(c1.classAverage()), static_cast<double>(c2.classAverage())) &&
            !approxEqual(static_cast<double>(c2.classAverage()), 0.0)) {
            cout << "[PASS] TC 08: CurvedSheet copy constructor" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 08: CurvedSheet copy constructor failed" << endl;
            ++failed;
        }
    }

    // TC 9: Child Copy Assignment Operator
    {
        CurvedSheet c1(3, initV, extV);
        CurvedSheet c2;
        c2 = c1;
        if (approxEqual(static_cast<double>(c1.classAverage()), static_cast<double>(c2.classAverage())) &&
            !approxEqual(static_cast<double>(c2.classAverage()), 0.0)) {
            cout << "[PASS] TC 09: CurvedSheet copy assignment operator" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 09: CurvedSheet copy assignment operator failed" << endl;
            ++failed;
        }
    }

    // TC 10: Virtual Destructor via Base Pointer Deletion
    {
        int before = g_activeAllocs;
        GradeSheet* heapChild = new CurvedSheet(4, initV, extV);
        delete heapChild;
        int after = g_activeAllocs;
        if (before == after) {
            cout << "[PASS] TC 10: CurvedSheet virtual destructor via GradeSheet*" << endl;
            ++passed;
        } else {
            cout << "[FAIL] TC 10: CurvedSheet virtual destructor leaked memory" << endl;
            ++failed;
        }
    }

    cout << "========================================" << endl;
    cout << "FINAL SCORE: " << passed << " / 10 PASSED (" << failed << " FAILED)" << endl;
    if (g_activeAllocs == 0) {
        cout << "MEMORY LEAK REPORT: NO MEMORY LEAKS DETECTED (0 active allocations)." << endl;
    } else {
        cout << "MEMORY LEAK REPORT: WARNING! " << g_activeAllocs << " active heap allocation(s) leaked!" << endl;
    }
    cout << "========================================" << endl;

    return (failed == 0 && g_activeAllocs == 0) ? 0 : 1;
}
