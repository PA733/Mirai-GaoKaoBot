#include <iostream>

// MiraiCP依赖文件(只需要引入这一个)
#include <MiraiCP.hpp>
#include <nlohmann/json.hpp>
#include <ctime>
using namespace MiraiCP;

std::string version = "0.0.1";
unsigned long long BotID = 1728525576;

const PluginConfig CPPPlugin::config{
    "Plugin com.ypemc.GaokaoBot",   // 插件id，如果和其他插件id重复将会被拒绝加载！
    "Plugin GaokaoBot",             // 插件名称
    ("Version " + version).c_str(), // 插件版本
    "Author PA733",                 // 插件作者
    "Plugin description",           // 可选：插件描述
    "Publish time"                  // 可选：日期
};

std::string getCountDownText()
{
  time_t now = time(0);
  tm *ltm = localtime(&now);

  int currentYear = 1900 + ltm->tm_year;
  int targetYear = currentYear;
  if (ltm->tm_mon > 5 || (ltm->tm_mon == 5 && ltm->tm_mday > 7) || (ltm->tm_mon == 5 && ltm->tm_mday == 7 && ltm->tm_hour >= 9))
  {
    targetYear++;
  }
  tm target = {0, 0, 9, 7, 5, targetYear - 1900};

  time_t targetTime = mktime(&target);
  double diff = difftime(targetTime, now);

  int days = diff / (24 * 60 * 60);
  diff -= days * 24 * 60 * 60;
  int hours = diff / (60 * 60);
  diff -= hours * 60 * 60;
  int mins = diff / 60;
  diff -= mins * 60;
  int secs = (int)diff;

  std::string result = "现在距离 " + std::to_string(targetYear) + " 年普通高等学校招生全国统一考试还有：" + "\n" + std::to_string(days) + "天 " + std::to_string(hours) + "小时 " + std::to_string(mins) + "分 " + std::to_string(secs) + "秒";
  return result;
}

// 插件实例
class PluginMain : public CPPPlugin
{
public:
  // 配置插件信息
  PluginMain() : CPPPlugin() {}
  ~PluginMain() override = default; // override关键字是为了防止内存泄漏

  // 入口函数。插件初始化时会被调用一次，请在此处注册监听
  void onEnable() override
  {
    // 设置logger是否启用
    Logger::logger.setHandleState(true);
    Event::registerEvent<GroupMessageEvent>([](GroupMessageEvent e)
                                            {
                                              if (e.bot.id() == BotID && e.message.toMiraiCode() == "/d")
                                              {
                                                e.group.sendMessage(getCountDownText());
                                              } });
    Event::registerEvent<PrivateMessageEvent>([](PrivateMessageEvent e)
                                              {
                                                if (e.bot.id() == BotID && e.message.toMiraiCode() == "/d")
                                                {
                                                  e.sender.sendMessage(getCountDownText());
                                                }
                                                else{
                                                  e.sender.sendMessage("很抱歉，机器人目前只支持\"/d\"命令。\n如果您需要其他功能，请前往项目仓库issue区反馈：https://github.com/pa733/mirai-GaoKaoBot/issues");
                                                } });
    Event::registerEvent<NewFriendRequestEvent>([](NewFriendRequestEvent e)
                                                { 
                                                  if(e.bot.id() == BotID){
                                                    e.accept();
                                                  } });
    Event::registerEvent<GroupInviteEvent>([](GroupInviteEvent e)
                                           {
                                             // if(e.bot.id() == BotID){
                                             //   e.accept();
                                             // }
                                           });
    Event::registerEvent<BotJoinGroupEvent>([](BotJoinGroupEvent e)
                                            { e.group.sendMessage("机器人初始化成功，版本：" + version + "\n使用方式：群内或私聊发送\"/d\"。\n本项目使用 GNU Affero General Public License v3.0 协议开源于GitHub：https://github.com/pa733/mirai-GaoKaoBot\n作者：PA733"); });
  }

  // 退出函数。请在这里结束掉所有子线程，否则可能会导致程序崩溃
  void onDisable() override
  {
    /*插件结束前执行*/
  }
};

// 创建当前插件实例。请不要进行其他操作，
// 初始化请在onEnable中进行
void MiraiCP::enrollPlugin()
{
  MiraiCP::enrollPlugin<PluginMain>();
}
