#include "dnscache.h"

void DNSCache::update(const std::string& name, const std::string& ip)
{
    std::unique_lock lock(m_mutex);
    if (m_DictIters.size() == m_Capacity) //После удаления элемента синхронизируем m_DictIters и m_UrlDict
    {
        auto iter = m_DictIters.front();
        m_UrlDict.erase(iter);
        m_DictIters.pop();
    }
    m_UrlDict[name] = ip;
    m_DictIters.push(m_UrlDict.begin());
}

std::string DNSCache::resolve(const std::string& name)
{
    try
    {
        std::shared_lock lock(m_mutex);
        return m_UrlDict.at(name);
    }
    catch(...)
    {
        return "Unknown host name";
    }
}
