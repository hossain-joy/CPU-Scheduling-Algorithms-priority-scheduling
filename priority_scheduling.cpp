#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Process {
    string id;
    int at, bt, pr, ct, tat, wt;
    bool completed = false;
};

bool compareProcesses(Process a, Process b) {
    if (a.pr != b.pr) return a.pr < b.pr; // Lower priority value = Higher priority
    if (a.at != b.at) return a.at < b.at; // FCFS if priority is same
    return a.bt < b.bt;                   // SJF if AT and Priority are same
}

int main() {
    vector<Process> p = {
        {"P1", 3, 2, 1},
        {"P2", 5, 1, 3},
        {"P3", 6, 7, 4},
        {"P4", 3, 3, 1}
    };

    int n = p.size();
    int currentTime = 0;
    int completedCount = 0;

    cout << "Gantt Chart: ";

    while (completedCount < n) {
        int idx = -1;
        
        // Find available processes at currentTime
        vector<int> available;
        for (int i = 0; i < n; i++) {
            if (!p[i].completed && p[i].at <= currentTime) {
                available.push_back(i);
            }
        }

        if (available.empty()) {
            currentTime++;
            continue;
        }

        // Apply tie-breaking rules among available processes
        sort(available.begin(), available.end(), [&](int a, int b) {
            if (p[a].pr != p[b].pr) return p[a].pr < p[b].pr;
            if (p[a].at != p[b].at) return p[a].at < p[b].at;
            return p[a].bt < p[b].bt;
        });

        idx = available[0];

        // Execute Process
        cout << "| " << p[idx].id << " ";
        currentTime += p[idx].bt;
        p[idx].ct = currentTime;
        p[idx].tat = p[idx].ct - p[idx].at;
        p[idx].wt = p[idx].tat - p[idx].bt;
        p[idx].completed = true;
        completedCount++;
    }
    cout << "|" << endl << endl;

    // Output Table
    cout << left << setw(10) << "PID" << setw(10) << "AT" << setw(10) << "BT" 
         << setw(10) << "Pri" << setw(10) << "CT" << setw(10) << "TAT" << "WT" << endl;
    
    for (const auto& proc : p) {
        cout << left << setw(10) << proc.id << setw(10) << proc.at << setw(10) << proc.bt 
             << setw(10) << proc.pr << setw(10) << proc.ct << setw(10) << proc.tat << proc.wt << endl;
    }

    return 0;
}