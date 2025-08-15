#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>
#include <string>
#include <vector>
#include <ctime>
#include <ratio>

// Real-time Web App Dashboard
class Dashboard {
private:
    std::mutex mtx;
    std::string current_time;
    int user_count = 0;
    int request_count = 0;
    double response_time_avg = 0.0;

    // Update current time
    void updateCurrTime() {
        auto now = std::chrono::system_clock::now();
        auto now_time_t = std::chrono::system_clock::to_time_t(now);
        current_time = std::ctime(&now_time_t);
    }

    // Calculate response time average
    void calculateResponseTimeAvg() {
        response_time_avg = static_cast<double>(request_count) / user_count;
    }

public:
    Dashboard() {
        updateCurrTime();
    }

    // Add user
    void addUser() {
        std::lock_guard<std::mutex> lock(mtx);
        user_count++;
    }

    // Add request
    void addRequest() {
        std::lock_guard<std::mutex> lock(mtx);
        request_count++;
    }

    // Get current time
    std::string getCurrTime() {
        return current_time;
    }

    // Get user count
    int getUserCount() {
        return user_count;
    }

    // Get request count
    int getRequestCount() {
        return request_count;
    }

    // Get response time average
    double getResponseTimeAvg() {
        calculateResponseTimeAvg();
        return response_time_avg;
    }
};

int main() {
    Dashboard dashboard;

    // Simulate users and requests
    std::thread t1([&dashboard]() {
        for (int i = 0; i < 10; i++) {
            dashboard.addUser();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    });

    std::thread t2([&dashboard]() {
        for (int i = 0; i < 20; i++) {
            dashboard.addRequest();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });

    t1.join();
    t2.join();

    // Print dashboard metrics
    std::cout << "Current Time: " << dashboard.getCurrTime() << std::endl;
    std::cout << "User Count: " << dashboard.getUserCount() << std::endl;
    std::cout << "Request Count: " << dashboard.getRequestCount() << std::endl;
    std::cout << "Response Time Average: " << dashboard.getResponseTimeAvg() << std::endl;

    return 0;
}