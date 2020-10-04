#include <stdio.h>
#include <tgbot/tgbot.h>
#include <unistd.h>
#include <string.h>
#include <string.h>
#include <definit.h>

using namespace TgBot;
using namespace std;
using namespace StringTools;

static bool running = false;

string runcommands(const char *command)
{
	FILE *cp;
	string out_string;
	array<char,PATH_MAX> rst;

	cp = popen(command,"r");
	
	if(cp==NULL)
		return string();
	else
	{
		while(fgets(rst.data(), PATH_MAX, cp) != NULL)
			out_string+= rst.data();
	}
	auto ret = pclose(cp);

	if (ret == -1){
		free(cp);
	}


	return out_string;
}

void runBot(Bot &build)
{
 vector <BotCommand::Ptr> commands;
 BotCommand::Ptr cmdarray (new BotCommand);
 cmdarray->command="startbuild";
 cmdarray->description="start the build";
 cmdarray->command="command";
 cmdarray->description="run whatever you want in bash terminal";
 commands.push_back(cmdarray);
 build.getApi().setMyCommands(commands); 
 vector<BotCommand::Ptr> vectCmd;
 vectCmd = build.getApi().getMyCommands();
 build.getEvents().onCommand("startbuild",[&build](Message::Ptr message) {
 	if(message->from->id==SUDO_USER)
 	{
 		if(system(COMMAND)!=0)
 			build.getApi().sendMessage(message->chat->id,"command executed with errors");
 		else
 			build.getApi().sendMessage(message->chat->id,"Command executed successfully");
 	}
 	else
 	{
 		build.getApi().sendMessage(message->chat->id,"You aren't a sudo user");
 	}
    });
build.getEvents().onCommand("command",[&build](Message::Ptr message){
	if(message->from->id==SUDO_USER)
	{
		build.getApi().sendMessage(message->chat->id,"Enter the command");
		running = true;
	}
	else{
		build.getApi().sendMessage(message->chat->id,"You aren't a sudo user");
	}
});
 build.getEvents().onAnyMessage([&build](Message::Ptr message) {
        if (startsWith(message->text, "/startbuild") || startsWith(message->text,"/command")) {
            return;
        }
        else{
        	if(message->from->id==SUDO_USER)
        	{
        		if(running) {
        			string out = runcommands(message->text.c_str());
        			if(out.empty())
						build.getApi().sendMessage(message->chat->id,"command executed with errors");
					else{
						build.getApi().sendMessage(message->chat->id,"Command executed successfully");
						build.getApi().sendMessage(message->chat->id,"Output \n \n"+out);
					}
					running=false;
        		}
        	}
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


