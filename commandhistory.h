#ifndef COMMANDHISTORY_H
#define COMMANDHISTORY_H

#include <stack>
#include <QHash>

template<typename... Args>
struct HistoryCommandBase {
    QString commandName;
    std::function<void(Args...)> applyFunction;
    std::function<void(Args...)> revertFunction;

	virtual ~HistoryCommandBase() = default;
};

struct ExecutionRecord {
    std::function<void()> applyAction;
    std::function<void()> revertAction;
};

class CommandHistory
{
public:
    CommandHistory();

    template<typename... Args>
    void registerTypedCommand(
        QString commandName,
        std::function<void(Args...)> applyFunc,
        std::function<void(Args...)> revertFunc
    );

    template<typename... Args>
    void executeCommand(
        QString commandName,
        Args... args
    );

    void redo();

    void undo();

private:
    QHash<QString, std::shared_ptr<HistoryCommandBase<>>> commandMap;

    std::stack<ExecutionRecord> undoStack;

    std::stack<ExecutionRecord> redoStack;
};

#endif // COMMANDHISTORY_H
