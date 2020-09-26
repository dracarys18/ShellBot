# ShellBot

A simple Telegram Bot written in C++ to run shell commands in a Linux system remotely
from a Telegram Bot.

# Deployment

1. Install C++ Telegram Bot Library. The guide to install the library is [here](https://github.com/reo7sp/tgbot-cpp#library-installation)
2. Go to [@BotFather](https://t.me/botfather) in telegram and create a Bot.
3. Get the Bot API Key and add it in TG_API_KEY in `include/definit.h`  header file.
4. Add your user id in SUDO_USER in `include/definit.h`  header file.
5. And finally to run the Bot run following commands:-
```
cmake .
make -j $(nproc --all)
```