#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>

using namespace std;

// Structure to hold process details
struct Process {
    string id;
    int at; // Arrival Time
    int bt; // Burst Time
    int pr; // Priority
    int ct; // Completion Time
    int tat; // Turnaround Time
    int wt; // Waiting Time
    bool isCompleted;
};

int main() {
    // Initializing the processes based on Table 1 of the lab report
    int n = 4;
    vector<Process> p = {
        {"P1", 3, 2, 1, 0, 0, 0, false},
        {"P2", 5, 1, 3, 0, 0, 0, false},
        {"P3", 6, 7, 4, 0, 0, 0, false},
        {"P4", 3, 3, 1, 0, 0, 0, false}
    };

    int currentTime = 0;
    int completedCount = 0;
    
    // Variables for calculating averages
    float totalTAT = 0;
    float totalWT = 0;
    
    // Array to store the execution sequence (Gantt Chart representation)
    vector<string> executionOrder;

    while (completedCount < n) {
        int idx = -1;
        
        // Find the appropriate process in the ready queue
        for (int i = 0; i < n; i++) {
            if (p[i].at <= currentTime && !p[i].isCompleted) {
                if (idx == -1) {
                    idx = i;
                } else {
                    // Rule 1: Lower priority value = Higher priority
                    if (p[i].pr < p[idx].pr) {
                        idx = i;
                    } 
                    else if (p[i].pr == p[idx].pr) {
                        // Rule 2: Tie on priority -> FCFS (earlier arrival time)
                        if (p[i].at < p[idx].at) {
                            idx = i;
                        } 
                        else if (p[i].at == p[idx].at) {
                            // Rule 3: Tie on priority & arrival time -> SJF (shorter burst time)
                            if (p[i].bt < p[idx].bt) {
                                idx = i;
                            }
                        }
                    }
                }
            }
        }

        if (idx != -1) {
            // A process was found to execute
            currentTime += p[idx].bt;            // Process runs to completion
            p[idx].ct = currentTime;             // Set Completion Time
            p[idx].tat = p[idx].ct - p[idx].at;  // Turnaround Time = CT - AT
            p[idx].wt = p[idx].tat - p[idx].bt;  // Waiting Time = TAT - BT
            
            p[idx].isCompleted = true;           // Mark as done
            completedCount++;
            
            totalTAT += p[idx].tat;
            totalWT += p[idx].wt;
            
            executionOrder.push_back(p[idx].id); // Record execution order
        } else {
            // CPU is idle, fast forward time to the next arriving process
            int nextArrival = 1e9;
            for (int i = 0; i < n; i++) {
                if (!p[i].isCompleted && p[i].at > currentTime) {
                    nextArrival = min(nextArrival, p[i].at);
                }
            }
            currentTime = nextArrival;
        }
    }

    // Output formatting to match the report's result tables
    cout << "---------------------------------------------------------\n";
    cout << "Execution Order: ";
    for (int i = 0; i < executionOrder.size(); i++) {
        cout << executionOrder[i];
        if (i < executionOrder.size() - 1) cout << " -> ";
    }
    cout << "\n---------------------------------------------------------\n";
    
    cout << left << setw(10) << "Process" 
         << setw(15) << "Arrival Time" 
         << setw(15) << "Burst Time" 
         << setw(10) << "Priority" 
         << setw(10) << "CT" 
         << setw(10) << "TAT" 
         << setw(10) << "WT" << "\n";
    cout << "--------------------------------------------------------------------------------\n";

    for (const auto& process : p) {
        cout << left << setw(10) << process.id 
             << setw(15) << process.at 
             << setw(15) << process.bt 
             << setw(10) << process.pr 
             << setw(10) << process.ct 
             << setw(10) << process.tat 
             << setw(10) << process.wt << "\n";
    }
    
    cout << "--------------------------------------------------------------------------------\n";
    cout << fixed << setprecision(2);
    cout << "Average Turnaround Time: " << (totalTAT / n) << " units\n";
    cout << "Average Waiting Time:    " << (totalWT / n) << " units\n";

    return 0;
}
