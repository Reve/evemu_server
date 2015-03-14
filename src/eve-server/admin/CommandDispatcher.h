#ifndef __COMMANDDISPATCHER_H_INCL__
#define __COMMANDDISPATCHER_H_INCL__

#include "admin/CommandDB.h"

class Player;
class Seperator;
class PyResult;
class PyServiceMgr;

class CommandDispatcher {
public:
    //this is the prototype for a command function:
    typedef PyResult (*CommandFunc)(Player *who, DBcore *db, CommandDB *command_db, PyServiceMgr *services, const Seperator &args);

    class CommandRecord {
    public:
        CommandRecord( const char * cmd, const char * desc, uint64 req_role, CommandFunc& callback ) : command(cmd),
            description(desc), required_role(req_role), function(callback) {}

        std::string command;
        std::string description;
        uint64 required_role;
        CommandFunc function;
    };

    CommandDispatcher(PyServiceMgr &services, DBcore &db);
    virtual ~CommandDispatcher();

    PyResult Execute(Player *from, const char *msg);

    void AddCommand(const char *cmd, const char *desc, uint64 required_role, CommandFunc function);

protected:
    PyServiceMgr &m_services;
	DBcore &m_db;
    CommandDB command_db;

    std::map<std::string, CommandRecord *> m_commands;    //we own these pointers
};

#endif
