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
InlineKeyboardMarkup::Ptr keyboard;

void escape_html(string& html) {
    string tmp;
    tmp.reserve(html.size());
    for(size_t pos = 0; pos != html.size(); ++pos) {
        switch(html[pos]) {
            case '&':  tmp.append("&amp;");       break;
            case '\"': tmp.append("&quot;");      break;
            case '\'': tmp.append("&apos;");      break;
            case '<':  tmp.append("&lt;");        break;
            case '>':  tmp.append("&gt;");        break;
            default:   tmp.append(&html[pos], 1); break;
        }
    }
    html.swap(tmp);
}

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
						escape_html(out);
						build.getApi().sendMessage(message->chat->id,"<b>Output:-</b> \n \n"+out,true, 0, keyboard, "HTML");
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
 signal(SIGINT, [](int sig) {
        printf("SIGINT got\n");
        exit(0);
 });      
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


