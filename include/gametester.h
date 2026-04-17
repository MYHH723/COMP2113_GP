#ifndef GAMETESTER_H
#define GAMETESTER_H

#include <string>
#include <vector>

// GameTester class - tests shop functions and generates test reports
class GameTester {
private:
    std::vector<std::string> bugs;
    std::string testReport;

public:
    GameTester();
    ~GameTester();

    // Testing
    void runAllTests();
    void recordBug(const std::string& desc);
    void generateTestReport();
};

#endif // GAMETESTER_H
