/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 *  @file
 *  @brief   Implementation of the CLITask class.
 *  @author  Anubisss <anubisss210@gmail.com>
 */

#include "CLITask.h"

#include <iostream>
#include <string>
#include <sstream>

#include "Command.h"
#include "CommandManager.h"

/**
 *  @brief  Allocate resources for commands and
 *          register them to the CommandHandler.
 *  @see    Impl_RegisterCommands()
 */

CLITask::CLITask()
{
    LOG_TRACE("Initializing CLITask...");
    void Impl_RegisterCommands(); // forward declaration
    Impl_RegisterCommands();
    LOG_TRACE("CLITask initialized.");
}

/**
 *  @brief  Implementation of the loop.
 */

/* virtual */ int CLITask::svc()
{
    LOG_TRACE("CLITask loop start.");
    while (_running)
    {
        std::cout << "TC> "; // prompt

        std::string buffer;
        std::string command;
        Command::CommandArgQueue args;

        getline(std::cin, buffer); // get all the input line
        std::stringbuf strBuf(buffer);
        // convert to istream, then we can easly extract args from it
        std::istream inputStream(&strBuf);

        inputStream >> command >> std::ws;

        // try to find a command
        if (Command* cmd = sCommandManager->FindCommand(command))
        {
            std::ostringstream oss;
            oss << "Command found: \"" << command << "\" and executing it...";
            LOG_TRACE(oss.str());

            while (!inputStream.eof())
            {
                std::string arg;
                inputStream >> arg >> std::ws;
                args.push(arg);
            }

            cmd->Execute(args); // if find a valid: execute it

            // reset the stream
            oss.str(std::string());
            oss.clear();

            oss << "Command executed: \"" << command << "\".";
            LOG_TRACE(oss.str());
        }
        else
        {
            std::ostringstream oss;
            oss << "Unknown command: \"" << command << "\".";
            LOG_TRACE(oss.str());
        }
    }
    LOG_TRACE("CLITask loop end.");
    return 0;
}
