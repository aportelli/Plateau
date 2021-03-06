/*
 * main.cpp, part of Plateau
 *
 * Copyright (C) 2019-2020 Antonin Portelli
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

#include <src/Global.h>
#include <src/MainWindow.h>

using namespace Latan;

int main(int argc, char* argv[])
{
    MainWindow     *mainWindow;
    QApplication   app(argc, argv);

    if (QStyleFactory::keys().contains("Fusion"))
    {
        QApplication::setStyle("Fusion");
    }
    mainWindow = new MainWindow;
    mainWindow->show();

    CATCH_FATAL(app.exec());

    return EXIT_SUCCESS;
}
