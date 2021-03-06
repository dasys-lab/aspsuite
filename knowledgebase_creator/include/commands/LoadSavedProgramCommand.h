#pragma once

#include <commands/Command.h>
#include <memory>

#include <QJsonObject>
#include <QByteArray>
#include <QJsonDocument>
#include <chrono>
#include <fstream>

//#define LSPC_EVALCODE

namespace kbcr
{
	class SettingsDialog;
	class KnowledgebaseCreator;
	/**
	 * Class inheriting from Command interface used to load a saved Json program
	 */
	class LoadSavedProgramCommand : public Command, public std::enable_shared_from_this<LoadSavedProgramCommand>
	{
	Q_OBJECT
	public:
		LoadSavedProgramCommand(KnowledgebaseCreator* gui, QString fileName, QByteArray loadedData);
		virtual ~LoadSavedProgramCommand();

		void execute();
		void undo();

		QJsonObject toJSON();

		QByteArray loadedData;
		QString fileName;
		KnowledgebaseCreator* gui;
#ifdef LSPC_EVALCODE
        std::ofstream outFS;
#endif

	signals:
		void cn5CallFinished();
	};

} /* namespace kbcr */

