/*
 * global.h, part of Plateau
 *
 * Copyright (C) 2019 Antonin Portelli
 *
 * Plateau is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Plateau is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Plateau.  If not, see <http://www.gnu.org/licenses/>.
 */

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

#define INFO(msg) \
qInfo("Plateau: %14s(%p): %s", qPrintable(this->metaObject()->className()),\
      this, qPrintable(msg))

#endif // GLOBAL_H
