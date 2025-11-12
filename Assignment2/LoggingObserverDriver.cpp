#include <iostream>
#include "LoggingObserver.h"
#include "Orders.h"
#include "CommandProcessing.h"
#include "GameEngine.h"

void testLoggingObserver() {


    // Create the log observer
    LogObserver logger;
    
    // Create subjects

    CommandProcessor processor;
    OrdersList orderList;
    GameEngine engine;
    
    // Attach observer to all subjects
    
    processor.Attach(&logger);
    orderList.Attach(&logger);
    engine.Attach(&logger);
    
    
    // Detach observer and release allocated memory
    processor.Detach(&logger);
    orderList.Detach(&logger);
    engine.Detach(&logger);
}