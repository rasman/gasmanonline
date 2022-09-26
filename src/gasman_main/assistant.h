#ifndef ASSISTANT_H
#define ASSISTANT_H

//Forward declaration
class QString;
class QProcess;

class Assistant
{
public:
	Assistant(void);
	~Assistant(void);

	void showDocumentation(const QString &page);

private:
	bool startAssistant();
	QProcess *proc;
};

#endif
