#include <algorithm>
#include <array>
#include <assert.h>
#include <cassert>
#include <charconv>
#include <chrono>
#include <compare>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <iterator>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <system_error>
#include <time.h>
#include <type_traits>
#include <tuple>
#include <vector>

using namespace std::chrono;
using namespace std::literals;
using Clock = std::chrono::system_clock;
using TimePoint = std::chrono::time_point<std::chrono::system_clock>;
using Duration = std::chrono::seconds;
using D_Range = std::pair<Duration,Duration>;
using ActivityList = std::vector<std::tuple<std::string, Duration>>;

// MSVC wants "safe" function_s, but does not implement strptime 
#ifdef _MSC_VER

extern "C" char* strptime(const char* s,
                          const char* f,
                          struct tm* tm) {
  std::istringstream input(s);
  input.imbue(std::locale(setlocale(LC_ALL, nullptr)));
  input >> std::get_time(tm, f);
  if (input.fail()) {
    return nullptr;
  }
  return (char*)(s + input.tellg());
}

#else

void localtime_s(std::tm *t,const std::time_t *tt)
{
  memcpy(t,::localtime(tt),sizeof(std::tm));
}
int sscanf_s(const char* in,const char *format,int* h,int* m, int* s) 
{
  return sscanf(in,format,h,m,s);
}

#endif

std::string ToString( const TimePoint& time, const std::string& format)
{
    const std::time_t tt = system_clock::to_time_t(time);
    std::tm tm({0});
    localtime_s(&tm,&tt); //Locale time-zone, usually UTC by default.
    std::stringstream ss;
    ss << std::put_time( &tm, format.c_str() );
    return ss.str();
}

TimePoint TimeTomorrowAt(int hour, int min)
{
  const time_t tt = system_clock::to_time_t(system_clock::now());
  std::tm tm({0});
  ::localtime_s(&tm,&tt);
  tm.tm_sec = 0;
  tm.tm_min = min;
  tm.tm_hour = hour;
  tm.tm_mday++;
  tm.tm_isdst = -1; // Use DST value from local time zone
  return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

TimePoint TimeTodayAt(int hour, int min)
{
  const time_t tt = system_clock::to_time_t(system_clock::now());
  tm local_tm({0});
  ::localtime_s(&local_tm,&tt);
  local_tm.tm_sec = 0;
  local_tm.tm_min = min;
  local_tm.tm_hour = hour;
  local_tm.tm_isdst = -1; // Use DST value from local time zone
  return std::chrono::system_clock::from_time_t(std::mktime(&local_tm));
}

TimePoint TimeTodayAt(std::string time)
{
  //parse str into hour & min
  int hour{0};
  int min{0};
  int sec{0};
  if (sscanf_s(time.c_str(), "%d:%d:%d", &hour, &min, &sec) >= 2)
  {
    if (time.ends_with("pm"))
    {
      hour += 12;
    }
    return TimeTodayAt(hour,min);
  }
  return TimeTodayAt(8,0);
}

TimePoint TimeTomorrowAt(std::string time)
{
  //parse str into hour & min
  int hour{0};
  int min{0};
  int sec{0};
  bool pm{false};
  if (sscanf_s(time.c_str(), "%d:%d:%d", &hour, &min, &sec) >= 2)
  {
    return TimeTomorrowAt(hour,min);
  }
  return TimeTomorrowAt(8,0);
}

void ShowPreBedtime(const std::string preface, const TimePoint now, const TimePoint bedtime)
{  //pre-wake
  const D_Range plan_tomorrow{ 5min, 5min };
  const D_Range no_screens{ 30min, 60min };
  const auto mins1 = std::chrono::duration_cast<std::chrono::minutes> ((bedtime - no_screens.first) - now);
  std::cout << preface << " Last Screen [ " 
    << ToString(bedtime - no_screens.second, "%I:%M%p") << " - "
    << ToString(bedtime - no_screens.first, "%I:%M%p") << " "
    << "]";
  const auto mins2 = std::chrono::duration_cast<std::chrono::minutes> ((bedtime - plan_tomorrow.first) - now);
  std::cout << " Planning [ " 
    << ToString(bedtime - plan_tomorrow.second, "%I:%M%p") << " - "
    << ToString(bedtime - plan_tomorrow.first, "%I:%M%p") << " "
    << "] ("  << mins2.count() << " minutes from now)\n";
}

void ShowAfterWakeTime(const std::string preface, TimePoint wake)
{
  const D_Range meditate_time{ 2min,10min };
  const D_Range exercize{15min,45min};
  const D_Range journaling{5min,15min};
  const auto focused_work = D_Range{ 15min, 60min };
  const auto first_email_time = 1h;

  auto soonest = wake + meditate_time.first;
  auto latest = wake + meditate_time.second;
  std::cout << preface << "\nMeditate\t" 
    << ToString(soonest, "%I:%M%p") << "\t"
    << ToString(latest, "%I:%M%p") << "\t"
    << "\n";   
  soonest += exercize.first;
  latest += exercize.second;
  std::cout << "Exercize\t"
    << ToString(soonest, "%I:%M%p") << "\t"
    << ToString(latest, "%I:%M%p") << "\t"
    << "\n";
  soonest += journaling.first;
  latest += journaling.second;
  std::cout << "Journaling\t"
    << ToString(soonest, "%I:%M%p") << "\t" 
    << ToString(latest, "%I:%M%p") << "\t"
    << "\n";
  soonest += focused_work.first;
  latest += focused_work.second;
  std::cout << "Focused Work\t"
    << ToString(soonest, "%I:%M%p") << "\t"
    << ToString(latest, "%I:%M%p") << "\t"
    << "\n";
  std::cout << "First Email\t"
    << ToString(wake+first_email_time, "%I:%M%p") 
    << "\n\n";
}

std::string PadRight( std::string msg, size_t length)
{
  auto result(msg);
  while(result.length() < length)
  {
    result = result + " ";
  }
  return result;
}

// TimePoint is only meaningful in the context of a clock
// and there's no "standard" clock
std::optional<TimePoint> TP_parse(const char* str)
{
  std::optional<TimePoint> result;
  std::tm tm{0}, tm0{};
  tm.tm_year = 2020-1900; // 2020
  tm.tm_mon  = 2-1;       // February
  tm.tm_mday = 15;        // 15th
  if (strptime(str, "%I:%M%p", &tm) != nullptr)
  {
    tm.tm_hour = tm0.tm_hour;
    tm.tm_min = tm0.tm_min;
    tm.tm_sec = tm0.tm_sec;
    auto temptime = std::mktime(&tm);
    if (temptime != -1)
    {
      result = TimePoint{std::chrono::system_clock::from_time_t(temptime)};
    }
  }
  return result;
}

void report_activity_list(const std::string& title, const TimePoint& deadline, const ActivityList& the_list)
{
  size_t longest_message{0};
  std::for_each(the_list.cbegin(), the_list.cend(), 
    [&](auto& item){ longest_message = std::max(longest_message, std::get<0>(item).length()); });

  Duration total_time{0};
  std::for_each(the_list.cbegin(), the_list.cend(), 
    [&](auto& item){ total_time += std::get<1>(item); });

  const auto startline = deadline - total_time;

  std::cout << "------------------------------------------\n";
  Duration accum{0};
  for(auto item : the_list)
  {
    std::cout << PadRight( std::get<0>(item), longest_message ) << " " << ToString(startline + accum, "%I:%M%p") << "\n";
    accum += std::get<1>(item);
  }
  std::cout << PadRight( title, longest_message ) << " " << ToString(startline + accum, "%I:%M%p") << "\n";
}
void report_race(
  const std::string& name, 
  const std::string& address, 
  const TimePoint& race_start, 
  const ActivityList& race_list)
{
  std::cout << "==========================================\n";
  std::cout << name << "\n";
  std::cout << address << "\n";
  report_activity_list("Race Starts"s, race_start, race_list);
  
}

int main(int argc,char* argv[])
{
  for(int i=0;i<argc;++i)
  {
    std::cout << "'" << argv[i] << "'\n";
  }

  const auto msg{
    "Spend 5 minutes before you go to bed mapping your top 1-3 priorities for the next day. "
    "Spend 10 minutes before bed thinking about your #1 goal or #1 problem you’re trying to solve. "
    "This will prime your subconscious while you sleep. As Thomas Edison has said, 'Never go "
    "to sleep without a request to your subconscious.' "
    "Don’t look at any electronic screens 30-60 minutes before going to bed"
    "Don’t check email/social media within 60 minutes of going to bed> "
    "Go to bed 7 hours before you intend to wake up"
    "Wake:::: "
    "Spend 2-10 minutes in prayer/meditation "
    "Pull out your journal and write by hand anything that comes to your mind related "
    "to your #1 goal or #1 problem you’re trying to solve. "
    "During your journal session, write your big picture vision/goals down "
    "in bullet point form and in present-tense.  "
    "Spend 15-45 minutes in intensive physical fitness "
    "Consume 30 grams of protein. "
    "Spend 15-60 minutes in focused activity on a big picture goal or passion project "
    "Don’t check email or social media for at least 60 minutes after waking up "
    "Take a cold shower. If this shower is immediately following physical fitness, just start with cold. "
    "Listen to or read uplifting content (you can do this while you exercise if that’s convenient) "
    "After you’ve spent a few minutes purposefully preparing yourself for HOW YOU INTEND TO BE that day. "
    "If you have creative endeavors, spend 60-90 minutes in focused activity on a big project. "
    "If you’re doing creative “Deep Work” immediately upon waking up, try listening to "
    "instrumental/ambient songs on repeat. "};

  const D_Range sleep_time{ 7h, 8h };

  const auto target_wake_up = TimeTomorrowAt("6:00am");
  const auto earliest_bedtime = target_wake_up - sleep_time.second;
  const auto latest_bedtime = target_wake_up - sleep_time.first;

  const auto now = system_clock::now();
  const auto ideal_bedtime = target_wake_up - 8h;
  const auto earliest_wake_up = now + sleep_time.first;
  const auto latest_wake_up = now + sleep_time.second;
  const auto mins = std::chrono::duration_cast<std::chrono::minutes> (ideal_bedtime - now);

  std::cout << "Target wake: " << ToString(target_wake_up,"%I:%M%p")
    << " Bedtime: " << ToString(ideal_bedtime, "%I:%M%p")
    << " ("  << mins.count() << " minutes from now)\n";
  if (mins.count() < 0)
  {
    std::cout << "Set alarm for " << ToString(now + 8h, "%I:%M%p") << "\n";
  }

  ShowPreBedtime("Early...", now, earliest_bedtime);
  ShowPreBedtime("LateR...", now, latest_bedtime);

  if (mins.count() > 0)
  {
    ShowAfterWakeTime("If I got up when I should...", target_wake_up);
  }
  else
  {
    ShowAfterWakeTime("If I go to bed now and sleep least...", earliest_wake_up);
    ShowAfterWakeTime("If I go to bed now and sleep most...", latest_wake_up);
  }

  //----------------------------------------------------------------------
  auto race_name = "Riverbank Run";
  auto race_address = "Corner Ottawa & Lyon Downtown Grand Rapids";
  auto drive_time = 15min;
  auto race_start{ TimeTomorrowAt("7:00am"s)};
  if (argc == 5)
  {
    race_name = argv[1];
    race_address = argv[2];
    drive_time = std::chrono::minutes(std::atoi(argv[3]));
    race_start = TimeTomorrowAt(argv[4]);
  }
  else
  {
    std::cout << "Usage: " << argv[0] << " <race-name> <race-address> <drive-time> <race-start>\n";
  }
  //----------------------------------------------------------------------
  const ActivityList race_list {
    {"Go to sleep"s, 9h},
    {"Wake up"s, 5min},
    {"Shower"s, 20min},
    {"Make coffee"s, 10min},
    {"Get dressed"s, 10min},
    {"Get checklist items"s, 5min},
    {"Drive to race"s, drive_time},
    {"Park near race"s, 5min},
    {"Pick up packet"s, 10min},
    {"Find Bathroom & Starting Line"s, 20min},
  };
  report_race(race_name, race_address, race_start, race_list);

  const ActivityList church_list {
    {"Go to sleep"s, 9h},
    {"Wake up"s, 5min},
    {"Shower"s, 20min},
    {"Get dressed"s, 10min},
    {"Walk to church", 35min},
    {"Find Pew", 15min},
  };
  report_activity_list("Sunday School Starts"s, TimeTomorrowAt("9:45am"), church_list);

  std::cout << "C++ version ID: " << __cplusplus << "\n";
  //199711L (C++98 or C++03)
  //201103L (C++11)
  //201402L (C++14)
  //201703L (C++17)
  //202002L (C++20)
  //202101L (C++2b)
  //202002L (C++latest /Zc:__cplusplus)
  //202302L (C++23)

  //nb: to_chars/from_chars is implemented before
  //GCC libstdc++ 13, Clang libc++ 16, MSVC STL 19.34
#ifdef _MSC_FULL_VER
  std::cout << "Visual Studio " << _MSC_FULL_VER << "\n";
#endif
#ifdef __GNUC__
  std::cout << "__GNUC__           " << __GNUC__ << "\n";
#endif
#if defined(__clang_major__) && defined(__clang_minor__)
  std::cout << "clang         " << __clang_major__ << "." << __clang_minor__ << "\n";
#endif

#if __cplusplus > 202002L
  auto const ufo = 1 <=> 2;
  auto it = "thing"sv.starts_with("th") || "thing"sv.ends_with("th");
  const auto haystack = std::string("haystack with needles");
  const auto needle = std::string("needle");
  if (haystack.contains(needle))
  {
    std::cout << "bazinga\n";
  }

  // play with various data types vs std::from_chars()
  std::array<char, 10> str{"42 xyz "};
  int result;
  if(auto [p, ec] = std::from_chars(str.data(), str.data()+str.size(), result); ec == std::errc())
  {
    std::cout << "Success: " << result << "\n" "p -> \"" << p << "\"\n";
  }
  //nb: to_chars/from_chars is implemented before
  //GCC libstdc++ 13, Clang libc++ 16, MSVC STL 19.34
#ifdef _MSC_FULL_VER
  std::cout << "Visual Studio " << _MSC_FULL_VER << "\n";
#endif
#ifdef __cpp_lib_to_chars  
  std::cout << "__cpp_lib_to_chars " << __cpp_lib_to_chars << "\n";
  std::string_view sv{"24 abc "};
  auto [p, ec] = std::from_chars(sv.begin(), sv.end(), result); 
  ec != std::errc()
    ? std::cout << "Couldn't convert value\n"
    : std::cout << result << "\n" "p -> \"" << p << "\"\n";

  //c++23: convert string "3.141592" to double 3.141592 using std::from_chars
  double pi = 3.141592;
  std::string_view pie = "3.141592";
  std::cout << "Pi is " << pi << '\n';
  double pi2 = 0;
  int pi_int = 0;
  const auto [p2, ec2] = std::from_chars(pie.begin(), pie.end(), pi2, std::chars_format::general);
  if (ec2 != std::errc())
  {
    std::cout << "Couldn't convert value\n";
  }
  else
  {
    std::cout << "Pi2 is " << pi2 << '\n';
  }
#endif
#endif
  return EXIT_SUCCESS;
}
