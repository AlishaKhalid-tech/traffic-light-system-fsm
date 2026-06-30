#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace chrono;

// States
enum State {
    S0,  // Road A Green, Road B Red
    S1,  // Road A Yellow, Road B Red
    S2,  // Road A Red, Road B Green
    S3,  // Road A Red, Road B Yellow
    S4,  // Emergency mode
    S5   // Night mode (blinking yellow)
};

// Timer function (in seconds)
void wait(int seconds) {
    cout << "[WAIT] " << seconds << " seconds";
    for(int i = seconds; i > 0; i--) {
        cout << " ." << i;
        this_thread::sleep_for(chrono::seconds(1));
    }
    cout << " [DONE]" << endl;
}

// Display current traffic signals
void displayState(State state) {
    cout << "\n" << string(50, '=') << endl;
    
    switch(state) {
        case S0:
            cout << "[STATE: S0] - NORMAL OPERATION" << endl;
            cout << string(50, '-') << endl;
            cout << "ROAD A: GREEN     |  ROAD B: RED" << endl;
            cout << ">> Road A: GO" << endl;
            cout << ">> Road B: STOP" << endl;
            break;
            
        case S1:
            cout << "[STATE: S1] - NORMAL OPERATION" << endl;
            cout << string(50, '-') << endl;
            cout << "ROAD A: YELLOW    |  ROAD B: RED" << endl;
            cout << ">> Road A: PREPARE TO STOP" << endl;
            cout << ">> Road B: STOP" << endl;
            break;
            
        case S2:
            cout << "[STATE: S2] - NORMAL OPERATION" << endl;
            cout << string(50, '-') << endl;
            cout << "ROAD A: RED       |  ROAD B: GREEN" << endl;
            cout << ">> Road A: STOP" << endl;
            cout << ">> Road B: GO" << endl;
            break;
            
        case S3:
            cout << "[STATE: S3] - NORMAL OPERATION" << endl;
            cout << string(50, '-') << endl;
            cout << "ROAD A: RED       |  ROAD B: YELLOW" << endl;
            cout << ">> Road A: STOP" << endl;
            cout << ">> Road B: PREPARE TO STOP" << endl;
            break;
            
        case S4:
            cout << "[STATE: S4] - EMERGENCY MODE" << endl;
            cout << string(50, '-') << endl;
            cout << "EMERGENCY VEHICLE HAS PRIORITY!" << endl;
            cout << "EMERGENCY ROAD: GREEN" << endl;
            cout << "ALL OTHER ROADS: RED" << endl;
            break;
            
        case S5:
            cout << "[STATE: S5] - NIGHT MODE" << endl;
            cout << string(50, '-') << endl;
            cout << "LOW TRAFFIC HOURS" << endl;
            cout << "ROAD A: BLINKING YELLOW" << endl;
            cout << "ROAD B: BLINKING YELLOW" << endl;
            cout << ">> Both roads: PROCEED WITH CAUTION" << endl;
            break;
    }
    cout << string(50, '=') << endl;
}

// Ask user for yes/no input
bool askQuestion(const string& question) {
    string input;
    cout << "\n[QUESTION] " << question << " (y/n): ";
    cin >> input;
    return (input == "y" || input == "Y" || input == "yes" || input == "YES");
}

// Run night mode (blinking yellow) until user exits
void runNightMode() {
    cout << "\n[ENTERING NIGHT MODE] - Blinking Yellow Signals" << endl;
    cout << "[INFO] Type 'stop' to exit night mode and return to normal" << endl;
    cout << string(50, '-') << endl;
    
    string command;
    bool nightActive = true;
    int blinkCount = 0;
    
    // Clear input buffer before starting night mode
    cin.clear();
    cin.ignore(10000, '\n');
    
    while(nightActive) {
        // Display state
        displayState(S5);
        
        // Blink 4 times (2 seconds of blinking)
        for(int i = 0; i < 4; i++) {
            cout << "\r[YELLOW BLINK]  ";
            cout.flush();
            this_thread::sleep_for(chrono::milliseconds(500));
            cout << "\r[OFF]          ";
            cout.flush();
            this_thread::sleep_for(chrono::milliseconds(500));
            blinkCount++;
        }
        
        cout << "\n[Blink cycle completed]" << endl;
        
        // Ask if user wants to exit night mode
        cout << "\n[QUESTION] Type 'stop' to end night mode: ";
        
        // Use getline to read the whole line
        getline(cin, command);
        
        // Check if user typed stop
        if(command == "stop" || command == "STOP" || command == "Stop") {
            nightActive = false;
            cout << "\n[EXITING NIGHT MODE] - Returning to normal operation" << endl;
            this_thread::sleep_for(chrono::seconds(1));
        }
    }
}

// Run emergency mode (5 seconds)
void runEmergencyMode() {
    cout << "\n[ENTERING EMERGENCY MODE]" << endl;
    displayState(S4);
    wait(5);  // Emergency lasts 5 seconds
    cout << "\n[EMERGENCY CLEARED] - Returning to normal cycle." << endl;
    this_thread::sleep_for(chrono::seconds(1));
}

// Main normal traffic cycle
void runNormalCycle(State& currentState) {
    switch(currentState) {
        case S0:
            displayState(S0);
            wait(10);  // Green light for 10 seconds
            currentState = S1;
            break;
            
        case S1:
            displayState(S1);
            wait(3);   // Yellow light for 3 seconds
            currentState = S2;
            break;
            
        case S2:
            displayState(S2);
            wait(10);  // Green light for 10 seconds
            currentState = S3;
            break;
            
        case S3:
            displayState(S3);
            wait(3);   // Yellow light for 3 seconds
            currentState = S0;
            break;
            
        default:
            currentState = S0;
            break;
    }
}

int main() {
    cout << "\n[TRAFFIC LIGHT CONTROL SYSTEM] - FSM IMPLEMENTATION" << endl;
    cout << string(60, '=') << endl;
    cout << "INSTRUCTIONS:" << endl;
    cout << "  - Before each cycle, you'll be asked about emergency" << endl;
    cout << "  - Emergency mode lasts 5 seconds" << endl;
    cout << "  - Night mode runs until you type 'stop'" << endl;
    cout << "  - Press Ctrl+C to exit the program completely" << endl;
    cout << string(60, '=') << "\n" << endl;
    
    State currentState = S0;
    int cycleNumber = 0;
    
    while(true) {
        cycleNumber++;
        cout << "\n[========== CYCLE #" << cycleNumber << " ==========]" << endl;
        
        // QUESTION 1: Emergency?
        bool emergency = askQuestion("Is there an emergency vehicle?");
        
        if(emergency) {
            runEmergencyMode();
            // After emergency, reset to S0
            currentState = S0;
            cout << "\n[RESUMING] - Normal cycle from S0" << endl;
            continue; // Start next cycle fresh
        }
        
        // QUESTION 2: Night mode?
        bool nightMode = askQuestion("Enable night mode?");
        
        if(nightMode) {
            runNightMode();
            // After night mode ends, reset to S0
            currentState = S0;
            cout << "\n[RESUMING] - Normal cycle from S0" << endl;
            continue; // Start next cycle fresh
        }
        
        // Normal operation - run one state transition
        runNormalCycle(currentState);
        
        // Small pause between cycles
        cout << "\n[PRESS ENTER] to continue to next cycle...";
        cin.ignore();
        cin.get();
    }
    
    return 0;
}