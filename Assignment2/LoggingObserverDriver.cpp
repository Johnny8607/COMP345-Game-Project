#include <iostream>
#include "LoggingObserver.h"
#include "Orders.h"
#include "CommandProcessing.h"
#include "GameEngine.h"

void testLoggingObserver(){


    // Create the log observer
    LogObserver logger();
    
    // Create subjects

    CommandProcessor processor;
    OrderList orderList;
    GameEngine engine;
    
    // Attach observer to all subjects
    
    processor.attach(&logger);
    orderList.attach(&logger);
    engine.attach(&logger);
    
    


}