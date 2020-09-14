#include <stdio.h>
#include <tgbot/tgbot.h>
#include <unistd.h>
#include <string.h>
#include <string.h>
#include <definit.h>

using namespace TgBot;
using namespace std;
using namespace StringTools;

int runcommands(char *command)
{
	return system(command);
}

void runBot(Bot &build)
{
 vector <BotCommand::Ptr> commands;
 BotCommand::Ptr cmdarray (new BotCommand);
 cmdarray->command="startbuild";
 cmdarray->description="start the build";
 commands.push_back(cmdarray);
 build.getApi().setMyCommands(commands); 
 vector<BotCommand::Ptr> vectCmd;
 vectCmd = build.getApi().getMyCommands();
 build.getEvents().onCommand("startbuild",[&build](Message::Ptr message) {
 	if(message->chat->id==SUDO_USER)
 	{
 		if((runcommands(COMMAND))!=0)
 			build.getApi().sendMessage(message->chat->id,"command executed with errors");
 		else
 			build.getApi().sendMessage(message->chat->id,"Command executed successfully");
 	}
 	else
 	{
 		build.getApi().sendMessage(message->chat->id,"You aren't a sudo user");
 	}
    });
 build.getEvents().onAnyMessage([&build](Message::Ptr message) {
        if (startsWith(message->text, "/startbuild")) {
            return;
        }
    });
}

int main()
{
 Bot bot(TG_API_KEY);
 runBot(bot);
 TgLongPoll longpoll(bot);
 try{
 while(true)
 {
 longpoll.start();	
}
}
catch(exception &e)
{
	bot.getApi().sendMessage(SUDO_USER,e.what());
}	
 return 0;
}

