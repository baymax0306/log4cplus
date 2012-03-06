

#include <exception>
#include <iostream>
#include <string>
#include "log4cplus/logger.h"
#include "log4cplus/spi/factory.h"
#include "log4cplus/helpers//property.h"
#include "log4cplus/consoleappender.h"
#include "log4cplus/helpers/appenderattachableimpl.h"
#include "log4cplus/helpers/loglog.h"
#include "log4cplus/helpers/pointer.h"
#include "log4cplus/spi/loggingevent.h"
#include "log4cplus/appendersexports.h"


using namespace std;
using namespace log4cplus;
using namespace log4cplus::helpers;
using namespace log4cplus::spi;

void
printAppenderList(SharedAppenderPtrList list)
{
   cout << "List size: " << list.size() << endl;
   for(SharedAppenderPtrList::iterator it=list.begin(); it!=list.end(); ++it) {
       log4cplus::tcout << "Loop Body: Appender name = " << (*it)->getName()
                        << endl;
   }
}


SharedObjectPtr<Appender>
createAppender (tstring const & name)
{
    return SharedObjectPtr<Appender> (
        log4cplus::spi::getAppenderFactoryRegistry ().get (name)
            ->createObject (helpers::Properties ()));
}


int
main()
{
    // In this test/example, calling initializeLog4cplusAppenders() is
    // necessary to create a dependency of this executable on
    // log4cplus_appenders library. Without this dependency the
    // library will not get loaded and thus no appenders will be
    // registered.
    log4cplus::initializeLog4cplusAppenders ();

    LogLog::getLogLog()->setInternalDebugging(true);
    {
        AppenderAttachableImpl aai;
        try {
            SharedObjectPtr<Appender> append_1(
                createAppender (LOG4CPLUS_TEXT("log4cplus::ConsoleAppender")));
            append_1->setName(LOG4CPLUS_TEXT("First"));

            SharedObjectPtr<Appender> append_2(
                createAppender (LOG4CPLUS_TEXT("log4cplus::ConsoleAppender")));
            append_2->setName(LOG4CPLUS_TEXT("Second"));

            InternalLoggingEvent event(
                Logger::getInstance(LOG4CPLUS_TEXT("test")).getName(),
                DEBUG_LOG_LEVEL, LOG4CPLUS_TEXT("This is a test..."), NULL, 0);

            aai.addAppender(append_1);
            aai.addAppender(append_2);
            aai.addAppender(append_1);
            aai.addAppender(append_2);
            printAppenderList(aai.getAllAppenders());

            aai.removeAppender(append_2);
            printAppenderList(aai.getAllAppenders());

            aai.removeAppender(LOG4CPLUS_TEXT("First"));
            printAppenderList(aai.getAllAppenders());

            aai.addAppender(append_1);
            aai.addAppender(append_2);
            aai.addAppender(append_1);
            aai.addAppender(append_2);
            log4cplus::tcout << "Should be First: "
                             << aai.getAppender(LOG4CPLUS_TEXT("First"))->getName() << endl;
            log4cplus::tcout << "Should be Second: "
                             << aai.getAppender(LOG4CPLUS_TEXT("Second"))->getName() << endl
                             << endl;
            append_1->doAppend(event);
            append_2->doAppend(event);
        }
        catch(std::exception const & e) {
            log4cplus::tcout << "**** Exception occured: " << e.what() << endl;
        }

//      log4cplus::tcout << "*** Calling close()..." << endl;
//      append_2->close();
//      log4cplus::tcout << "*** Done calling close()..." << endl;
    }

    log4cplus::tcout << "Exiting main()..." << endl;
    return 0;
}

