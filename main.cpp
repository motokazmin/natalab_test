#include "dnscache.h"

//Тестируем метод update, используем дополнительную синхронизацию с помощью cout_mutex, чтобы вывод на экран был читабельным
void update_sity(std::unordered_map<std::string, std::string>& test, DNSCache& dnsCache, std::shared_mutex& cout_mutex)
{
    std::thread::id this_id = std::this_thread::get_id();
    for (auto& s : test)
    {
        dnsCache.update(s.first, s.second);
        {
            std::unique_lock lock(cout_mutex);
            std::cout << this_id << " : update sity " << s.first << " / " << s.second << std::endl;
        }
    }
}

//Тестируем метод resolve, используем дополнительную синхронизацию с помощью cout_mutex, чтобы вывод на экран был читабельным
void resolve_sity(std::unordered_map<std::string, std::string>& test, DNSCache& dnsCache, std::shared_mutex& cout_mutex)
{
    std::thread::id this_id = std::this_thread::get_id();
    for (auto& s : test)
    {
        auto ip = dnsCache.resolve(s.first);
        {
            std::unique_lock lock(cout_mutex);
            std::cout << this_id << " : resolve sity " << s.first << " / " << ip << std::endl;
        }
    }
}

int main (void)
{
  auto& dnsCache = DNSCache::getInstance(10);
  std::unordered_map<std::string, std::string> m_ForTest = {
      {"google.com" , "8.8.8.8"}, {"rambler.ru", "81.19.82.98"}, {"er-telecom.ru", "87.236.16.8"}
  };
  
  
  std::shared_mutex cout_mutex;
  std::thread thread1(update_sity,  std::ref(m_ForTest), std::ref(dnsCache), std::ref(cout_mutex));
  std::thread thread2(resolve_sity, std::ref(m_ForTest),  std::ref(dnsCache), std::ref(cout_mutex));
  std::thread thread3(resolve_sity, std::ref(m_ForTest),  std::ref(dnsCache), std::ref(cout_mutex));
  std::thread thread4(resolve_sity, std::ref(m_ForTest),  std::ref(dnsCache), std::ref(cout_mutex));
 
  thread1.join();
  thread2.join();
  thread3.join();
  thread4.join();

  return 0;
}
