#include <string>
#include <iostream>
#include <unordered_map>
#include <queue>
#include <shared_mutex>
#include <thread>

class DNSCache
{
private:
    const int m_Capacity; //Задает максимальное колличество хранимых элементов
    std::shared_mutex m_mutex; //Синхронизирует доступ к кэшу, для обнавления используем эксклюзивную блокировку, для резолва - общую
    std::unordered_map<std::string, std::string> m_UrlDict; //Маппа кэша: name/ip
    /*Маппа позволяет делать быструю вставку и поиск. К сожалению для неупорядоченной маппы нет поддержки обратного итератора, поэтому
    нет возможности добраться до последнего элемента в кеше(при уже полностью заполненном кэше). Поэтому используется дополнительная очередь,
    хранящая итераторы на вставляемые элементы маппы m_UrlDict. Это позволяет быстро нати последний элемент и удалить его из m_UrlDict.
    Минусом такого подхода является дополнительно потребляемая память порядка m_Capacity*sizeof(iterator)
    */
    std::queue<std::unordered_map<std::string, std::string>::iterator> m_DictIters; 
   
private:
    DNSCache() = default; // прошу компилятор сгенерировать самому этот метод чтобы не писать тело самому
    DNSCache(const int capacity) : m_Capacity(capacity) {}

    ~DNSCache() = default;
    DNSCache(const DNSCache&) = delete; // запрещает создание конструктора копирования 
    DNSCache& operator=(const DNSCache&) = delete;
 
public:
    static DNSCache& getInstance(const int capacity){
        static DNSCache instance(capacity);
        return instance;
    }
    
    void update(const std::string& name, const std::string& ip);
    std::string resolve(const std::string& name);
};
