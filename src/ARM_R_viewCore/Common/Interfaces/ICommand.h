#ifndef ICOMMAND_H
#define ICOMMAND_H

class ICommand
{
public:
	virtual ~ICommand(){}
	virtual void exec() = 0;
};

#endif // ICOMMAND_H
