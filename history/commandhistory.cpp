#include "commandhistory.h"

CommandHistory::CommandHistory() {}

template<typename... Args>
void CommandHistory::registerTypedCommand(
    QString commandName,
    std::function<void(Args...)> applyFunc,
    std::function<void(Args...)> revertFunc
) {
    auto cmd = std::make_shared<HistoryCommandBase<Args...>>();
    cmd->commandName = commandName;
    cmd->applyFunction = applyFunc;
    cmd->revertFunction = revertFunc;
    commandMap[commandName] = cmd;
}

template<typename... Args>
void CommandHistory::executeCommand(
    QString commandName,
    Args... args
) {
    if (auto it = commandMap.find(commandName); it != commandMap.end()) {
        if (auto cmd = dynamic_cast<HistoryCommandBase<Args...>*>(it->get())) {
            cmd->applyFunction(args...);
            auto record = ExecutionRecord{
                [cmd, args...](){ cmd->applyFunction(args...); },
                [cmd, args...](){ cmd->revertFunction(args...); }
            };
            undoStack.push(record);
        } else {
            throw std::invalid_argument("Argument type mismatch for command");
        }
    }
}

void CommandHistory::undo() {
    if (undoStack.empty()) return;

    ExecutionRecord record = undoStack.top();
    undoStack.pop();

    record.revertAction();
    redoStack.push(record);
}

void CommandHistory::redo() {
    if (redoStack.empty()) return;

    ExecutionRecord record = redoStack.top();
    redoStack.pop();

    record.applyAction();
    undoStack.push(record);
}
