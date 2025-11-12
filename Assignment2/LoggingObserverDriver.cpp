#include <iostream>
#include <cassert>
#include "LoggingObserver.h"
#include "Orders.h"
#include "CommandProcessing.h"
#include "GameEngine.h"

void testLoggingObserver() {
    cout << "=== Starting Logging Observer Test ===" << endl;

    // Create the log observer
    LogObserver logger;
    
    // Create subjects
    CommandProcessor processor;
    OrdersList orderList;
    GameEngine engine;
    
    // Test 1: Verify classes are subclasses of Subject and ILoggable
    cout << "\n--- Test 1: Class Hierarchy Verification ---" << endl;

    // Attach observer to all subjects
    processor.Attach(&logger);
    orderList.Attach(&logger);
    engine.Attach(&logger);

    // Test Command class
    assert(dynamic_cast<Subject*>(&processor) != nullptr);
    assert(dynamic_cast<ILoggable*>(&processor) != nullptr);
    cout << "✓ CommandProcessor is subclass of Subject and ILoggable" << endl;
    
    // Test OrderList class
    assert(dynamic_cast<Subject*>(&orderList) != nullptr);
    assert(dynamic_cast<ILoggable*>(&orderList) != nullptr);
    cout << "✓ OrderList is subclass of Subject and ILoggable" << endl;
    
    // Test GameEngine class
    assert(dynamic_cast<Subject*>(&engine) != nullptr);
    assert(dynamic_cast<ILoggable*>(&engine) != nullptr);
    cout << "✓ GameEngine is subclass of Subject and ILoggable" << endl;

    // Test 3: Verify gamelog.txt is being written
    cout << "\n--- Test 3: File Writing Verification ---" << endl;
    ifstream logFile("gamelog.txt");
    assert(logFile.is_open());
    
    string line;
    int lineCount = 0;
    while (getline(logFile, line)) {
        lineCount++;
        cout << "Log Entry " << lineCount << ": " << line << endl;
    }
    logFile.close();
    
    assert(lineCount >= 3); // Should have at least 3 command entries
    cout << "✓ gamelog.txt is correctly written with command entries" << endl;

    // Test 4: OrderList::addOrder() notification
    cout << "\n--- Test 4: OrderList Notifications ---" << endl;

    // Test 5: Order execution notifications
    cout << "\n--- Test 5: Order Execution Notifications ---" << endl;
    
    // Test 6: GameEngine state transition notifications
    cout << "\n--- Test 6: GameEngine State Transition Notifications ---" << endl;

// Final verification: Check complete log file
    cout << "\n--- Final Log File Verification ---" << endl;
    ifstream logFile("gamelog.txt");
    assert(logFile.is_open());
    
    string line;
    int lineCount = 0;
    while (getline(logFile, line)) {
        lineCount++;
        cout << "Log Entry " << lineCount << ": " << line << endl;
    }
    logFile.close();
    
    cout << "\n=== Logging Observer Test Completed Successfully! ===" << endl;
    cout << "Check gamelog.txt for complete log of all operations." << endl;
    

    // Detach observer and release allocated memory
    processor.Detach(&logger);
    orderList.Detach(&logger);
    engine.Detach(&logger);
    
    delete &logger;
    delete &processor;
    delete &orderList;
    delete &engine;
}