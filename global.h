#ifndef GLOBAL_H
#define GLOBAL_H

#include <QMessageBox>

#define CATCH_WARNING(...) \
try\
{\
    __VA_ARGS__;\
}\
catch (const std::exception &e)\
{\
    QMessageBox messageBox;\
    \
    messageBox.warning(0, "Error", e.what());\
    messageBox.setFixedSize(500, 200);\
}

#define CATCH_FATAL(...) \
try\
{\
    __VA_ARGS__;\
}\
catch (const std::exception &e)\
{\
    QMessageBox messageBox;\
    \
    messageBox.critical(0, "Error", e.what());\
    messageBox.setFixedSize(500, 200);\
    exit(EXIT_FAILURE);\
}

#endif // GLOBAL_H
